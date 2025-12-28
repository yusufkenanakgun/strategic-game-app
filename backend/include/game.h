#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "types.h"

class Game {
private:
    Board board;
    Player currentPlayer;
    bool gameOver;
    Player winner;
    int turnCount;

public:
    // Constructor
    Game();

    // Initialize a new game
    void initialize();

    // Get current board state
    const Board& getBoard() const { return board; }
    Board& getBoard() { return board; }

    // Get current player
    Player getCurrentPlayer() const { return currentPlayer; }

    // Check if game is over
    bool isGameOver() const { return gameOver; }

    // Get winner (only valid if game is over)
    Player getWinner() const { return winner; }

    // Get turn count
    int getTurnCount() const { return turnCount; }

    // Make a move (returns true if successful)
    bool makeMove(const Move& move);

    // Switch to next player
    void switchPlayer();

    // Check and update game over status
    void checkGameOver();

    // Get all valid moves for current player
    std::vector<Move> getValidMoves() const;

    // Display game state (for debugging)
    void display() const;
};

#endif // GAME_H
