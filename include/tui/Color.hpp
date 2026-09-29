#pragma once

#include <cstdint>

namespace tui {

/// Cor no formato RGB (true color), cada componente entre 0 e 255.
///
/// Exemplos:
///     Color branco;                 // 255, 255, 255
///     Color vermelho(255, 0, 0);
///     Color azul_claro{80, 160, 255};
///     Color c = Color::Orange;      // paleta pronta
struct Color {
    std::uint8_t r = 255;
    std::uint8_t g = 255;
    std::uint8_t b = 255;

    constexpr Color() = default;
    constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
        : r(red), g(green), b(blue) {}

    constexpr bool operator==(const Color& o) const {
        return r == o.r && g == o.g && b == o.b;
    }
    constexpr bool operator!=(const Color& o) const { return !(*this == o); }

    // ---- Paleta básica (definições no final deste header) ----
    static const Color Black;
    static const Color White;
    static const Color Gray;
    static const Color DarkGray;
    static const Color LightGray;

    static const Color Red;
    static const Color Green;
    static const Color Blue;

    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;

    static const Color Orange;
    static const Color Purple;
    static const Color Pink;
    static const Color Brown;
    static const Color Navy;
    static const Color Teal;
    static const Color Maroon;
    static const Color Gold;
    static const Color Crimson;
};

// Definições fora da classe: dentro dela o tipo Color ainda estaria
// incompleto. Em C++17 esses membros são implicitamente inline.
constexpr Color Color::Black{0, 0, 0};
constexpr Color Color::White{255, 255, 255};
constexpr Color Color::Gray{128, 128, 128};
constexpr Color Color::DarkGray{64, 64, 64};
constexpr Color Color::LightGray{192, 192, 192};

constexpr Color Color::Red{255, 0, 0};
constexpr Color Color::Green{0, 255, 0};
constexpr Color Color::Blue{0, 0, 255};

constexpr Color Color::Yellow{255, 255, 0};
constexpr Color Color::Cyan{0, 255, 255};
constexpr Color Color::Magenta{255, 0, 255};

constexpr Color Color::Orange{255, 165, 0};
constexpr Color Color::Purple{128, 0, 128};
constexpr Color Color::Pink{255, 105, 180};
constexpr Color Color::Brown{139, 69, 19};
constexpr Color Color::Navy{0, 0, 128};
constexpr Color Color::Teal{0, 128, 128};
constexpr Color Color::Maroon{128, 0, 0};
constexpr Color Color::Gold{255, 215, 0};
constexpr Color Color::Crimson{220, 20, 60};

} // namespace tui
