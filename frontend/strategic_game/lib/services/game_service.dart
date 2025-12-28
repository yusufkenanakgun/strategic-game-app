import 'package:flutter/foundation.dart';
import '../models/game_state.dart';
import '../models/position.dart';
import 'game_ffi.dart';

class GameService extends ChangeNotifier {
  late GameFFI _ffi;
  GameState _gameState = GameState.initial();
  Position? _selectedCell;
  Position? _selectedMove;
  bool _isAIThinking = false;
  String? _errorMessage;
  Position? _lastAIFrom;
  Position? _lastAITo;
  Position? _lastAIRemoved;

  GameState get gameState => _gameState;
  Position? get selectedCell => _selectedCell;
  Position? get selectedMove => _selectedMove;
  bool get isAIThinking => _isAIThinking;
  String? get errorMessage => _errorMessage;
  Position? get lastAIFrom => _lastAIFrom;
  Position? get lastAITo => _lastAITo;
  Position? get lastAIRemoved => _lastAIRemoved;

  GameService() {
    try {
      _ffi = GameFFI();
      _syncState();

      // If AI starts first, make AI move automatically
      if (_gameState.currentPlayer == Player.player1 && !_gameState.gameOver) {
        Future.delayed(const Duration(milliseconds: 500), makeAIMove);
      }
    } catch (e) {
      _errorMessage = 'Failed to initialize game: $e';
      debugPrint('GameService initialization error: $e');
    }
  }

  void _syncState() {
    try {
      final stateData = _ffi.getGameState();
      final boardList = stateData['board'] as List<int>;

      // Convert board (1D array to 2D)
      final board = List.generate(
        7,
        (row) => List.generate(
          7,
          (col) {
            final index = row * 7 + col;
            final cellValue = boardList[index];
            return CellState.values[cellValue];
          },
        ),
      );

      // Create new game state
      _gameState = GameState(
        board: board,
        player1Position: Position(stateData['player1Row'], stateData['player1Col']),
        player2Position: Position(stateData['player2Row'], stateData['player2Col']),
        currentPlayer:
            stateData['currentPlayer'] == 1 ? Player.player1 : Player.player2,
        turnCount: stateData['turnCount'],
        gameOver: stateData['gameOver'] == 1,
        winner: stateData['gameOver'] == 1
            ? (stateData['winner'] == 1 ? Player.player1 : Player.player2)
            : null,
      );

      _errorMessage = null;
      notifyListeners();
    } catch (e) {
      _errorMessage = 'Failed to sync game state: $e';
      debugPrint('Sync state error: $e');
      notifyListeners();
    }
  }

  void resetGame() {
    try {
      _ffi.initialize();
      _selectedCell = null;
      _selectedMove = null;
      _isAIThinking = false;
      _errorMessage = null;
      _lastAIFrom = null;
      _lastAITo = null;
      _lastAIRemoved = null;
      _syncState();

      // If AI starts first, make AI move automatically
      if (_gameState.currentPlayer == Player.player1 && !_gameState.gameOver) {
        Future.delayed(const Duration(milliseconds: 500), makeAIMove);
      }
    } catch (e) {
      _errorMessage = 'Failed to reset game: $e';
      debugPrint('Reset game error: $e');
      notifyListeners();
    }
  }

  void clearError() {
    _errorMessage = null;
    notifyListeners();
  }

  void selectCell(Position pos) {
    if (_gameState.gameOver || _isAIThinking) return;
    if (_gameState.currentPlayer != Player.player2) return;

    final playerPos = _gameState.player2Position;

    // If clicking on player's current position, deselect
    if (pos == playerPos && _selectedCell != null) {
      _selectedCell = null;
      _selectedMove = null;
      notifyListeners();
      return;
    }

    // If no cell selected yet, select the player's piece
    if (_selectedCell == null) {
      if (pos == playerPos) {
        _selectedCell = pos;
        notifyListeners();
      }
      return;
    }

    // If cell selected, check if this is a valid move
    if (_selectedCell != null && _selectedMove == null) {
      final validNeighbors = _gameState.getValidNeighbors(playerPos);
      if (validNeighbors.contains(pos)) {
        _selectedMove = pos;
        notifyListeners();
      } else {
        // Invalid move, reset selection
        _selectedCell = null;
        notifyListeners();
      }
      return;
    }

    // If both piece and move selected, this click is for removing a cell
    if (_selectedCell != null && _selectedMove != null) {
      // Try to make the move
      makePlayerMove(_selectedMove!, pos);
    }
  }

  void makePlayerMove(Position moveTo, Position removeCell) {
    if (_gameState.currentPlayer != Player.player2) return;

    final from = _gameState.player2Position;

    try {
      final success = _ffi.makeMove(
        from.row,
        from.col,
        moveTo.row,
        moveTo.col,
        removeCell.row,
        removeCell.col,
      );

      _selectedCell = null;
      _selectedMove = null;

      if (success) {
        _syncState();

        // If game not over and it's AI's turn, make AI move
        if (!_gameState.gameOver && _gameState.currentPlayer == Player.player1) {
          Future.delayed(const Duration(milliseconds: 800), makeAIMove);
        }
      } else {
        _errorMessage = 'Invalid move! Try again.';
        notifyListeners();
      }
    } catch (e) {
      _errorMessage = 'Move failed: $e';
      debugPrint('Make player move error: $e');
      notifyListeners();
    }
  }

  Future<void> makeAIMove() async {
    if (_gameState.currentPlayer != Player.player1) return;
    if (_gameState.gameOver) return;

    _isAIThinking = true;
    _lastAIFrom = null;
    _lastAITo = null;
    _lastAIRemoved = null;
    notifyListeners();

    try {
      // Run AI computation in isolate to prevent UI freeze
      await Future.delayed(const Duration(milliseconds: 200));

      final aiMove = await Future(() => _ffi.getAIMove())
          .timeout(const Duration(seconds: 10));

      if (aiMove != null) {
        _lastAIFrom = Position(aiMove.fromRow, aiMove.fromCol);
        _lastAITo = Position(aiMove.toRow, aiMove.toCol);
        _lastAIRemoved = Position(aiMove.removeRow, aiMove.removeCol);

        final success = _ffi.makeMove(
          aiMove.fromRow,
          aiMove.fromCol,
          aiMove.toRow,
          aiMove.toCol,
          aiMove.removeRow,
          aiMove.removeCol,
        );

        if (success) {
          _syncState();
          // Keep AI move highlight for a moment
          await Future.delayed(const Duration(milliseconds: 1500));
          _lastAIFrom = null;
          _lastAITo = null;
          _lastAIRemoved = null;
          notifyListeners();
        } else {
          _errorMessage = 'AI made an invalid move!';
        }
      } else {
        _errorMessage = 'AI could not find a valid move!';
      }
    } catch (e) {
      _errorMessage = 'AI move failed: $e';
      debugPrint('AI move error: $e');
    } finally {
      _isAIThinking = false;
      notifyListeners();
    }
  }

  @override
  void dispose() {
    try {
      _ffi.dispose();
    } catch (e) {
      debugPrint('Dispose error: $e');
    }
    super.dispose();
  }
}
