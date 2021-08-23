============
Chess Engine
============

A chess engine powered by lookup tables and bad code.

Basic Overview
==============

The chess board is represented by a series of 16 bits per square,
although only 9 bits are needed per piece. This holds the piece type,
color, and whether any special moves may involve said piece (namely
castling or en passant). The bit combinations, defined in
`include/ChessEngine.h`, are as follows:

code:: cpp

    enum Piece {
        Empty  = 0x0000,
        Bishop = 0x0001,
        King   = 0x0002,
        Knight = 0x0004,
        Pawn   = 0x0008,
        Queen  = 0x0010,
        Rook   = 0x0020,
    };

    enum Color {
        Black = 0x0040,
        White = 0x0080,
    };

    enum Special {
        Normal  = 0x0100,
        Special = 0x0200, /* Denotes castling or en passant */
    };

Pseudolegal and legal moves are validated based on their "signature".
Each square of importance compares the AND between its bitflags and an
inverse bitmask of consistent of all possible flags. If all squares
return 0, then the move is considered pseudolegal. Special consideration
is given to squares that cannot match an empty square to the inverse
bitmask and therefore must be verified using different patterns.

A more detailed description is given in `DOCUMENTATION.rst`.

Creating a Game Loop
====================

Initializing Components
-----------------------

In order to use a game loop, it is first necessary to create a
`ChessGame` and a `MovesList`. The former is responsible for managing
the chess game, while the latter is responsible for storing all
possible moves. `void init_game(ChessGame* game)`, as defined in
`src/chessengine.c`, initializes a new chess game with the black player
starting on the first two rows and the white player on the bottom two.

If using the history module, a `History` object should also be created
at this time, and space should be allocated using `int
init_history(History* history, size_t capacity)`, as defined in
`src/chesshistory.c`.

The  Game Loop
--------------

Although no functions check for the existence of checkmate or
stalemate, it is possible to assume the game is in one of two such
positions if `unsigned char generate_moves_list(ChessGame* game,
MovesList* moves_list)`, as defined in `src/chessengine.c`, returns 0 -
meaning that there are no legal moves possible.

Restrictions based on the number of turns or half moves may also be
applied by referencing `ChessGame.turn_counter` or
`ChessGame.hm_clock`, respectively. Note that `ChessGame.hm_clock`
resets every time a piece is captured or a pawn moves.

Within the loop, `void do_move(ChessGame* game, MovesList* moves_list,
unsigned char index)`, as defined in `src/chessengine.c`, executes the
move described at `moves_list.moves[index]`. This also updates the half
move clock according to what piece was moved. The only other required
function, `void update(ChessGame* game)` updates the active player, turn
counter, and check status for the game.

A basic game loop that always make the last generated move would appear
as follows:

code:: cpp

    ChessGame game;
    MovesList moves;

    init_game(&game);

    while(
        game.hm_clock < 50 && generate_moves_list(&game, &moves) > 0
    ) {
        do_move(&game, &moves, moves.size - 1);
        update(&game);
    }

If using the history module, it is necessary to initialize a `History`
object with `size_t init_history(History* history, size_t capacity)`,
as defined in `src/chesshistory.c`.  Additionally, `void
do_move(...)` function should be replaced with its alternative, `void
do_move_and_record(ChessGame* game, MovesList* moves_list,
unsigned char index, History* history)`, as defined in
`src/chesshistory.c`. This records the move before making it, so that
it is possible to later revert them using `size_t undo_moves(
ChessGame* game, ChessHistory* history, size_t count_undo)`.

Using This in a Project
=======================

You can use this within your projects by including:

code:: cpp

    #include <ChessEngine.h>

at the top of the appropriate file(s), and adding `-I$(PROJDIR)/include
-L $(PROJDIR)/lib -lchessengine.so` to your make command, where
`$(PROJDIR)` is the location of this folder (or more precisely, the
location of the `include/` and `lib/` folders from this project).

If you wish to use the history module to save and unmake moves, also
include:

code:: cpp

    #include <ChessHistory.h>

at the top of the appropriate file(s).

If you wish to use the static library over the shared library, either
add `-static` to your make command or make sure that the shared
library, `libchessengine.so`, does not exist in `lib/` at compilation
time.

Note that in order to use this as a shared library, you must add
`$(PROJDIR)/lib` to your `LD_LIBRARY_PATH` environment variable before
running the program or move `libchessengine.so` to a more traditional
location for shared libraries.

`make install` will install libraries to `/usr/local/lib` by default,
which may not be automatically searched by your distribution. If not,
please update your `/etc/ld.so.conf` file to include this directory (or
create a config file in `/etc/ld.so.conf.d/` that contains this path).
Make sure to run `ldconfig` with root permissions afterwards for this
change to take effect.

Contributing
============

Any contributions are welcome! However, please refrain from directly
modifying `src/moveinfo.c`, as this file is generated by
`generators/moves.py` as opposed to being manually written.

