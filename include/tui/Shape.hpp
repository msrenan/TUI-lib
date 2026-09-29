#pragma once

#include <cstdint>

#include "tui/Color.hpp"
#include "tui/Drawable.hpp"

namespace tui {

/// Ponto 2D em células (x = coluna, y = linha).
struct Point {
    int x = 0;
    int y = 0;
};

/// Retângulo com preenchimento e contorno opcionais.
///
/// Exemplo:
///     RectangleShape ret(20, 6);          // 20 colunas x 6 linhas
///     ret.setPosition(5, 10);
///     ret.setFillColor(Color{30, 30, 80});
///     ret.setOutlineColor(Color::Blue);
///     window.draw(ret);
///
/// O preenchimento usa a cor de fundo das células (visual "sólido").
/// Para um preenchimento "padronizado", troque o caractere de preenchimento
/// com `setFillCharacter()` (ex.: '#' ou U'░').
class RectangleShape : public Drawable {
public:
    RectangleShape() = default;
    RectangleShape(int width, int height) { setSize(width, height); }

    void setSize(int width, int height) {
        w_ = width < 0 ? 0 : width;
        h_ = height < 0 ? 0 : height;
    }
    int getWidth() const { return w_; }
    int getHeight() const { return h_; }

    void setPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }
    int getX() const { return x_; }
    int getY() const { return y_; }

    void setFillColor(const Color& color) { fillColor_ = color; }
    const Color& getFillColor() const { return fillColor_; }
    void setOutlineColor(const Color& color) { outlineColor_ = color; }
    const Color& getOutlineColor() const { return outlineColor_; }

    /// Caractere usado no interior (padrão: espaço + cor de fundo).
    void setFillCharacter(char32_t ch) { fillCh_ = ch; }
    char32_t getFillCharacter() const { return fillCh_; }

    void setFilled(bool filled) { filled_ = filled; }
    bool isFilled() const { return filled_; }
    void setOutlined(bool outlined) { outlined_ = outlined; }
    bool isOutlined() const { return outlined_; }

    void draw(detail::Canvas& canvas) const override;

protected:
    int x_ = 0;
    int y_ = 0;
    int w_ = 0;
    int h_ = 0;
    Color fillColor_{255, 255, 255};
    Color outlineColor_{255, 255, 255};
    char32_t fillCh_ = U' ';
    bool filled_ = true;
    bool outlined_ = true;
};

/// Quadrado — um `RectangleShape` com lados iguais.
///
/// Exemplo:
///     SquareShape quad(5);               // 5 x 5 células
///     quad.setPosition(10, 10);
///     quad.setFillColor(Color::Red);
class SquareShape : public RectangleShape {
public:
    SquareShape() = default;
    explicit SquareShape(int side) : RectangleShape(side, side) {}

    void setSide(int side) { setSize(side, side); }
    int getSide() const { return w_; }
};

/// Triângulo definido por 3 pontos, com preenchimento (scanline) e contorno.
///
/// Exemplo:
///     TriangleShape tri({2, 10}, {20, 10}, {11, 2}); // base embaixo, ponta em cima
///     tri.setFillColor(Color::Green);
///     window.draw(tri);
class TriangleShape : public Drawable {
public:
    TriangleShape() = default;
    TriangleShape(int x1, int y1, int x2, int y2, int x3, int y3) {
        setPoint(0, x1, y1);
        setPoint(1, x2, y2);
        setPoint(2, x3, y3);
    }
    TriangleShape(Point a, Point b, Point c) {
        pts_[0] = a;
        pts_[1] = b;
        pts_[2] = c;
    }

    /// Define o ponto de índice 0..2.
    void setPoint(int index, int x, int y);
    void setPoints(Point a, Point b, Point c) {
        pts_[0] = a;
        pts_[1] = b;
        pts_[2] = c;
    }
    Point getPoint(int index) const { return pts_[index]; }

    /// Desloca todos os pontos (offset somado a cada vértice).
    void setPosition(int x, int y) {
        ox_ = x;
        oy_ = y;
    }
    int getX() const { return ox_; }
    int getY() const { return oy_; }

    void setFillColor(const Color& color) { fillColor_ = color; }
    const Color& getFillColor() const { return fillColor_; }
    void setOutlineColor(const Color& color) { outlineColor_ = color; }
    const Color& getOutlineColor() const { return outlineColor_; }

    void setFillCharacter(char32_t ch) { fillCh_ = ch; }
    char32_t getFillCharacter() const { return fillCh_; }

    void setFilled(bool filled) { filled_ = filled; }
    bool isFilled() const { return filled_; }
    void setOutlined(bool outlined) { outlined_ = outlined; }
    bool isOutlined() const { return outlined_; }

    void draw(detail::Canvas& canvas) const override;

private:
    Point pts_[3] = {};
    int ox_ = 0;
    int oy_ = 0;
    Color fillColor_{255, 255, 255};
    Color outlineColor_{255, 255, 255};
    char32_t fillCh_ = U' ';
    bool filled_ = true;
    bool outlined_ = true;
};

/// Segmento de reta (rasterizado com Bresenham).
///
/// O caractere padrão é escolhido pela inclinação ('│', '─', '/' ou '\\').
/// Use `setCharacter()` para forçar outro (ex.: '·' ou U'█').
class LineShape : public Drawable {
public:
    LineShape() = default;
    LineShape(int x1, int y1, int x2, int y2) { setPoints(x1, y1, x2, y2); }

    void setPoints(int x1, int y1, int x2, int y2) {
        x1_ = x1;
        y1_ = y1;
        x2_ = x2;
        y2_ = y2;
    }

    void setColor(const Color& color) { color_ = color; }
    const Color& getColor() const { return color_; }

    /// 0 (padrão) = caractere automático conforme a inclinação.
    void setCharacter(char32_t ch) { ch_ = ch; }
    char32_t getCharacter() const { return ch_; }

    void draw(detail::Canvas& canvas) const override;

private:
    int x1_ = 0;
    int y1_ = 0;
    int x2_ = 0;
    int y2_ = 0;
    Color color_{255, 255, 255};
    char32_t ch_ = 0;
};

} // namespace tui
