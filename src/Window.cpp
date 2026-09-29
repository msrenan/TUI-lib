#include "tui/Window.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include "tui/Key.hpp"
#include "tui/detail/Canvas.hpp"
#include "tui/detail/TitleFont.hpp"
#include "tui/detail/Utf8.hpp"

namespace tui {

namespace {

// ---------------------------------------------------------------------------
// Estado global do terminal (uma janela por processo é o caso de uso típico).
// ---------------------------------------------------------------------------

struct TerminalState {
    int fd = -1;                 // fd usado para leitura de teclas (/dev/tty)
    bool raw = false;            // modo raw ativo?
    termios saved{};             // configuração original do terminal
    bool altScreen = false;      // alternate screen ativo?
    bool cursorHidden = false;   // cursor escondido?
};

TerminalState g_term;

void writeOut(const char* s) {
    ::write(STDOUT_FILENO, s, std::char_traits<char>::length(s));
}

// Melhor esforço: restaura o terminal antes de encerrar por sinal
// (ex.: Ctrl+\ ou `kill`). Não é async-signal-safe estrito, mas é o que
// bibliotecas TUI fazem na prática.
extern "C" void tuiSignalHandler(int sig) {
    if (g_term.cursorHidden) writeOut("\033[?25h");
    if (g_term.altScreen) writeOut("\033[?1049l");
    if (g_term.raw && g_term.fd >= 0) ::tcsetattr(g_term.fd, TCSANOW, &g_term.saved);
    std::signal(sig, SIG_DFL);
    ::raise(sig);
}

// Coloca o terminal em modo raw (sem echo, sem buffer de linha, sem ISIG).
// Chamado de forma preguiçosa no primeiro uso de entrada.
void ensureRawMode() {
    if (g_term.raw) return;
    if (g_term.fd < 0) {
        g_term.fd = ::open("/dev/tty", O_RDWR | O_NOCTTY);
        if (g_term.fd < 0) g_term.fd = STDIN_FILENO;
    }
    if (::tcgetattr(g_term.fd, &g_term.saved) == 0) {
        termios raw = g_term.saved;
        cfmakeraw(&raw);
        raw.c_cc[VMIN] = 0;  // leitura não-bloqueante
        raw.c_cc[VTIME] = 0;
        ::tcsetattr(g_term.fd, TCSANOW, &raw);
        g_term.raw = true;
    }
    // Garante não-bloqueio mesmo no fallback para STDIN.
    const int flags = ::fcntl(g_term.fd, F_GETFL, 0);
    if (flags >= 0) ::fcntl(g_term.fd, F_SETFL, flags | O_NONBLOCK);
}

void restoreTerminal() {
    if (g_term.raw && g_term.fd >= 0) {
        ::tcsetattr(g_term.fd, TCSANOW, &g_term.saved);
        g_term.raw = false;
    }
}

} // namespace

// ---------------------------------------------------------------------------
// Impl
// ---------------------------------------------------------------------------

struct Window::Impl {
    int width = 0;
    int height = 0;
    detail::Canvas current;
    detail::Canvas previous;
    bool open = true;
    bool firstFrame = true;

    bool started = false;          // primeiro display() já aconteceu?
    bool altScreenRequested = false;
    bool altScreenActive = false;

    int fps = 0;
    std::chrono::steady_clock::time_point lastFrame{};

