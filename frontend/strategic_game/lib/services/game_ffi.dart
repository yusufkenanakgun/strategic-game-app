import 'dart:ffi' as ffi;
import 'dart:io';
import 'package:ffi/ffi.dart';

// GameState structure (matches C++ struct)
final class GameStateNative extends ffi.Struct {
  @ffi.Array(49)
  external ffi.Array<ffi.Int32> board;

  @ffi.Int32()
  external int player1Row;

  @ffi.Int32()
  external int player1Col;

  @ffi.Int32()
  external int player2Row;

  @ffi.Int32()
  external int player2Col;

  @ffi.Int32()
  external int currentPlayer;

  @ffi.Int32()
  external int turnCount;

  @ffi.Int32()
  external int gameOver;

  @ffi.Int32()
  external int winner;
}

// MoveData structure (matches C++ struct)
final class MoveDataNative extends ffi.Struct {
  @ffi.Int32()
  external int fromRow;

  @ffi.Int32()
  external int fromCol;

  @ffi.Int32()
  external int toRow;

  @ffi.Int32()
  external int toCol;

  @ffi.Int32()
  external int removeRow;

  @ffi.Int32()
  external int removeCol;
}

// Function signatures
typedef CreateGameNative = ffi.Pointer<ffi.Void> Function();
typedef CreateGameDart = ffi.Pointer<ffi.Void> Function();

typedef DeleteGameNative = ffi.Void Function(ffi.Pointer<ffi.Void>);
typedef DeleteGameDart = void Function(ffi.Pointer<ffi.Void>);

typedef InitializeGameNative = ffi.Void Function(ffi.Pointer<ffi.Void>);
typedef InitializeGameDart = void Function(ffi.Pointer<ffi.Void>);

typedef GetGameStateNative = ffi.Void Function(
    ffi.Pointer<ffi.Void>, ffi.Pointer<GameStateNative>);
typedef GetGameStateDart = void Function(
    ffi.Pointer<ffi.Void>, ffi.Pointer<GameStateNative>);

typedef MakeMoveNative = ffi.Int32 Function(
    ffi.Pointer<ffi.Void>, ffi.Pointer<MoveDataNative>);
typedef MakeMoveDart = int Function(
    ffi.Pointer<ffi.Void>, ffi.Pointer<MoveDataNative>);

typedef GetAIMoveNative = ffi.Int32 Function(
    ffi.Pointer<ffi.Void>, ffi.Pointer<MoveDataNative>);
typedef GetAIMoveDart = int Function(
    ffi.Pointer<ffi.Void>, ffi.Pointer<MoveDataNative>);

typedef IsGameOverNative = ffi.Int32 Function(ffi.Pointer<ffi.Void>);
typedef IsGameOverDart = int Function(ffi.Pointer<ffi.Void>);

typedef GetWinnerNative = ffi.Int32 Function(ffi.Pointer<ffi.Void>);
typedef GetWinnerDart = int Function(ffi.Pointer<ffi.Void>);

typedef GetCurrentPlayerNative = ffi.Int32 Function(ffi.Pointer<ffi.Void>);
typedef GetCurrentPlayerDart = int Function(ffi.Pointer<ffi.Void>);

class GameFFI {
  late ffi.DynamicLibrary _dylib;
  late ffi.Pointer<ffi.Void> _gameInstance;

  late CreateGameDart _createGame;
  late DeleteGameDart _deleteGame;
  late InitializeGameDart _initializeGame;
  late GetGameStateDart _getGameState;
  late MakeMoveDart _makeMove;
  late GetAIMoveDart _getAIMove;
  late IsGameOverDart _isGameOver;
  late GetWinnerDart _getWinner;
  late GetCurrentPlayerDart _getCurrentPlayer;

  GameFFI() {
    // Load the DLL
    final libraryPath = _getLibraryPath();
    _dylib = ffi.DynamicLibrary.open(libraryPath);

    // Bind functions
    _createGame = _dylib
        .lookup<ffi.NativeFunction<CreateGameNative>>('createGame')
        .asFunction();
    _deleteGame = _dylib
        .lookup<ffi.NativeFunction<DeleteGameNative>>('deleteGame')
        .asFunction();
    _initializeGame = _dylib
        .lookup<ffi.NativeFunction<InitializeGameNative>>('initializeGame')
        .asFunction();
    _getGameState = _dylib
        .lookup<ffi.NativeFunction<GetGameStateNative>>('getGameState')
        .asFunction();
    _makeMove = _dylib
        .lookup<ffi.NativeFunction<MakeMoveNative>>('makeMove')
        .asFunction();
    _getAIMove = _dylib
        .lookup<ffi.NativeFunction<GetAIMoveNative>>('getAIMove')
        .asFunction();
    _isGameOver = _dylib
        .lookup<ffi.NativeFunction<IsGameOverNative>>('isGameOver')
        .asFunction();
    _getWinner = _dylib
        .lookup<ffi.NativeFunction<GetWinnerNative>>('getWinner')
        .asFunction();
    _getCurrentPlayer = _dylib
        .lookup<ffi.NativeFunction<GetCurrentPlayerNative>>('getCurrentPlayer')
        .asFunction();

    // Create game instance
    _gameInstance = _createGame();
  }

  String _getLibraryPath() {
    // Path to the DLL
    final exePath = Platform.resolvedExecutable;
    final exeDir = File(exePath).parent.path;
    return '$exeDir\\libgame_engine.dll';
  }

  void initialize() {
    _initializeGame(_gameInstance);
  }

  Map<String, dynamic> getGameState() {
    final statePtr = calloc<GameStateNative>();
    _getGameState(_gameInstance, statePtr);
    final state = statePtr.ref;

    // Convert board array to List
    final boardList = <int>[];
    for (int i = 0; i < 49; i++) {
      boardList.add(state.board[i]);
    }

    final result = {
      'board': boardList,
      'player1Row': state.player1Row,
      'player1Col': state.player1Col,
      'player2Row': state.player2Row,
      'player2Col': state.player2Col,
      'currentPlayer': state.currentPlayer,
      'turnCount': state.turnCount,
      'gameOver': state.gameOver,
      'winner': state.winner,
    };

    calloc.free(statePtr);
    return result;
  }

  bool makeMove(int fromRow, int fromCol, int toRow, int toCol,
      int removeRow, int removeCol) {
    final movePtr = calloc<MoveDataNative>();
    movePtr.ref.fromRow = fromRow;
    movePtr.ref.fromCol = fromCol;
    movePtr.ref.toRow = toRow;
    movePtr.ref.toCol = toCol;
    movePtr.ref.removeRow = removeRow;
    movePtr.ref.removeCol = removeCol;

    final result = _makeMove(_gameInstance, movePtr);
    calloc.free(movePtr);
    return result == 1;
  }

  MoveDataNative? getAIMove() {
    final movePtr = calloc<MoveDataNative>();
    final result = _getAIMove(_gameInstance, movePtr);

    if (result == 1) {
      final move = movePtr.ref;
      calloc.free(movePtr);
      return move;
    }

    calloc.free(movePtr);
    return null;
  }

  bool isGameOver() {
    return _isGameOver(_gameInstance) == 1;
  }

  int getWinner() {
    return _getWinner(_gameInstance);
  }

  int getCurrentPlayer() {
    return _getCurrentPlayer(_gameInstance);
  }

  void dispose() {
    _deleteGame(_gameInstance);
  }
}
