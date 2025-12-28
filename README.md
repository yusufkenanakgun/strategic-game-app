# Strategic Board Game

A modern two-player strategic board game built with C++ backend and Flutter desktop frontend. Features an intelligent AI opponent powered by MinMax algorithm with Alpha-Beta pruning.

![Game Version](https://img.shields.io/badge/version-1.0.0-blue)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Technology Stack](#technology-stack)
- [Project Structure](#project-structure)
- [Game Rules](#game-rules)
- [Installation & Build](#installation--build)
- [Running the Application](#running-the-application)
- [Technical Details](#technical-details)
- [Screenshots](#screenshots)
- [Development Process](#development-process)

---

## Overview

This project was developed as part of CSE462 course, implementing a strategic board game where players compete on a 7x7 grid that shrinks with each turn. The game combines tactical positioning with resource management as players remove cells from the board, forcing both players to adapt their strategies dynamically.

**Key Highlights:**
- Cross-platform architecture (C++ backend + Flutter frontend)
- Advanced AI using MinMax algorithm with Alpha-Beta pruning
- Modern, gradient-based UI with smooth animations
- Real-time game state synchronization via FFI
- Optimized performance with depth-limited search

---

## Features

### Gameplay Features
- **Two-Player Mode:** Human vs AI
- **Smart AI Opponent:** Implements MinMax algorithm with depth-2 search
- **Dynamic Board:** Cells are progressively removed, creating strategic challenges
- **Move Validation:** Real-time validation of legal moves
- **Visual Feedback:** Highlighted valid moves and AI move indicators
- **Game Statistics:** Turn counter, player tracking, and game stats

### UI Features
- **Modern Gradient Design:** Professional UI with depth and shadows
- **Smooth Animations:** Piece placement and hover effects (200-300ms transitions)
- **Responsive Layout:** Fixed-size board (500x500px) with adaptive side panels
- **Information Panels:**
  - Left: Game rules and strategy tips
  - Center: Game board with status bar
  - Right: Turn instructions and live statistics
- **Visual States:**
  - Selected piece highlighting (amber border)
  - Valid move indicators (green cells)
  - AI move highlighting (orange fade effect)
  - Hover effects with scale transformation

---

## Technology Stack

### Backend (C++)
- **Language:** C++17
- **Build System:** CMake 4.2.1
- **Compiler:** MinGW-w64 (g++ with SEH exception handling)
- **AI Algorithm:** MinMax with Alpha-Beta Pruning
- **Export Method:** C-style DLL with FFI-compatible API

### Frontend (Flutter)
- **Framework:** Flutter 3.x (Dart SDK ^3.10.4)
- **Platform:** Windows Desktop
- **State Management:** Provider pattern (provider ^6.1.1)
- **FFI Integration:** dart:ffi with native bindings (ffi ^2.1.0)
- **UI Components:** Material Design 3

### Development Tools
- **Version Manager:** FVM (Flutter Version Management)
- **Package Manager:** pub/chocolatey
- **IDE:** Visual Studio 2022 Community (for C++ builds)

---

## Project Structure

```
462/
├── backend/                    # C++ Game Engine
│   ├── include/
│   │   ├── types.h            # Core data structures (Position, Move, CellState)
│   │   ├── board.h            # Board class declaration
│   │   ├── game.h             # Game state management
│   │   ├── ai.h               # AI MinMax implementation
│   │   └── api.h              # FFI export functions
│   ├── src/
│   │   ├── board.cpp          # Board logic implementation
│   │   ├── game.cpp           # Game flow control
│   │   ├── ai.cpp             # MinMax algorithm
│   │   ├── api.cpp            # DLL export implementation
│   │   └── main.cpp           # CLI test program
│   ├── build/
│   │   └── libgame_engine.dll # Compiled game engine
│   └── CMakeLists.txt         # CMake build configuration
│
├── frontend/strategic_game/    # Flutter Desktop App
│   ├── lib/
│   │   ├── main.dart          # App entry point
│   │   ├── models/
│   │   │   ├── position.dart  # Position model
│   │   │   └── game_state.dart # Game state model
│   │   ├── services/
│   │   │   ├── game_ffi.dart  # FFI bindings to C++ DLL
│   │   │   └── game_service.dart # State management service
│   │   ├── widgets/
│   │   │   ├── cell_widget.dart   # Individual cell component
│   │   │   └── board_widget.dart  # 7x7 board grid
│   │   └── screens/
│   │       └── game_screen.dart   # Main game UI
│   ├── windows/               # Windows platform configuration
│   └── pubspec.yaml           # Flutter dependencies
│
├── StrategicGameApp/          # Distributable Windows Application
│   ├── strategic_game.exe     # Main executable
│   ├── libgame_engine.dll     # Game engine
│   ├── flutter_windows.dll    # Flutter runtime
│   ├── libgcc_s_seh-1.dll     # MinGW runtime
│   ├── libstdc++-6.dll        # C++ standard library
│   ├── libwinpthread-1.dll    # POSIX threads
│   └── data/                  # Flutter assets
│
├── Term_Project.pdf           # Original project specification
└── README.md                  # This file
```

---

## Game Rules

### Objective
Trap your opponent so they cannot make a valid move.

### Setup
- 7x7 grid board
- Player 1 (AI) starts at position (0, 3) - top center
- Player 2 (Human) starts at position (6, 3) - bottom center
- All cells initially empty except removed corners

### Turn Flow
Each turn consists of **3 steps**:

1. **Select Your Piece:** Click on your colored piece (red for human, blue for AI)
2. **Move:** Click on an adjacent cell (8 directions: up, down, left, right, and diagonals)
3. **Remove Cell:** Click on any empty cell to remove it from the board permanently

### Valid Moves
- Can only move to adjacent empty cells (not removed cells)
- Cannot move to opponent's position
- Cannot move to removed cells
- Must have at least one valid move available

### Winning Condition
A player wins when their opponent has **no valid moves** available on their turn.

### Strategy Tips
- Control the center of the board for maximum mobility
- Plan 2-3 moves ahead
- Remove cells strategically to limit opponent's options
- Watch AI patterns to predict future moves
- Corner your opponent gradually

---

## Installation & Build

### Prerequisites

1. **MinGW-w64 (for C++ backend)**
   ```powershell
   choco install mingw
   ```

2. **CMake**
   ```powershell
   choco install cmake
   ```

3. **Flutter SDK & FVM**
   - Install Flutter: https://flutter.dev/docs/get-started/install/windows
   - Install FVM: `dart pub global activate fvm`
   - Set Flutter version: `fvm use stable`

4. **Visual Studio 2022** (for Windows desktop development)

### Build Backend (C++ DLL)

```bash
cd backend
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

Output: `backend/build/libgame_engine.dll`

### Build Frontend (Flutter)

```bash
cd frontend/strategic_game
fvm flutter pub get
fvm flutter build windows --release
```

Output: `frontend/strategic_game/build/windows/x64/runner/Release/`

### Package for Distribution

1. Copy all files from Release folder to a new directory:
   ```powershell
   xcopy "frontend\strategic_game\build\windows\x64\runner\Release\*" "StrategicGameApp\" /E /I /H /Y
   ```

2. Copy game engine DLL:
   ```powershell
   copy "backend\build\libgame_engine.dll" "StrategicGameApp\"
   ```

3. Copy MinGW runtime DLLs to `StrategicGameApp\`:
   - `libgcc_s_seh-1.dll`
   - `libstdc++-6.dll`
   - `libwinpthread-1.dll`

---

## Running the Application

### For Development

**Debug Mode:**
```bash
cd frontend/strategic_game
fvm flutter run -d windows
```

### For End Users

1. Navigate to `StrategicGameApp` folder
2. Double-click `strategic_game.exe`
3. Enjoy the game!

**Distribution:**
- Zip the entire `StrategicGameApp` folder
- Send to other Windows computers
- No installation required - just unzip and run!

---

## Technical Details

### MinMax Algorithm Implementation

The AI uses **MinMax with Alpha-Beta Pruning** for optimal move selection:

```cpp
int AI::minmax(Board& board, int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0 || board.isGameOver()) {
        return evaluateBoard(board);
    }

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (auto& move : board.getAllPossibleMoves(aiPlayer)) {
            board.applyMove(move);
            int eval = minmax(board, depth - 1, false, alpha, beta);
            board.undoMove(move);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break;  // Alpha-Beta pruning
        }
        return maxEval;
    }
    // ... minimizing logic
}
```

**Evaluation Function:**
- **Mobility Score:** Number of valid moves available
- **Position Value:** Bonus for controlling center positions
- **Endgame Bonus:** High value for winning states

**Performance:**
- Search Depth: 2 (configurable)
- Average Response Time: 1-2 seconds
- Pruned Nodes: ~60-70% reduction via Alpha-Beta

### FFI Integration

**Challenge:** Dart FFI doesn't support multi-dimensional arrays directly.

**Solution:** Flattened array representation

**C++ Side (api.cpp):**
```cpp
typedef struct {
    int board[49];  // 7x7 flattened to 1D
    int player1Row, player1Col;
    int player2Row, player2Col;
    int currentPlayer, turnCount, gameOver, winner;
} GameState;

extern "C" __declspec(dllexport)
void getGameState(Game* game, GameState* state) {
    for (int i = 0; i < 49; i++) {
        int row = i / 7;
        int col = i % 7;
        state->board[i] = static_cast<int>(game->getBoard().getCellState(row, col));
    }
    // ... populate other fields
}
```

**Dart Side (game_ffi.dart):**
```dart
class GameStateNative extends Struct {
  @Array(49)
  external Array<Int32> board;

  @Int32()
  external int player1Row;
  // ... other fields
}

Map<String, dynamic> getGameState() {
  final statePtr = calloc<GameStateNative>();
  _getGameState(_gameInstance, statePtr);

  final boardList = <int>[];
  for (int i = 0; i < 49; i++) {
    boardList.add(statePtr.ref.board[i]);
  }

  return {'board': boardList, /* ... */};
}
```

### State Management Flow

1. **User Action** → `CellWidget.onTap()`
2. **Service Update** → `GameService.selectCell()`
3. **FFI Call** → `GameFFI.makeMove()`
4. **C++ Execution** → `Game::makeMove()`
5. **State Sync** → `GameFFI.getGameState()`
6. **UI Update** → `ChangeNotifier.notifyListeners()`
7. **Widget Rebuild** → `Consumer<GameService>`

### Error Handling

- **FFI Timeout:** 10-second limit on AI moves
- **Move Validation:** Client and server-side validation
- **DLL Loading:** Try-catch with error messages
- **State Corruption:** Automatic state re-sync on errors

---

## Development Process

### Phase 1: Architecture Planning
1. Analyzed project requirements from PDF specification
2. Decided on C++ backend + Flutter frontend architecture
3. Designed FFI communication protocol
4. Sketched UI mockups

### Phase 2: Backend Development
1. Implemented core data structures (`Position`, `Move`, `CellState`)
2. Created `Board` class with move validation logic
3. Developed MinMax AI algorithm
   - Initial depth: 5 (too slow)
   - Optimized to depth: 3
   - Final optimization: depth 2 (~1-2s response)
4. Built FFI export layer with C-style API
5. Fixed 2D array FFI compatibility (flattened to 1D)

### Phase 3: Frontend Development
1. Set up Flutter desktop project with FVM
2. Created FFI bindings to C++ DLL
3. Implemented state management with Provider
4. Built core game widgets:
   - `CellWidget` with hover effects and animations
   - `BoardWidget` with 7x7 grid layout
   - `GameScreen` with 3-panel layout
5. Added visual feedback for game states

### Phase 4: UI/UX Enhancement
1. Redesigned with modern gradient theme
2. Added smooth animations (TweenAnimationBuilder)
3. Implemented hover scale effects (Matrix4 transforms)
4. Created information panels with game rules
5. Optimized layout to prevent overflow

### Phase 5: Integration & Testing
1. Resolved DLL loading issues (MinGW runtime dependencies)
2. Fixed FFI array access errors
3. Optimized AI performance (timeout handling)
4. Tested on multiple screen resolutions
5. Created distributable package

### Phase 6: Deployment
1. Built release version with optimizations
2. Packaged all dependencies
3. Created standalone executable
4. Tested on clean Windows environment
5. Documented build and distribution process

---

## Screenshots

### Main Game Screen
- **Left Panel:** Game rules, instructions, and strategy tips
- **Center:** 7x7 board with gradient styling and shadows
- **Right Panel:** Turn instructions and live game statistics

### Visual States
- **Player Pieces:** Gradient circles with "AI" and "YOU" labels
- **Selected State:** Amber border with highlight
- **Valid Moves:** Green cells with circle indicators
- **AI Thinking:** Loading spinner with message
- **Game Over:** Victory/defeat screen with celebration icons

---

## Future Enhancements

Potential improvements for future versions:

- [ ] Difficulty levels (Easy/Medium/Hard with different depths)
- [ ] Move history panel with undo functionality
- [ ] Custom board sizes (5x5, 9x9)
- [ ] Multiplayer over network (TCP/UDP)
- [ ] Save/load game states
- [ ] AI vs AI spectator mode
- [ ] Move time limits
- [ ] Statistics tracking (win rate, average game length)
- [ ] Custom themes and color schemes
- [ ] Sound effects and music

---

## License

MIT License - Feel free to use and modify for educational purposes.

---

## Credits

**Developer:** Yusuf Kenan Akgün
**Course:** CSE462
**Institution:** Yeditepe University
**Year:** 2025-2026

**Technologies Used:**
- C++ MinMax Algorithm
- Flutter Desktop Framework
- CMake Build System
- FFI (Foreign Function Interface)
- Provider State Management
- Material Design 3

---

## Contact

For questions, suggestions, or bug reports, please contact:
- Email: yusufkenanakgun@gmail.com
- GitHub: https://github.com/yusufkenanakgun

---

**Enjoy the game and may the best strategist win!**
