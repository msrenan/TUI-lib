// Testes de sanidade (sem framework externo): fonte de títulos, canvas,
// formas, texto e conversão UTF-8.
//
// Rode: ./build/test_tui   (ou: make test / ctest)

#include <cassert>
#include <string>

#include "tui/TUI.hpp"
#include "tui/detail/Canvas.hpp"
#include "tui/detail/TitleFont.hpp"
#include "tui/detail/Utf8.hpp"

using namespace tui;

static void testColor() {
    const Color c(10, 20, 30);
    assert(c.r == 10 && c.g == 20 && c.b == 30);
    assert(Color::Red == Color(255, 0, 0));
    assert(Color::Black != Color::White);
    assert(Color::Orange.r == 255 && Color::Orange.g == 165 && Color::Orange.b == 0);
}

static void testCanvas() {
    detail::Canvas cv(10, 5);
    cv.clear(Color::Blue);
    assert(cv.at(0, 0).bg == Color::Blue);

    cv.put(2, 3, U'x', Color::Red);          // preserva o fundo
    assert(cv.at(2, 3).ch == U'x');
    assert(cv.at(2, 3).fg == Color::Red);
    assert(cv.at(2, 3).bg == Color::Blue);

    cv.put(99, 99, U'y', Color::Red);        // fora dos limites: ignorado
    assert(cv.at(9, 4).ch == U' ');

    cv.put(4, 4, U'z', Color::Green, Color::Black);
    assert(cv.at(4, 4).bg == Color::Black);
}

static void testShapes() {
    detail::Canvas cv(20, 10);
    cv.clear(Color::Black);

    // Retângulo: cantos, interior preenchido, fora intocado.
    RectangleShape r(6, 4);
    r.setPosition(2, 2);
    r.setFillColor(Color::Red);
    r.setOutlineColor(Color::White);
    r.draw(cv);
    assert(cv.at(2, 2).ch == U'┌');
    assert(cv.at(7, 2).ch == U'┐');
    assert(cv.at(2, 5).ch == U'└');
    assert(cv.at(7, 5).ch == U'┘');
    assert(cv.at(4, 3).bg == Color::Red);
    assert(cv.at(4, 3).ch == U' ');
    assert(cv.at(0, 0).bg == Color::Black);

    // Triângulo com base plana: interior preenchido, contorno '─' na base.
    TriangleShape t(4, 2, 10, 2, 7, 8);
    t.setFillColor(Color::Green);
    t.draw(cv);
    assert(cv.at(7, 4).bg == Color::Green);  // interior
    assert(cv.at(7, 1).bg == Color::Black);  // acima da base
    assert(cv.at(5, 2).ch == U'─');          // base plana
    assert(cv.at(2, 4).bg != Color::Green);  // fora, à esquerda

    // Quadrado.
    SquareShape s(5);
    assert(s.getSide() == 5);
    assert(s.getWidth() == 5 && s.getHeight() == 5);

    // Triângulo só com contorno (wireframe): não pinta o interior.
    detail::Canvas cv2(20, 10);
    cv2.clear(Color::Black);
    TriangleShape wf(4, 2, 10, 2, 7, 8);
    wf.setFilled(false);
    wf.setOutlineColor(Color::Yellow);
    wf.draw(cv2);
    assert(cv2.at(7, 4).bg == Color::Black);
    assert(cv2.at(7, 4).ch == U' ');
    assert(cv2.at(5, 2).ch == U'─');
    assert(cv2.at(5, 2).fg == Color::Yellow);

    // Linha horizontal e vertical.
    detail::Canvas cv3(10, 10);
    cv3.clear(Color::Black);
    LineShape h(1, 1, 5, 1);
    h.draw(cv3);
    assert(cv3.at(3, 1).ch == U'─');
    LineShape v(1, 1, 1, 5);
    v.draw(cv3);
    assert(cv3.at(1, 3).ch == U'│');
}

static void testText() {
    detail::Canvas cv(10, 3);
    cv.clear(Color::Black);

    Text t("olá", 1, 1);
    t.setColor(Color::Yellow);
    t.draw(cv);
    assert(cv.at(1, 1).ch == U'o');
    assert(cv.at(2, 1).ch == U'l');
    assert(cv.at(3, 1).ch == U'á');           // UTF-8 decodificado
    assert(cv.at(1, 1).fg == Color::Yellow);
    assert(cv.at(1, 1).bg == Color::Black);   // fundo transparente

    Text t2("a\nb", 0, 0);
    t2.setBackground(Color::White);
    t2.draw(cv);
    assert(cv.at(0, 0).ch == U'a');
    assert(cv.at(0, 1).ch == U'b');           // '\n' pula de linha
    assert(cv.at(0, 0).bg == Color::White);   // fundo opaco
}

static void testTitle() {
    const auto& g = detail::titleGlyph(U'A');
    assert(g.width == 5);
    for (const auto& row : g.rows) assert(row.size() == 5);

    assert(detail::titleGlyph(U' ').width == 3);

    // Normalização: acento e minúscula caem na mesma letra.
    assert(&detail::titleGlyph(U'É') == &detail::titleGlyph(U'E'));
    assert(&detail::titleGlyph(U'a') == &detail::titleGlyph(U'A'));
    assert(&detail::titleGlyph(U'Ç') == &detail::titleGlyph(U'C'));

    // Largura: A(5) + espaço(1) + B(5).
    assert(detail::titleTextWidth(std::u32string{U"AB"}) == 11);
    assert(detail::titleTextWidth(std::u32string{U"A"}) == 5);

    // Renderização pinta blocos na canvas.
    detail::Canvas cv(30, 10);
    cv.clear(Color::Black);
    detail::renderTitle(cv, std::u32string{U"A"}, 0, 0, Color::Cyan, 1);
    bool any = false;
    for (int y = 0; y < 7 && !any; ++y)
        for (int x = 0; x < 5 && !any; ++x)
            if (cv.at(x, y).ch == U'█') any = true;
    assert(any);

    // Escala 2 dobra a área pintada.
    detail::Canvas cv2(30, 20);
    cv2.clear(Color::Black);
    detail::renderTitle(cv2, std::u32string{U"A"}, 0, 0, Color::Red, 2);
    int count1 = 0, count2 = 0;
    for (int y = 0; y < 7; ++y)
        for (int x = 0; x < 5; ++x)
            if (cv.at(x, y).ch == U'█') ++count1;
    for (int y = 0; y < 14; ++y)
        for (int x = 0; x < 10; ++x)
            if (cv2.at(x, y).ch == U'█') ++count2;
    assert(count2 == count1 * 4);
}

static void testUtf8() {
    assert(detail::utf8ToUtf32("abc") == std::u32string{U"abc"});
    assert(detail::utf8ToUtf32("ção") == std::u32string{U"ção"});
    assert(detail::utf8ToUtf32("") == std::u32string{});

    std::string out;
    detail::appendUtf8(U'█', out);
    assert(out == "\xE2\x96\x88");
    detail::appendUtf8(U'A', out);
    assert(out == "\xE2\x96\x88" "A");
}

int main() {
    testColor();
    testCanvas();
    testShapes();
    testText();
    testTitle();
    testUtf8();
    return 0;
}
