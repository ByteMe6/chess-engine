<div align="center">

# ♟️ Chess Engine

**A minimal chess engine written in TypeScript — from scratch, zero runtime dependencies.**

[![TypeScript](https://img.shields.io/badge/TypeScript-3178C6?logo=typescript&logoColor=white)](https://www.typescriptlang.org/)
[![Runtime deps: zero](https://img.shields.io/badge/runtime%20deps-zero-brightgreen)](package.json)
[![Code size](https://img.shields.io/github/languages/code-size/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine)
[![Last commit](https://img.shields.io/github/last-commit/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine/commits)
[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-green)](LICENSE)
[![PRs welcome](https://img.shields.io/badge/PRs-welcome-blueviolet)](CONTRIBUTING.md)

**English** · [Українська](README.uk.md) · [Русский](README.ru.md)

<img src="assets/board.svg" width="440" alt="Chessboard after 1. e4 e5 2. Nf3, with the knight move highlighted">

*Position after `1. e4 e5 2. Nf3` — exactly what the built-in demo plays.*

</div>

---

## 📖 Table of contents

- [What is this?](#-what-is-this)
- [Quick start](#-quick-start)
- [Features](#-features)
- [Usage](#-usage)
- [API reference](#-api-reference)
- [How it works](#-how-it-works)
- [Project structure](#-project-structure)
- [Roadmap](#%EF%B8%8F-roadmap)
- [Contributing](#-contributing)
- [License](#-license)

## ✨ What is this?

A small, readable chess engine with no libraries under the hood — every rule is hand-rolled. It knows how the pieces move, validates moves, understands algebraic notation, and renders the board right in your terminal.

It's a learning project first: the code is written to be read. If you ever wondered how chess programs generate moves, `src/chessEngine.ts` is a ~250-line answer.

## 🚀 Quick start

```bash
git clone git@github.com:ByteMe6/chess-engine.git
cd chess-engine
npm install
npm start
```

You'll see the opening `1. e4 e5 2. Nf3` played out, board by board:

```console
$ npm start

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
...
  a b c d e f g h
8 r n b q k b n r 8
7 p p p p   p p p 7
6                 6
5         p       5
4         P       4
3           N     3
2 P P P P   P P P 2
1 R N B Q K B   R 1
  a b c d e f g h
```

Uppercase = white, lowercase = black. Old-school FEN vibes.

## 🧠 Features

- ✅ **Move generation for every piece** — king, queen, rook, bishop, knight, pawn
- ✅ **Sliding pieces done right** — queen/rook/bishop walk each ray until blocked; captures end the ray, friendly fire is not allowed
- ✅ **Full pawn logic** — single push, double push from the start rank, diagonal captures only when an enemy is actually there
- ✅ **Algebraic notation in and out** — `makeMove("e2", "e4")` just works
- ✅ **Move validation** — illegal moves are rejected
- ✅ **ASCII board rendering** — with file/rank coordinates
- ✅ **Strict TypeScript** — passes `tsc --strict`, with an exhaustive `never`-check on piece types

| Piece | Letter | Movement |
|:-----:|:------:|----------|
| ♔ King | `K` | one square in any direction |
| ♕ Queen | `Q` | any distance along rank, file or diagonal |
| ♖ Rook | `R` | any distance along rank or file |
| ♗ Bishop | `B` | any distance along a diagonal |
| ♘ Knight | `N` | L-shape jump — the only piece that jumps over others |
| ♙ Pawn | `P` | one forward (two from the start rank), captures diagonally |

## 🎮 Usage

```ts
import { Board, Piece, FigureName } from "./src/chessEngine";

// board setup — see src/index.ts for the full starting position

board.printBoard();               // render the board to the console

board.makeMove("e2", "e4");       // moves are plain algebraic notation
board.makeMove("e7", "e5");
board.makeMove("g1", "f3");       // illegal moves are simply rejected

const knight = board.board[5][5];         // the knight that just landed on f3
board.getPossibleMoves(knight);           // -> e5 (capture!), g5, d4, h4, g1
```

## 📚 API reference

| Method | Signature | What it does |
|--------|-----------|--------------|
| `makeMove` | `(from: string, to: string) => void` | Applies a move given in algebraic notation; validates it first |
| `getPossibleMoves` | `(piece: Piece) => [number, number][]` | Every square the piece can legally move to |
| `printBoard` | `() => void` | Renders the position as ASCII with coordinates |
| `translateCoords` | `(square: string) => [number, number]` | `"e2"` → `[6, 4]`; throws on an invalid square |
| `translateToNotation` | `(coords: [number, number]) => string` | `[6, 4]` → `"e2"` |
| `isInsideBoard` | `(row: number, col: number) => boolean` | Bounds check for the 8×8 board |

## 🔍 How it works

**Board representation.** The board is a plain 8×8 array of `Piece` objects. Row 0 is rank 8 (black's back rank), row 7 is rank 1 (white's back rank):

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

**Type safety.** The piece `switch` ends with a `never`-typed exhaustiveness check, so adding a new piece type without handling it becomes a compile error.

## 📁 Project structure

```
├── src/
│   ├── chessEngine.ts   # Board, Piece, move generation — the brains
│   └── index.ts         # Demo: set up a board, play 1. e4 e5 2. Nf3
├── assets/
│   └── board.svg        # the board diagram above
├── tsconfig.json        # strict mode on
└── package.json
```

## 🗺️ Roadmap

- [ ] Turn order enforcement
- [ ] Check & checkmate detection
- [ ] Castling
- [ ] En passant
- [ ] Pawn promotion
- [ ] Stalemate & draw rules
- [ ] FEN import/export
- [ ] Test suite

## 🤝 Contributing

Issues and pull requests are welcome — the [roadmap](#%EF%B8%8F-roadmap) is full of well-scoped ideas. See [CONTRIBUTING.md](CONTRIBUTING.md) for the two-minute version of the rules.

```bash
npm install          # setup
npm start            # run the demo
npx tsc --noEmit     # type check
```

## 📜 License

[GPL-3.0](LICENSE) — free as in freedom.

---

<div align="center">

*Made with ♟️ and TypeScript. If this repo taught you something, a ⭐ makes the pawns happy.*

</div>
