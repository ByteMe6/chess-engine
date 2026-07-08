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
    std::string castleRights = "HAha";
    std::array<int, 2> enPassant = {-1, -1};
    int halfmoveClock = 0;
    int fullmoveNumber = 1;
    std::vector<std::string> positionHistory;
    std::array<int, 2> promotionPending = {-1, -1};

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

    bool isCastlingPathClear(int row, int kingCol, int rookCol, int kingTarget, int rookTarget, PieceColor enemyColor) {
        if (kingTarget != kingCol) {
            int kingStep = kingTarget > kingCol ? 1 : -1;
            for (int col = kingCol + kingStep; ; col += kingStep) {
                if (isSquareAttacked(row, col, enemyColor)) return false;
                if (col != rookCol && board[row][col].name != FigureName::Empty) return false;
                if (col == kingTarget) break;
            }
        }

        if (rookTarget != rookCol) {
            int rookStep = rookTarget > rookCol ? 1 : -1;
            for (int col = rookCol + rookStep; ; col += rookStep) {
                if (col != kingCol && board[row][col].name != FigureName::Empty) return false;
                if (col == rookTarget) break;
            }
        }

        return true;
    }

    void addCastlingMoves(const Piece& piece, std::vector<std::array<int, 2>>& possibleMoves) {
        int row = piece.color == "white" ? 7 : 0;
        PieceColor enemyColor = piece.color == "white" ? "black" : "white";
        int kingCol = piece.coords[1];
        char base = piece.color == "white" ? 'A' : 'a';

        if (piece.coords[0] != row) return;
        if (isSquareAttacked(row, kingCol, enemyColor)) return;

        for (int i = 0; i < (int)castleRights.size(); i++) {
            char right = castleRights[i];
            if (right < base || right > (char)(base + 7)) continue;

            int rookCol = right - base;
            const Piece& rook = board[row][rookCol];
            if (rook.name != FigureName::Rook || rook.color != piece.color) continue;

            int kingTarget = rookCol > kingCol ? 6 : 2;
            int rookTarget = rookCol > kingCol ? 5 : 3;

            if (!isCastlingPathClear(row, kingCol, rookCol, kingTarget, rookTarget, enemyColor)) continue;

            possibleMoves.push_back({row, rookCol});
            if ((kingTarget - kingCol >= 2 || kingCol - kingTarget >= 2) && kingTarget != rookCol) {
                possibleMoves.push_back({row, kingTarget});
            }
        }
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

    // "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    void loadFen(const std::string& fen) {
        for (int emptyRow = 0; emptyRow < 8; emptyRow++) {
            for (int emptyCol = 0; emptyCol < 8; emptyCol++) {
                board[emptyRow][emptyCol] = Piece(FigureName::Empty, " ", {emptyRow, emptyCol});
            }
        }

        std::vector<std::string> fields;
        std::string current = "";

        for (int i = 0; i < (int)fen.size(); i++) {
            if (fen[i] == ' ') {
                fields.push_back(current);
                current = "";
            } else {
                current += fen[i];
            }
        }
        fields.push_back(current);

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
                char name = (char)std::toupper(symbol);

                if (std::string("KQRBNP").find(name) == std::string::npos || !isInsideBoard(row, col)) {
                    throw std::invalid_argument("Invalid FEN symbol: " + std::string(1, symbol));
                }

                PieceColor color = std::isupper(symbol) ? "white" : "black";
                board[row][col] = Piece((FigureName)name, color, {row, col});
                col++;
            }
        }

        currColor = fields.size() > 1 && fields[1] == "b" ? "black" : "white";

        castleRights = "";
        if (fields.size() > 2 && fields[2] != "-") {
            for (int i = 0; i < (int)fields[2].size(); i++) {
                char right = fields[2][i];
                char upper = (char)std::toupper(right);
                PieceColor rightColor = std::isupper(right) ? "white" : "black";
                int rightRow = rightColor == "white" ? 7 : 0;
                char base = rightColor == "white" ? 'A' : 'a';
                std::array<int, 2> king = findKing(rightColor);

                if (king[0] != rightRow) continue;

                if (upper == 'K' || upper == 'Q') {
                    if (king[1] != 4) continue;

                    if (upper == 'K') {
                        for (int col = 7; col > king[1]; col--) {
                            if (board[rightRow][col].name == FigureName::Rook && board[rightRow][col].color == rightColor) {
                                castleRights += (char)(base + col);
                                break;
                            }
                        }
                    } else {
                        for (int col = 0; col < king[1]; col++) {
                            if (board[rightRow][col].name == FigureName::Rook && board[rightRow][col].color == rightColor) {
                                castleRights += (char)(base + col);
                                break;
                            }
                        }
                    }
                } else if (upper >= 'A' && upper <= 'H') {
                    int col = upper - 'A';
                    if (board[rightRow][col].name == FigureName::Rook && board[rightRow][col].color == rightColor) {
                        castleRights += right;
                    }
                }
            }
        }

        enPassant = {-1, -1};
        if (fields.size() > 3 && fields[3] != "-") {
            enPassant = translateCoords(fields[3]);
        }

        halfmoveClock = 0;
        if (fields.size() > 4 && fields[4] != "-") {
            halfmoveClock = std::stoi(fields[4]);
        }

        fullmoveNumber = 1;
        if (fields.size() > 5 && fields[5] != "-") {
            fullmoveNumber = std::stoi(fields[5]);
        }

        positionHistory.clear();
        positionHistory.push_back(toFen(false));

        promotionPending = {-1, -1};
        gameOver = false;
        checkGameOver();
    }

    std::string toFen(bool withCounters = true) {
        std::string fen = "";

        for (int row = 0; row < 8; row++) {
            int emptyCount = 0;

            for (int col = 0; col < 8; col++) {
                const Piece& piece = board[row][col];

                if (piece.name == FigureName::Empty) {
                    emptyCount++;
                    continue;
                }

                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }

                char letter = (char)piece.name;
                fen += piece.color == "white" ? (char)std::toupper(letter) : (char)std::tolower(letter);
            }

            if (emptyCount > 0) fen += std::to_string(emptyCount);
            if (row < 7) fen += '/';
        }

        std::string rights = "";
        for (int i = 0; i < (int)castleRights.size(); i++) {
            char right = castleRights[i];
            PieceColor rightColor = std::isupper(right) ? "white" : "black";
            std::array<int, 2> king = findKing(rightColor);

            if (king[1] == 4) {
                if (right == 'H') { rights += 'K'; continue; }
                if (right == 'A') { rights += 'Q'; continue; }
                if (right == 'h') { rights += 'k'; continue; }
                if (right == 'a') { rights += 'q'; continue; }
            }

            rights += right;
        }

        fen += currColor == "white" ? " w" : " b";
        fen += rights == "" ? " -" : " " + rights;
        fen += enPassant[0] == -1 ? " -" : " " + translateToNotation(enPassant);

        if (withCounters) {
            fen += " " + std::to_string(halfmoveClock) + " " + std::to_string(fullmoveNumber);
        }

        return fen;
    }

    // Castling

    void removeCastleRight(char right) {
        int pos = (int)castleRights.find(right);
        if (pos != -1) castleRights.erase(pos, 1);
    }

    void removeRookRights(std::array<int, 2> coords) {
        if (coords[0] == 7) removeCastleRight((char)('A' + coords[1]));
        if (coords[0] == 0) removeCastleRight((char)('a' + coords[1]));
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

        int fromRow = piece.coords[0];
        int fromCol = piece.coords[1];

        bool isCastling = false;
        int castlingRookCol = -1;

        if (piece.name == FigureName::King) {
            const Piece& targetSquare = board[finalCoords[0]][finalCoords[1]];

            if (targetSquare.name == FigureName::Rook && targetSquare.color == piece.color) {
                isCastling = true;
                castlingRookCol = finalCoords[1];
            } else if (finalCoords[1] - fromCol >= 2 || fromCol - finalCoords[1] >= 2) {
                isCastling = true;
                char base = piece.color == "white" ? 'A' : 'a';

                for (int i = 0; i < (int)castleRights.size(); i++) {
                    char right = castleRights[i];
                    if (right < base || right > (char)(base + 7)) continue;
                    if ((right - base > fromCol) == (finalCoords[1] > fromCol)) castlingRookCol = right - base;
                }
            }
        }

        if (!isCastling && wouldBeInCheck(piece, finalCoords)) {
            std::cout << "This move would leave the king in check" << std::endl;
            return;
        }

        bool isCapture = !isCastling && (board[finalCoords[0]][finalCoords[1]].name != FigureName::Empty || isEnPassantMove(piece, finalCoords));

        if (isEnPassantMove(piece, finalCoords)) {
            board[fromRow][finalCoords[1]] = Piece(FigureName::Empty, " ", {fromRow, finalCoords[1]});
        }

        // piece comes in as a copy, so it's safe to clear its old square first
        if (isCastling) {
            int kingTarget = castlingRookCol > fromCol ? 6 : 2;
            int rookTarget = castlingRookCol > fromCol ? 5 : 3;

            board[fromRow][fromCol] = Piece(FigureName::Empty, " ", {fromRow, fromCol});
            board[fromRow][castlingRookCol] = Piece(FigureName::Empty, " ", {fromRow, castlingRookCol});
            board[fromRow][kingTarget] = Piece(FigureName::King, piece.color, {fromRow, kingTarget});
            board[fromRow][rookTarget] = Piece(FigureName::Rook, piece.color, {fromRow, rookTarget});
        } else {
            board[fromRow][fromCol] = Piece(FigureName::Empty, " ", {fromRow, fromCol});
            piece.coords = finalCoords;
            board[finalCoords[0]][finalCoords[1]] = piece;
        }

        enPassant = {-1, -1};
        if (piece.name == FigureName::Pawn && (fromRow - finalCoords[0] == 2 || finalCoords[0] - fromRow == 2)) {
            enPassant = {(fromRow + finalCoords[0]) / 2, finalCoords[1]};
        }

        if (piece.name == FigureName::King) {
            std::string keptRights = "";
            for (int i = 0; i < (int)castleRights.size(); i++) {
                bool isWhiteRight = std::isupper(castleRights[i]) != 0;
                if ((piece.color == "white") != isWhiteRight) keptRights += castleRights[i];
            }
            castleRights = keptRights;
        }
        removeRookRights({fromRow, fromCol});
        removeRookRights(finalCoords);

        halfmoveClock = piece.name == FigureName::Pawn || isCapture ? 0 : halfmoveClock + 1;
        if (piece.color == "black") fullmoveNumber++;

        currColor = currColor == "white" ? "black" : "white";

        if (piece.name == FigureName::Pawn && (finalCoords[0] == 0 || finalCoords[0] == 7)) {
            promotionPending = finalCoords;
            return;
        }

        positionHistory.push_back(toFen(false));
        checkGameOver();
    }

    void promote(FigureName name) {
        if (promotionPending[0] == -1) {
            std::cout << "Nothing to promote" << std::endl;
            return;
        }

        if (name != FigureName::Queen && name != FigureName::Rook && name != FigureName::Bishop && name != FigureName::Knight) {
            std::cout << "A pawn can't become that" << std::endl;
            return;
        }

        PieceColor color = board[promotionPending[0]][promotionPending[1]].color;
        board[promotionPending[0]][promotionPending[1]] = Piece(name, color, promotionPending);
        promotionPending = {-1, -1};

        positionHistory.push_back(toFen(false));
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

    bool isFiftyMoveRule() {
        return halfmoveClock >= 100;
    }

    bool isThreefoldRepetition() {
        std::string current = toFen(false);
        int count = 0;

        for (int i = 0; i < (int)positionHistory.size(); i++) {
            if (positionHistory[i] == current) count++;
        }

        return count >= 3;
    }

    bool isNotMaterial() {
        std::vector<FigureName> pieces;

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const Piece& piece = board[row][col];
                if (piece.name != FigureName::Empty && piece.name != FigureName::King) {
                    pieces.push_back(piece.name);
                }
            }
        }

        if ((int)pieces.size() == 0) return true;
        if ((int)pieces.size() == 1 && (pieces[0] == FigureName::Bishop || pieces[0] == FigureName::Knight)) return true;
        return false;
    }

    void checkGameOver() {
        if (isMate(currColor)) {
            gameOver = true;
            std::cout << "Game over: " << (currColor == "white" ? "Black" : "White") << " won" << std::endl;
        } else if (isStalemate(currColor) || isNotMaterial() || isThreefoldRepetition() || isFiftyMoveRule()) {
            gameOver = true;
            std::cout << "Game over: draw" << std::endl;
        }
    }

    bool isPawnLastLine(std::string color) {
        int lastRow = color == "white" ? 0 : 7;

        for (int col = 0; col < 8; col++) {
            const Piece& piece = board[lastRow][col];
            if (piece.name == FigureName::Pawn && piece.color == color) {
                return true;
            }
        }

        return false;
    }

    // takes a move in chess notation like ("e2", "e4") and applies it on the board
    void makeMove(const std::string& from, const std::string& to) {
        if (promotionPending[0] != -1) {
            promote(FigureName::Queen);
        }

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
