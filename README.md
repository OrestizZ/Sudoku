# Terminal Sudoku in C

A classic 9x9 Sudoku game that runs directly in your terminal. This project is written in pure C and uses a simple command-line interface for input.

## Authors

This project was created by:

* **Orestis Zappas** - @OrestizZ (https://github.com/OrestizZ)
* **Periklis Vovos** - @PerimanV (https://github.com/PerimanV)

as a university assignment for the **Department of Informatics and Telecommunications (University of Peloponnese)**.

---

## Features

* **Classic Sudoku Gameplay:** Play on a standard 9x9 grid.
* **Puzzle Generation:** Randomly hides cells from a complete puzzle (`solved.sud`) to create a new game board.
* **In-Game Help:**
    * Get a list of all possible valid numbers for any empty cell.
    * Reveal the correct solution for a specific cell.
* **Logical Solver:** Includes a feature to automatically solve the puzzle using logic (finds cells with only one possible solution).
* **Real-time Validation:** Checks your input against the correct solution instantly.

---

## Getting Started

Follow these instructions to compile and run the game on your local machine.

### Prerequisites

1.  **A C Compiler:** You will need `gcc` or any standard C compiler.
2.  **A Solved Puzzle File:** This program **requires** a file named `solved.sud` to be present in the same directory. This file must contain a valid, solved 9x9 Sudoku puzzle, with numbers separated by spaces.

    *Example `solved.sud` content:*
    ```
    5 3 4 6 7 8 9 1 2
    6 7 2 1 9 5 3 4 8
    1 9 8 3 4 2 5 6 7
    8 5 9 7 6 1 4 2 3
    4 2 6 8 5 3 7 9 1
    7 1 3 9 2 4 8 5 6
    9 6 1 5 3 7 2 8 4
    2 8 7 4 1 9 6 3 5
    3 4 5 2 8 6 1 7 9
    ```

### Compilation

1.  Clone the repository (or download the files):

2.  Compile the source code:
    ```bash
    gcc mySudoku.c -o mySudoku
    ```

---

## How to Play

### 1. Start the Game

You must use the `-n` flag to start a new game:

```bash
./mySudoku -n
```

Other command-line options:
* `./mySudoku -h`: Show the help message.
* `./mySudoku -l <filename>`: (This feature is planned but not fully implemented in the current version).

### 2. Game Commands

The game uses a specific `(row,column)=value` format for all inputs. **Rows and columns are 1-indexed (from 1 to 9).**

* **To Place a Number:**
    `(2,3)=4` (This will place the number **4** at **row 2, column 3**)

* **To Get Hints for a Cell:**
    `(2,3)=0` (This will show all possible valid numbers for **row 2, column 3**)

* **To Reveal a Cell's Solution:**
    `(2,3)=-1` (This will reveal the correct answer for **row 2, column 3**)

* **To Auto-Solve the Puzzle:**
    `(0,0)=0` (This will trigger the logical solver to fill in all "easy" cells)

* **To Quit the Game:**
    `(-1,-1)=-1`
