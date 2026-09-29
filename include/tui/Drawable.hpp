#pragma once

namespace tui {

namespace detail {
class Canvas; // framebuffer interno
}

/// Interface de tudo o que pode ser desenhado em uma `Window`.
///
/// `Text`, `RectangleShape`, `SquareShape`, `TriangleShape` e `LineShape`
/// derivam desta classe, então todos são aceitos por `Window::draw()`.
class Drawable {
public:
    virtual ~Drawable() = default;

    /// Desenha o objeto no framebuffer interno. Uso interno — o usuário
    /// chama `window.draw(obj)` em vez disso.
    virtual void draw(detail::Canvas& canvas) const = 0;
};

} // namespace tui
