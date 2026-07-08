#include "../src/chessEngine.hpp"

int failedCount = 0;

void check(const std::string& name, const std::string& got, const std::string& expected) {
    if (got == expected) {
        std::cout << "OK   " << name << std::endl;
    } else {
        std::cout << "FAIL " << name << "\n  got:      " << got << "\n  expected: " << expected << std::endl;
        failedCount++;
    }
}

void checkTrue(const std::string& name, bool condition) {
    if (condition) {
        std::cout << "OK   " << name << std::endl;
    } else {
        std::cout << "FAIL " << name << std::endl;
        failedCount++;
    }
}

int main() {
    std::string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board b1(startFen);
    check("fen: start position round-trip", b1.toFen(), startFen);

    b1.makeMove("e2", "e4");
    check("fen: after e4 with ep square", b1.toFen(), "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    Board b2(startFen);
    checkTrue("moves: e2 pawn has 2 moves", (int)b2.getPossibleMoves(b2.board[6][4]).size() == 2);

    b2.makeMove("b1", "b3");
    check("moves: illegal knight move rejected", b2.toFen(), startFen);

    b2.makeMove("e7", "e5");
    check("moves: black can't move first", b2.toFen(), startFen);

    b2.makeMove("e4", "e5");
    check("moves: empty square rejected", b2.toFen(), startFen);

    Board b3(startFen);
    b3.makeMove("e2", "e4");
    b3.makeMove("d7", "d5");
    b3.makeMove("e4", "d5");
    check("moves: capture works", b3.toFen(), "rnbqkbnr/ppp1pppp/8/3P4/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2");

    Board b4("k3r3/8/8/8/8/8/4B3/4K3 w - - 0 1");
    b4.makeMove("e2", "d3");
    check("check: pinned bishop can't move", b4.toFen(), "k3r3/8/8/8/8/8/4B3/4K3 w - - 0 1");

    Board b5("6k1/5ppp/8/8/8/8/8/R3K3 w - - 0 1");
    b5.makeMove("a1", "a8");
    checkTrue("mate: back rank mate ends the game", b5.gameOver && b5.isMate("black"));

    Board b6("rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3");
    checkTrue("mate: fool's mate detected on load", b6.gameOver);
    std::string foolsMate = b6.toFen();
    b6.makeMove("a2", "a3");
    check("mate: moves blocked after game over", b6.toFen(), foolsMate);

    Board b7("k7/2Q5/8/8/8/8/8/1K6 b - - 0 1");
    checkTrue("stalemate: detected on load", b7.gameOver && b7.isStalemate("black"));

    Board b8("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    b8.makeMove("e1", "g1");
    b8.makeMove("e8", "c8");
    check("castling: O-O and O-O-O", b8.toFen(), "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 w - - 2 2");

    Board b9("4k3/8/8/8/8/5r2/8/R3K2R w KQ - 0 1");
    b9.makeMove("e1", "g1");
    check("castling: not through an attacked square", b9.toFen(), "4k3/8/8/8/8/5r2/8/R3K2R w KQ - 0 1");
    b9.makeMove("e1", "c1");
    check("castling: queenside still works", b9.toFen(), "4k3/8/8/8/8/5r2/8/2KR3R b - - 1 1");

    Board b10("4k3/8/8/8/8/4r3/8/R3K2R w KQ - 0 1");
    b10.makeMove("e1", "g1");
    b10.makeMove("e1", "c1");
    check("castling: not while in check", b10.toFen(), "4k3/8/8/8/8/4r3/8/R3K2R w KQ - 0 1");

    Board b11("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    b11.makeMove("h1", "g1");
    b11.makeMove("a8", "b8");
    b11.makeMove("g1", "h1");
    b11.makeMove("b8", "a8");
    b11.makeMove("e1", "g1");
    check("castling: right lost after rook trip", b11.toFen(), "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w Qk - 4 3");

    Board b12(startFen);
    b12.makeMove("e2", "e4");
    b12.makeMove("a7", "a6");
    b12.makeMove("e4", "e5");
    b12.makeMove("d7", "d5");
    b12.makeMove("e5", "d6");
    check("en passant: exd6", b12.toFen(), "rnbqkbnr/1pp1pppp/p2P4/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3");

    Board b13(startFen);
    b13.makeMove("e2", "e4");
    b13.makeMove("a7", "a6");
    b13.makeMove("e4", "e5");
    b13.makeMove("d7", "d5");
    b13.makeMove("h2", "h3");
    b13.makeMove("h7", "h6");
    b13.makeMove("e5", "d6");
    check("en passant: expires after one move", b13.toFen(), "rnbqkbnr/1pp1ppp1/p6p/3pP3/8/7P/PPPP1PP1/RNBQKBNR w KQkq - 0 4");

    Board b14("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    b14.makeMove("e5", "d6");
    check("en passant: square comes from FEN", b14.toFen(), "rnbqkbnr/1pp1pppp/p2P4/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3");

    Board b15("8/8/8/4k3/8/8/8/R3K3 w - - 99 80");
    b15.makeMove("a1", "b1");
    checkTrue("draw: fifty-move rule", b15.gameOver);
    check("draw: halfmove clock hits 100", b15.toFen(), "8/8/8/4k3/8/8/8/1R2K3 b - - 100 80");

    Board b16("8/8/8/4k3/8/8/P7/4K3 w - - 99 80");
    b16.makeMove("a2", "a3");
    check("draw: pawn move resets the clock", b16.toFen(), "8/8/8/4k3/8/P7/8/4K3 b - - 0 80");

    Board b17(startFen);
    b17.makeMove("g1", "f3");
    b17.makeMove("g8", "f6");
    b17.makeMove("f3", "g1");
    b17.makeMove("f6", "g8");
    b17.makeMove("g1", "f3");
    b17.makeMove("g8", "f6");
    b17.makeMove("f3", "g1");
    b17.makeMove("f6", "g8");
    checkTrue("draw: threefold repetition", b17.gameOver);

    Board b18("8/8/8/4k3/8/8/4K3/4N3 w - - 0 1");
    checkTrue("draw: insufficient material on load", b18.gameOver);

    Board b19("1r2k3/8/8/8/8/8/8/KR6 b - - 0 1");
    b19.makeMove("b8", "b1");
    b19.makeMove("a1", "b1");
    checkTrue("draw: capture down to bare kings", b19.gameOver);

    Board b20("8/8/8/4k3/8/8/4K3/4R3 w - - 12 34");
    check("fen: counters round-trip", b20.toFen(), "8/8/8/4k3/8/8/4K3/4R3 w - - 12 34");

    bool threw = false;
    try {
        Board b21("rnbqkbnr/ppXppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    checkTrue("fen: invalid symbol throws", threw);

    Board b22(startFen);
    b22.makeMove("e2", "e4");
    b22.loadFen("8/8/8/4k3/8/8/8/4KQ2 w - - 0 1");
    check("fen: loadFen resets the board", b22.toFen(), "8/8/8/4k3/8/8/8/4KQ2 w - - 0 1");

    Board b23("8/P7/8/8/8/4k3/8/4K3 w - - 0 1");
    b23.makeMove("a7", "a8");
    b23.makeMove("e3", "d3");
    check("promotion: queen by default on the next move", b23.toFen(), "Q7/8/8/8/8/3k4/8/4K3 w - - 1 2");

    Board b24("8/P7/8/8/8/4k3/6p1/4K3 w - - 0 1");
    b24.makeMove("a7", "a8");
    b24.promote(FigureName::Knight);
    b24.makeMove("e3", "d3");
    check("promotion: explicit knight", b24.toFen(), "N7/8/8/8/8/3k4/6p1/4K3 w - - 1 2");

    Board b25("r3k3/1P6/8/8/8/8/8/4K3 w q - 0 1");
    b25.makeMove("b7", "a8");
    b25.promote(FigureName::Queen);
    check("promotion: capture into promotion kills the castle right", b25.toFen(), "Q3k3/8/8/8/8/8/8/4K3 b - - 0 1");

    Board b26(startFen);
    b26.promote(FigureName::Knight);
    check("promotion: nothing pending is rejected", b26.toFen(), startFen);

    Board b27("4k3/8/8/8/8/8/p7/4K3 b - - 0 1");
    b27.makeMove("a2", "a1");
    b27.promote(FigureName::Rook);
    check("promotion: black promotes to a rook", b27.toFen(), "4k3/8/8/8/8/8/8/r3K3 w - - 0 2");

    Board b28("rk5r/pppppppp/8/8/8/8/PPPPPPPP/RK5R w HAha - 0 1");
    check("960: rights survive the round-trip", b28.toFen(), "rk5r/pppppppp/8/8/8/8/PPPPPPPP/RK5R w HAha - 0 1");
    b28.makeMove("b1", "g1");
    check("960: kingside castle from b1", b28.toFen(), "rk5r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b ha - 1 1");
    b28.makeMove("b8", "a8");
    check("960: queenside as king takes own rook", b28.toFen(), "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 w - - 2 2");

    Board b29("4r2k/8/8/8/8/8/8/1K5R w H - 0 1");
    b29.makeMove("b1", "g1");
    check("960: no castling through an attacked square", b29.toFen(), "4r2k/8/8/8/8/8/8/1K5R w H - 0 1");

    Board b30("1r2k2r/8/8/8/8/8/8/1R2K2R w HBhb - 0 1");
    b30.makeMove("e1", "b1");
    check("960: castle with an inner rook", b30.toFen(), "1r2k2r/8/8/8/8/8/8/2KR3R b kb - 1 1");

    Board b31("4k3/8/8/8/8/8/8/R2K3R w KQ - 0 1");
    check("960: moved king drops stale KQ rights", b31.toFen(), "4k3/8/8/8/8/8/8/R2K3R w - - 0 1");
    b31.makeMove("d1", "b1");
    check("960: and castling is impossible", b31.toFen(), "4k3/8/8/8/8/8/8/R2K3R w - - 0 1");

    if (failedCount == 0) {
        std::cout << "\nAll tests passed" << std::endl;
    } else {
        std::cout << "\n" << failedCount << " tests failed" << std::endl;
    }

    return failedCount == 0 ? 0 : 1;
}
