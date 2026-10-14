# Console-Based Minesweeper (C++)

A terminal-based implementation of the classic Minesweeper game written in C++. The project features multiple difficulty presets, custom board sizing via dynamic memory allocation, recursive tile-revealing mechanics, and persistent player statistics tracking.

---

## Features

- **Multiple Difficulty Modes & Custom Sizing:**
  - **Beginner:** 9×9 board with 10 mines
  - **Intermediate:** 16×16 board with 40 mines
  - **Hard:** 16×30 board with 99 mines
  - **Custom:** Fully configurable row, column, and mine counts
- **Core Game Mechanics:** Tile uncovering, flag placement and removal, coordinate input validation, and boundary checking.
- **Recursive Flood-Fill:** Automatically clears contiguous zero-mine tiles using recursive expansion.
- **Performance Timer:** Measures session duration from start to finish via `clock()`.
- **Persistent Leaderboard:**
  - Saves and loads player profiles, win/loss records, and fastest completion times to/from a flat file (`tableScore.txt`) using C++ file streams[cite: 1].
  - Sorts players hierarchically by wins, losses, and best time records[cite: 1].

---

## Prerequisites

- A standard C++ compiler supporting C++11 or higher (such as `g++`, `clang++`, or MSVC).
- **OS Support:** Windows, Linux, or macOS. *(Note: The game calls `system("Color 70")` for Windows terminal styling, but the core logic is cross-platform)*[cite: 1].

---

## Build & Run

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/Liyan-Kh/MineSweeper.git](https://github.com/Liyan-Kh/MineSweeper.git)
   cd MineSweeper
