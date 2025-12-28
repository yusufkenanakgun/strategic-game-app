#include "../include/ai.h"
#include <algorithm>
#include <iostream>

AI::AI(Player player, int depth)
    : aiPlayer(player), maxDepth(depth), nodesEvaluated(0) {
    opponent = (player == Player::PLAYER1) ? Player::PLAYER2 : Player::PLAYER1;
}

Move AI::getBestMove(const Board& board) {
    nodesEvaluated = 0;

    std::vector<Move> possibleMoves = board.getAllPossibleMoves(aiPlayer);

    if (possibleMoves.empty()) {
        // No valid moves available
        return Move();
    }

    Move bestMove = possibleMoves[0];
    int bestScore = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    std::cout << "AI evaluating " << possibleMoves.size() << " possible moves..." << std::endl;

    // Evaluate each possible move
    for (const auto& move : possibleMoves) {
        // Create a copy of the board and apply the move
        Board tempBoard = board.copy();
        tempBoard.applyMove(move, aiPlayer);

        // Run minmax from opponent's perspective (minimizing)
        int score = minmax(tempBoard, maxDepth - 1, false, alpha, beta);

        // Update best move if this is better
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        // Update alpha
        alpha = std::max(alpha, bestScore);
    }

    std::cout << "Best move score: " << bestScore << " (Nodes evaluated: " << nodesEvaluated << ")" << std::endl;

    return bestMove;
}

int AI::minmax(Board& board, int depth, bool isMaximizing, int alpha, int beta) {
    nodesEvaluated++;

    // Terminal conditions
    if (depth == 0) {
        return evaluate(board);
    }

    Player currentPlayer = isMaximizing ? aiPlayer : opponent;

    // Check if current player can move
    if (!board.canPlayerMove(currentPlayer)) {
        // Current player loses
        if (isMaximizing) {
            // AI loses
            return -100000 - depth; // Prefer later losses
        } else {
            // Opponent loses, AI wins
            return 100000 + depth; // Prefer earlier wins
        }
    }

    std::vector<Move> possibleMoves = board.getAllPossibleMoves(currentPlayer);

    if (possibleMoves.empty()) {
        // No valid moves, current player loses
        if (isMaximizing) {
            return -100000 - depth;
        } else {
            return 100000 + depth;
        }
    }

    if (isMaximizing) {
        // Maximizing player (AI)
        int maxEval = std::numeric_limits<int>::min();

        for (const auto& move : possibleMoves) {
            Board tempBoard = board.copy();
            tempBoard.applyMove(move, currentPlayer);

            int eval = minmax(tempBoard, depth - 1, false, alpha, beta);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            // Alpha-Beta pruning
            if (beta <= alpha) {
                break;
            }
        }

        return maxEval;
    } else {
        // Minimizing player (Opponent)
        int minEval = std::numeric_limits<int>::max();

        for (const auto& move : possibleMoves) {
            Board tempBoard = board.copy();
            tempBoard.applyMove(move, currentPlayer);

            int eval = minmax(tempBoard, depth - 1, true, alpha, beta);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            // Alpha-Beta pruning
            if (beta <= alpha) {
                break;
            }
        }

        return minEval;
    }
}

int AI::evaluate(const Board& board) const {
    // Mobility heuristic: difference in number of possible moves
    int aiMobility = getMobility(board, aiPlayer);
    int oppMobility = getMobility(board, opponent);
    int mobilityScore = (aiMobility - oppMobility) * 10;

    // Position value heuristic: prefer central positions
    Position aiPos = board.getPlayerPosition(aiPlayer);
    Position oppPos = board.getPlayerPosition(opponent);
    int positionScore = getPositionValue(aiPos) - getPositionValue(oppPos);

    // Total score
    return mobilityScore + positionScore;
}

int AI::getMobility(const Board& board, Player player) const {
    Position playerPos = board.getPlayerPosition(player);
    std::vector<Position> validNeighbors = board.getValidNeighbors(playerPos);
    return validNeighbors.size();
}

int AI::getPositionValue(const Position& pos) const {
    // Prefer central positions
    // Center of 7x7 board is (3, 3)
    int centerRow = BOARD_SIZE / 2;
    int centerCol = BOARD_SIZE / 2;

    // Manhattan distance from center
    int distanceFromCenter = std::abs(pos.row - centerRow) + std::abs(pos.col - centerCol);

    // Higher value for positions closer to center
    return (BOARD_SIZE - distanceFromCenter) * 2;
}
