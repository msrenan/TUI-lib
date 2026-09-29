# TUI-lib

Uma biblioteca **C++** para criar interfaces de terminal (TUI) com a mesma
filosofia da **SFML** e do **PyGame**: você tem uma **janela** com
**resolução**, desenha **texto** e **formas 2D** por **coordenadas (x, y)**,
controla **cores em RGB** e usa um **loop principal** com
`clear()` / `draw()` / `display()` — só que tudo roda dentro do terminal.

```cpp
#include "tui/TUI.hpp"

using namespace tui;

int main() {
    Window window(80, 24);              // resolução: 80 colunas x 24 linhas

    while (window.isOpen()) {
        window.clear();                 // apaga o quadro anterior
        window.title("MEU JOGO", Color::Cyan);
        window.display();               // mostra o quadro no terminal
        if (window.pollKey() == Key::Escape) window.close();
    }
    return 0;
}
```

## ✨ Recursos

| Recurso | Onde está |
|---|---|
| Resolução configurável (células) | `Window(width, height)` / `setResolution()` |
| Escrever texto em (x, y) | `Text` + `window.draw(texto)` |
| Quadrados, retângulos e triângulos posicionados em (x, y) | `SquareShape`, `RectangleShape`, `TriangleShape` |
| Segmentos de reta (bônus) | `LineShape` |
| Cores simplificadas em RGB | `Color(r, g, b)` + paleta pronta (`Color::Red`, ...) |
| Título grande estilizado em ASCII | `window.title("TÍTULO")` |
| Loop principal estilo SFML/PyGame | `clear()` / `draw()` / `display()` / `isOpen()` / `close()` |
| Sem *flicker* | Duplo buffer + renderização diferencial |
| Entrada de teclado | `pollKey()` / `waitKey()` (setas, F1–F12, ...) |
| Limite de FPS | `window.setFps(60)` |
| Tela alternativa (restaura o terminal ao sair) | `window.setAlternateScreen(true)` |

Sem dependências externas — apenas C++17 e um terminal ANSI.

## 📦 Requisitos

- **Linux** ou **macOS** (usa `termios`/`ioctl` e sequências de escape ANSI).
- Compilador com suporte a **C++17** (g++ ou clang++).
- Terminal com suporte a **UTF-8** e **true color** (24 bits) — praticamente
  todos os emuladores modernos (GNOME Terminal, Konsole, iTerm2, kitty,
  Windows Terminal, ...).

## 🔨 Compilação

### Com CMake

```bash
cmake -S . -B build-cmake
cmake --build build-cmake -j
./build-cmake/01_hello          # exemplos em build-cmake/
ctest --test-dir build-cmake    # testes
```

### Com Make

```bash
make            # compila a biblioteca estática (build/libtui.a)
make examples   # compila os 3 exemplos
make test       # compila e roda os testes
make run-hello  # roda o exemplo (idem: run-bounce, run-input)
make clean
```

### Manual (sem build system)

```bash
g++ -std=c++17 -Iinclude meu_programa.cpp src/*.cpp -o meu_programa
```

---

# 📖 Manual de uso

## 1. Criando a janela (resolução)

A "resolução" da janela é o número de **células** do terminal: colunas ×
linhas. Cada célula comporta um caractere.

```cpp
Window window(80, 24);   // 80 colunas x 24 linhas
// Window window;        // ou: usa o tamanho atual do terminal
```

A **origem das coordenadas** fica no canto superior esquerdo: `x` cresce para
a direita, `y` cresce para baixo (como no PyGame/SFML).

```cpp
window.setResolution(60, 20);          // troca a resolução em tempo de execução
int w = window.getWidth();             // 60
int h = window.getHeight();            // 20
```

> 💡 Se a resolução for maior que o seu terminal, parte do conteúdo ficará
> fora da tela (e pode rolar). Confira `Window::terminalSize()` para saber o
> tamanho disponível.

## 2. O loop principal

Assim como na SFML (`RenderWindow`) e no PyGame (`pygame.display`), o desenho
acontece em dois tempos: primeiro você monta o quadro em memória, depois o
envia ao terminal:

```cpp
Window window(80, 24);

while (window.isOpen()) {
    window.clear();            // 1. apaga o quadro (fundo preto por padrão)
                               //    ou: window.clear(Color{10, 10, 30});
    window.draw(objeto);       // 2. desenha texto/formas (pode repetir)
    window.display();          // 3. mostra o quadro no terminal
}

// close() faz isOpen() virar false:
window.close();
```

