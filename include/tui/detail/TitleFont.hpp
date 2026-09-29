#pragma once

#include <array>
#include <string>

#include "tui/Color.hpp"
#include "tui/detail/Canvas.hpp"

namespace tui::detail {

/// Um glifo da fonte de títulos (7 linhas de `width` colunas, usando '█').
struct Glyph {
    int width = 5;
    std::array<std::u32string, 7> rows{};
};

/// Retorna o glifo para o caractere pedido.
///
/// Apenas A-Z, 0-9, espaço e alguns símbolos (`.:,!?-/`) têm glifos próprios.
/// Letras minúsculas viram maiúsculas, acentos são normalizados
/// (ex.: 'é' vira 'E', 'ç' vira 'C') e o restante vira '?'.
const Glyph& titleGlyph(char32_t c);

/// Largura total (em células) do título, já com espaçamento entre glifos.
/// `scale` multiplica cada "pixel" do glifo.
int titleTextWidth(const std::u32string& text, int scale = 1);

/// Desenha o título em ASCII art no canvas a partir de (x, y).
/// O fundo das células é preservado (apenas os blocos são pintados).
void renderTitle(Canvas& canvas, const std::u32string& text, int x, int y,
                 const Color& color, int scale);

} // namespace tui::detail
