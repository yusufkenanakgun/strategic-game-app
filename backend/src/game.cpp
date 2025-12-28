#include "../include/game.h"
#include <iostream>

Game::Game() {
    initialize();
}

void Game::initialize() {
    board.initialize();
    currentPlayer = Player::PLAYER1; // AI starts first
    gameOver = false;
    winner = Player::PLAYER1; // Default, not meaningful until game is over
    turnCount = 0;
}

bool Game::makeMove(const Move& move) {
    // Check if game is already over
    if (gameOver) {
        return false;
    }

    // Try to apply the move
    if (!board.applyMove(move, currentPlayer)) {
        return false;
    }

    // Move was successful
    turnCount++;

    // Switch to the other player
    switchPlayer();

    // Check if the game is over
    checkGameOver();

    return true;
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == Player::PLAYER1) ? Player::PLAYER2 : Player::PLAYER1;
}

void Game::checkGameOver() {
    // Game is over if the current player cannot make any moves
    if (!board.canPlayerMove(currentPlayer)) {
        gameOver = true;
        // The winner is the OTHER player (the one who made the last move)
        winner = (currentPlayer == Player::PLAYER1) ? Player::PLAYER2 : Player::PLAYER1;
    }
}

std::vector<Move> Game::getValidMoves() const {
    return board.getAllPossibleMoves(currentPlayer);
}

void Game::display() const {
    std::cout << "=== Strategic Board Game ===" << std::endl;
    std::cout << "Turn: " << turnCount << std::endl;
    std::cout << "Current Player: " << (currentPlayer == Player::PLAYER1 ? "Player 1 (AI/Blue)" : "Player 2 (Human/Red)") << std::endl;

    board.display();

    if (gameOver) {
        std::cout << "*** GAME OVER ***" << std::endl;
        std::cout << "Winner: " << (winner == Player::PLAYER1 ? "Player 1 (AI)" : "Player 2 (Human)") << std::endl;
    }
}
