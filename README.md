<div align="center">

# ♟️ Chess Engine

**A minimal chess engine written in C++ — from scratch, no dependencies beyond the STL.**

[![C++17](https://img.shields.io/badge/C%2B%2B17-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=white)](CMakeLists.txt)
[![Release](https://img.shields.io/github/v/release/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine/releases/latest)
[![Tests](https://img.shields.io/github/actions/workflow/status/ByteMe6/chess-engine/test.yml?label=tests)](https://github.com/ByteMe6/chess-engine/actions/workflows/test.yml)
[![Dependencies: STL only](https://img.shields.io/badge/dependencies-STL%20only-brightgreen)](src/chessEngine.hpp)
[![Code size](https://img.shields.io/github/languages/code-size/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine)
[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-green)](LICENSE)
[![PRs welcome](https://img.shields.io/badge/PRs-welcome-blueviolet)](CONTRIBUTING.md)

**English** · [Українська](README.uk.md) · [Русский](README.ru.md)

<img src="assets/board.svg" width="440" alt="Final position of the demo game — the white queen has just delivered mate on d7">

*Final position of the built-in demo — the white queen has just delivered mate on d7.*

</div>

---

## 📖 Table of contents

- [What is this?](#-what-is-this)
- [Quick start](#-quick-start)
- [Use as a library](#-use-as-a-library)
- [Features](#-features)
- [Usage](#-usage)
- [API reference](#-api-reference)
- [How it works](#-how-it-works)
- [Project structure](#-project-structure)
- [Roadmap](#%EF%B8%8F-roadmap)
- [Contributing](#-contributing)
- [License](#-license)

## ✨ What is this?

A small, readable chess engine with no libraries under the hood — every rule is hand-rolled on top of the C++ standard library. It knows how the pieces move, validates moves, understands algebraic notation, and renders the board right in your terminal.

It's a learning project first: the code is written to be read. If you ever wondered how chess programs generate moves, `src/chessEngine.hpp` is a ~250-line answer.

## 🚀 Quick start

**Prebuilt binaries** — grab one from the [releases page](https://github.com/ByteMe6/chess-engine/releases/latest): Linux (x86_64 / arm64), Windows (x86_64 / arm64), macOS (universal). The repo also ships one at `bin/chess-engine` (macOS universal — Apple Silicon & Intel):

```bash
git clone git@github.com:ByteMe6/chess-engine.git
cd chess-engine
./bin/chess-engine
```

**Build from source** (any platform with CMake ≥ 3.16 and a C++17 compiler):

```bash
cmake -B build
cmake --build build
./build/chess-engine
```

The demo plays a short game that ends in checkmate — watch the last lines:

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

Uppercase = white, lowercase = black. Old-school FEN vibes.

## 📦 Use as a library

The whole engine is a single header. Pick your flavor, then just `#include <chessEngine.hpp>`.

**CMake FetchContent** — for minimalists, no package manager at all:

```cmake
include(FetchContent)
FetchContent_Declare(chess-engine
    GIT_REPOSITORY https://github.com/ByteMe6/chess-engine.git
    GIT_TAG v1.0.0)
FetchContent_MakeAvailable(chess-engine)

target_link_libraries(your-app PRIVATE chess-engine::chessEngine)
```

**vcpkg** — the repo ships an [overlay port](ports/chess-engine):

```bash
git clone https://github.com/ByteMe6/chess-engine.git
vcpkg install chess-engine --overlay-ports=chess-engine/ports
```

**Conan** — from a clone of this repo:

```bash
conan create .
```

```ini
# then in your project's conanfile.txt
[requires]
chess-engine/1.0.0
```

## 🧠 Features

- ✅ **Move generation for every piece** — king, queen, rook, bishop, knight, pawn
- ✅ **Sliding pieces done right** — queen/rook/bishop walk each ray until blocked; captures end the ray, friendly fire is not allowed
- ✅ **Full pawn logic** — single push, double push from the start rank, diagonal captures only when an enemy is actually there
- ✅ **Algebraic notation in and out** — `makeMove("e2", "e4")` just works
- ✅ **Move validation** — illegal moves are rejected
- ✅ **Turn order** — `currColor` tracks whose move it is; moving out of turn is rejected
- ✅ **Check, checkmate & stalemate** — moves that leave your own king in check are illegal; the game ends with `Game over: White won` or `Game over: draw`
- ✅ **Castling & en passant** — full rules: rights are tracked and lost when the king or a rook moves, the e.p. window lasts exactly one move
- ✅ **Draw rules** — the fifty-move rule, threefold repetition and insufficient material all end the game with `Game over: draw`
- ✅ **Pawn promotion** — a pawn on the last rank becomes whatever you pass to `promote()`; keep playing without it and it's a queen by default
- ✅ **FEN import/export** — start from any custom position with `loadFen()` or `Board(fen)`, dump the current one with `toFen()`; castling rights and the e.p. square are real FEN fields
- ✅ **ASCII board rendering** — with file/rank coordinates
- ✅ **Clean modern C++17** — `enum class` pieces, structured bindings, an exhaustive `switch` with a defensive `throw`, STL only

| Piece | Letter | Movement |
|:-----:|:------:|----------|
| ♔ King | `K` | one square in any direction |
| ♕ Queen | `Q` | any distance along rank, file or diagonal |
| ♖ Rook | `R` | any distance along rank or file |
| ♗ Bishop | `B` | any distance along a diagonal |
| ♘ Knight | `N` | L-shape jump — the only piece that jumps over others |
| ♙ Pawn | `P` | one forward (two from the start rank), captures diagonally |

## 🎮 Usage

```cpp
#include "src/chessEngine.hpp"

// board setup — see src/main.cpp for the full starting position

board.printBoard();               // render the board to the console

board.makeMove("e2", "e4");       // moves are plain algebraic notation
board.makeMove("e7", "e5");
board.makeMove("g1", "f3");       // illegal moves are simply rejected

Piece knight = board.board[5][5];         // the knight that just landed on f3
board.getPossibleMoves(knight);           // -> e5 (capture!), g5, d4, h4, g1

// or start from any custom position
Board endgame("6k1/5ppp/8/8/8/8/8/R3K3 w - - 0 1");
endgame.makeMove("a1", "a8");             // back rank mate -> "Game over: White won"

board.makeMove("a7", "a8");               // pawn reaches the last rank...
board.promote(FigureName::Knight);        // ...pick a piece, or skip for a queen
```

## 📚 API reference

| Method | Signature | What it does |
|--------|-----------|--------------|
| `makeMove` | `void makeMove(const std::string& from, const std::string& to)` | Applies a move given in algebraic notation; validates it first |
| `getPossibleMoves` | `std::vector<std::array<int, 2>> getPossibleMoves(const Piece& piece)` | Every square the piece can legally move to |
| `printBoard` | `void printBoard()` | Renders the position as ASCII with coordinates |
| `translateCoords` | `std::array<int, 2> translateCoords(const std::string& square)` | `"e2"` → `{6, 4}`; throws on an invalid square |
| `translateToNotation` | `std::string translateToNotation(std::array<int, 2> coords)` | `{6, 4}` → `"e2"` |
| `isInsideBoard` | `bool isInsideBoard(int row, int col)` | Bounds check for the 8×8 board |
| `isInCheck` | `bool isInCheck(PieceColor side)` | Is this side's king under attack right now |
| `isMate` | `bool isMate(PieceColor side)` | Checkmate: in check and no legal move left |
| `isStalemate` | `bool isStalemate(PieceColor side)` | Stalemate: not in check, but no legal move either |
| `promote` | `void promote(FigureName name)` | Picks the piece for a pending promotion; skip it and the pawn queens on the next move |
| `loadFen` | `void loadFen(const std::string& fen)` | Loads a position from FEN (also available as the `Board(fen)` constructor) |
| `toFen` | `std::string toFen()` | The current position as FEN |

## 🔍 How it works

**Board representation.** The board is a plain 8×8 `std::vector` of `Piece` objects. Row 0 is rank 8 (black's back rank), row 7 is rank 1 (white's back rank):

```
board[row][col]
                 col:  0  1  2  3  4  5  6  7
                       a  b  c  d  e  f  g  h
row 0  →  rank 8   ┌─ black's back rank
row 7  →  rank 1   └─ white's back rank

"e2"  →  board[6][4]
```

**Move generation.** Each piece type gets its own case in one `switch`. The queen, rook and bishop share `addSlidingMoves()`: walk in each direction step by step — an empty square keeps the ray going, an enemy piece is captured and ends it, a friendly piece blocks it. Knights and kings check a fixed set of offsets; pawns get their own rules for pushes and diagonal captures.

**Validation.** `makeMove` translates the notation, finds the piece, and only applies the move if the target square is in `getPossibleMoves` — otherwise nothing happens.

**Type safety.** Pieces are an `enum class`, so square contents can't be confused with plain chars, and the piece `switch` ends with a defensive `throw` for the unreachable case.

## 📁 Project structure

```
├── src/
│   ├── chessEngine.hpp   # Board, Piece, move generation — the brains
│   └── main.cpp          # Demo: a short game that ends in checkmate
├── tests/
│   └── tests.cpp         # the test suite — run with ctest
├── bin/
│   └── chess-engine      # prebuilt binary (macOS universal)
├── assets/
│   └── board.svg         # the board diagram above
├── ports/
│   └── chess-engine/     # vcpkg overlay port
├── conanfile.py          # Conan package
└── CMakeLists.txt        # demo + tests + chess-engine::chessEngine library target
```

## 🗺️ Roadmap

- [x] Turn order enforcement
- [x] Check & checkmate detection
- [x] Stalemate
- [x] Castling
- [x] En passant
- [x] Pawn promotion
- [x] Draw rules (repetition, fifty-move, insufficient material)
- [x] FEN import/export
- [x] Test suite

## 🤝 Contributing

Issues and pull requests are welcome — the [roadmap](#%EF%B8%8F-roadmap) is full of well-scoped ideas. See [CONTRIBUTING.md](CONTRIBUTING.md) for the two-minute version of the rules.

```bash
cmake -B build            # setup
cmake --build build       # compile (must stay warning-clean)
ctest --test-dir build    # run the tests (must stay green)
./build/chess-engine      # run the demo
```

## 📜 License

[GPL-3.0](LICENSE) — free as in freedom.

---

<div align="center">

*Made with ♟️ and C++. If this repo taught you something, a ⭐ makes the pawns happy.*

</div>
