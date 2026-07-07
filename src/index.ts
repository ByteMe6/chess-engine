import { Board, Piece, FigureName } from "./chessEngine";

const board = new Board("white", Array(8).fill(Array(8).fill(null)));

board.board = [
    [new Piece(FigureName.Rook, "black", [0, 0]), new Piece(FigureName.Knight, "black", [0, 1]), new Piece(FigureName.Bishop, "black", [0, 2]), new Piece(FigureName.Queen, "black", [0, 3]), new Piece(FigureName.King, "black", [0, 4]), new Piece(FigureName.Bishop, "black", [0, 5]), new Piece(FigureName.Knight, "black", [0, 6]), new Piece(FigureName.Rook, "black", [0, 7])],
    [new Piece(FigureName.Pawn, "black", [1, 0]), new Piece(FigureName.Pawn, "black", [1, 1]), new Piece(FigureName.Pawn, "black", [1, 2]), new Piece(FigureName.Pawn, "black", [1, 3]), new Piece(FigureName.Pawn, "black", [1, 4]), new Piece(FigureName.Pawn, "black", [1, 5]), new Piece(FigureName.Pawn, "black", [1, 6]), new Piece(FigureName.Pawn, "black", [1, 7])],
    [new Piece(FigureName.Empty, " ", [2, 0]), new Piece(FigureName.Empty, " ", [2, 1]), new Piece(FigureName.Empty, " ", [2, 2]), new Piece(FigureName.Empty, " ", [2, 3]), new Piece(FigureName.Empty, " ", [2, 4]), new Piece(FigureName.Empty, " ", [2, 5]), new Piece(FigureName.Empty, " ", [2, 6]), new Piece(FigureName.Empty, " ", [2, 7])],
    [new Piece(FigureName.Empty, " ", [3, 0]), new Piece(FigureName.Empty, " ", [3, 1]), new Piece(FigureName.Empty, " ", [3, 2]), new Piece(FigureName.Empty, " ", [3, 3]), new Piece(FigureName.Empty, " ", [3, 4]), new Piece(FigureName.Empty, " ", [3, 5]), new Piece(FigureName.Empty, " ", [3, 6]), new Piece(FigureName.Empty, " ", [3, 7])],
    [new Piece(FigureName.Empty, " ", [4, 0]), new Piece(FigureName.Empty, " ", [4, 1]), new Piece(FigureName.Empty, " ", [4, 2]), new Piece(FigureName.Empty, " ", [4, 3]), new Piece(FigureName.Empty, " ", [4, 4]), new Piece(FigureName.Empty, " ", [4, 5]), new Piece(FigureName.Empty, " ", [4, 6]), new Piece(FigureName.Empty, " ", [4, 7])],
    [new Piece(FigureName.Empty, " ", [5, 0]), new Piece(FigureName.Empty, " ", [5, 1]), new Piece(FigureName.Empty, " ", [5, 2]), new Piece(FigureName.Empty, " ", [5, 3]), new Piece(FigureName.Empty, " ", [5, 4]), new Piece(FigureName.Empty, " ", [5, 5]), new Piece(FigureName.Empty, " ", [5, 6]), new Piece(FigureName.Empty, " ", [5, 7])],
    [new Piece(FigureName.Pawn, "white", [6, 0]), new Piece(FigureName.Pawn, "white", [6, 1]), new Piece(FigureName.Pawn, "white", [6, 2]), new Piece(FigureName.Pawn, "white", [6, 3]), new Piece(FigureName.Pawn, "white", [6, 4]), new Piece(FigureName.Pawn, "white", [6, 5]), new Piece(FigureName.Pawn, "white", [6, 6]), new Piece(FigureName.Pawn, "white", [6, 7])],
    [new Piece(FigureName.Rook, "white", [7, 0]), new Piece(FigureName.Knight, "white", [7, 1]), new Piece(FigureName.Bishop, "white", [7, 2]), new Piece(FigureName.Queen, "white", [7, 3]), new Piece(FigureName.King, "white", [7, 4]), new Piece(FigureName.Bishop, "white", [7, 5]), new Piece(FigureName.Knight, "white", [7, 6]), new Piece(FigureName.Rook, "white", [7, 7])]
];

board.printBoard();

console.log("Pawn e2:", board.getPossibleMoves(board.board[6][4]));
board.makeMove("e2", "e4");

board.printBoard();

board.makeMove("e7", "e5");
board.makeMove("g1", "f3");

board.printBoard();