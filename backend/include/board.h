#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include <vector>
#include <array>

class Board {
private:
    // 7x7 grid to store cell states
    std::array<std::array<CellState, BOARD_SIZE>, BOARD_SIZE> grid;

    // Current positions of players
    Position player1Pos;
    Position player2Pos;

public:
    // Constructor
    Board();

    // Initialize board to starting configuration
    void initialize();

    // Get cell state at position
    CellState getCellState(const Position& pos) const;
    CellState getCellState(int row, int col) const;

    // Set cell state at position
    void setCellState(const Position& pos, CellState state);
    void setCellState(int row, int col, CellState state);

    // Get player positions
    Position getPlayerPosition(Player player) const;

    // Check if position is valid (within bounds)
    bool isValidPosition(const Position& pos) const;

    // Check if a cell is walkable (empty and not removed)
    bool isWalkable(const Position& pos) const;

    // Get all valid neighbor positions for a given position
    std::vector<Position> getValidNeighbors(const Position& pos) const;

    // Check if a move is valid
    bool isValidMove(const Move& move, Player player) const;

    // Apply a move to the board (returns true if successful)
    bool applyMove(const Move& move, Player player);

    // Get all possible moves for a player
    std::vector<Move> getAllPossibleMoves(Player player) const;

    // Check if a player can make any moves
    bool canPlayerMove(Player player) const;

    // Copy board state
    Board copy() const;

    // Display board (for debugging/CLI)
    void display() const;

    // Get grid (for direct access if needed)
    const std::array<std::array<CellState, BOARD_SIZE>, BOARD_SIZE>& getGrid() const {
        return grid;
    }
};

#endif // BOARD_H