    // Estado do cursor/cores mantido entre quadros (coordenadas 1-based).
    int curRow = 0;
    int curCol = 0;
    bool haveColor = false;
    Color lastFg;
    Color lastBg;
};

// ---------------------------------------------------------------------------
// Construção / destruição
// ---------------------------------------------------------------------------

Window::Window() : impl_(std::make_unique<Impl>()) {
    const auto size = terminalSize();
    init(size.first, size.second);
}

Window::Window(int width, int height) : impl_(std::make_unique<Impl>()) {
    init(width, height);
}

void Window::init(int width, int height) {
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("tui::Window: a resolução deve ser maior que zero");

    impl_->width = width;
    impl_->height = height;
    impl_->current = detail::Canvas(width, height);
    impl_->previous = detail::Canvas(width, height);

    std::signal(SIGINT, tuiSignalHandler);
    std::signal(SIGTERM, tuiSignalHandler);
    std::signal(SIGHUP, tuiSignalHandler);
}

Window::~Window() {
    if (!impl_) return;
    if (impl_->started) {
        writeOut("\033[0m");
        if (g_term.cursorHidden) writeOut("\033[?25h");
        if (impl_->altScreenActive) writeOut("\033[?1049l");
        g_term.cursorHidden = false;
        g_term.altScreen = false;
    }
    restoreTerminal();
    if (g_term.fd >= 0) {
        ::close(g_term.fd);
        g_term.fd = -1;
    }
}

// ---------------------------------------------------------------------------
// Propriedades
// ---------------------------------------------------------------------------

int Window::getWidth() const { return impl_->width; }
int Window::getHeight() const { return impl_->height; }

void Window::setResolution(int width, int height) {
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("tui::Window: a resolução deve ser maior que zero");
    impl_->width = width;
    impl_->height = height;
    impl_->current = detail::Canvas(width, height);
    impl_->previous = detail::Canvas(width, height);
    impl_->firstFrame = true;
    impl_->curRow = 0;
    impl_->curCol = 0;
}

bool Window::isOpen() const { return impl_->open; }

void Window::close() { impl_->open = false; }

void Window::setFps(int fps) { impl_->fps = fps < 0 ? 0 : fps; }
int Window::getFps() const { return impl_->fps; }

void Window::setAlternateScreen(bool enabled) { impl_->altScreenRequested = enabled; }

std::pair<int, int> Window::terminalSize() {
    winsize ws{};
    if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0 && ws.ws_row > 0)
        return {ws.ws_col, ws.ws_row};

    const char* cols = std::getenv("COLUMNS");
    const char* lines = std::getenv("LINES");
    int w = cols ? std::atoi(cols) : 80;
    int h = lines ? std::atoi(lines) : 24;
    return {w > 0 ? w : 80, h > 0 ? h : 24};
}

// ---------------------------------------------------------------------------
// Desenho
// ---------------------------------------------------------------------------

void Window::clear(const Color& background) {
    impl_->current.clear(background);
}

void Window::draw(const Drawable& drawable) {
    drawable.draw(impl_->current);
}

void Window::title(const std::string& text, const Color& color, int scale) {
    const std::u32string u = detail::utf8ToUtf32(text);
    const int total = detail::titleTextWidth(u, scale);
    const int x = (impl_->width - total) / 2;
    detail::renderTitle(impl_->current, u, x > 0 ? x : 0, 1, color, scale);
}

void Window::title(const std::string& text, int x, int y,
                   const Color& color, int scale) {
    detail::renderTitle(impl_->current, detail::utf8ToUtf32(text), x, y, color, scale);
}

void Window::display() {
    Impl& im = *impl_;

    // Primeiro quadro: prepara o terminal.
    if (!im.started) {
        im.started = true;
        if (im.altScreenRequested) {
            writeOut("\033[?1049h");
            im.altScreenActive = true;
            g_term.altScreen = true;
        }
        writeOut("\033[?25l");
        g_term.cursorHidden = true;
    }

    std::string out;
    out.reserve(4096);
    const int w = im.width;
    const int h = im.height;

    // Renderização diferencial: emite apenas as células que mudaram.
    for (int y = 0; y < h; ++y) {
        int x = 0;
        while (x < w) {
            const bool changed = im.firstFrame || !(im.current.at(x, y) == im.previous.at(x, y));
            if (!changed) {
                ++x;
                continue;
            }

            const int runStart = x;
            while (x < w && (im.firstFrame || !(im.current.at(x, y) == im.previous.at(x, y))))
                ++x;

            for (int xx = runStart; xx < x; ++xx) {
                const detail::Cell& c = im.current.at(xx, y);

                if (!im.haveColor || c.fg != im.lastFg || c.bg != im.lastBg) {
                    if (!im.haveColor) out += "\033[0m"; // limpa atributos pendentes
                    out += "\033[38;2;";
                    out += std::to_string(c.fg.r);
                    out += ';';
                    out += std::to_string(c.fg.g);
                    out += ';';
                    out += std::to_string(c.fg.b);
                    out += ";48;2;";
                    out += std::to_string(c.bg.r);
                    out += ';';
                    out += std::to_string(c.bg.g);
                    out += ';';
                    out += std::to_string(c.bg.b);
                    out += 'm';
                    im.haveColor = true;
                    im.lastFg = c.fg;
                    im.lastBg = c.bg;
                }

                if (im.curRow != y + 1 || im.curCol != xx + 1) {
                    out += "\033[";
                    out += std::to_string(y + 1);
                    out += ';';
                    out += std::to_string(xx + 1);
                    out += 'H';
                    im.curRow = y + 1;
                    im.curCol = xx + 1;
                }

                detail::appendUtf8(c.ch, out);
                ++im.curCol;
            }
        }
    }

    if (!out.empty()) {
        std::cout << out << std::flush;
    }

    im.firstFrame = false;
    im.previous = im.current;

    // Limite de FPS.
    if (im.fps > 0) {
        const auto frameTime = std::chrono::microseconds(1000000 / im.fps);
        const auto now = std::chrono::steady_clock::now();
        if (im.lastFrame.time_since_epoch().count() == 0 || now >= im.lastFrame + frameTime) {
            im.lastFrame = now;
        } else {
            std::this_thread::sleep_until(im.lastFrame + frameTime);
            im.lastFrame += frameTime;
        }
    }
}

