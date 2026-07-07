#pragma once

#include <array>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

enum class FigureName : char {
    King = 'K',
    Queen = 'Q',
    Rook = 'R',
    Bishop = 'B',
    Knight = 'N',
    Pawn = 'P',
    Empty = ' '
};

using PieceColor = std::string; // "white" | "black" | " "

struct Figure {
    FigureName name;
    PieceColor color;
    std::array<int, 2> coords;
};

class Piece : public Figure {
public:
    Piece(FigureName name, PieceColor color, std::array<int, 2> coords)
        : Figure{name, color, coords} {}
};

class Board {
public:
    PieceColor currColor;
    std::vector<std::vector<Piece>> board;
    bool gameOver = false;
    std::string castleRights = "KQkq";
    std::array<int, 2> enPassant = {-1, -1};

    Board(PieceColor currColor, std::vector<std::vector<Piece>> board)
        : currColor(currColor), board(board) {}

    // start straight from a FEN string
    Board(const std::string& fen)
        : currColor("white"), board(8, std::vector<Piece>(8, Piece(FigureName::Empty, " ", {0, 0}))) {
        loadFen(fen);
    }

    void printBoard() {
        std::cout << "\n  a b c d e f g h" << std::endl;
        for (int row = 0; row < (int)board.size(); row++) {
            std::string line = std::to_string(8 - row) + " ";
            for (int col = 0; col < (int)board[row].size(); col++) {
                const Piece& piece = board[row][col];
                char symbol = piece.color == "white" ? (char)std::toupper((char)piece.name) : (char)std::tolower((char)piece.name);
                line += symbol;
                line += ' ';
            }
            std::cout << line + std::to_string(8 - row) << std::endl;
        }
        std::cout << "  a b c d e f g h\n" << std::endl;
    }