Objetos desenhados **depois** ficam **por cima** (algoritmo do pintor), como
no PyGame.

## 3. Cores (RGB)

```cpp
Color cor(255, 128, 0);            // laranja
Color outra{80, 160, 255};         // azul claro
```

Paleta pronta em `Color::`:

| Constantes | Cor |
|---|---|
| `Black`, `White`, `Gray`, `DarkGray`, `LightGray` | tons de cinza |
| `Red`, `Green`, `Blue` | primárias |
| `Yellow`, `Cyan`, `Magenta` | secundárias |
| `Orange`, `Purple`, `Pink`, `Brown`, `Navy`, `Teal`, `Maroon`, `Gold`, `Crimson` | extras |

Toda cor pode ser usada como **cor de frente** (dos caracteres) ou **cor de
fundo** (preenchimento de formas e fundo opaco de texto).

## 4. Escrevendo texto em (x, y)

```cpp
Text texto("Olá, mundo!", 5, 2);        // string, x, y
texto.setColor(Color::Yellow);          // cor da letra
window.draw(texto);
```

Principais métodos:

```cpp
texto.setString("novo texto");          // troca o conteúdo
texto.setPosition(x, y);                // reposiciona
texto.setColor(Color::Red);
texto.setBackground(Color::Blue);       // fundo opaco (padrão: transparente)
texto.clearBackground();                // volta a ser transparente
```

- `\n` quebra a linha (o x volta à origem do texto);
- `\t` vira 4 espaços;
- acentos e símbolos UTF-8 funcionam normalmente.

## 5. Posicionando formas em (x, y)

### Retângulo — `RectangleShape`

```cpp
RectangleShape ret(20, 6);              // 20 colunas x 6 linhas
ret.setPosition(5, 10);                 // canto superior esquerdo
ret.setFillColor(Color{30, 30, 80});    // preenchimento
ret.setOutlineColor(Color::Blue);       // borda
window.draw(ret);
```

### Quadrado — `SquareShape`

```cpp
SquareShape quad(5);                    // 5 x 5 células
quad.setPosition(10, 10);
quad.setFillColor(Color::Red);
window.draw(quad);
```

### Triângulo — `TriangleShape`

Definido por **3 vértices**; o preenchimento é rasterizado por scanline e a
borda acompanha a inclinação de cada aresta:

```cpp
TriangleShape tri({2, 10}, {20, 10}, {11, 2});   // base embaixo, ponta em cima
tri.setFillColor(Color::Green);
tri.setOutlineColor(Color::Yellow);
window.draw(tri);

// ou vértice a vértice:
tri.setPoint(0, x, y);
tri.setPosition(dx, dy);                // desloca os 3 vértices de uma vez
```

### Linha — `LineShape` (bônus)

```cpp
LineShape linha(0, 0, 79, 23);          // diagonal de canto a canto
linha.setColor(Color::Orange);
linha.setCharacter(U'·');               // caractere opcional (padrão: auto)
window.draw(linha);
```

### Opções comuns a todas as formas

```cpp
forma.setFilled(false);                 // só a borda (wireframe)
forma.setOutlined(false);               // só o preenchimento
forma.setFillCharacter(U'░');           // padrão de preenchimento
                                        // (padrão: ' ' + cor de fundo = sólido)
```

## 6. Título estilizado em ASCII art — `title()`

```cpp
window.title("MEU JOGO");                                // centralizado no topo
window.title("GAME OVER", 5, 3, Color::Red, 2);          // x, y, cor, escala
```

- Cada letra é desenhada com uma fonte 5×7 de blocos `█` (7 linhas de
  altura × `escala`);
- `escala` multiplica cada "pixel" do glifo (`2` = título duas vezes maior);
- **centralizado**: `window.title("TÍTULO", cor, escala)` posiciona sozinho;
- suporta `A-Z`, `0-9`, espaço e `. , : ! ? - /`. Minúsculas viram
  maiúsculas, acentos são normalizados (`ç` → `C`, `é` → `E`) e o restante
  vira `?`.

> O título é desenhado como qualquer outro elemento: chame `clear()` e
> `title()` a cada quadro no loop.

## 7. Entrada de teclado

```cpp
int tecla = window.pollKey();      // não-bloqueante; -1 (Key::None) se vazio
int tecla = window.waitKey();      // bloqueia até uma tecla ser pressionada
```

