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

    Board(PieceColor currColor, std::vector<std::vector<Piece>> board)
        : currColor(currColor), board(board) {}

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

    std::vector<std::array<int, 2>> getPossibleMoves(const Piece& piece) {
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

    void doAMove(Piece piece, std::array<int, 2> finalCoords) {
        std::vector<std::array<int, 2>> possibleMoves = getPossibleMoves(piece);

        bool isMovePossible = false;

        for (int i = 0; i < (int)possibleMoves.size(); i++) {
            if (possibleMoves[i][0] == finalCoords[0] && possibleMoves[i][1] == finalCoords[1]) {
                isMovePossible = true;
                break;
            }
        }

        if (isMovePossible) {
            // piece comes in as a copy, so it's safe to clear its old square first
            board[piece.coords[0]][piece.coords[1]] = Piece(FigureName::Empty, " ", {piece.coords[0], piece.coords[1]});
            piece.coords = finalCoords;
            board[finalCoords[0]][finalCoords[1]] = piece;
        }
    }

    // takes a move in chess notation like ("e2", "e4") and applies it on the board
    void makeMove(const std::string& from, const std::string& to) {
        std::array<int, 2> fromCoords = translateCoords(from);
        std::array<int, 2> toCoords = translateCoords(to);

        Piece piece = board[fromCoords[0]][fromCoords[1]];

        if (piece.name == FigureName::Empty) {
            std::cout << "No piece on " << from << std::endl;
            return;
        }

        doAMove(piece, toCoords);
    }
};
