# Contributing

Thanks for stopping by! This is a small learning project, so contributing is deliberately simple.

## Setup

```bash
git clone git@github.com:ByteMe6/chess-engine.git
cd chess-engine
cmake -B build
cmake --build build       # must compile clean
./build/chess-engine      # run the demo
```

Any C++17 compiler works (clang, gcc); CMake ≥ 3.16.

## What to work on

The [roadmap in the README](README.md#%EF%B8%8F-roadmap) is the backlog — castling, en passant, promotion, check detection are all up for grabs. For anything bigger than a roadmap item, open an issue first so we can talk it over.

## Ground rules

- **STL only.** The whole point is that every rule is hand-rolled — please don't add chess libraries or external dependencies.
- **Readable beats clever.** This code is meant to be read; a clear `for` loop wins over a smart one-liner.
- **Keep the switch exhaustive.** When touching the piece `switch`, every `FigureName` gets its own case and the `default` stays a defensive `throw`.
- **Comment the non-obvious.** Board math (row/col ↔ notation) deserves a comment; the rest usually doesn't.

## Submitting

1. Fork, create a branch, make the change.
2. Make sure the build is clean and `./build/chess-engine` still plays its little opening.
3. Open a PR with a short description of *why*, not just *what*.

That's it. ♟️
