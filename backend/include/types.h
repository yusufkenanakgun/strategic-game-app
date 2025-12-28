#ifndef TYPES_H
#define TYPES_H

#include <utility>

// Cell states on the board
enum class CellState {
    EMPTY,      // Empty cell
    PLAYER1,    // AI player (Blue)
    PLAYER2,    // Human player (Red)
    REMOVED     // Removed cell (Gray)
};

// Player enumeration
enum class Player {
    PLAYER1,    // AI
    PLAYER2     // Human
};

// Position on the board (row, col)
struct Position {
    int row;
    int col;

    Position() : row(0), col(0) {}
    Position(int r, int c) : row(r), col(c) {}

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

// A complete move consists of:
// 1. Moving the piece to a new position
// 2. Removing a cell from the board
struct Move {
    Position from;          // Current position of the piece
    Position to;            // Target position for the piece
    Position removeCell;    // Cell to be removed from board

    Move() : from(), to(), removeCell() {}
    Move(Position f, Position t, Position r)
        : from(f), to(t), removeCell(r) {}

    bool isValid() const {
        // Basic validation
        return from != to;
    }
};

// Game constants
constexpr int BOARD_SIZE = 7;
constexpr int MIN_COORD = 0;
constexpr int MAX_COORD = BOARD_SIZE - 1;

// Initial positions
const Position PLAYER1_INITIAL = Position(0, 3);  // Row a, Column 4 (0-indexed: 0,3)
const Position PLAYER2_INITIAL = Position(6, 3);  // Row g, Column 4 (0-indexed: 6,3)

#endif // TYPES_H
