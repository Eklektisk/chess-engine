=============
DOCUMENTATION
=============

In order to gain a base understanding of how this libary works and
pieces are structured, please read `README.rst`.

ChessPlayer
===========

A ChessPlayer stores information on where its pieces are located on a
board. The ChessPlayer struct, defined in `include/ChessEngine.h`, is
as follows:

code:: cpp

    typedef struct ChessPlayer {
        char pieces[16];
        unsigned char king_index;
    } ChessPlayer;

`ChessPlayer.king_index` stores the index of where the king's position
is stored within `ChessPlayer.pieces`. If a piece is removed, the
player puts a -1 in place of where the piece was previously found
within the `pieces` array.

ChessGame
=========

A ChessGame object stores the board state along with player information
and game statistics. The ChessGame struct, defined in
`include/ChessEngine.h`, is as follows:

code:: cpp

    typedef struct ChessGame {
        unsigned short int board[64];

        ChessPlayer white;
        ChessPlayer black;

        enum CheckStatus check_status;

        char en_passant;

        enum Color active_player;

        unsigned char hm_clock;
        unsigned short int turn_counter;
    } ChessGame;

`ChessGame.board` stores the bitset that defines each piece, assuming
one exists.

Check and Mate
==============

While no official support for game-ending conditions exists yet, it is
possible to determing whether a player is in check. The possible
options for `ChessGame.check_status`, as defined in
`include/ChessEngine.h`, are as follows:

code:: cpp

    enum CheckStatus {
        NotInCheck  = 0x00,
        SingleCheck = 0x01, /* King is only in check due to one piece */
        DoubleCheck = 0x02, /* King is in check due to several pieces */
    };

When a player is in check, their king may be put in check by either one
piece or several pieces. While a player may move their king to get out
of check in both situations, a player can only move another piece to
get their king out of check if it was put in check by a single piece.
As this affects the behavior of a player in check, this distinction is
noted in the `CheckStatus` enumeration.

The following example details the white player in `SingleCheck`::

      12345678
    A ........
    B ........
    C ...q....
    D ........
    E ......R.
    F ........
    G ...K....
    H ........

The white king (G4) can move, for example, to G3 to get out of check
from the black queen (C4). Alternatively, the white rook (E7) can move
to E4 and get between the king and the queen. Either option ensures that
the white king no longer in check.

The following example details the white player in `DoubleCheck`::

    DoubleCheck example:

      12345678
    A Q.......
    B ...r....
    C ........
    D ........
    E ........
    F ........
    G n.......
    H ...K....

The white king (H4) can move, for example, to H3 to get out of check
from the black knight (G1) and rook (B4). While the white queen (A1)
could capture the black knight or move to D4 to get in the way of the
king and the rook, the king would still be in check due to the
unmitigated presence of the other piece.

