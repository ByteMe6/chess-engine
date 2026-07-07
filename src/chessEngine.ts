export enum FigureName {
    King = "K",
    Queen = "Q",
    Rook = "R",
    Bishop = "B",
    Knight = "N",
    Pawn = "P",
    Empty = " "
}

type PieceColor = "white" | "black" | " ";

interface Figure {
    name: FigureName;
    color: PieceColor;
    coords: [number, number];
}

export class Piece implements Figure {
    constructor(
        public name: FigureName,
        public color: PieceColor,
        public coords: [number, number]
    ) {}
}

export class Board {
    currColor: "white" | "black";
    board: Array<Array<Piece>>;

    constructor(
        currColor: "white" | "black",
        board: Array<Array<Piece>>
    ) {
        this.currColor = currColor;
        this.board = board;
    }

    printBoard() {
        console.log("\n  a b c d e f g h");
        for (let row = 0; row < this.board.length; row++) {
            let line = (8 - row) + " ";
            for (let col = 0; col < this.board[row].length; col++) {
                const piece = this.board[row][col];
                const symbol = piece.color === "white" ? piece.name.toUpperCase() : piece.name.toLowerCase();
                line += symbol + " ";
            }
            console.log(line + (8 - row));
        }
        console.log("  a b c d e f g h\n");
    }

    isInsideBoard(row: number, col: number): boolean {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    // for queen, rook and bishop we walk in each direction until we can't
    addSlidingMoves(piece: Piece, directions: number[][], possibleMoves: [number, number][]) {
        const row = piece.coords[0];
        const col = piece.coords[1];

        for (const [rowStep, colStep] of directions) {
            let newRow = row + rowStep;
            let newCol = col + colStep;

            while (this.isInsideBoard(newRow, newCol)) {
                const targetPiece = this.board[newRow][newCol];

                if (targetPiece.name === FigureName.Empty) {
                    // empty square - can move here, keep going
                    possibleMoves.push([newRow, newCol]);
                } else if (targetPiece.color !== piece.color) {
                    // enemy piece - can capture, but can't go further
                    possibleMoves.push([newRow, newCol]);
                    break;
                } else {
                    // our piece - blocked
                    break;
                }

                newRow += rowStep;
                newCol += colStep;
            }
        }
    }

    getPossibleMoves(piece: Piece): [number, number][] {
        // Coords: 0 - row; 1 - column
        const row = piece.coords[0];
        const col = piece.coords[1];
        const possibleMoves: [number, number][] = [];

        switch (piece.name) {
            case FigureName.King: {
                for (let rowStep = -1; rowStep <= 1; rowStep++) {
                    for (let colStep = -1; colStep <= 1; colStep++) {
                        if (rowStep === 0 && colStep === 0) continue; // skip own square

                        const newRow = row + rowStep;
                        const newCol = col + colStep;

                        if (!this.isInsideBoard(newRow, newCol)) continue;

                        const targetPiece = this.board[newRow][newCol];
                        if (targetPiece.color !== piece.color) {
                            possibleMoves.push([newRow, newCol]);
                        }
                    }
                }
                break;
            }

            case FigureName.Queen: {
                const directions = [
                    [-1, 0], [1, 0], [0, -1], [0, 1], // vertical + horizontal
                    [-1, -1], [-1, 1], [1, -1], [1, 1]// diagonals
                ];
                this.addSlidingMoves(piece, directions, possibleMoves);
                break;
            }

            case FigureName.Rook: {
                const directions = [
                    [-1, 0], [1, 0], [0, -1], [0, 1]
                ];
                this.addSlidingMoves(piece, directions, possibleMoves);
                break;
            }

            case FigureName.Bishop: {
                const directions = [
                    [-1, -1], [-1, 1], [1, -1], [1, 1]
                ];
                this.addSlidingMoves(piece, directions, possibleMoves);
                break;
            }

            case FigureName.Knight: {
                const jumps = [
                    [-2, -1], [-2, 1], [2, -1], [2, 1],
                    [-1, -2], [-1, 2], [1, -2], [1, 2]
                ];

                for (const [rowStep, colStep] of jumps) {
                    const newRow = row + rowStep;
                    const newCol = col + colStep;

                    if (!this.isInsideBoard(newRow, newCol)) continue;

                    const targetPiece = this.board[newRow][newCol];
                    if (targetPiece.color !== piece.color) {
                        possibleMoves.push([newRow, newCol]);
                    }
                }
                break;
            }

            case FigureName.Pawn: {
                const direction = piece.color === "white" ? -1 : 1;
                const startRow = piece.color === "white" ? 6 : 1;

                const oneForward = row + direction;
                if (this.isInsideBoard(oneForward, col) && this.board[oneForward][col].name === FigureName.Empty) {
                    possibleMoves.push([oneForward, col]);

                    // 2 squares forward (only from start row, both squares empty)
                    const twoForward = row + direction * 2;
                    if (row === startRow && this.board[twoForward][col].name === FigureName.Empty) {
                        possibleMoves.push([twoForward, col]);
                    }
                }

                // diagonal captures (only if enemy piece is there)
                for (const colStep of [-1, 1]) {
                    const newRow = row + direction;
                    const newCol = col + colStep;

                    if (!this.isInsideBoard(newRow, newCol)) continue;

                    const targetPiece = this.board[newRow][newCol];
                    if (targetPiece.name !== FigureName.Empty && targetPiece.color !== piece.color) {
                        possibleMoves.push([newRow, newCol]);
                    }
                }
                break;
            }

            case FigureName.Empty:
                break;

            default: {
                const _exhaustiveCheck: never = piece.name;
                throw new Error(`Unhandled piece: ${_exhaustiveCheck}`);
            }
        }

        return possibleMoves;
    }

    // "e2" -> [6, 4], "a8" -> [0, 0], "h1" -> [7, 7]
    translateCoords(square: string): [number, number] {
        const col = square.charCodeAt(0) - "a".charCodeAt(0); // 'a' -> 0, 'h' -> 7
        const row = 8 - Number(square[1]);                    // '8' -> 0, '1' -> 7

        if (!this.isInsideBoard(row, col)) {
            throw new Error(`Invalid square: ${square}`);
        }

        return [row, col];
    }

    // [6, 4] -> "e2"
    translateToNotation(coords: [number, number]): string {
        const letter = String.fromCharCode("a".charCodeAt(0) + coords[1]);
        const number = 8 - coords[0];
        return letter + number;
    }

    doAMove(piece: Piece, finalCoords: [number, number]) {
        const possibleMoves = this.getPossibleMoves(piece);

        let isMovePossible: boolean = false;

        for (let i: number = 0; i < possibleMoves.length; i++) {
            if (possibleMoves[i][0] === finalCoords[0] && possibleMoves[i][1] === finalCoords[1]) {
                isMovePossible = true;
                break;
            }
        }

        if (isMovePossible) {
            this.board[piece.coords[0]][piece.coords[1]] = new Piece(FigureName.Empty, " ", [piece.coords[0], piece.coords[1]]);
            this.board[finalCoords[0]][finalCoords[1]] = piece;
            piece.coords = finalCoords;
        }
    }

    // takes a move in chess notation like ("e2", "e4") and applies it on the board
    makeMove(from: string, to: string) {
        const fromCoords = this.translateCoords(from);
        const toCoords = this.translateCoords(to);

        const piece = this.board[fromCoords[0]][fromCoords[1]];

        if (piece.name === FigureName.Empty) {
            console.log(`No piece on ${from}`);
            return;
        }

        this.doAMove(piece, toCoords);
    }
}