import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../models/game_state.dart';
import '../models/position.dart';
import '../services/game_service.dart';
import 'cell_widget.dart';

class BoardWidget extends StatelessWidget {
  const BoardWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return Consumer<GameService>(
      builder: (context, gameService, child) {
        final gameState = gameService.gameState;
        final selectedCell = gameService.selectedCell;
        final selectedMove = gameService.selectedMove;

        // Get valid neighbors if player has selected their piece
        final validNeighbors = selectedCell != null
            ? gameState.getValidNeighbors(gameState.player2Position)
            : <Position>[];

        return Container(
          padding: const EdgeInsets.all(16),
          child: AspectRatio(
            aspectRatio: 1,
            child: Column(
              children: [
                // Column headers
                _buildColumnHeaders(),
                const SizedBox(height: 4),
                // Board
                Expanded(
                  child: Row(
                    children: [
                      // Row headers
                      _buildRowHeaders(),
                      const SizedBox(width: 4),
                      // Board grid
                      Expanded(
                        child: _buildBoard(
                          gameState,
                          gameService,
                          selectedCell,
                          selectedMove,
                          validNeighbors,
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
        );
      },
    );
  }

  Widget _buildColumnHeaders() {
    return Row(
      children: [
        const SizedBox(width: 30), // Space for row headers
        const SizedBox(width: 4),
        Expanded(
          child: Row(
            children: List.generate(7, (col) {
              return Expanded(
                child: Center(
                  child: Text(
                    '${col + 1}',
                    style: const TextStyle(
                      fontWeight: FontWeight.bold,
                      fontSize: 16,
                    ),
                  ),
                ),
              );
            }),
          ),
        ),
      ],
    );
  }

  Widget _buildRowHeaders() {
    return Column(
      children: List.generate(7, (row) {
        return Expanded(
          child: SizedBox(
            width: 30,
            child: Center(
              child: Text(
                String.fromCharCode(97 + row), // a, b, c, ...
                style: const TextStyle(
                  fontWeight: FontWeight.bold,
                  fontSize: 16,
                ),
              ),
            ),
          ),
        );
      }),
    );
  }

  Widget _buildBoard(
    GameState gameState,
    GameService gameService,
    Position? selectedCell,
    Position? selectedMove,
    List<Position> validNeighbors,
  ) {
    return Column(
      children: List.generate(7, (row) {
        return Expanded(
          child: Row(
            children: List.generate(7, (col) {
              final pos = Position(row, col);
              final cellState = gameState.board[row][col];
              final isSelected = selectedCell == pos || selectedMove == pos;
              final isValidMove = selectedMove == null &&
                  validNeighbors.contains(pos) &&
                  cellState == CellState.empty;

              // Highlight AI's last move
              final isAIHighlight = gameService.lastAIFrom == pos ||
                  gameService.lastAITo == pos ||
                  gameService.lastAIRemoved == pos;

              return Expanded(
                child: Padding(
                  padding: const EdgeInsets.all(2),
                  child: CellWidget(
                    position: pos,
                    state: cellState,
                    isSelected: isSelected,
                    isValidMove: isValidMove,
                    isAIHighlight: isAIHighlight,
                    onTap: () => gameService.selectCell(pos),
                  ),
                ),
              );
            }),
          ),
        );
      }),
    );
  }
}
