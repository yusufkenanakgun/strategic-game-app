#ifndef API_H
#define API_H

#ifdef _WIN32
    #define API_EXPORT __declspec(dllexport)
#else
    #define API_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Game state structure for FFI
typedef struct {
    int board[49];          // Flattened 7x7 board: 0=empty, 1=player1, 2=player2, 3=removed
    int player1Row;
    int player1Col;
    int player2Row;
    int player2Col;
    int currentPlayer;      // 1=player1, 2=player2
    int turnCount;
    int gameOver;           // 0=not over, 1=game over
    int winner;             // 1=player1, 2=player2
} GameState;

// Move structure for FFI
typedef struct {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    int removeRow;
    int removeCol;
} MoveData;

// API Functions
API_EXPORT void* createGame();
API_EXPORT void deleteGame(void* game);
API_EXPORT void initializeGame(void* game);
API_EXPORT void getGameState(void* game, GameState* state);
API_EXPORT int makeMove(void* game, MoveData* move);
API_EXPORT int getAIMove(void* game, MoveData* move);
API_EXPORT int isGameOver(void* game);
API_EXPORT int getWinner(void* game);
API_EXPORT int getCurrentPlayer(void* game);
API_EXPORT int canPlayerMove(void* game, int player);

#ifdef __cplusplus
}
#endif

#endif // API_H