// ---------------------------------------------------------------------------
// Entrada
// ---------------------------------------------------------------------------

int Window::pollKey() {
    ensureRawMode();
    const int fd = g_term.fd;
    if (fd < 0) return Key::None;

    unsigned char b = 0;
    const ssize_t n = ::read(fd, &b, 1);
    if (n != 1) return Key::None;

    if (b == '\r' || b == '\n') return Key::Enter;
    if (b == 0x7f || b == 0x08) return Key::Backspace;
    if (b == 27) return readEscapeSequence(fd);
    return b; // caractere imprimível (inclui Ctrl+C = 3, em modo raw)
}

int Window::readEscapeSequence(int fd) const {
    // Lê os próximos bytes de uma possível sequência ESC [ ... / ESC O ...
    const auto readByte = [fd](unsigned char& out) -> bool {
        pollfd p{fd, POLLIN, 0};
        if (::poll(&p, 1, 25) <= 0) return false;
        return ::read(fd, &out, 1) == 1;
    };

    unsigned char b2 = 0;
    if (!readByte(b2)) return Key::Escape;

    if (b2 == '[') {
        unsigned char b3 = 0;
        if (!readByte(b3)) return Key::None;
        switch (b3) {
            case 'A': return Key::Up;
            case 'B': return Key::Down;
            case 'C': return Key::Right;
            case 'D': return Key::Left;
            case 'H': return Key::Home;
            case 'F': return Key::End;
            case '3': case '5': case '6': {
                unsigned char b4 = 0;
                if (readByte(b4) && b4 == '~') {
                    if (b3 == '3') return Key::Delete;
                    if (b3 == '5') return Key::PageUp;
                    return Key::PageDown;
                }
                return Key::None;
            }
            case '1': {
                unsigned char b4 = 0;
                if (!readByte(b4)) return Key::None;
                unsigned char b5 = 0;
                if (!readByte(b5) || b5 != '~') return Key::None;
                switch (b4) {
                    case '1': return Key::F1;
                    case '2': return Key::F2;
                    case '3': return Key::F3;
                    case '4': return Key::F4;
                    case '5': return Key::F5;
                    case '7': return Key::F6;
                    case '8': return Key::F7;
                    case '9': return Key::F8;
                    default: return Key::None;
                }
            }
            case '2': {
                unsigned char b4 = 0;
                if (!readByte(b4)) return Key::None;
                unsigned char b5 = 0;
                if (!readByte(b5) || b5 != '~') return Key::None;
                switch (b4) {
                    case '0': return Key::F9;
                    case '1': return Key::F10;
                    case '3': return Key::F11;
                    case '4': return Key::F12;
                    default: return Key::None;
                }
            }
            default: return Key::None;
        }
    }

    if (b2 == 'O') {
        unsigned char b3 = 0;
        if (!readByte(b3)) return Key::None;
        switch (b3) {
            case 'P': return Key::F1;
            case 'Q': return Key::F2;
            case 'R': return Key::F3;
            case 'S': return Key::F4;
            case 'H': return Key::Home;
            case 'F': return Key::End;
            default: return Key::None;
        }
    }

    return Key::None;
}

int Window::waitKey() {
    ensureRawMode();
    for (;;) {
        const int k = pollKey();
        if (k != Key::None) return k;
        if (!impl_->open) return Key::None;
        pollfd p{g_term.fd, POLLIN, 0};
        ::poll(&p, 1, 20);
    }
}

} // namespace tui
