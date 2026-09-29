#pragma once

#include <string>

namespace tui::detail {

/// Converte uma string UTF-8 para UTF-32 (bytes inválidos viram U+FFFD).
std::u32string utf8ToUtf32(const std::string& s);

/// Anexa a codificação UTF-8 do code point `cp` a `out`.
void appendUtf8(char32_t cp, std::string& out);

} // namespace tui::detail
