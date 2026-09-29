#pragma once

// TUI-lib: uma biblioteca para criar TUIs (Text User Interfaces) em C++,
// inspirada na SFML e no PyGame.

#include "tui/Color.hpp"
#include "tui/Drawable.hpp"
#include "tui/Key.hpp"
#include "tui/Shape.hpp"
#include "tui/Text.hpp"
#include "tui/Window.hpp"

// Tudo o que você precisa está no namespace tui:
//
//     #include "tui/TUI.hpp"
//
//     int main() {
//         tui::Window window(80, 24);
//         while (window.isOpen()) {
//             window.clear();
//             window.title("DEMO", tui::Color::Cyan);
//             window.display();
//             if (window.pollKey() == tui::Key::Escape) window.close();
//         }
//     }
