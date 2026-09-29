// 02_bounce.cpp — loop de jogo: um quadrado quicando a 30 FPS.
//
// Rode: ./build/02_bounce   (ESC para sair)

#include "tui/TUI.hpp"

using namespace tui;

int main() {
    Window window(80, 24);
    window.setFps(30); // 30 quadros por segundo

    const int size = 4;
    int x = 10;
    int y = 12;
    int vx = 1;
    int vy = 1;

    SquareShape square(size);
    square.setFillColor(Color::Orange);
    square.setOutlineColor(Color::Yellow);

    Text hint("ESC para sair", 2, 22);
    hint.setColor(Color::DarkGray);

    while (window.isOpen()) {
        window.clear();                 // apaga o quadro anterior
        window.title("BOUNCE", Color::Yellow); // desenhado todo quadro;
                                               // o render diferencial só
                                               // reescreve o que mudou
        window.draw(hint);
        square.setPosition(x, y);
        window.draw(square);
        window.display();               // mostra o quadro + limite de FPS

        x += vx;
        y += vy;
        if (x <= 0 || x + size >= window.getWidth()) vx = -vx;
        if (y <= 7 || y + size >= window.getHeight()) vy = -vy; // 7 = fim do título

        if (window.pollKey() == Key::Escape) window.close();
    }
    return 0;
}
