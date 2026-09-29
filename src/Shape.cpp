#include "tui/Shape.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <utility>

#include "tui/detail/Canvas.hpp"

namespace tui {

void TriangleShape::setPoint(int index, int x, int y) {
    pts_[index].x = x;
    pts_[index].y = y;
}

// ---------------------------------------------------------------------------
// RectangleShape
// ---------------------------------------------------------------------------

void RectangleShape::draw(detail::Canvas& canvas) const {
    if (w_ <= 0 || h_ <= 0) return;

    const int x0 = x_;
    const int y0 = y_;
    const int x1 = x_ + w_ - 1;
    const int y1 = y_ + h_ - 1;

    for (int yy = y0; yy <= y1; ++yy) {
        for (int xx = x0; xx <= x1; ++xx) {
            const bool border =
                outlined_ && (xx == x0 || xx == x1 || yy == y0 || yy == y1);
            if (border) {
                char32_t ch;
                if (xx == x0 && yy == y0)
                    ch = U'┌';
                else if (xx == x1 && yy == y0)
                    ch = U'┐';
                else if (xx == x0 && yy == y1)
                    ch = U'└';
                else if (xx == x1 && yy == y1)
                    ch = U'┘';
                else if (yy == y0 || yy == y1)
                    ch = U'─';
                else
                    ch = U'│';

                if (filled_)
                    canvas.put(xx, yy, ch, outlineColor_, fillColor_);
                else
                    canvas.put(xx, yy, ch, outlineColor_);
            } else if (filled_) {
                canvas.put(xx, yy, fillCh_, fillColor_, fillColor_);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// TriangleShape (scanline)
// ---------------------------------------------------------------------------

void TriangleShape::draw(detail::Canvas& canvas) const {
    const Point a{pts_[0].x + ox_, pts_[0].y + oy_};
    const Point b{pts_[1].x + ox_, pts_[1].y + oy_};
    const Point c{pts_[2].x + ox_, pts_[2].y + oy_};

    std::array<Point, 3> v = {a, b, c};
    std::sort(v.begin(), v.end(), [](const Point& p, const Point& q) {
        return p.y != q.y ? p.y < q.y : p.x < q.x;
    });

    const int ymin = v[0].y;
    const int ymax = v[2].y;

    // Caractere de contorno conforme a inclinação da aresta.
    const auto edgeChar = [](const Point& p, const Point& q) -> char32_t {
        const int dx = q.x - p.x;
        const int dy = q.y - p.y;
        if (dy == 0) return U'─';
        if (dx == 0) return U'│';
        return (dx > 0) == (dy > 0) ? U'\\' : U'/';
    };

    for (int y = ymin; y <= ymax; ++y) {
        // Interseções da linha horizontal central da célula (y + 0.5)
        // com as arestas não-horizontais do triângulo.
        std::array<std::pair<double, char32_t>, 2> xs;
        int count = 0;

        for (int e = 0; e < 3; ++e) {
            const Point& p = v[static_cast<std::size_t>(e)];
            const Point& q = v[static_cast<std::size_t>((e + 1) % 3)];
            if (p.y == q.y) continue; // aresta horizontal: tratada depois

            const double t = (y + 0.5 - p.y) / static_cast<double>(q.y - p.y);
            if (t < 0.0 || t > 1.0) continue;
            const double x = p.x + t * static_cast<double>(q.x - p.x);
            if (count < 2) xs[static_cast<std::size_t>(count++)] = {x, edgeChar(p, q)};
        }

        if (count < 2) continue;
        if (xs[0].first > xs[1].first) std::swap(xs[0], xs[1]);

        const int xl = static_cast<int>(std::ceil(xs[0].first - 0.5));
        const int xr = static_cast<int>(std::floor(xs[1].first + 0.5));

        if (filled_) {
            for (int x = xl + 1; x <= xr - 1; ++x)
                canvas.put(x, y, fillCh_, fillColor_, fillColor_);
        }

        if (outlined_) {
            if (filled_) {
                canvas.put(xl, y, xs[0].second, outlineColor_, fillColor_);
                canvas.put(xr, y, xs[1].second, outlineColor_, fillColor_);
            } else {
                canvas.put(xl, y, xs[0].second, outlineColor_);
                canvas.put(xr, y, xs[1].second, outlineColor_);
            }
        }
    }

    // Arestas horizontais (topo/base planos).
    if (outlined_) {
        for (int e = 0; e < 3; ++e) {
            const Point& p = v[static_cast<std::size_t>(e)];
            const Point& q = v[static_cast<std::size_t>((e + 1) % 3)];
            if (p.y != q.y) continue;
            const int xmin = std::min(p.x, q.x);
            const int xmax = std::max(p.x, q.x);
            for (int x = xmin; x <= xmax; ++x) {
                if (filled_)
                    canvas.put(x, p.y, U'─', outlineColor_, fillColor_);
                else
                    canvas.put(x, p.y, U'─', outlineColor_);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// LineShape (Bresenham)
// ---------------------------------------------------------------------------

void LineShape::draw(detail::Canvas& canvas) const {
    int x = x1_;
    int y = y1_;
    const int dx = std::abs(x2_ - x1_);
    const int sx = x1_ < x2_ ? 1 : -1;
    const int dy = -std::abs(y2_ - y1_);
    const int sy = y1_ < y2_ ? 1 : -1;
    int err = dx + dy;

    char32_t ch = ch_;
    if (ch == 0) {
        if (x1_ == x2_)
            ch = U'│';
        else if (y1_ == y2_)
            ch = U'─';
        else
            ch = (sx == sy) ? U'\\' : U'/';
    }

    for (;;) {
        canvas.put(x, y, ch, color_);
        if (x == x2_ && y == y2_) break;
        const int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y += sy;
        }
    }
}

} // namespace tui
