#pragma once

#include <memory>
#include <string>
#include <utility>

#include "tui/Color.hpp"
#include "tui/Drawable.hpp"

namespace tui {

/// Janela da TUI — o análogo da `RenderWindow` da SFML ou da superfície
/// principal do PyGame.
///
/// A "resolução" é o tamanho da janela em células (colunas x linhas).
/// A origem das coordenadas fica no canto superior esquerdo: x cresce para
/// a direita e y cresce para baixo.
///
/// Ciclo de vida típico:
///     Window window(80, 24);
///     while (window.isOpen()) {
///         window.clear();            // apaga o quadro anterior
///         window.draw(objeto);       // desenha no quadro atual
///         window.display();          // mostra o quadro no terminal
///     }
///
/// A janela usa duplo buffer + renderização diferencial: apenas as células
/// que mudaram entre dois quadros são reescritas, o que evita "flicker".
class Window {
public:
    /// Cria uma janela do tamanho atual do terminal.
    Window();

    /// Cria uma janela com a resolução dada (células: colunas x linhas).
    Window(int width, int height);

    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    int getWidth() const;
    int getHeight() const;

    /// Redefine a resolução (redesenha do zero no próximo display()).
    void setResolution(int width, int height);

    /// Apaga o quadro com a cor de fundo dada (padrão: preto).
    void clear(const Color& background = Color::Black);

    /// Desenha um desenhável (Text, formas...) no quadro atual.
    /// Objetos desenhados depois ficam "por cima" (algoritmo do pintor).
    void draw(const Drawable& drawable);

    /// Grande título em ASCII art, centralizado no topo da janela.
    void title(const std::string& text, const Color& color = Color::White, int scale = 1);

    /// Grande título em ASCII art em uma posição explícita (x, y).
    /// `scale` multiplica o tamanho de cada "pixel" do glifo.
    void title(const std::string& text, int x, int y,
               const Color& color = Color::White, int scale = 1);

    /// Envia o quadro atual para o terminal (e aplica o limite de FPS).
    void display();

    /// Falso depois de `close()`. Use para controlar o loop principal.
    bool isOpen() const;

    /// Fecha a janela (o loop que testa isOpen() termina).
    void close();

    /// Limite de quadros por segundo. 0 = sem limite (padrão).
    void setFps(int fps);
    int getFps() const;

    /// Tecla disponível agora, ou `Key::None` (-1) se nada foi pressionado.
    /// Teclas imprimíveis voltam como o próprio código do caractere.
    int pollKey();

    /// Bloqueia até uma tecla ser pressionada (ou a janela ser fechada).
    int waitKey();

    /// Usa o "alternate screen" (tela separada que é restaurada ao sair).
    /// Chame antes do primeiro `display()`. Desativado por padrão.
    void setAlternateScreen(bool enabled);

    /// Tamanho atual do terminal (colunas, linhas).
    static std::pair<int, int> terminalSize();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    void init(int width, int height);
    int readEscapeSequence(int fd) const;
};

} // namespace tui
