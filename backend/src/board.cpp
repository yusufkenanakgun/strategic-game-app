#include "../include/board.h"
#include <iostream>
#include <iomanip>

Board::Board() {
    initialize();
}

void Board::initialize() {
    // Set all cells to empty
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            grid[i][j] = CellState::EMPTY;
        }
    }

    // Set initial player positions
    player1Pos = PLAYER1_INITIAL;
    player2Pos = PLAYER2_INITIAL;

    grid[player1Pos.row][player1Pos.col] = CellState::PLAYER1;
    grid[player2Pos.row][player2Pos.col] = CellState::PLAYER2;
}

CellState Board::getCellState(const Position& pos) const {
    return grid[pos.row][pos.col];
}

CellState Board::getCellState(int row, int col) const {
    return grid[row][col];
}

void Board::setCellState(const Position& pos, CellState state) {
    grid[pos.row][pos.col] = state;
}

void Board::setCellState(int row, int col, CellState state) {
    grid[row][col] = state;
}

Position Board::getPlayerPosition(Player player) const {
    return (player == Player::PLAYER1) ? player1Pos : player2Pos;
}

bool Board::isValidPosition(const Position& pos) const {
    return pos.row >= MIN_COORD && pos.row <= MAX_COORD &&
           pos.col >= MIN_COORD && pos.col <= MAX_COORD;
}

bool Board::isWalkable(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return false;
    }
    CellState state = getCellState(pos);
    return state == CellState::EMPTY;
}

std::vector<Position> Board::getValidNeighbors(const Position& pos) const {
    std::vector<Position> neighbors;

    // 8 directions: up, down, left, right, and 4 diagonals
    int dRow[] = {-1, -1, -1,  0,  0,  1,  1,  1};
    int dCol[] = {-1,  0,  1, -1,  1, -1,  0,  1};

    for (int i = 0; i < 8; i++) {
        Position newPos(pos.row + dRow[i], pos.col + dCol[i]);

        if (isValidPosition(newPos) && isWalkable(newPos)) {
            neighbors.push_back(newPos);
        }
    }

    return neighbors;
}

bool Board::isValidMove(const Move& move, Player player) const {
    // Check if the move structure is valid
    if (!move.isValid()) {
        return false;
    }

    // Verify the 'from' position matches the player's current position
    Position currentPos = getPlayerPosition(player);
    if (move.from != currentPos) {
        return false;
    }

    // Check if 'to' position is a valid neighbor
    std::vector<Position> validNeighbors = getValidNeighbors(currentPos);
    bool toIsValid = false;
    for (const auto& neighbor : validNeighbors) {
        if (neighbor == move.to) {
            toIsValid = true;
            break;
        }
    }
    if (!toIsValid) {
        return false;
    }

    // Check if removeCell is valid
    if (!isValidPosition(move.removeCell)) {
        return false;
    }

    // Cannot remove a cell that's already removed
    if (getCellState(move.removeCell) == CellState::REMOVED) {
        return false;
    }

    // Cannot remove a cell with a piece on it
    // After moving, check if removeCell has a piece
    if (move.removeCell == move.to) {
        return false; // Can't remove the cell we just moved to
    }

    // Check if removeCell has opponent's piece
    Position opponentPos = getPlayerPosition(
        player == Player::PLAYER1 ? Player::PLAYER2 : Player::PLAYER1
    );
    if (move.removeCell == opponentPos) {
        return false; // Can't remove opponent's cell
    }

    return true;
}

bool Board::applyMove(const Move& move, Player player) {
    if (!isValidMove(move, player)) {
        return false;
    }

    // Update player position
    setCellState(move.from, CellState::EMPTY);
    setCellState(move.to, player == Player::PLAYER1 ? CellState::PLAYER1 : CellState::PLAYER2);

    if (player == Player::PLAYER1) {
        player1Pos = move.to;
    } else {
        player2Pos = move.to;
    }

    // Remove the specified cell
    setCellState(move.removeCell, CellState::REMOVED);

    return true;
}

std::vector<Move> Board::getAllPossibleMoves(Player player) const {
    std::vector<Move> possibleMoves;

    Position currentPos = getPlayerPosition(player);
    std::vector<Position> validNeighbors = getValidNeighbors(currentPos);

    // For each valid neighbor position
    for (const auto& toPos : validNeighbors) {
        // For each cell on the board, check if it can be removed
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                Position removePos(row, col);

                // Skip if cell is already removed
                if (getCellState(removePos) == CellState::REMOVED) {
                    continue;
                }

                // Skip if cell is the target position
                if (removePos == toPos) {
                    continue;
                }

                // Skip if cell is opponent's position
                Player opponent = (player == Player::PLAYER1) ? Player::PLAYER2 : Player::PLAYER1;
                Position opponentPos = getPlayerPosition(opponent);
                if (removePos == opponentPos) {
                    continue;
                }

                // This is a valid move
                Move move(currentPos, toPos, removePos);
                possibleMoves.push_back(move);
            }
        }
    }

    return possibleMoves;
}

bool Board::canPlayerMove(Player player) const {
    Position currentPos = getPlayerPosition(player);
    std::vector<Position> validNeighbors = getValidNeighbors(currentPos);
    return !validNeighbors.empty();
}

Board Board::copy() const {
    Board newBoard;
    newBoard.grid = this->grid;
    newBoard.player1Pos = this->player1Pos;
    newBoard.player2Pos = this->player2Pos;
    return newBoard;
}

void Board::display() const {
    std::cout << std::endl << "  ";
    for (int col = 0; col < BOARD_SIZE; col++) {
        std::cout << "  " << (col + 1) << " ";
    }
    std::cout << std::endl;

    for (int row = 0; row < BOARD_SIZE; row++) {
        std::cout << "  " << char('a' + row) << " ";
        for (int col = 0; col < BOARD_SIZE; col++) {
            CellState state = grid[row][col];
            switch (state) {
                case CellState::EMPTY:
                    std::cout << "[ ]";
                    break;
                case CellState::PLAYER1:
                    std::cout << "[B]"; // Blue (AI)
                    break;
                case CellState::PLAYER2:
                    std::cout << "[R]"; // Red (Human)
                    break;
                case CellState::REMOVED:
                    std::cout << "[X]"; // Removed
                    break;
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
