<div align="center">

# ♟️ Шаховий рушій

**Мінімалістичний шаховий рушій на C++ — з нуля, без залежностей окрім STL.**

[![C++17](https://img.shields.io/badge/C%2B%2B17-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=white)](CMakeLists.txt)
[![Release](https://img.shields.io/github/v/release/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine/releases/latest)
[![Tests](https://img.shields.io/github/actions/workflow/status/ByteMe6/chess-engine/test.yml?label=tests)](https://github.com/ByteMe6/chess-engine/actions/workflows/test.yml)
[![Dependencies: STL only](https://img.shields.io/badge/dependencies-STL%20only-brightgreen)](src/chessEngine.hpp)
[![Code size](https://img.shields.io/github/languages/code-size/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine)
[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-green)](LICENSE)
[![PRs welcome](https://img.shields.io/badge/PRs-welcome-blueviolet)](CONTRIBUTING.md)

[English](README.md) · **Українська** · [Русский](README.ru.md)

<img src="assets/board.svg" width="440" alt="Фінальна позиція демо-партії — білий ферзь щойно поставив мат на d7">

*Фінальна позиція вбудованого демо — білий ферзь щойно поставив мат на d7.*

</div>

---

## 📖 Зміст

- [Що це?](#-shcho-tse)
- [Швидкий старт](#-shvydkyi-start)
- [Як підключити бібліотекою](#-yak-pidkliuchyty)
- [Можливості](#-mozhlyvosti)
- [Використання](#-vykorystannia)
- [Довідник API](#-dovidnyk-api)
- [Як це працює](#-yak-tse-pratsiuie)
- [Структура проєкту](#-struktura-proiektu)
- [Плани](#%EF%B8%8F-plany)
- [Внесок](#-vnesok)
- [Ліцензія](#-litsenziia)

<a id="-shcho-tse"></a>
## ✨ Що це?

Невеликий і читабельний шаховий рушій без жодної бібліотеки під капотом — кожне правило написане вручну поверх стандартної бібліотеки C++. Він знає, як ходять фігури, перевіряє легальність ходів, розуміє шахову нотацію та малює дошку прямо в терміналі.

Це насамперед навчальний проєкт: код написано так, щоб його було приємно читати. Якщо вам колись було цікаво, як шахові програми генерують ходи, — `src/chessEngine.hpp` відповідає на це за ~250 рядків.

<a id="-shvydkyi-start"></a>
## 🚀 Швидкий старт

**Готові бінарники** — беріть на [сторінці релізів](https://github.com/ByteMe6/chess-engine/releases/latest): Linux (x86_64 / arm64), Windows (x86_64 / arm64), macOS (universal). У репозиторії теж лежить один — `bin/chess-engine` (macOS universal — Apple Silicon та Intel):

```bash
git clone git@github.com:ByteMe6/chess-engine.git
cd chess-engine
./bin/chess-engine
```

**Збірка з коду** (будь-яка платформа з CMake ≥ 3.16 і компілятором C++17):

```bash
cmake -B build
cmake --build build
./build/chess-engine
```

Демо розігрує коротку партію, що завершується матом — дивіться на останні рядки:

```console
$ ./bin/chess-engine

  a b c d e f g h
8 r n b q k b n r 8
7 p p p p p p p p 7
6                 6
5                 5
4                 4
3                 3
2 P P P P P P P P 2
1 R N B Q K B N R 1
  a b c d e f g h

Pawn e2: [ [ 5, 4 ], [ 4, 4 ] ]
Game over: White won
The game is over

  a b c d e f g h
8   r   k   b n r 8
7 p p p Q p p p p 7
6     B           6
5       p         5
4         P       4
3             P   3
2 P P P P     P P 2
1 R N B   K   N R 1
  a b c d e f g h
```

Великі літери — білі, малі — чорні. Олдскульний FEN-вайб.

<a id="-yak-pidkliuchyty"></a>
## 📦 Як підключити бібліотекою

Весь рушій — один заголовковий файл. Обери свій варіант і просто `#include <chessEngine.hpp>`.

**CMake FetchContent** — для мінімалістів, без пакетного менеджера:

```cmake
include(FetchContent)
FetchContent_Declare(chess-engine
    GIT_REPOSITORY https://github.com/ByteMe6/chess-engine.git
    GIT_TAG v1.0.0)
FetchContent_MakeAvailable(chess-engine)

target_link_libraries(your-app PRIVATE chess-engine::chessEngine)
```

**vcpkg** — у репозиторії є [overlay-порт](ports/chess-engine):

```bash
git clone https://github.com/ByteMe6/chess-engine.git
vcpkg install chess-engine --overlay-ports=chess-engine/ports
```

**Conan** — з клону цього репозиторію:

```bash
conan create .
```

```ini
# далі у conanfile.txt твого проєкту
[requires]
chess-engine/1.0.0
```

<a id="-mozhlyvosti"></a>
## 🧠 Можливості

- ✅ **Генерація ходів для всіх фігур** — король, ферзь, тура, слон, кінь, пішак
- ✅ **Правильні далекобійні фігури** — ферзь/тура/слон ідуть по променю, доки не впруться; взяття завершує промінь, своїх бити не можна
- ✅ **Повна логіка пішака** — хід на одну клітинку, на дві з початкової горизонталі, взяття по діагоналі лише тоді, коли там справді стоїть ворог
- ✅ **Шахова нотація на вхід і вихід** — `makeMove("e2", "e4")` просто працює
- ✅ **Валідація ходів** — нелегальні ходи відхиляються
- ✅ **Черговість ходів** — `currColor` пам'ятає, чий хід; хід не у свою чергу відхиляється
- ✅ **Шах, мат і пат** — ходи, що залишають власного короля під шахом, нелегальні; партія завершується `Game over: White won` або `Game over: draw`
- ✅ **Рокіровка та взяття на проході** — за повними правилами: права відстежуються і втрачаються після ходу короля чи тури, вікно e.p. живе рівно один хід
- ✅ **Правила нічиєї** — правило 50 ходів, триразове повторення позиції та недостатній матеріал завершують партію `Game over: draw`
- ✅ **Перетворення пішака** — пішак на останній горизонталі стає тим, що передаси в `promote()`; грай далі без нього — і за замовчуванням буде ферзь
- ✅ **Імпорт/експорт FEN** — стартуйте з будь-якої позиції через `loadFen()` чи `Board(fen)`, вивантажуйте поточну через `toFen()`; права на рокіровку та клітинка e.p. — справжні поля FEN
- ✅ **ASCII-відображення дошки** — з координатами
- ✅ **Чистий сучасний C++17** — фігури як `enum class`, structured bindings, вичерпний `switch` із захисним `throw`, тільки STL

| Фігура | Літера | Як ходить |
|:------:|:------:|-----------|
| ♔ Король | `K` | на одну клітинку в будь-якому напрямку |
| ♕ Ферзь | `Q` | на будь-яку відстань по вертикалі, горизонталі чи діагоналі |
| ♖ Тура | `R` | на будь-яку відстань по вертикалі чи горизонталі |
| ♗ Слон | `B` | на будь-яку відстань по діагоналі |
| ♘ Кінь | `N` | буквою «Г» — єдина фігура, що перестрибує через інші |
| ♙ Пішак | `P` | на одну вперед (на дві з початкової горизонталі), б'є по діагоналі |

<a id="-vykorystannia"></a>
## 🎮 Використання

```cpp
#include "src/chessEngine.hpp"

// налаштування дошки — повна початкова позиція у src/main.cpp

board.printBoard();               // вивести дошку в консоль

board.makeMove("e2", "e4");       // ходи — звичайна шахова нотація
board.makeMove("e7", "e5");
board.makeMove("g1", "f3");       // нелегальні ходи просто відхиляються

Piece knight = board.board[5][5];         // кінь, що щойно став на f3
board.getPossibleMoves(knight);           // -> e5 (взяття!), g5, d4, h4, g1

// або стартуємо з будь-якої позиції
Board endgame("6k1/5ppp/8/8/8/8/8/R3K3 w - - 0 1");
endgame.makeMove("a1", "a8");             // мат по останній горизонталі -> "Game over: White won"

board.makeMove("a7", "a8");               // пішак дійшов до останньої горизонталі...
board.promote(FigureName::Knight);        // ...обери фігуру або пропусти заради ферзя
```

<a id="-dovidnyk-api"></a>
## 📚 Довідник API

| Метод | Сигнатура | Що робить |
|-------|-----------|-----------|
| `makeMove` | `void makeMove(const std::string& from, const std::string& to)` | Виконує хід у шаховій нотації; спершу перевіряє його легальність |
| `getPossibleMoves` | `std::vector<std::array<int, 2>> getPossibleMoves(const Piece& piece)` | Усі клітинки, куди фігура може легально піти |
| `printBoard` | `void printBoard()` | Малює позицію в ASCII з координатами |
| `translateCoords` | `std::array<int, 2> translateCoords(const std::string& square)` | `"e2"` → `{6, 4}`; кидає виняток на некоректній клітинці |
| `translateToNotation` | `std::string translateToNotation(std::array<int, 2> coords)` | `{6, 4}` → `"e2"` |
| `isInsideBoard` | `bool isInsideBoard(int row, int col)` | Перевірка меж дошки 8×8 |
| `isInCheck` | `bool isInCheck(PieceColor side)` | Чи під боєм король цієї сторони просто зараз |
| `isMate` | `bool isMate(PieceColor side)` | Мат: шах і жодного легального ходу |
| `isStalemate` | `bool isStalemate(PieceColor side)` | Пат: шаху немає, але й легальних ходів немає |
| `promote` | `void promote(FigureName name)` | Обирає фігуру для перетворення; пропусти — і пішак стане ферзем наступним ходом |
| `loadFen` | `void loadFen(const std::string& fen)` | Завантажує позицію з FEN (є і конструктор `Board(fen)`) |
| `toFen` | `std::string toFen()` | Поточна позиція у форматі FEN |

<a id="-yak-tse-pratsiuie"></a>
## 🔍 Як це працює

**Представлення дошки.** Дошка — звичайний `std::vector` 8×8 з об'єктів `Piece`. Рядок 0 — це 8-ма горизонталь (тил чорних), рядок 7 — 1-ша горизонталь (тил білих):

```
board[row][col]
                 col:  0  1  2  3  4  5  6  7
                       a  b  c  d  e  f  g  h
row 0  →  8-ма горизонталь   ┌─ тил чорних
row 7  →  1-ша горизонталь   └─ тил білих

"e2"  →  board[6][4]
```

**Генерація ходів.** Кожен тип фігури має свій case в одному `switch`. Ферзь, тура і слон використовують спільний `addSlidingMoves()`: рушій крокує в кожному напрямку — порожня клітинка продовжує промінь, ворожа фігура береться і завершує його, своя фігура блокує. Коні та королі перевіряють фіксований набір зсувів; пішаки мають власні правила для ходів і взяттів.

**Валідація.** `makeMove` перекладає нотацію, знаходить фігуру і виконує хід лише тоді, коли цільова клітинка є в `getPossibleMoves` — інакше нічого не відбувається.

**Типобезпека.** Фігури — це `enum class`, тож вміст клітинки не сплутати зі звичайним символом, а `switch` по фігурах завершується захисним `throw` для недосяжного випадку.

<a id="-struktura-proiektu"></a>
## 📁 Структура проєкту

```
├── src/
│   ├── chessEngine.hpp   # Board, Piece, генерація ходів — мозок проєкту
│   └── main.cpp          # Демо: коротка партія до мату
├── tests/
│   └── tests.cpp         # тести — запуск через ctest
├── bin/
│   └── chess-engine      # зібраний бінарник (macOS universal)
├── assets/
│   └── board.svg         # діаграма дошки вище
├── ports/
│   └── chess-engine/     # overlay-порт для vcpkg
├── conanfile.py          # пакет Conan
└── CMakeLists.txt        # демо + тести + бібліотека chess-engine::chessEngine
```

<a id="-plany"></a>
## 🗺️ Плани

- [x] Контроль черговості ходів
- [x] Шах і мат
- [x] Пат
- [x] Рокіровка
- [x] Взяття на проході
- [x] Перетворення пішака
- [x] Правила нічиєї (повторення, 50 ходів, недостатній матеріал)
- [x] Імпорт/експорт FEN
- [x] Тести

<a id="-vnesok"></a>
## 🤝 Внесок

Issues та pull requests вітаються — у [планах](#%EF%B8%8F-plany) повно добре окреслених ідей. Правила на дві хвилини читання — у [CONTRIBUTING.md](CONTRIBUTING.md).

```bash
cmake -B build            # налаштування
cmake --build build       # компіляція (має лишатися без попереджень)
ctest --test-dir build    # тести (мають лишатися зеленими)
./build/chess-engine      # запустити демо
```

<a id="-litsenziia"></a>
## 📜 Ліцензія

[GPL-3.0](LICENSE) — вільна, як і має бути.

---

<div align="center">

*Зроблено з ♟️ та C++. Якщо цей репозиторій вас чогось навчив — ⭐ потішить пішаків.*

</div>
