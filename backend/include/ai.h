#ifndef AI_H
#define AI_H

#include "board.h"
#include "game.h"
#include "types.h"
#include <limits>

class AI {
private:
    Player aiPlayer;        // AI's player (PLAYER1)
    Player opponent;        // Opponent player (PLAYER2)
    int maxDepth;           // Maximum search depth

    // Node count for performance tracking
    mutable long long nodesEvaluated;

    // MinMax with Alpha-Beta Pruning
    int minmax(Board& board, int depth, bool isMaximizing, int alpha, int beta);

    // Evaluation function (heuristic)
    int evaluate(const Board& board) const;

    // Mobility heuristic: count of valid moves
    int getMobility(const Board& board, Player player) const;

    // Position value heuristic: prefer central positions
    int getPositionValue(const Position& pos) const;

public:
    // Constructor
    AI(Player player = Player::PLAYER1, int depth = 5);

    // Get the best move for the current board state
    Move getBestMove(const Board& board);

    // Set search depth
    void setDepth(int depth) { maxDepth = depth; }

    // Get nodes evaluated (for debugging)
    long long getNodesEvaluated() const { return nodesEvaluated; }

    // Reset node counter
    void resetNodeCounter() { nodesEvaluated = 0; }
};

#endif // AI_H