- **Teclas de caractere** voltam como o próprio código ASCII:
  `if (tecla == 'q')`, `if (tecla == ' ')`, etc.
- **Teclas especiais** usam as constantes `Key::`:

| Constantes |
|---|
| `Key::Up`, `Key::Down`, `Key::Left`, `Key::Right` |
| `Key::Enter`, `Key::Tab`, `Key::Backspace`, `Key::Escape`, `Key::Space` |
| `Key::Home`, `Key::End`, `Key::PageUp`, `Key::PageDown`, `Key::Delete` |
| `Key::F1` ... `Key::F12` |

```cpp
switch (window.pollKey()) {
    case Key::Up:    y--; break;
    case Key::Down:  y++; break;
    case 'q':
    case Key::Escape: window.close(); break;
}
```

> Em modo raw, `Ctrl+C` chega como tecla de código 3 (não interrompe o
> programa). Para matar à força, use `Ctrl+\` ou `kill` — o terminal é
> restaurado automaticamente via tratadores de sinal.

## 8. FPS

```cpp
window.setFps(30);   // display() espera para manter ~30 quadros/s
window.setFps(0);    // sem limite (padrão)
```

## 9. Tela alternativa

Por padrão, a janela desenha no terminal atual e o quadro final permanece
visível após o programa sair. Para se comportar como uma "janela" que some ao
fechar (restaurando o terminal), use a *alternate screen*:

```cpp
Window window(80, 24);
window.setAlternateScreen(true);   // chame ANTES do primeiro display()
```

---

# 📚 Referência da API

Tudo vive no namespace `tui`. Resumo:

### `tui::Color`
| Método | Descrição |
|---|---|
| `Color(r, g, b)` | constrói com componentes RGB 0–255 |
| `Color::Black` ... `Color::Crimson` | paleta pronta |
| `==` / `!=` | comparação |

### `tui::Text`
| Método | Descrição |
|---|---|
| `Text(str, x, y)` | cria texto em (x, y) |
| `setString(s)` / `getString()` | conteúdo |
| `setPosition(x, y)` / `getX()` / `getY()` | posição |
| `setColor(c)` / `getColor()` | cor da frente |
| `setBackground(c)` / `clearBackground()` / `hasBackground()` | fundo opaco |

### `tui::RectangleShape`
| Método | Descrição |
|---|---|
| `RectangleShape(w, h)` / `setSize(w, h)` | tamanho |
| `setPosition(x, y)` | canto superior esquerdo |
| `setFillColor(c)` / `setOutlineColor(c)` | cores |
| `setFillCharacter(ch)` | caractere do interior |
| `setFilled(bool)` / `setOutlined(bool)` | liga/desliga preenchimento e borda |

### `tui::SquareShape` — herda de `RectangleShape`
| Método | Descrição |
|---|---|
| `SquareShape(lado)` / `setSide(lado)` / `getSide()` | lado do quadrado |

### `tui::TriangleShape`
| Método | Descrição |
|---|---|
| `TriangleShape(a, b, c)` | por 3 `Point` ou 6 inteiros |
| `setPoint(i, x, y)` / `setPoints(a, b, c)` / `getPoint(i)` | vértices |
| `setPosition(x, y)` | desloca todos os vértices |
| `setFillColor(c)` / `setOutlineColor(c)` / `setFillCharacter(ch)` | cores/caractere |
| `setFilled(bool)` / `setOutlined(bool)` | preenchimento/borda |

### `tui::LineShape`
| Método | Descrição |
|---|---|
| `LineShape(x1, y1, x2, y2)` / `setPoints(...)` | extremidades |
| `setColor(c)` | cor |
| `setCharacter(ch)` | caractere (0 = automático pela inclinação) |

### `tui::Window`
| Método | Descrição |
|---|---|
| `Window()` / `Window(w, h)` | janela do tamanho do terminal / resolução dada |
| `getWidth()` / `getHeight()` / `setResolution(w, h)` | resolução |
| `clear(bg = Color::Black)` | apaga o quadro |
| `draw(drawable)` | desenha Text/forma |
| `title(texto)` / `title(texto, x, y, cor, escala)` | título em ASCII art |
| `display()` | envia o quadro ao terminal (+ limite de FPS) |
| `isOpen()` / `close()` | estado da janela |
| `setFps(n)` / `getFps()` | limite de FPS |
| `pollKey()` / `waitKey()` | entrada (retornam `int`; ver `tui::Key`) |
| `setAlternateScreen(bool)` | tela alternativa |
| `terminalSize()` (estático) | tamanho do terminal (colunas, linhas) |

### `tui::Key`
Namespace com constantes de teclas especiais (ver seção 7).

---

# 🎮 Exemplos

| Arquivo | O que demonstra | Como rodar |
|---|---|---|
| `examples/01_hello.cpp` | título, texto, cores e todas as formas (estático) | `make run-hello` |
| `examples/02_bounce.cpp` | loop de jogo + animação a 30 FPS | `make run-bounce` (ESC sai) |
| `examples/03_input.cpp` | movimentação em tempo real com as setas | `make run-input` (Q/ESC sai) |

Exemplo completo (equivale ao `01_hello`):

```cpp
#include "tui/TUI.hpp"

