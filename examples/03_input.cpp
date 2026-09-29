// 03_input.cpp — controle em tempo real com as setas do teclado.
//
// Rode: ./build/03_input   (setas movem o quadrado; Q ou ESC sai)

#include "tui/TUI.hpp"

using namespace tui;

int main() {
    Window window(60, 20);
    window.setFps(60);

    int px = 28;
    int py = 9;

    SquareShape player(3);
    player.setFillColor(Color::Green);

    Text instructions("Setas: mover   Q/ESC: sair", 2, 18);
    instructions.setColor(Color::DarkGray);

    while (window.isOpen()) {
        window.clear();
        window.title("CONTROLE", Color::Cyan);

        player.setPosition(px, py);
        window.draw(player);
        window.draw(instructions);

        window.display();

        // pollKey() é não-bloqueante: devolve Key::None (-1) se não há tecla.
        const int key = window.pollKey();
        switch (key) {
            case Key::Up:    --py; break;
            case Key::Down:  ++py; break;
            case Key::Left:  --px; break;
            case Key::Right: ++px; break;
            case Key::Escape:
            case 'q':
            case 'Q': window.close(); break;
            default: break;
        }

        // Mantém o quadrado dentro da janela.
        if (px < 0) px = 0;
        if (py < 8) py = 8;
        if (px > window.getWidth() - 3) px = window.getWidth() - 3;
        if (py > window.getHeight() - 3) py = window.getHeight() - 3;
    }
    return 0;
}
