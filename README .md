# Chess

🚧 **Work in progress / academic project** — built a couple of years ago for a university course on Unreal Engine, and not actively maintained since. Shared here mainly as a code sample.

A grid-based two-player board game built in **Unreal Engine 5.2 (C++)**. One player is human-controlled (mouse input), the other is a simple AI that picks a random legal cell each turn.

## Features

- 8x8 board generated procedurally at runtime, with an alternating tile material pattern
- Turn-based flow shared between a human player and an AI opponent through a common `IPlayerInterface`
- Human input handled via Unreal's **Enhanced Input** system (click-to-select on the board)
- Basic AI opponent that selects a random empty tile on its turn
- Score tracking and turn/status messages via a `GameInstance`
- Board reset flow when a match ends

## Architecture

The project follows a fairly standard Unreal actor/GameState split:

| Class | Role |
|---|---|
| `AChess_gamemode` | Game state: owns the player list, current turn, win/reset flow |
| `AGameField` | Builds the board, spawns and tracks `ATile` actors |
| `ATile` | A single board cell; tracks whether it's empty/occupied and by whom |
| `IPlayerInterface` | Shared contract (`OnTurn` / `OnWin` / `OnLose`) implemented by both player types |
| `AHumanPlayer` | Pawn controlled by mouse click, implements `IPlayerInterface` |
| `ARandomPlayer` | Simple AI opponent, implements `IPlayerInterface` |
| `AChess_PlayerController` | Sets up Enhanced Input bindings for the human player |
| `UChess_GameInstance` | Persists score and turn messages across a match |

## Requirements

- Unreal Engine **5.2** or later
- Visual Studio 2022 (Windows) with the "Game development with C++" workload

## Getting started

1. Clone the repository
2. Right-click `Chess.uproject` → **Generate Visual Studio project files**
3. Open the generated `.sln` (or open `Chess.uproject` directly) and build the `Chess` target
4. Run from the editor or launch the built executable

## Known limitations / roadmap

This project started from a tic-tac-toe-style base and is still being adapted towards real chess rules, so a few things are intentionally incomplete:

- [x] Win condition implemented, but as an N-in-a-row check (`WinSize`, default 3) rather than real chess rules — this is a placeholder, not a from-scratch chess engine
- [ ] There's only one generic piece type (`ChessPawn`) — no distinct piece types or movement rules yet
- [ ] No legal-move validation (any empty tile can currently be selected)
- [ ] AI opponent is random rather than rule-aware

## License

No license file yet — all rights reserved by default until one is added.

## Author

Matteo Colombo