using namespace tui;

int main() {
    Window window(90, 30);
    window.clear(Color{10, 10, 30});            // fundo azul-escuro

    window.title("HELLO TUI", Color::Cyan);     // título centralizado

    Text texto("Olá, mundo!", 5, 10);           // texto em (5, 10)
    texto.setColor(Color::Yellow);
    window.draw(texto);

    SquareShape quadrado(6);                    // quadrado 6x6 em (46, 15)
    quadrado.setPosition(46, 15);
    quadrado.setFillColor(Color::Red);
    window.draw(quadrado);

    TriangleShape triangulo({56, 15}, {70, 15}, {63, 23});
    triangulo.setFillColor(Color::Green);
    window.draw(triangulo);

    window.display();
    window.waitKey();                           // espera uma tecla
    return 0;
}
```

---

# 🧠 Como funciona por dentro

1. **Framebuffer de células** — a janela mantém um grid `w × h` onde cada
   célula guarda um caractere + cores de frente/fundo (`detail::Canvas`).
   `draw()` apenas escreve nesse grid (clip automático).
2. **Duplo buffer** — o quadro atual é comparado com o quadro anterior e
   `display()` só reescreve as células que **mudaram** (posicionamento de
   cursor + cores ANSI `\x1b[38;2;r;g;b;48;2;r;g;bm`). Por isso animações não
   piscam e o uso de CPU é baixo.
3. **Terminal raw** — na primeira chamada de entrada, o terminal entra em
   modo raw via `termios` (sem echo, sem buffer de linha) e é **restaurado**
   ao sair (destrutor + tratadores de `SIGINT`/`SIGTERM`/`SIGHUP`).
4. **Cursor** — escondido durante a execução e reexibido no final.

---

# ⚠️ Limitações

- **1 caractere = 1 célula.** Glifos "largos" (CJK, emojis) ocupam 2 células
  no terminal, mas a biblioteca conta 1 — podem desalinhar o quadro. Texto
  latino e box-drawing funcionam perfeitamente.
- O `title()` cobre `A-Z`, `0-9` e poucos símbolos; acentos são aproximados
  pela letra base e o restante vira `?`.
- A detecção de teclas especiais (F5–F12, Home, ...) depende da sequência
  enviada pelo terminal; terminais fora do padrão podem não ser reconhecidos.
- Uma janela por processo é o caso de uso suportado (o estado do terminal é
  global). Windows (cmd/PowerShell) não é suportado.
- Sem suporte a mouse ou redimensionamento dinâmico do terminal em execução.

---

# 🗂️ Estrutura do projeto

```
TUI-lib/
├── include/tui/            # headers públicos (inclua "tui/TUI.hpp")
│   ├── TUI.hpp             #   umbrella header
│   ├── Color.hpp           #   cor RGB + paleta
│   ├── Key.hpp             #   códigos de teclas
│   ├── Drawable.hpp        #   interface Drawable
│   ├── Text.hpp            #   texto posicionável
│   ├── Shape.hpp           #   retângulo, quadrado, triângulo, linha
│   ├── Window.hpp          #   janela (resolução, loop, entrada, FPS)
│   └── detail/             #   internals (Canvas, UTF-8, fonte do título)
├── src/                    # implementação (biblioteca estática)
├── examples/               # 01_hello, 02_bounce, 03_input
├── tests/test_tui.cpp      # testes de sanidade
├── CMakeLists.txt          # build com CMake
├── Makefile                # build com make
└── README.md               # este arquivo
```

---

# 📄 Licença

[Apache License 2.0](LICENSE)
