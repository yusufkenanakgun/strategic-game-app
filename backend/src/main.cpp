#include "../include/game.h"
#include "../include/ai.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <string>

// Helper function to parse user input
bool parseInput(const std::string& input, Position& to, Position& removeCell) {
    // Expected format: "to_row to_col remove_row remove_col"
    // Example: "b 4 c 5" means move to (b,4) and remove (c,5)
    std::istringstream iss(input);
    char toRowChar, removeRowChar;
    int toCol, removeCol;

    if (!(iss >> toRowChar >> toCol >> removeRowChar >> removeCol)) {
        return false;
    }

    // Convert to 0-indexed positions
    to.row = toRowChar - 'a';
    to.col = toCol - 1;
    removeCell.row = removeRowChar - 'a';
    removeCell.col = removeCol - 1;

    return true;
}

void displayHelp() {
    std::cout << "\n=== How to Play ===" << std::endl;
    std::cout << "Enter your move in the format: to_row to_col remove_row remove_col" << std::endl;
    std::cout << "Example: 'b 4 c 5' means:" << std::endl;
    std::cout << "  - Move your piece to position (b, 4)" << std::endl;
    std::cout << "  - Remove cell at position (c, 5)" << std::endl;
    std::cout << "Rows are labeled a-g, columns are labeled 1-7" << std::endl;
    std::cout << "Type 'help' for this message, 'quit' to exit" << std::endl << std::endl;
}

int main() {
    std::cout << "=== Strategic Board Game - AI Demo ===" << std::endl;
    std::cout << "Player 1 (Blue/AI) vs Player 2 (Red/Human)" << std::endl << std::endl;

    displayHelp();

    std::cout << "[DEBUG] Initializing game..." << std::endl;
    // Initialize game
    Game game;
    AI ai(Player::PLAYER1, 3); // AI with depth 3 (faster for testing)

    std::cout << "[DEBUG] Starting game loop..." << std::endl;
    // Game loop
    while (!game.isGameOver()) {
        std::cout << "[DEBUG] Displaying board..." << std::endl;
        game.display();
        std::cout << "[DEBUG] Board displayed." << std::endl;

        Player currentPlayer = game.getCurrentPlayer();

        if (currentPlayer == Player::PLAYER1) {
            // AI's turn
            std::cout << "\n>>> AI (Player 1) is thinking..." << std::endl;

            Move aiMove = ai.getBestMove(game.getBoard());

            std::cout << "AI moves from (" << char('a' + aiMove.from.row) << ", " << (aiMove.from.col + 1) << ") ";
            std::cout << "to (" << char('a' + aiMove.to.row) << ", " << (aiMove.to.col + 1) << ")" << std::endl;
            std::cout << "AI removes cell (" << char('a' + aiMove.removeCell.row) << ", " << (aiMove.removeCell.col + 1) << ")" << std::endl;

            if (!game.makeMove(aiMove)) {
                std::cout << "ERROR: AI made an invalid move!" << std::endl;
                break;
            }

            std::cout << "\nPress Enter to continue..." << std::flush;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        } else {
            // Human's turn
            std::cout << "\n>>> Your turn (Player 2)!" << std::endl;

            Position currentPos = game.getBoard().getPlayerPosition(Player::PLAYER2);
            std::cout << "Your current position: (" << char('a' + currentPos.row) << ", " << (currentPos.col + 1) << ")" << std::endl;

            // Show valid neighbor positions
            std::vector<Position> validNeighbors = game.getBoard().getValidNeighbors(currentPos);
            std::cout << "Valid moves: ";
            for (const auto& pos : validNeighbors) {
                std::cout << "(" << char('a' + pos.row) << "," << (pos.col + 1) << ") ";
            }
            std::cout << std::endl;

            std::cout << "Enter your move (or 'help'/'quit'): " << std::flush;
            std::string input;
            std::getline(std::cin, input);

            if (input == "quit" || input == "q") {
                std::cout << "Exiting game..." << std::endl;
                break;
            }

            if (input == "help" || input == "h") {
                displayHelp();
                continue;
            }

            Position to, removeCell;
            if (!parseInput(input, to, removeCell)) {
                std::cout << "Invalid input format! Try again." << std::endl;
                continue;
            }

            Move playerMove(currentPos, to, removeCell);

            if (!game.makeMove(playerMove)) {
                std::cout << "Invalid move! Try again." << std::endl;
                continue;
            }
        }
    }

    // Game over
    game.display();

    return 0;
}
