<div align="center">

# ♟️ Шахматный движок

**Минималистичный шахматный движок на TypeScript — с нуля, без единой runtime-зависимости.**

[![TypeScript](https://img.shields.io/badge/TypeScript-3178C6?logo=typescript&logoColor=white)](https://www.typescriptlang.org/)
[![Runtime deps: zero](https://img.shields.io/badge/runtime%20deps-zero-brightgreen)](package.json)
[![Code size](https://img.shields.io/github/languages/code-size/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine)
[![Last commit](https://img.shields.io/github/last-commit/ByteMe6/chess-engine)](https://github.com/ByteMe6/chess-engine/commits)
[![License: GPL-3.0](https://img.shields.io/badge/license-GPL--3.0-green)](LICENSE)
[![PRs welcome](https://img.shields.io/badge/PRs-welcome-blueviolet)](CONTRIBUTING.md)

[English](README.md) · [Українська](README.uk.md) · **Русский**

<img src="assets/board.svg" width="440" alt="Шахматная доска после 1. e4 e5 2. Nf3 с подсвеченным ходом коня">

*Позиция после `1. e4 e5 2. Nf3` — именно её разыгрывает встроенное демо.*

</div>

---

## 📖 Содержание

- [Что это?](#-chto-eto)
- [Быстрый старт](#-bystryj-start)
- [Возможности](#-vozmozhnosti)
- [Использование](#-ispolzovanie)
- [Справочник API](#-spravochnik-api)
- [Как это работает](#-kak-eto-rabotaet)
- [Структура проекта](#-struktura-proekta)
- [Планы](#%EF%B8%8F-plany)
- [Вклад](#-vklad)
- [Лицензия](#-litsenziya)

<a id="-chto-eto"></a>
## ✨ Что это?

Небольшой и читаемый шахматный движок без единой библиотеки под капотом — каждое правило написано вручную. Он знает, как ходят фигуры, проверяет легальность ходов, понимает шахматную нотацию и рисует доску прямо в терминале.

Это в первую очередь учебный проект: код написан так, чтобы его было приятно читать. Если вам когда-нибудь было интересно, как шахматные программы генерируют ходы, — `src/chessEngine.ts` отвечает на это за ~250 строк.

<a id="-bystryj-start"></a>
## 🚀 Быстрый старт

```bash
git clone git@github.com:ByteMe6/chess-engine.git
cd chess-engine
npm install
npm start
```

Вы увидите, как разыгрывается дебют `1. e4 e5 2. Nf3`, доска за доской:

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

Заглавные буквы — белые, строчные — чёрные. Олдскульный FEN-вайб.

<a id="-vozmozhnosti"></a>
## 🧠 Возможности

- ✅ **Генерация ходов для всех фигур** — король, ферзь, ладья, слон, конь, пешка
- ✅ **Правильные дальнобойные фигуры** — ферзь/ладья/слон идут по лучу, пока не упрутся; взятие завершает луч, своих бить нельзя
- ✅ **Полная логика пешки** — ход на одну клетку, на две с начальной горизонтали, взятие по диагонали только когда там действительно стоит враг
- ✅ **Шахматная нотация на вход и выход** — `makeMove("e2", "e4")` просто работает
- ✅ **Валидация ходов** — нелегальные ходы отклоняются
- ✅ **ASCII-отрисовка доски** — с координатами
- ✅ **Строгий TypeScript** — проходит `tsc --strict`, с исчерпывающей `never`-проверкой типов фигур

| Фигура | Буква | Как ходит |
|:------:|:-----:|-----------|
| ♔ Король | `K` | на одну клетку в любом направлении |
| ♕ Ферзь | `Q` | на любое расстояние по вертикали, горизонтали или диагонали |
| ♖ Ладья | `R` | на любое расстояние по вертикали или горизонтали |
| ♗ Слон | `B` | на любое расстояние по диагонали |
| ♘ Конь | `N` | буквой «Г» — единственная фигура, перепрыгивающая через другие |
| ♙ Пешка | `P` | на одну вперёд (на две с начальной горизонтали), бьёт по диагонали |

<a id="-ispolzovanie"></a>
## 🎮 Использование

```ts
import { Board, Piece, FigureName } from "./src/chessEngine";

// настройка доски — полная начальная позиция в src/index.ts

board.printBoard();               // вывести доску в консоль

board.makeMove("e2", "e4");       // ходы — обычная шахматная нотация
board.makeMove("e7", "e5");
board.makeMove("g1", "f3");       // нелегальные ходы просто отклоняются

const knight = board.board[5][5];         // конь, только что вставший на f3
board.getPossibleMoves(knight);           // -> e5 (взятие!), g5, d4, h4, g1
```

<a id="-spravochnik-api"></a>
## 📚 Справочник API

| Метод | Сигнатура | Что делает |
|-------|-----------|------------|
| `makeMove` | `(from: string, to: string) => void` | Выполняет ход в шахматной нотации; сначала проверяет его легальность |
| `getPossibleMoves` | `(piece: Piece) => [number, number][]` | Все клетки, куда фигура может легально пойти |
| `printBoard` | `() => void` | Рисует позицию в ASCII с координатами |
| `translateCoords` | `(square: string) => [number, number]` | `"e2"` → `[6, 4]`; бросает ошибку на некорректной клетке |
| `translateToNotation` | `(coords: [number, number]) => string` | `[6, 4]` → `"e2"` |
| `isInsideBoard` | `(row: number, col: number) => boolean` | Проверка границ доски 8×8 |

<a id="-kak-eto-rabotaet"></a>
## 🔍 Как это работает

**Представление доски.** Доска — обычный массив 8×8 из объектов `Piece`. Строка 0 — это 8-я горизонталь (тыл чёрных), строка 7 — 1-я горизонталь (тыл белых):

```
board[row][col]
                 col:  0  1  2  3  4  5  6  7
                       a  b  c  d  e  f  g  h
row 0  →  8-я горизонталь   ┌─ тыл чёрных
row 7  →  1-я горизонталь   └─ тыл белых

"e2"  →  board[6][4]
```

**Генерация ходов.** У каждого типа фигуры свой case в одном `switch`. Ферзь, ладья и слон используют общий `addSlidingMoves()`: движок шагает в каждом направлении — пустая клетка продолжает луч, вражеская фигура берётся и завершает его, своя фигура блокирует. Кони и короли проверяют фиксированный набор смещений; у пешек свои правила для ходов и взятий.

**Валидация.** `makeMove` переводит нотацию, находит фигуру и выполняет ход только если целевая клетка есть в `getPossibleMoves` — иначе ничего не происходит.

**Типобезопасность.** `switch` по фигурам завершается исчерпывающей проверкой с типом `never`: добавить новый тип фигуры и забыть его обработать — ошибка компиляции.

<a id="-struktura-proekta"></a>
## 📁 Структура проекта

```
├── src/
│   ├── chessEngine.ts   # Board, Piece, генерация ходов — мозг проекта
│   └── index.ts         # Демо: создаёт доску и играет 1. e4 e5 2. Nf3
├── assets/
│   └── board.svg        # диаграмма доски выше
├── tsconfig.json        # строгий режим включён
└── package.json
```

<a id="-plany"></a>
## 🗺️ Планы

- [ ] Контроль очерёдности ходов
- [ ] Шах и мат
- [ ] Рокировка
- [ ] Взятие на проходе
- [ ] Превращение пешки
- [ ] Пат и правила ничьей
- [ ] Импорт/экспорт FEN
- [ ] Тесты

<a id="-vklad"></a>
## 🤝 Вклад

Issues и pull requests приветствуются — в [планах](#%EF%B8%8F-plany) полно хорошо очерченных идей. Правила на две минуты чтения — в [CONTRIBUTING.md](CONTRIBUTING.md).

```bash
npm install          # настройка
npm start            # запустить демо
npx tsc --noEmit     # проверка типов
```

<a id="-litsenziya"></a>
## 📜 Лицензия

[GPL-3.0](LICENSE) — свободная, как и положено.

---

<div align="center">

*Сделано с ♟️ и TypeScript. Если этот репозиторий вас чему-то научил — ⭐ порадует пешек.*

</div>
