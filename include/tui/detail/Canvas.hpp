#pragma once

#include <cstdint>
#include <vector>

#include "tui/Color.hpp"

namespace tui::detail {

/// Uma célula da tela: um caractere + cores de frente e fundo.
struct Cell {
    char32_t ch = U' ';
    Color fg{255, 255, 255};
    Color bg{0, 0, 0};

    bool operator==(const Cell& o) const {
        return ch == o.ch && fg == o.fg && bg == o.bg;
    }
    bool operator!=(const Cell& o) const { return !(*this == o); }
};

/// Framebuffer interno: um grid de células onde os desenháveis escrevem.
///
/// Coordenadas: origem no canto superior esquerdo, x cresce para a direita
/// e y cresce para baixo. Escritas fora dos limites são ignoradas (clip).
class Canvas {
public:
    Canvas() : Canvas(0, 0) {}
    Canvas(int width, int height)
        : width_(width), height_(height), cells_(static_cast<std::size_t>(width) * height) {}

    int width() const { return width_; }
    int height() const { return height_; }

    /// Pinta todas as células com o fundo dado (caractere vira espaço).
    void clear(const Color& bg) {
        for (auto& c : cells_) c = Cell{U' ', bg, bg};
    }

    /// Escreve um caractere com a cor de frente dada, preservando o fundo atual.
    void put(int x, int y, char32_t ch, const Color& fg) {
        if (!inBounds(x, y)) return;
        Cell& c = cells_[static_cast<std::size_t>(y) * width_ + x];
        c.ch = ch;
        c.fg = fg;
    }

    /// Escreve um caractere com cores de frente e fundo definidas.
    void put(int x, int y, char32_t ch, const Color& fg, const Color& bg) {
        if (!inBounds(x, y)) return;
        cells_[static_cast<std::size_t>(y) * width_ + x] = Cell{ch, fg, bg};
    }

    Cell& at(int x, int y) { return cells_[static_cast<std::size_t>(y) * width_ + x]; }
    const Cell& at(int x, int y) const {
        return cells_[static_cast<std::size_t>(y) * width_ + x];
    }

private:
    bool inBounds(int x, int y) const {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

    int width_ = 0;
    int height_ = 0;
    std::vector<Cell> cells_;
};

} // namespace tui::detail
