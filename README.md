# Gaming Hub (C++ / SFML)

A GUI desktop app that puts three games, **Tic-Tac-Toe**, **Snake** and **Pong**, behind one main menu, with a shared leaderboard.
Built for the Object-Oriented Programming course (CS-1004) at FAST-NUCES Islamabad.

## Features

- Three playable games: Tic-Tac-Toe, Snake and Pong
- Player name entry before each game
- Leaderboard that saves the top 10 scores to `scores.txt`, sorts them highest first, and can be filtered by game
- Sound effects through a shared sound manager
- Screen flow handled by a game-state machine (menu → name input → game → leaderboard)

## Built With

- C++ (object-oriented design: classes, encapsulation, file I/O)
- SFML 3 (Graphics and Audio)
- Visual Studio

## Project Structure

| File | Purpose |
|---|---|
| `MainHub` | Main loop and switching between screens |
| `MainMenu` | Game selection menu |
| `NameInput` | Player name entry |
| `TicTacToe`, `SnakeGame`, `PongGame` | The three games |
| `Leaderboard` | Loads, saves, sorts, filters and draws scores |
| `common.h` | Shared constants, structs and the sound manager |

## How to Run

1. Install SFML 3 and link it in Visual Studio.
2. Open `i252086_Assignment2.slnx` and build the project.
3. Put the background images (`bg_menu.png`, `bg_tictactoe.png`, `bg_snake.png`, `bg_pong.png`, `bg_leaderboard.png`) and the `sounds/` folder next to the executable.
4. Run.
