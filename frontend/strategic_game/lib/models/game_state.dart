import 'position.dart';

enum CellState {
  empty,
  player1,
  player2,
  removed,
}

enum Player {
  player1,
  player2,
}

class GameState {
  final List<List<CellState>> board;
  final Position player1Position;
  final Position player2Position;
  final Player currentPlayer;
  final int turnCount;
  final bool gameOver;
  final Player? winner;

  GameState({
    required this.board,
    required this.player1Position,
    required this.player2Position,
    required this.currentPlayer,
    required this.turnCount,
    required this.gameOver,
    this.winner,
  });

  factory GameState.initial() {
    final board = List.generate(
      7,
      (_) => List.generate(7, (_) => CellState.empty),
    );

    board[0][3] = CellState.player1;
    board[6][3] = CellState.player2;

    return GameState(
      board: board,
      player1Position: const Position(0, 3),
      player2Position: const Position(6, 3),
      currentPlayer: Player.player1,
      turnCount: 0,
      gameOver: false,
    );
  }

  GameState copyWith({
    List<List<CellState>>? board,
    Position? player1Position,
    Position? player2Position,
    Player? currentPlayer,
    int? turnCount,
    bool? gameOver,
    Player? winner,
  }) {
    return GameState(
      board: board ?? this.board,
      player1Position: player1Position ?? this.player1Position,
      player2Position: player2Position ?? this.player2Position,
      currentPlayer: currentPlayer ?? this.currentPlayer,
      turnCount: turnCount ?? this.turnCount,
      gameOver: gameOver ?? this.gameOver,
      winner: winner ?? this.winner,
    );
  }

  Position getPlayerPosition(Player player) {
    return player == Player.player1 ? player1Position : player2Position;
  }

  String getPlayerName(Player player) {
    return player == Player.player1 ? 'Player 1 (AI)' : 'Player 2 (Human)';
  }

  List<Position> getValidNeighbors(Position pos) {
    final neighbors = <Position>[];
    final directions = [
      [-1, -1], [-1, 0], [-1, 1],
      [0, -1], [0, 1],
      [1, -1], [1, 0], [1, 1],
    ];

    for (final dir in directions) {
      final newRow = pos.row + dir[0];
      final newCol = pos.col + dir[1];

      if (newRow >= 0 &&
          newRow < 7 &&
          newCol >= 0 &&
          newCol < 7 &&
          board[newRow][newCol] == CellState.empty) {
        neighbors.add(Position(newRow, newCol));
      }
    }

    return neighbors;
  }
}
