Chess Engine
====================

A chess engine powered by lookup tables and bad code.

## How this works ##
To start a game, initialize a `struct Board` and define the basics for each player. I'll work on creating a function to set up the board later.

The game loop follows as such:
1. At the start of each turn, check if the game is over.
  1. The active player generates a list of valid moves using `genMoves(struct Board* board)`. If no valid moves can be generated, the game is over.
  2. `checkStalemate(struct Board* board)` checks if either player has lost all pieces except for their king. If so, the game is over.
2. If the game is not over, a move can then be selected from the table, and `doMove(struct Board* board, int8_t moveNum)`.
3. `postMoveActions(struct Board* board, int8_t moveNum)` handles any additional actions such as taking pieces and clears all generated moves.
4. `switchTurn(struct Board* board)` changes the turn, enabling the game loop to start again.

## Using ##
You can either compile it as is, or include it in your project.

### Compiling ###
```make
make all
cd examples
make all
```

This produces three files:
* `random.sh` shows a game where the computer makes random moves until the game is over.
* `insrand.sh` reproduces a random game similar to random.sh, but only displays instructions.
* `twoplayer.sh` creates an ncurses interface for two players to play chess.

### Including ###
This can be included as a shared/static library or directly from source code.

#### As a Shared Library ####
Compile the code as a shared library,...
```make
make shared
```
... add the following header file,...
```cpp
#include <ChessEngine.h>
```
...and link the library.
```make
gcc -std=c99 <your_files> -L<path_to_folder/chessengine> -lchessengine -I<path_to_folder/chessengine> -o <program_name>
```

When running your program, either move libchessengine.so to a valid folder or prepend `LD_LIBRARY_PATH=<path_to_folder/chessengine>` to your application.

#### As a Static Library ####
Compile the code as a shared library,...
```make
make static
```
...add the following header file,...
```cpp
#include <ChessEngine.h>
```
...and link the library.
```make
gcc -std=c99 -static <your_files> -L<path_to_folder/chessengine> -lchessengine -I<path_to_folder/chessengine> -o <program_name>
```

#### From Source ####
Add the following header file,...
```cpp
#include "ChessEngine.h"
```
...and include the appropriate files
```make
gcc -std=c99 <your_files> <path_to_folder/chessengine>/*.c -I<path_to_folder/chessengine> -o <program_name>
```
## Contributing ##
All help is appreciated.

Please do not directly modify chessconstants.c. It is comprised of five generators plus a wrapper for those generators. If there is a problem with any of the lookup tables, fix the associated generator file (or the wrapper). I'm alright if you regenerate chessconstants.c - just don't make manual changes to the file.

