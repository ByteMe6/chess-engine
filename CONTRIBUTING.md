# Contributing

Thanks for stopping by! This is a small learning project, so contributing is deliberately simple.

## Setup

```bash
git clone git@github.com:ByteMe6/chess-engine.git
cd chess-engine
npm install
npm start            # run the demo
npx tsc --noEmit     # type check (must stay green)
```

## What to work on

The [roadmap in the README](README.md#%EF%B8%8F-roadmap) is the backlog — castling, en passant, promotion, check detection are all up for grabs. For anything bigger than a roadmap item, open an issue first so we can talk it over.

## Ground rules

- **Zero runtime dependencies.** The whole point is that every rule is hand-rolled — please don't add chess libraries.
- **Readable beats clever.** This code is meant to be read; a clear `for` loop wins over a smart one-liner.
- **Strict TypeScript.** `npx tsc --noEmit` must pass. Keep the exhaustive `never`-check pattern when touching the piece `switch`.
- **Comment the non-obvious.** Board math (row/col ↔ notation) deserves a comment; the rest usually doesn't.

## Submitting

1. Fork, create a branch, make the change.
2. Make sure `npm start` still plays its little opening and the types are green.
3. Open a PR with a short description of *why*, not just *what*.

That's it. ♟️
