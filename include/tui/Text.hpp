#pragma once

#include <string>

#include "tui/Color.hpp"
#include "tui/Drawable.hpp"

namespace tui {

/// Texto posicionável por coordenadas (x, y).
///
/// Por padrão o fundo é transparente (a célula de baixo aparece). Use
/// `setBackground()` para dar fundo opaco ao texto.
///
/// Exemplo:
///     Text texto("Olá, mundo!", 5, 2);
///     texto.setColor(Color::Yellow);
///     window.draw(texto);
class Text : public Drawable {
public:
    Text() = default;
    Text(const std::string& str, int x = 0, int y = 0) {
        setString(str);
        setPosition(x, y);
    }

    /// Define o conteúdo. Aceita '\n' (nova linha) e '\t' (4 espaços).
    void setString(const std::string& str);
    const std::string& getString() const { return original_; }

    /// Posição do canto superior esquerdo do texto, em células.
    void setPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }
    int getX() const { return x_; }
    int getY() const { return y_; }

    /// Cor da frente (cor dos caracteres).
    void setColor(const Color& color) { fg_ = color; }
    const Color& getColor() const { return fg_; }

    /// Fundo opaco. Use `clearBackground()` para voltar a ser transparente.
    void setBackground(const Color& bg) {
        hasBg_ = true;
        bg_ = bg;
    }
    void clearBackground() { hasBg_ = false; }
    bool hasBackground() const { return hasBg_; }
    const Color& getBackground() const { return bg_; }

    void draw(detail::Canvas& canvas) const override;

private:
    std::string original_;
    std::u32string chars_;
    int x_ = 0;
    int y_ = 0;
    Color fg_{255, 255, 255};
    bool hasBg_ = false;
    Color bg_{0, 0, 0};
};

} // namespace tui
