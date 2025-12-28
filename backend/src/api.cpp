#include "../include/api.h"
#include "../include/game.h"
#include "../include/ai.h"
#include <cstring>

// Internal helper to convert CellState to int
int cellStateToInt(CellState state) {
    switch (state) {
        case CellState::EMPTY: return 0;
        case CellState::PLAYER1: return 1;
        case CellState::PLAYER2: return 2;
        case CellState::REMOVED: return 3;
        default: return 0;
    }
}

// Create a new game instance
void* createGame() {
    return new Game();
}

// Delete a game instance
void deleteGame(void* game) {
    if (game) {
        delete static_cast<Game*>(game);
    }
}

// Initialize the game
void initializeGame(void* game) {
    if (game) {
        static_cast<Game*>(game)->initialize();
    }
}

// Get current game state
void getGameState(void* game, GameState* state) {
    if (!game || !state) return;

    Game* g = static_cast<Game*>(game);
    const Board& board = g->getBoard();

    // Copy board state (flattened to 1D array)
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 7; col++) {
            state->board[row * 7 + col] = cellStateToInt(board.getCellState(row, col));
        }
    }

    // Copy player positions
    Position p1Pos = board.getPlayerPosition(Player::PLAYER1);
    Position p2Pos = board.getPlayerPosition(Player::PLAYER2);

    state->player1Row = p1Pos.row;
    state->player1Col = p1Pos.col;
    state->player2Row = p2Pos.row;
    state->player2Col = p2Pos.col;

    // Game info
    state->currentPlayer = (g->getCurrentPlayer() == Player::PLAYER1) ? 1 : 2;
    state->turnCount = g->getTurnCount();
    state->gameOver = g->isGameOver() ? 1 : 0;
    state->winner = (g->getWinner() == Player::PLAYER1) ? 1 : 2;
}

// Make a move
int makeMove(void* game, MoveData* moveData) {
    if (!game || !moveData) return 0;

    Game* g = static_cast<Game*>(game);

    Move move(
        Position(moveData->fromRow, moveData->fromCol),
        Position(moveData->toRow, moveData->toCol),
        Position(moveData->removeRow, moveData->removeCol)
    );

    return g->makeMove(move) ? 1 : 0;
}

// Get AI's best move
int getAIMove(void* game, MoveData* moveData) {
    if (!game || !moveData) return 0;

    Game* g = static_cast<Game*>(game);
    AI ai(Player::PLAYER1, 2); // Depth 2 for fast performance

    Move bestMove = ai.getBestMove(g->getBoard());

    if (!bestMove.isValid()) {
        return 0;
    }

    moveData->fromRow = bestMove.from.row;
    moveData->fromCol = bestMove.from.col;
    moveData->toRow = bestMove.to.row;
    moveData->toCol = bestMove.to.col;
    moveData->removeRow = bestMove.removeCell.row;
    moveData->removeCol = bestMove.removeCell.col;

    return 1;
}

// Check if game is over
int isGameOver(void* game) {
    if (!game) return 0;
    return static_cast<Game*>(game)->isGameOver() ? 1 : 0;
}

// Get winner
int getWinner(void* game) {
    if (!game) return 0;
    Game* g = static_cast<Game*>(game);
    return (g->getWinner() == Player::PLAYER1) ? 1 : 2;
}

// Get current player
int getCurrentPlayer(void* game) {
    if (!game) return 0;
    Game* g = static_cast<Game*>(game);
    return (g->getCurrentPlayer() == Player::PLAYER1) ? 1 : 2;
}

// Check if a player can move
int canPlayerMove(void* game, int player) {
    if (!game) return 0;
    Game* g = static_cast<Game*>(game);
    Player p = (player == 1) ? Player::PLAYER1 : Player::PLAYER2;
    return g->getBoard().canPlayerMove(p) ? 1 : 0;
}
