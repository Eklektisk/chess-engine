Chess Engine
====================

A chess engine powered by lookup tables and bad code.

## How this works ##
To start a game, initialize a `struct Board` and define the basics for each player. I'll work on creating a function to set up the board later.

The game loop follows as such:
1. At the start of each turn, players can generate moves using `genmoves(struct Board* board)`. This populates each Player with a list of valid moves.
2. `updateGameStatus(struct Board* board)` checks if any moves are possible and sets the `board.isOver` if there are no legal moves.
3. If the game is not over, a move can then be selected from the table, and `doMove(struct Board* board, int8_t moveNum)`.
4. `postMoveActions(struct Board* board, int8_t moveNum)` handles any additional actions such as taking pieces and clears all generated moves.
5. `switchTurn(struct Board* board)` changes the turn, enabling the game loop to start again.

I haven't implemented any way of distinguishing between stalemate and checkmate. I'm fairly certain that there are a variety of reasons why stalemate occurs, but I'm only counting a game over when there are no legal moves (checkmate if the king is in check at the time, stalemate if not).

## Using ##
You can either compile it as is, or include it in your project.

### Compiling ###
```make
gcc demo.c chessconstants.c datatypes.c chessengine.c -o demo
```

As of now, demo.c only prints a test statement confirming that chessconstants.c was properly implemented.
This will eventually allow you to play a chess game in the terminal against another player.

### Including ###
```cpp
#include "ChessEngine.h"
```

It should work alright, but I still need to update `demo.c` and run some tests to make sure that everything works as expected.

## Contributing ##
All help is appreciated.

Please do not directly modify chessconstants.c. It is comprised of five generators plus a wrapper for those generators. If there is a problem with any of the lookup tables, fix the associated generator file (or the wrapper). I'm alright if you regenerate chessconstants.c - just don't make manual changes to the file.

