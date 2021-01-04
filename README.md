Chess Engine
====================

A chess engine powered by lookup tables and bad code.

## How this works ##
There are two key files to consider reading:
* `ChessEngine.h` contains all the necessary structs and functions needed to run the engine.
* `ChessConstants.h` contains all contants used by the engine.

To start a game, initialize a `struct Board` and call `void initBoard(struct Board* board)` to setup the board with logical defaults.
```c
struct Board defaults = (struct Board) {
	white: (struct Player) {
		color: CP_WHITE,
		kingCheck: CP_NONE,
		kingPos: 60,
		rook_Q:   { pos: 56, color: CP_WHITE, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
		knight_Q: { pos: 57, color: CP_WHITE, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
		bishop_Q: { pos: 58, color: CP_WHITE, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
		queen:    { pos: 59, color: CP_WHITE, type: CP_QUEEN,  taken: false, hasMoved: false, jumped: false },
		king:     { pos: 60, color: CP_WHITE, type: CP_KING,   taken: false, hasMoved: false, jumped: false },
		bishop_K: { pos: 61, color: CP_WHITE, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
		knight_K: { pos: 62, color: CP_WHITE, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
		rook_K:   { pos: 63, color: CP_WHITE, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
		pawn_RQ:  { pos: 48, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_NQ:  { pos: 49, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_BQ:  { pos: 50, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_Q:   { pos: 51, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_K:   { pos: 52, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_BK:  { pos: 53, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_NK:  { pos: 54, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_RK:  { pos: 55, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
	},
	black: (struct Player) {
		color: CP_BLACK,
		kingCheck: CP_NONE,
		kingPos: 4,
		rook_Q:   { pos:  0, color: CP_BLACK, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
		knight_Q: { pos:  1, color: CP_BLACK, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
		bishop_Q: { pos:  2, color: CP_BLACK, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
		queen:    { pos:  3, color: CP_BLACK, type: CP_QUEEN,  taken: false, hasMoved: false, jumped: false },
		king:     { pos:  4, color: CP_BLACK, type: CP_KING,   taken: false, hasMoved: false, jumped: false },
		bishop_K: { pos:  5, color: CP_BLACK, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
		knight_K: { pos:  6, color: CP_BLACK, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
		rook_K:   { pos:  7, color: CP_BLACK, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
		pawn_RQ:  { pos:  8, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_NQ:  { pos:  9, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_BQ:  { pos: 10, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_Q:   { pos: 11, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_K:   { pos: 12, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_BK:  { pos: 13, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_NK:  { pos: 14, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		pawn_RK:  { pos: 15, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
	},
	active: CP_WHITE
};
```

The game loop follows as such:
1. At the start of each turn, check if the game is over.
   1. The active player generates a list of valid moves using `size_t genMoves(struct Board* board)`. If no valid moves can be generated, the game is over.
   2. `bool checkStalemate(struct Board* board)` checks if either player has lost all pieces except for their king. If so, the game is over.
2. If the game is not over, a move can then be selected from the table, and `void doMove(struct Board* board, int8_t moveNum)`.
3. `void postMoveActions(struct Board* board, int8_t moveNum)` handles any additional actions such as taking/transforming pieces, updating check statuses, and clearing all generated moves.
4. `void switchTurn(struct Board* board)` changes the active player.

## Using ##
You can either compile it as is, or include it in your project.

### Compiling ###
```make
make all
cd examples
make all
```

This produces three files:
* `random` shows a game where the computer makes random moves until the game is over.
* `insrand` reproduces a random game similar to random, but only displays instructions.
* `twoplayer` creates an ncurses interface for two players to play chess.

### Including ###
This can be included as a shared/static library or directly from source code.

#### As a Shared Library ####
Compile the code as a shared library,...
```make
make shared
```
... add the following header file,...
```c
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
```c
#include <ChessEngine.h>
```
...and link the library.
```make
gcc -std=c99 -static <your_files> -L<path_to_folder/chessengine> -lchessengine -I<path_to_folder/chessengine> -o <program_name>
```

#### From Source ####
Add the following header file,...
```c
#include "ChessEngine.h"
```
...and include the appropriate files
```make
gcc -std=c99 <your_files> <path_to_folder/chessengine>/*.c -I<path_to_folder/chessengine> -o <program_name>
```
## Contributing ##
All help is appreciated.

Please do not directly modify lookups.c. It is comprised of four generators plus a wrapper for those generators. If there is a problem with any of the lookup tables, fix the associated generator file (or the wrapper). I'm alright if you regenerate lookups.c - just don't make manual changes to the file.