    bool isInsideBoard(int row, int col) {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    // for queen, rook and bishop we walk in each direction until we can't
    void addSlidingMoves(const Piece& piece, const std::vector<std::array<int, 2>>& directions, std::vector<std::array<int, 2>>& possibleMoves) {
        int row = piece.coords[0];
        int col = piece.coords[1];

        for (const auto& [rowStep, colStep] : directions) {
            int newRow = row + rowStep;
            int newCol = col + colStep;

            while (isInsideBoard(newRow, newCol)) {
                const Piece& targetPiece = board[newRow][newCol];

                if (targetPiece.name == FigureName::Empty) {
                    // empty square - can move here, keep going
                    possibleMoves.push_back({newRow, newCol});
                } else if (targetPiece.color != piece.color) {
                    // enemy piece - can capture, but can't go further
                    possibleMoves.push_back({newRow, newCol});
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

    void addCastlingMoves(const Piece& piece, std::vector<std::array<int, 2>>& possibleMoves) {
        int row = piece.color == "white" ? 7 : 0;
        char kingSideRight = piece.color == "white" ? 'K' : 'k';
        char queenSideRight = piece.color == "white" ? 'Q' : 'q';
        PieceColor enemyColor = piece.color == "white" ? "black" : "white";

        if (piece.coords[0] != row || piece.coords[1] != 4) return;
        if (isSquareAttacked(row, 4, enemyColor)) return;

        bool canKingSide = castleRights.find(kingSideRight) != std::string::npos
            && board[row][7].name == FigureName::Rook && board[row][7].color == piece.color
            && board[row][5].name == FigureName::Empty && board[row][6].name == FigureName::Empty
            && !isSquareAttacked(row, 5, enemyColor) && !isSquareAttacked(row, 6, enemyColor);

        bool canQueenSide = castleRights.find(queenSideRight) != std::string::npos
            && board[row][0].name == FigureName::Rook && board[row][0].color == piece.color
            && board[row][1].name == FigureName::Empty && board[row][2].name == FigureName::Empty && board[row][3].name == FigureName::Empty
            && !isSquareAttacked(row, 2, enemyColor) && !isSquareAttacked(row, 3, enemyColor);

        if (canKingSide) possibleMoves.push_back({row, 6});
        if (canQueenSide) possibleMoves.push_back({row, 2});
    }

    bool isEnPassantMove(const Piece& piece, std::array<int, 2> finalCoords) {
        return piece.name == FigureName::Pawn && finalCoords[0] == enPassant[0] && finalCoords[1] == enPassant[1];
    }

    std::vector<std::array<int, 2>> getPossibleMoves(const Piece& piece, bool withCastling = true) {
        // Coords: 0 - row; 1 - column
        int row = piece.coords[0];
        int col = piece.coords[1];
        std::vector<std::array<int, 2>> possibleMoves;

        switch (piece.name) {
            case FigureName::King: {
                for (int rowStep = -1; rowStep <= 1; rowStep++) {
                    for (int colStep = -1; colStep <= 1; colStep++) {
                        if (rowStep == 0 && colStep == 0) continue; // skip own square

                        int newRow = row + rowStep;
                        int newCol = col + colStep;

                        if (!isInsideBoard(newRow, newCol)) continue;

                        const Piece& targetPiece = board[newRow][newCol];
                        if (targetPiece.color != piece.color) {
                            possibleMoves.push_back({newRow, newCol});
                        }
                    }
                }

                if (withCastling) {
                    addCastlingMoves(piece, possibleMoves);
                }
                break;
            }

            case FigureName::Queen: {
                std::vector<std::array<int, 2>> directions = {
                    {-1, 0}, {1, 0}, {0, -1}, {0, 1}, // vertical + horizontal
                    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}// diagonals
                };
                addSlidingMoves(piece, directions, possibleMoves);
                break;
            }

            case FigureName::Rook: {
                std::vector<std::array<int, 2>> directions = {
                    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
                };
                addSlidingMoves(piece, directions, possibleMoves);
                break;
            }

            case FigureName::Bishop: {
                std::vector<std::array<int, 2>> directions = {
                    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
                };
                addSlidingMoves(piece, directions, possibleMoves);
                break;
            }

            case FigureName::Knight: {
                std::vector<std::array<int, 2>> jumps = {
                    {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
                    {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
                };

                for (const auto& [rowStep, colStep] : jumps) {
                    int newRow = row + rowStep;
                    int newCol = col + colStep;

                    if (!isInsideBoard(newRow, newCol)) continue;

                    const Piece& targetPiece = board[newRow][newCol];
                    if (targetPiece.color != piece.color) {
                        possibleMoves.push_back({newRow, newCol});
                    }
                }
                break;
            }

            case FigureName::Pawn: {
                int direction = piece.color == "white" ? -1 : 1;
                int startRow = piece.color == "white" ? 6 : 1;

                int oneForward = row + direction;
                if (isInsideBoard(oneForward, col) && board[oneForward][col].name == FigureName::Empty) {
                    possibleMoves.push_back({oneForward, col});

                    // 2 squares forward (only from start row, both squares empty)
                    int twoForward = row + direction * 2;
                    if (row == startRow && board[twoForward][col].name == FigureName::Empty) {
                        possibleMoves.push_back({twoForward, col});
                    }
                }

                // diagonal captures (only if enemy piece is there)
                for (int colStep : {-1, 1}) {
                    int newRow = row + direction;
                    int newCol = col + colStep;

                    if (!isInsideBoard(newRow, newCol)) continue;

                    const Piece& targetPiece = board[newRow][newCol];
                    if (targetPiece.name != FigureName::Empty && targetPiece.color != piece.color) {
                        possibleMoves.push_back({newRow, newCol});
                    } else if (targetPiece.name == FigureName::Empty && isEnPassantMove(piece, {newRow, newCol})) {
                        const Piece& jumpedPawn = board[row][newCol];
                        if (jumpedPawn.name == FigureName::Pawn && jumpedPawn.color != piece.color) {
                            possibleMoves.push_back({newRow, newCol});
                        }
                    }
                }
                break;
            }

            case FigureName::Empty:
                break;

            default:
                // unreachable - every FigureName is handled above
                throw std::runtime_error("Unhandled piece");
        }

        return possibleMoves;
    }

    // "e2" -> [6, 4], "a8" -> [0, 0], "h1" -> [7, 7]
    std::array<int, 2> translateCoords(const std::string& square) {
        int col = square[0] - 'a'; // 'a' -> 0, 'h' -> 7
        int row = 8 - (square[1] - '0'); // '8' -> 0, '1' -> 7

        if (!isInsideBoard(row, col)) {
            throw std::invalid_argument("Invalid square: " + square);
        }

        return {row, col};
    }

    // [6, 4] -> "e2"
    std::string translateToNotation(std::array<int, 2> coords) {
        char letter = (char)('a' + coords[1]);
        int number = 8 - coords[0];
        return std::string(1, letter) + std::to_string(number);
    }

    // 'P' for a white pawn, 'p' for a black one
    char pieceSymbol(const Piece& piece) {
        char letter = (char)piece.name;
        return piece.color == "white" ? (char)std::toupper(letter) : (char)std::tolower(letter);
    }

    void clearBoard() {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                board[row][col] = Piece(FigureName::Empty, " ", {row, col});
            }
        }
    }

    // one FEN letter -> a piece on the board, uppercase is white
    void putFenPiece(char symbol, int row, int col) {
        char name = (char)std::toupper(symbol);
        bool isRealPiece = std::string("KQRBNP").find(name) != std::string::npos;

        if (!isRealPiece || !isInsideBoard(row, col)) {
            throw std::invalid_argument("Invalid FEN symbol: " + std::string(1, symbol));
        }

        PieceColor color = std::isupper(symbol) ? "white" : "black";
        board[row][col] = Piece((FigureName)name, color, {row, col});
    }

    std::vector<std::string> splitBySpaces(const std::string& text) {
        std::vector<std::string> parts;
        std::string current = "";

        for (int i = 0; i < (int)text.size(); i++) {
            if (text[i] == ' ') {
                parts.push_back(current);
                current = "";
            } else {
                current += text[i];
            }
        }

        parts.push_back(current);
        return parts;
    }

    // "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    void loadFen(const std::string& fen) {
        clearBoard();

        std::vector<std::string> fields = splitBySpaces(fen);
        std::string pieces = fields[0];

        int row = 0;
        int col = 0;

        for (int i = 0; i < (int)pieces.size(); i++) {
            char symbol = pieces[i];

            if (symbol == '/') {
                row++;
                col = 0;
            } else if (symbol >= '1' && symbol <= '8') {
                col += symbol - '0'; // a digit skips empty squares
            } else {
                putFenPiece(symbol, row, col);
                col++;
            }
        }

        currColor = fields.size() > 1 && fields[1] == "b" ? "black" : "white";

        castleRights = "";
        if (fields.size() > 2 && fields[2] != "-") {
            castleRights = fields[2];
        }

        enPassant = {-1, -1};
        if (fields.size() > 3 && fields[3] != "-") {
            enPassant = translateCoords(fields[3]);
        }

        gameOver = false;
        checkGameOver();
    }

    std::string rowToFen(int row) {
        std::string result = "";
        int emptyCount = 0;

        for (int col = 0; col < 8; col++) {
            const Piece& piece = board[row][col];

            if (piece.name == FigureName::Empty) {
                emptyCount++;
                continue;
            }

            if (emptyCount > 0) {
                result += std::to_string(emptyCount);
                emptyCount = 0;
            }

            result += pieceSymbol(piece);
        }

        if (emptyCount > 0) result += std::to_string(emptyCount);

        return result;
    }

    std::string toFen() {
        std::string fen = "";

        for (int row = 0; row < 8; row++) {
            fen += rowToFen(row);
            if (row < 7) fen += '/';
        }

        fen += currColor == "white" ? " w" : " b";
        fen += castleRights == "" ? " -" : " " + castleRights;
        fen += enPassant[0] == -1 ? " -" : " " + translateToNotation(enPassant);
        fen += " 0 1";
        return fen;
    }

    void removeCastleRight(char right) {
        int pos = (int)castleRights.find(right);
        if (pos != -1) castleRights.erase(pos, 1);
    }

    void updateCornerRights(std::array<int, 2> coords) {
        if (coords[0] == 7 && coords[1] == 7) removeCastleRight('K');
        if (coords[0] == 7 && coords[1] == 0) removeCastleRight('Q');
        if (coords[0] == 0 && coords[1] == 7) removeCastleRight('k');
        if (coords[0] == 0 && coords[1] == 0) removeCastleRight('q');
    }

    void moveRookForCastling(int row, int rookCol, int newRookCol) {
        Piece rook = board[row][rookCol];
        board[row][rookCol] = Piece(FigureName::Empty, " ", {row, rookCol});
        rook.coords = {row, newRookCol};
        board[row][newRookCol] = rook;
    }

    void doAMove(Piece piece, std::array<int, 2> finalCoords) {
        std::vector<std::array<int, 2>> possibleMoves = getPossibleMoves(piece);

        bool isMovePossible = false;

        for (int i = 0; i < (int)possibleMoves.size(); i++) {
            if (possibleMoves[i][0] == finalCoords[0] && possibleMoves[i][1] == finalCoords[1]) {
                isMovePossible = true;
                break;
            }
        }

        if (!isMovePossible) return;

        if (wouldBeInCheck(piece, finalCoords)) {
            std::cout << "This move would leave the king in check" << std::endl;
            return;
        }

        int fromRow = piece.coords[0];
        int fromCol = piece.coords[1];

        if (isEnPassantMove(piece, finalCoords)) {
            board[fromRow][finalCoords[1]] = Piece(FigureName::Empty, " ", {fromRow, finalCoords[1]});
        }

        if (piece.name == FigureName::King && finalCoords[1] - fromCol == 2) {
            moveRookForCastling(fromRow, 7, 5);
        }
        if (piece.name == FigureName::King && fromCol - finalCoords[1] == 2) {
            moveRookForCastling(fromRow, 0, 3);
        }

        // piece comes in as a copy, so it's safe to clear its old square first
        board[fromRow][fromCol] = Piece(FigureName::Empty, " ", {fromRow, fromCol});
        piece.coords = finalCoords;
        board[finalCoords[0]][finalCoords[1]] = piece;

        enPassant = {-1, -1};
        if (piece.name == FigureName::Pawn && (fromRow - finalCoords[0] == 2 || finalCoords[0] - fromRow == 2)) {
            enPassant = {(fromRow + finalCoords[0]) / 2, finalCoords[1]};
        }

        if (piece.name == FigureName::King) {
            removeCastleRight(piece.color == "white" ? 'K' : 'k');
            removeCastleRight(piece.color == "white" ? 'Q' : 'q');
        }
        updateCornerRights({fromRow, fromCol});
        updateCornerRights(finalCoords);

        currColor = currColor == "white" ? "black" : "white";
        checkGameOver();
    }

    // where the king of this color stands
    std::array<int, 2> findKing(PieceColor color) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const Piece& piece = board[row][col];
                if (piece.name == FigureName::King && piece.color == color) {
                    return {row, col};
                }
            }
        }
        return {-1, -1}; // no king on the board
    }

