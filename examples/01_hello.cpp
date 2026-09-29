// 01_hello.cpp — vitrine estática: título, texto, cores e formas.
//
// Rode: ./build/01_hello

#include "tui/TUI.hpp"

using namespace tui;

int main() {
    // Resolução: 90 colunas x 30 linhas.
    Window window(90, 30);

    // Fundo azul-escuro.
    window.clear(Color{10, 10, 30});

    // Grande título em ASCII art, centralizado no topo.
    window.title("HELLO TUI", Color::Cyan);

    // Texto centralizado na linha 10.
    Text subtitle("Uma TUI com cara de SFML/PyGame");
    subtitle.setColor(Color::LightGray);
    subtitle.setPosition((window.getWidth() - static_cast<int>(subtitle.getString().size())) / 2, 10);
    window.draw(subtitle);

    // Retângulo com preenchimento e contorno.
    RectangleShape panel(36, 8);
    panel.setPosition(4, 14);
    panel.setFillColor(Color{25, 25, 55});
    panel.setOutlineColor(Color::Blue);
    window.draw(panel);

    Text inside("Texto dentro de um retângulo", 8, 17);
    inside.setColor(Color::White);
    window.draw(inside);

    // Quadrado.
    SquareShape square(6);
    square.setPosition(46, 15);
    square.setFillColor(Color{120, 30, 30});
    square.setOutlineColor(Color::Red);
    window.draw(square);

    // Triângulo (3 vértices).
    TriangleShape triangle({56, 15}, {70, 15}, {63, 23});
    triangle.setFillColor(Color{20, 90, 20});
    triangle.setOutlineColor(Color::Green);
    window.draw(triangle);

    // Uma linha diagonal de exemplo.
    LineShape line(74, 23, 88, 27);
    line.setColor(Color::Orange);
    window.draw(line);

    Text hint("Pressione qualquer tecla para sair...", 4, 28);
    hint.setColor(Color::Gray);
    window.draw(hint);

    window.display();
    window.waitKey();
    window.close();
    return 0;
}
