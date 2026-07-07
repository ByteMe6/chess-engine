<div align="center">

# ♟️ Шаховий рушій

**Мінімалістичний шаховий рушій на TypeScript — з нуля, без жодної runtime-залежності.**

[![TypeScript](https://img.shields.io/badge/TypeScript-3178C6?logo=typescript&logoColor=white)](https://www.typescriptlang.org/)
[![Runtime deps: zero](https://img.shields.io/badge/runtime%20deps-zero-brightgreen)](package.json)
[![Code size](https://img.shields.io/github/languages/code-size/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine)
[![Last commit](https://img.shields.io/github/last-commit/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine/commits)
[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-green)](LICENSE)
[![PRs welcome](https://img.shields.io/badge/PRs-welcome-blueviolet)](CONTRIBUTING.md)

[English](README.md) · **Українська** · [Русский](README.ru.md)

<img src="assets/board.svg" width="440" alt="Шахівниця після 1. e4 e5 2. Nf3 з підсвіченим ходом коня">

*Позиція після `1. e4 e5 2. Nf3` — саме її розігрує вбудоване демо.*

</div>

---

## 📖 Зміст

- [Що це?](#-shcho-tse)
- [Швидкий старт](#-shvydkyi-start)
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

Невеликий і читабельний шаховий рушій без жодної бібліотеки під капотом — кожне правило написане вручну. Він знає, як ходять фігури, перевіряє легальність ходів, розуміє шахову нотацію та малює дошку прямо в терміналі.

Це насамперед навчальний проєкт: код написано так, щоб його було приємно читати. Якщо вам колись було цікаво, як шахові програми генерують ходи, — `src/chessEngine.ts` відповідає на це за ~250 рядків.

<a id="-shvydkyi-start"></a>
## 🚀 Швидкий старт

```bash
git clone git@github.com:ByteMe6/chess-engine.git
cd chess-engine
npm install
npm start
```

Ви побачите, як розігрується дебют `1. e4 e5 2. Nf3`, дошка за дошкою:

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

Великі літери — білі, малі — чорні. Олдскульний FEN-вайб.

<a id="-mozhlyvosti"></a>
## 🧠 Можливості

- ✅ **Генерація ходів для всіх фігур** — король, ферзь, тура, слон, кінь, пішак
- ✅ **Правильні далекобійні фігури** — ферзь/тура/слон ідуть по променю, доки не впруться; взяття завершує промінь, своїх бити не можна
- ✅ **Повна логіка пішака** — хід на одну клітинку, на дві з початкової горизонталі, взяття по діагоналі лише тоді, коли там справді стоїть ворог
- ✅ **Шахова нотація на вхід і вихід** — `makeMove("e2", "e4")` просто працює
- ✅ **Валідація ходів** — нелегальні ходи відхиляються
- ✅ **ASCII-відображення дошки** — з координатами
- ✅ **Строгий TypeScript** — проходить `tsc --strict`, з вичерпною `never`-перевіркою типів фігур

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

```ts
import { Board, Piece, FigureName } from "./src/chessEngine";

// налаштування дошки — повна початкова позиція у src/index.ts

board.printBoard();               // вивести дошку в консоль

board.makeMove("e2", "e4");       // ходи — звичайна шахова нотація
board.makeMove("e7", "e5");
board.makeMove("g1", "f3");       // нелегальні ходи просто відхиляються

const knight = board.board[5][5];         // кінь, що щойно став на f3
board.getPossibleMoves(knight);           // -> e5 (взяття!), g5, d4, h4, g1
```

<a id="-dovidnyk-api"></a>
## 📚 Довідник API

| Метод | Сигнатура | Що робить |
|-------|-----------|-----------|
| `makeMove` | `(from: string, to: string) => void` | Виконує хід у шаховій нотації; спершу перевіряє його легальність |
| `getPossibleMoves` | `(piece: Piece) => [number, number][]` | Усі клітинки, куди фігура може легально піти |
| `printBoard` | `() => void` | Малює позицію в ASCII з координатами |
| `translateCoords` | `(square: string) => [number, number]` | `"e2"` → `[6, 4]`; кидає помилку на некоректній клітинці |
| `translateToNotation` | `(coords: [number, number]) => string` | `[6, 4]` → `"e2"` |
| `isInsideBoard` | `(row: number, col: number) => boolean` | Перевірка меж дошки 8×8 |

<a id="-yak-tse-pratsiuie"></a>
## 🔍 Як це працює

**Представлення дошки.** Дошка — звичайний масив 8×8 з об'єктів `Piece`. Рядок 0 — це 8-ма горизонталь (тил чорних), рядок 7 — 1-ша горизонталь (тил білих):

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

**Типобезпека.** `switch` по фігурах завершується вичерпною перевіркою з типом `never`: додати новий тип фігури й забути його обробити — помилка компіляції.

<a id="-struktura-proiektu"></a>
## 📁 Структура проєкту

```
├── src/
│   ├── chessEngine.ts   # Board, Piece, генерація ходів — мозок проєкту
│   └── index.ts         # Демо: створює дошку і грає 1. e4 e5 2. Nf3
├── assets/
│   └── board.svg        # діаграма дошки вище
├── tsconfig.json        # строгий режим увімкнено
└── package.json
```

<a id="-plany"></a>
## 🗺️ Плани

- [ ] Контроль черговості ходів
- [ ] Шах і мат
- [ ] Рокіровка
- [ ] Взяття на проході
- [ ] Перетворення пішака
- [ ] Пат і правила нічиєї
- [ ] Імпорт/експорт FEN
- [ ] Тести

<a id="-vnesok"></a>
## 🤝 Внесок

Issues та pull requests вітаються — у [планах](#%EF%B8%8F-plany) повно добре окреслених ідей. Правила на дві хвилини читання — у [CONTRIBUTING.md](CONTRIBUTING.md).

```bash
npm install          # налаштування
npm start            # запустити демо
npx tsc --noEmit     # перевірка типів
```

<a id="-litsenziia"></a>
## 📜 Ліцензія

[GPL-3.0](LICENSE) — вільна, як і має бути.

---

<div align="center">

*Зроблено з ♟️ та TypeScript. Якщо цей репозиторій вас чогось навчив — ⭐ потішить пішаків.*

</div>