    bool isSquareAttacked(int row, int col, PieceColor byColor) {
        for (int pieceRow = 0; pieceRow < 8; pieceRow++) {
            for (int pieceCol = 0; pieceCol < 8; pieceCol++) {
                const Piece& piece = board[pieceRow][pieceCol];
                if (piece.color != byColor) continue;

                std::vector<std::array<int, 2>> moves = getPossibleMoves(piece, false);
                for (int i = 0; i < (int)moves.size(); i++) {
                    if (moves[i][0] == row && moves[i][1] == col) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool isInCheck(PieceColor side) {
        std::array<int, 2> king = findKing(side);
        if (king[0] == -1) return false; // no king - nothing to check

        return isSquareAttacked(king[0], king[1], side == "white" ? "black" : "white");
    }

    // try the move on the board, see if our king stays safe, then put everything back
    bool wouldBeInCheck(Piece piece, std::array<int, 2> finalCoords) {
        std::vector<std::vector<Piece>> backup = board;

        if (isEnPassantMove(piece, finalCoords)) {
            board[piece.coords[0]][finalCoords[1]] = Piece(FigureName::Empty, " ", {piece.coords[0], finalCoords[1]});
        }

        board[piece.coords[0]][piece.coords[1]] = Piece(FigureName::Empty, " ", {piece.coords[0], piece.coords[1]});
        piece.coords = finalCoords;
        board[finalCoords[0]][finalCoords[1]] = piece;

        bool inCheck = isInCheck(piece.color);
        board = backup;
        return inCheck;
    }

    bool hasAnyLegalMove(PieceColor side) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Piece piece = board[row][col];
                if (piece.color != side) continue;

                std::vector<std::array<int, 2>> moves = getPossibleMoves(piece);
                for (int i = 0; i < (int)moves.size(); i++) {
                    if (!wouldBeInCheck(piece, moves[i])) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool isMate(PieceColor side) {
        return isInCheck(side) && !hasAnyLegalMove(side);
    }

    // stalemate - not in check, but every move would put the king in check
    bool isStalemate(PieceColor side) {
        return !isInCheck(side) && !hasAnyLegalMove(side);
    }

    // mate or stalemate for the side to move ends the game
    void checkGameOver() {
        if (isMate(currColor)) {
            gameOver = true;
            std::cout << "Game over: " << (currColor == "white" ? "Black" : "White") << " won" << std::endl;
        } else if (isStalemate(currColor)) {
            gameOver = true;
            std::cout << "Game over: draw" << std::endl;
        }
    }

    // takes a move in chess notation like ("e2", "e4") and applies it on the board
    void makeMove(const std::string& from, const std::string& to) {
        if (gameOver) {
            std::cout << "The game is over" << std::endl;
            return;
        }

        std::array<int, 2> fromCoords = translateCoords(from);
        std::array<int, 2> toCoords = translateCoords(to);

        Piece piece = board[fromCoords[0]][fromCoords[1]];

        if (piece.name == FigureName::Empty) {
            std::cout << "No piece on " << from << std::endl;
            return;
        }

        if (piece.color != currColor) {
            std::cout << "It's " << currColor << "'s turn" << std::endl;
            return;
        }

        doAMove(piece, toCoords);
    }
};
