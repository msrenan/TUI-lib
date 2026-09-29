#pragma once

namespace tui {

/// Códigos de tecla retornados por `Window::pollKey()` e `Window::waitKey()`.
///
/// Teclas "imprimíveis" são retornadas como o próprio código ASCII do
/// caractere (ex.: 'a' vira 97). Use `static_cast<char>(k)` se precisar do
/// caractere. Teclas especiais usam os códigos abaixo (>= 256).
///
/// Uso:
///     int tecla = window.pollKey();
///     if (tecla == Key::Up)      { ... }
///     if (tecla == 'q')          { ... } // compara com caractere
namespace Key {

enum : int {
    None = -1,   ///< Nenhuma tecla disponível.

    // Teclas "de caractere"
    Enter = 10,      ///< Enter (também '\n').
    Tab = 9,         ///< Tab.
    Backspace = 127, ///< Backspace (aceita 127 e 8).
    Escape = 27,     ///< ESC sozinho.
    Space = 32,      ///< Espaço.

    // Teclas especiais
    Up = 256,
    Down,
    Left,
    Right,
    Home,
    End,
    PageUp,
    PageDown,
    Delete,

    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
};

} // namespace Key
} // namespace tui
