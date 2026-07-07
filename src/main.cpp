#include "chessEngine.hpp"

int main() {
    Board board("white", std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece(FigureName::Empty, " ", {0, 0}))));

    board.board = {
        {Piece(FigureName::Rook, "black", {0, 0}), Piece(FigureName::Knight, "black", {0, 1}), Piece(FigureName::Bishop, "black", {0, 2}), Piece(FigureName::Queen, "black", {0, 3}), Piece(FigureName::King, "black", {0, 4}), Piece(FigureName::Bishop, "black", {0, 5}), Piece(FigureName::Knight, "black", {0, 6}), Piece(FigureName::Rook, "black", {0, 7})},
        {Piece(FigureName::Pawn, "black", {1, 0}), Piece(FigureName::Pawn, "black", {1, 1}), Piece(FigureName::Pawn, "black", {1, 2}), Piece(FigureName::Pawn, "black", {1, 3}), Piece(FigureName::Pawn, "black", {1, 4}), Piece(FigureName::Pawn, "black", {1, 5}), Piece(FigureName::Pawn, "black", {1, 6}), Piece(FigureName::Pawn, "black", {1, 7})},
        {Piece(FigureName::Empty, " ", {2, 0}), Piece(FigureName::Empty, " ", {2, 1}), Piece(FigureName::Empty, " ", {2, 2}), Piece(FigureName::Empty, " ", {2, 3}), Piece(FigureName::Empty, " ", {2, 4}), Piece(FigureName::Empty, " ", {2, 5}), Piece(FigureName::Empty, " ", {2, 6}), Piece(FigureName::Empty, " ", {2, 7})},
        {Piece(FigureName::Empty, " ", {3, 0}), Piece(FigureName::Empty, " ", {3, 1}), Piece(FigureName::Empty, " ", {3, 2}), Piece(FigureName::Empty, " ", {3, 3}), Piece(FigureName::Empty, " ", {3, 4}), Piece(FigureName::Empty, " ", {3, 5}), Piece(FigureName::Empty, " ", {3, 6}), Piece(FigureName::Empty, " ", {3, 7})},
        {Piece(FigureName::Empty, " ", {4, 0}), Piece(FigureName::Empty, " ", {4, 1}), Piece(FigureName::Empty, " ", {4, 2}), Piece(FigureName::Empty, " ", {4, 3}), Piece(FigureName::Empty, " ", {4, 4}), Piece(FigureName::Empty, " ", {4, 5}), Piece(FigureName::Empty, " ", {4, 6}), Piece(FigureName::Empty, " ", {4, 7})},
        {Piece(FigureName::Empty, " ", {5, 0}), Piece(FigureName::Empty, " ", {5, 1}), Piece(FigureName::Empty, " ", {5, 2}), Piece(FigureName::Empty, " ", {5, 3}), Piece(FigureName::Empty, " ", {5, 4}), Piece(FigureName::Empty, " ", {5, 5}), Piece(FigureName::Empty, " ", {5, 6}), Piece(FigureName::Empty, " ", {5, 7})},
        {Piece(FigureName::Pawn, "white", {6, 0}), Piece(FigureName::Pawn, "white", {6, 1}), Piece(FigureName::Pawn, "white", {6, 2}), Piece(FigureName::Pawn, "white", {6, 3}), Piece(FigureName::Pawn, "white", {6, 4}), Piece(FigureName::Pawn, "white", {6, 5}), Piece(FigureName::Pawn, "white", {6, 6}), Piece(FigureName::Pawn, "white", {6, 7})},
        {Piece(FigureName::Rook, "white", {7, 0}), Piece(FigureName::Knight, "white", {7, 1}), Piece(FigureName::Bishop, "white", {7, 2}), Piece(FigureName::Queen, "white", {7, 3}), Piece(FigureName::King, "white", {7, 4}), Piece(FigureName::Bishop, "white", {7, 5}), Piece(FigureName::Knight, "white", {7, 6}), Piece(FigureName::Rook, "white", {7, 7})}
    };

    board.printBoard();

    std::vector<std::array<int, 2>> pawnMoves = board.getPossibleMoves(board.board[6][4]);
    std::string movesText = "[";
    for (int i = 0; i < (int)pawnMoves.size(); i++) {
        movesText += " [ " + std::to_string(pawnMoves[i][0]) + ", " + std::to_string(pawnMoves[i][1]) + " ]";
        if (i < (int)pawnMoves.size() - 1) movesText += ",";
    }
    movesText += " ]";
    std::cout << "Pawn e2: " << movesText << std::endl;

    board.makeMove("e2", "e4");

    board.printBoard();

    board.makeMove("e7", "e5");
    board.makeMove("g1", "f3");

    board.printBoard();

    return 0;
}
