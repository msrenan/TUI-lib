#include "tui/detail/TitleFont.hpp"

#include <initializer_list>

#include "tui/detail/Utf8.hpp"

namespace tui::detail {

namespace {

/// Monta um glifo a partir de 7 linhas de texto (usando '█' e ' ').
Glyph makeGlyph(int width, std::initializer_list<const char*> rows) {
    Glyph g;
    g.width = width;
    int i = 0;
    for (const char* r : rows) {
        if (i < 7) g.rows[static_cast<std::size_t>(i)] = utf8ToUtf32(r);
        ++i;
    }
    return g;
}

} // namespace

const Glyph& titleGlyph(char32_t c) {
    // Minúsculas viram maiúsculas.
    if (c >= U'a' && c <= U'z') c = c - 32;

    // Acentos (latim-1) são normalizados para a letra base.
    switch (c) {
        case U'Á': case U'À': case U'Â': case U'Ã': case U'Ä': case U'Å':
            c = U'A'; break;
        case U'É': case U'È': case U'Ê': case U'Ë':
            c = U'E'; break;
        case U'Í': case U'Ì': case U'Î': case U'Ï':
            c = U'I'; break;
        case U'Ó': case U'Ò': case U'Ô': case U'Õ': case U'Ö':
            c = U'O'; break;
        case U'Ú': case U'Ù': case U'Û': case U'Ü':
            c = U'U'; break;
        case U'Ç': c = U'C'; break;
        case U'Ñ': c = U'N'; break;
        case U'Ý': c = U'Y'; break;
        default: break;
    }

    switch (c) {
        case U'A': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█   █",
                "█████",
                "█   █",
                "█   █",
                "█   █",
            });
            return g;
        }
        case U'B': {
            static const Glyph g = makeGlyph(5, {
                "████ ",
                "█   █",
                "█   █",
                "████ ",
                "█   █",
                "█   █",
                "████ ",
            });
            return g;
        }
        case U'C': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█    ",
                "█    ",
                "█    ",
                "█   █",
                " ███ ",
            });
            return g;
        }
        case U'D': {
            static const Glyph g = makeGlyph(5, {
                "████ ",
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                "████ ",
            });
            return g;
        }
        case U'E': {
            static const Glyph g = makeGlyph(5, {
                "█████",
                "█    ",
                "█    ",
                "████ ",
                "█    ",
                "█    ",
                "█████",
            });
            return g;
        }
        case U'F': {
            static const Glyph g = makeGlyph(5, {
                "█████",
                "█    ",
                "█    ",
                "████ ",
                "█    ",
                "█    ",
                "█    ",
            });
            return g;
        }
        case U'G': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█    ",
                "█  ██",
                "█   █",
                "█   █",
                " ███ ",
            });
            return g;
        }
        case U'H': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "█   █",
                "█   █",
                "█████",
                "█   █",
                "█   █",
                "█   █",
            });
            return g;
        }
        case U'I': {
            static const Glyph g = makeGlyph(5, {
                "█████",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
                "█████",
            });
            return g;
        }
        case U'J': {
            static const Glyph g = makeGlyph(5, {
                "  ███",
                "   █ ",
                "   █ ",
                "   █ ",
                "   █ ",
                "█  █ ",
                " ██  ",
            });
            return g;
        }
        case U'K': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "█  █ ",
                "█ █  ",
                "██   ",
                "█ █  ",
                "█  █ ",
                "█   █",
            });
            return g;
        }
        case U'L': {
            static const Glyph g = makeGlyph(5, {
                "█    ",
                "█    ",
                "█    ",
                "█    ",
                "█    ",
                "█    ",
                "█████",
            });
            return g;
        }
        case U'M': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "██ ██",
                "█ █ █",
                "█ █ █",
                "█   █",
                "█   █",
                "█   █",
            });
            return g;
        }
        case U'N': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "██  █",
                "█ █ █",
                "█  ██",
                "█   █",
                "█   █",
                "█   █",
            });
            return g;
        }
        case U'O': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                " ███ ",
            });
            return g;
        }
        case U'P': {
            static const Glyph g = makeGlyph(5, {
                "████ ",
                "█   █",
                "█   █",
                "████ ",
                "█    ",
                "█    ",
                "█    ",
            });
            return g;
        }
        case U'Q': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█   █",
                "█   █",
                "█ █ █",
                "█  █ ",
                " ██ █",
            });
            return g;
        }
        case U'R': {
            static const Glyph g = makeGlyph(5, {
                "████ ",
                "█   █",
                "█   █",
                "████ ",
                "█ █  ",
                "█  █ ",
                "█   █",
            });
            return g;
        }
        case U'S': {
            static const Glyph g = makeGlyph(5, {
                " ████",
                "█    ",
                "█    ",
                " ███ ",
                "    █",
                "    █",
                "████ ",
            });
            return g;
        }
        case U'T': {
            static const Glyph g = makeGlyph(5, {
                "█████",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
            });
            return g;
        }
        case U'U': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                " ███ ",
            });
            return g;
        }
        case U'V': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                "█   █",
                " █ █ ",
                "  █  ",
            });
            return g;
        }
        case U'W': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "█   █",
                "█   █",
                "█ █ █",
                "█ █ █",
                "██ ██",
                "█   █",
            });
            return g;
        }
        case U'X': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "█   █",
                " █ █ ",
                "  █  ",
                " █ █ ",
                "█   █",
                "█   █",
            });
            return g;
        }
        case U'Y': {
            static const Glyph g = makeGlyph(5, {
                "█   █",
                "█   █",
                " █ █ ",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
            });
            return g;
        }
        case U'Z': {
            static const Glyph g = makeGlyph(5, {
                "█████",
                "    █",
                "   █ ",
                "  █  ",
                " █   ",
                "█    ",
                "█████",
            });
            return g;
        }
        case U'0': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█  ██",
                "█ █ █",
                "██  █",
                "█   █",
                " ███ ",
            });
            return g;
        }
        case U'1': {
            static const Glyph g = makeGlyph(5, {
                "  █  ",
                " ██  ",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
                "█████",
            });
            return g;
        }
        case U'2': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "    █",
                "   █ ",
                "  █  ",
                " █   ",
                "█████",
            });
            return g;
        }
        case U'3': {
            static const Glyph g = makeGlyph(5, {
                "████ ",
                "    █",
                "    █",
                " ███ ",
                "    █",
                "    █",
                "████ ",
            });
            return g;
        }
        case U'4': {
            static const Glyph g = makeGlyph(5, {
                "   █ ",
                "  ██ ",
                " █ █ ",
                "█  █ ",
                "█████",
                "   █ ",
                "   █ ",
            });
            return g;
        }
        case U'5': {
            static const Glyph g = makeGlyph(5, {
                "█████",
                "█    ",
                "████ ",
                "    █",
                "    █",
                "    █",
                "████ ",
            });
            return g;
        }
        case U'6': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█    ",
                "█    ",
                "████ ",
                "█   █",
                "█   █",
                " ███ ",
            });
            return g;
        }
        case U'7': {
            static const Glyph g = makeGlyph(5, {
                "█████",
                "    █",
                "   █ ",
                "  █  ",
                "  █  ",
                "  █  ",
                "  █  ",
            });
            return g;
        }
        case U'8': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█   █",
                " ███ ",
                "█   █",
                "█   █",
                " ███ ",
            });
            return g;
        }
        case U'9': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "█   █",
                " ████",
                "    █",
                "    █",
                " ███ ",
            });
            return g;
        }
        case U' ': {
            static const Glyph g = makeGlyph(3, {
                "   ", "   ", "   ", "   ", "   ", "   ", "   ",
            });
            return g;
        }
        case U'.': {
            static const Glyph g = makeGlyph(3, {
                "   ", "   ", "   ", "   ", "   ", "██ ", "██ ",
            });
            return g;
        }
        case U',': {
            static const Glyph g = makeGlyph(3, {
                "   ", "   ", "   ", "   ", "   ", " █ ", "█  ",
            });
            return g;
        }
        case U':': {
            static const Glyph g = makeGlyph(3, {
                "   ", " █ ", " █ ", "   ", " █ ", " █ ", "   ",
            });
            return g;
        }
        case U'!': {
            static const Glyph g = makeGlyph(3, {
                " █ ", " █ ", " █ ", " █ ", " █ ", "   ", " █ ",
            });
            return g;
        }
        case U'?': {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "    █",
                "   █ ",
                "  █  ",
                "     ",
                "  █  ",
            });
            return g;
        }
        case U'-': {
            static const Glyph g = makeGlyph(5, {
                "     ", "     ", "     ", "█████", "     ", "     ", "     ",
            });
            return g;
        }
        case U'/': {
            static const Glyph g = makeGlyph(5, {
                "    █",
                "    █",
                "   █ ",
                "   █ ",
                "  █  ",
                " █   ",
                "█    ",
            });
            return g;
        }
        default: {
            static const Glyph g = makeGlyph(5, {
                " ███ ",
                "█   █",
                "    █",
                "   █ ",
                "  █  ",
                "     ",
                "  █  ",
            });
            return g;
        }
    }
}

int titleTextWidth(const std::u32string& text, int scale) {
    if (text.empty()) return 0;
    if (scale < 1) scale = 1;
    int total = 0;
    for (char32_t ch : text) total += titleGlyph(ch).width * scale;
    total += (static_cast<int>(text.size()) - 1) * scale; // espaçamento
    return total;
}

void renderTitle(Canvas& canvas, const std::u32string& text, int x, int y,
                 const Color& color, int scale) {
    if (scale < 1) scale = 1;
    int cx = x;
    for (char32_t ch : text) {
        const Glyph& g = titleGlyph(ch);
        if (ch != U' ') {
            for (int row = 0; row < 7; ++row) {
                const std::u32string& line = g.rows[static_cast<std::size_t>(row)];
                for (int col = 0; col < g.width; ++col) {
                    if (line[static_cast<std::size_t>(col)] != U' ') {
                        for (int dy = 0; dy < scale; ++dy) {
                            for (int dx = 0; dx < scale; ++dx) {
                                canvas.put(cx + col * scale + dx, y + row * scale + dy,
                                           U'█', color);
                            }
                        }
                    }
                }
            }
        }
        cx += g.width * scale + scale;
    }
}

} // namespace tui::detail
