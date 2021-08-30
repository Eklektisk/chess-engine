#!/usr/bin/python

# Generate all constants used by chess-engine

# There are 14 possible pieceFlags:
#  6 types * 2 colors for pieces unable to begin a castle
#  1 type  * 2 colors for kings able to initiate a castle
#
#  0: Normal  Black Bishop
#  1: Normal  Black King
#  2: Normal  Black Knight
#  3: Normal  Black Pawn
#     Special Black Pawn
#  4: Normal  Black Queen
#  5: Normal  Black Rook
#     Special Black Rook
#  6: Normal  White Bishop
#  7: Normal  White King
#  8: Normal  White Knight
#  9: Normal  White Pawn
#     Special White Pawn
# 10: Normal  White Queen
# 11: Normal  White Rook
#     Special White Rook
# 12: Special Black King
# 13: Special White King
#
# Note that while pawns and rooks may be marked as special, they are
# unable to make special moves themselves. Therefore, they share the
# same index for moves as their "normal" counterparts

# Rays:
#
# 7   0   1
#  \  |  /
#   \ | /
#    \|/
# 6---X---2
#    /|\
#   / | \
#  /  |  \
# 5   4   3

# All inverted masks plus AllowEmpty bit used for checks are listed here
MAY_BE_WHITE           = 0x0040
MAY_BE_BLACK           = 0x0080
MUST_BE_EMPTY          = 0x01ff
MUST_BE_WHITE          = 0x0240
MUST_BE_BLACK          = 0x0280
MUST_BE_SPL_WHITE_PAWN = 0x0347
MUST_BE_SPL_WHITE_ROOK = 0x035f
MUST_BE_SPL_BLACK_PAWN = 0x0387
MUST_BE_SPL_BLACK_ROOK = 0x039f

# All move flags
NORM = 0x00
RKNG = 0x01
CSLQ = 0x02
CSLK = 0x03
EPSN = 0x04
PJMP = 0x05
TRNS = 0x06

class Move(dict):
    def __init__(self, end, flag = NORM):
        self.end  = end
        self.flag = flag

    def print_flags(self):
        return [
                'NormalMove',
                'RegKingMove',
                'CastleQueen',
                'CastleKing',
                'EnPassant',
                'PawnJump',
                'Transform'][self.flag]

moves_list = [list([list() for j in range(14)]) for i in range(64)]

index_names = [
        'Normal Black Bishop',
        'Normal Black King',
        'Normal Black Knight',
        'Normal Black Pawn',
        'Normal Black Queen',
        'Normal/Special Black Rook',
        'Normal White Bishop',
        'Normal White King',
        'Normal White Knight',
        'Normal/Special White Pawn',
        'Normal White Queen',
        'Normal White Rook',
        'Special Black King',
        'Special White King']

for start in range(64):
    row = start // 8
    col = start % 8

    recorded = []

    # Populate all moves along Ray 0 -----------------------------------
    for end in range(start - 8, -1, -8):
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 5].append(Move(end)) # Black Rook
        moves_list[start][10].append(Move(end)) # White Queen
        moves_list[start][11].append(Move(end)) # White Rook

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 5][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY
            moves_list[start][11][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 5][-1][end] = MAY_BE_WHITE
        moves_list[start][10][-1][end] = MAY_BE_BLACK
        moves_list[start][11][-1][end] = MAY_BE_BLACK

        # Record the current end position - this must be empty for
        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # Populate all moves along Ray 2 -----------------------------------
    for end in range(start + 1, 8 * row + 8, 1):
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 5].append(Move(end)) # Black Rook
        moves_list[start][10].append(Move(end)) # White Queen
        moves_list[start][11].append(Move(end)) # White Rook

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 5][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY
            moves_list[start][11][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 5][-1][end] = MAY_BE_WHITE
        moves_list[start][10][-1][end] = MAY_BE_BLACK
        moves_list[start][11][-1][end] = MAY_BE_BLACK

        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # Populate all moves along Ray 4 -----------------------------------
    for end in range(start + 8, 64, 8):
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 5].append(Move(end)) # Black Rook
        moves_list[start][10].append(Move(end)) # White Queen
        moves_list[start][11].append(Move(end)) # White Rook

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 5][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY
            moves_list[start][11][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 5][-1][end] = MAY_BE_WHITE
        moves_list[start][10][-1][end] = MAY_BE_BLACK
        moves_list[start][11][-1][end] = MAY_BE_BLACK

        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # Populate all moves along Ray 6 -----------------------------------
    for end in range(start - 1, 8 * row - 1, -1):
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 5].append(Move(end)) # Black Rook
        moves_list[start][10].append(Move(end)) # White Queen
        moves_list[start][11].append(Move(end)) # White Rook

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 5][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY
            moves_list[start][11][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 5][-1][end] = MAY_BE_WHITE
        moves_list[start][10][-1][end] = MAY_BE_BLACK
        moves_list[start][11][-1][end] = MAY_BE_BLACK

        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # Populate all moves along Ray 1 -----------------------------------
    for end_row, end_col in zip(
            range(row + 1,  8,  1),
            range(col - 1, -1, -1)):
        end = end_row * 8 + end_col

        moves_list[start][ 0].append(Move(end)) # Black Bishop
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 6].append(Move(end)) # White Bishop
        moves_list[start][10].append(Move(end)) # White Queen

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 0][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 6][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 0][-1][end] = MAY_BE_WHITE
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 6][-1][end] = MAY_BE_BLACK
        moves_list[start][10][-1][end] = MAY_BE_BLACK

        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # Populate all moves along Ray 3 -----------------------------------
    for end_row, end_col in zip(
            range(row + 1,  8,  1),
            range(col + 1,  8,  1)):
        end = end_row * 8 + end_col

        moves_list[start][ 0].append(Move(end)) # Black Bishop
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 6].append(Move(end)) # White Bishop
        moves_list[start][10].append(Move(end)) # White Queen

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 0][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 6][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 0][-1][end] = MAY_BE_WHITE
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 6][-1][end] = MAY_BE_BLACK
        moves_list[start][10][-1][end] = MAY_BE_BLACK

        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # Populate all moves along Ray 5 -----------------------------------
    for end_row, end_col in zip(
            range(row - 1, -1, -1),
            range(col + 1,  8,  1)):
        end = end_row * 8 + end_col

        moves_list[start][ 0].append(Move(end)) # Black Bishop
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 6].append(Move(end)) # White Bishop
        moves_list[start][10].append(Move(end)) # White Queen

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 0][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 6][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 0][-1][end] = MAY_BE_WHITE
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 6][-1][end] = MAY_BE_BLACK
        moves_list[start][10][-1][end] = MAY_BE_BLACK

        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # Populate all moves along Ray 7 -----------------------------------
    for end_row, end_col in zip(
            range(row - 1, -1, -1),
            range(col - 1, -1, -1)):
        end = end_row * 8 + end_col

        moves_list[start][ 0].append(Move(end)) # Black Bishop
        moves_list[start][ 4].append(Move(end)) # Black Queen
        moves_list[start][ 6].append(Move(end)) # White Bishop
        moves_list[start][10].append(Move(end)) # White Queen

        # Squares between the start and end position must be empty
        for pos in recorded:
            moves_list[start][ 0][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 4][-1][pos] = MUST_BE_EMPTY
            moves_list[start][ 6][-1][pos] = MUST_BE_EMPTY
            moves_list[start][10][-1][pos] = MUST_BE_EMPTY

        # The end square may contain a piece of the opposite color
        moves_list[start][ 0][-1][end] = MAY_BE_WHITE
        moves_list[start][ 4][-1][end] = MAY_BE_WHITE
        moves_list[start][ 6][-1][end] = MAY_BE_BLACK
        moves_list[start][10][-1][end] = MAY_BE_BLACK

        # longer moves along this ray to also be valid
        recorded.append(end)

    # All recorded end positions have no bearing on other rays
    recorded.clear()

    # A king may move one space in any direction around it
    #
    # A knight may move in an L along vertical and horizontal rays
    # * Two spaces along ray 0/2/4/6, and
    # * One space along a ray perpendicular to the previously chosen ray

    if row > 0: # Checking along ray 0
        end = start - 8

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

        if col > 1: # Checking along ray 2
            end = start - 10

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

        if col < 6: # Checking along ray 6
            end = start - 6

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

    if col < 7: # Checking along ray 2
        end = start + 1

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

        if row > 1: # Checking along ray 0
            end = start - 15

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

        if row < 6: # Checking along ray 4
            end = start + 17

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

    if row < 7: # Checking along ray 4
        end = start + 8

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

        if col < 6: # Checking along ray 2
            end = start + 10

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

        if col > 1: # Checking along ray 6
            end = start + 6

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

    if col > 0: # Checking along ray 6
        end = start - 1

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

        if row < 6: # Checking along ray 4
            end = start + 15

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

        if row > 1: # Checking along ray 0
            end = start - 17

            moves_list[start][2].append(Move(end)) # Black knight
            moves_list[start][8].append(Move(end)) # White knight

            moves_list[start][2][-1][end] = MAY_BE_WHITE
            moves_list[start][8][-1][end] = MAY_BE_BLACK

    if row > 0 and col < 7: # Checking along ray 1
        end = start - 7

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

    if row < 7 and col < 7: # Checking along ray 3
        end = start + 9

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

    if row < 7 and col > 0: # Checking along ray 5
        end = start + 7

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

    if row > 0 and col > 0: # Checking along ray 7
        end = start - 9

        moves_list[start][ 1].append(Move(end, RKNG)) # Black King
        moves_list[start][ 7].append(Move(end, RKNG)) # White King
        moves_list[start][12].append(Move(end, RKNG)) # ! Black King
        moves_list[start][13].append(Move(end, RKNG)) # ! White King

        moves_list[start][ 1][-1][end] = MAY_BE_WHITE
        moves_list[start][ 7][-1][end] = MAY_BE_BLACK
        moves_list[start][12][-1][end] = MAY_BE_WHITE
        moves_list[start][13][-1][end] = MAY_BE_BLACK

    # A pawn may move forward one space
    # The exact direction of forward differs depending on the color

    if row > 0 and row < 7:
        # A black pawn may move:
        # * One space along ray 4 to an empty square
        # * Capture a piece one space along either rays 3 or 5
        #
        # Additionally, a black pawn transforms into another piece when
        # it reaches the final row H across the board
        #
        # A white pawn may move:
        # * One space along ray 0 to an empty square
        # * Capture a piece one space along either rays 1 or 7
        #
        # Additionally, a white pawn transforms into another piece when
        # it reaches the final row A across the board

        moves_list[start][ 3].append(Move(start + 8)) # Black Pawn
        moves_list[start][ 9].append(Move(start - 8)) # White Pawn

        moves_list[start][ 3][-1][start + 8] = MUST_BE_EMPTY
        moves_list[start][ 9][-1][start - 8] = MUST_BE_EMPTY

        if row == 1:   # White is about to reach row A and transform
            moves_list[start][ 9][-1].flag = TRNS
        elif row == 6: # Black is about to reach row A and transform
            moves_list[start][ 3][-1].flag = TRNS

        if col > 0:
            moves_list[start][ 3].append(Move(start + 7)) # Black Pawn
            moves_list[start][ 9].append(Move(start - 9)) # White Pawn

            moves_list[start][ 3][-1][start + 7] = MUST_BE_WHITE
            moves_list[start][ 9][-1][start - 9] = MUST_BE_BLACK

            if row == 1:
                moves_list[start][ 9][-1].flag = TRNS
            elif row == 6:
                moves_list[start][ 3][-1].flag = TRNS

        if col < 7:
            moves_list[start][ 3].append(Move(start + 9)) # Black Pawn
            moves_list[start][ 9].append(Move(start - 7)) # White Pawn

            moves_list[start][ 3][-1][start + 9] = MUST_BE_WHITE
            moves_list[start][ 9][-1][start - 7] = MUST_BE_BLACK

            if row == 1:
                moves_list[start][ 9][-1].flag = TRNS
            elif row == 6:
                moves_list[start][ 3][-1].flag = TRNS

# Some moves can only occur if a certain piece is within a defined
# subset of positions, leading to special moves

# Pawns can move forward two spaces if they have yet to move
#
# This is automatically true for black pawns on row B (consistent of
# positions 8 through 15) and for white pawns on row G (consistent of
# positions 48 through 55)

for start in range( 8, 16):
    moves_list[start][ 3].append(Move(start + 16, PJMP)) # Black pawn
    moves_list[start][ 3][-1][start +  8] = MUST_BE_EMPTY
    moves_list[start][ 3][-1][start + 16] = MUST_BE_EMPTY

for start in range(48, 56):
    moves_list[start][ 9].append(Move(start - 16, PJMP)) # White pawn
    moves_list[start][ 9][-1][start -  8] = MUST_BE_EMPTY
    moves_list[start][ 9][-1][start - 16] = MUST_BE_EMPTY

# Additionally, pawns may capture other pawns en passant if their first
# move moved them two spaces forward instead of one
#
# This is only possible for white pawns on row C (consistent of
# positions 16 through 23) and for black pawns on row F (consistent of
# positions 40 through 47)

for start in range(24, 32):
    if start < 31:
        moves_list[start][ 9].append(Move(start - 7, EPSN)) # White pawn

        moves_list[start][ 9][-1][start + 1] = MUST_BE_SPL_BLACK_PAWN
        moves_list[start][ 9][-1][start - 7] = MUST_BE_EMPTY

    if start > 24:
        moves_list[start][ 9].append(Move(start - 9, EPSN)) # White pawn

        moves_list[start][ 9][-1][start - 1] = MUST_BE_SPL_BLACK_PAWN
        moves_list[start][ 9][-1][start - 9] = MUST_BE_EMPTY

for start in range(32, 40):
    if start < 39:
        moves_list[start][ 3].append(Move(start + 9, EPSN)) # Black pawn

        moves_list[start][ 3][-1][start + 1] = MUST_BE_SPL_WHITE_PAWN
        moves_list[start][ 3][-1][start + 9] = MUST_BE_EMPTY

    if start > 32:
        moves_list[start][ 3].append(Move(start + 7, EPSN)) # Black pawn

        moves_list[start][ 3][-1][start - 1] = MUST_BE_SPL_WHITE_PAWN
        moves_list[start][ 3][-1][start + 7] = MUST_BE_EMPTY

# Kings can castle if neither they nor the rook they are castling
# towards have yet to move
#
# This may only be true for:
# * Black kings at A5 (position  4)
# * White kings at H5 (position 60)

moves_list[ 4][12].append(Move( 6, CSLK)) # ! Black King
moves_list[ 4][12][-1][ 7] = MUST_BE_SPL_BLACK_ROOK
moves_list[ 4][12][-1][ 5] = MUST_BE_EMPTY
moves_list[ 4][12][-1][ 6] = MUST_BE_EMPTY

moves_list[ 4][12].append(Move( 2, CSLQ)) # ! Black King
moves_list[ 4][12][-1][ 0] = MUST_BE_SPL_BLACK_ROOK
moves_list[ 4][12][-1][ 1] = MUST_BE_EMPTY
moves_list[ 4][12][-1][ 3] = MUST_BE_EMPTY
moves_list[ 4][12][-1][ 2] = MUST_BE_EMPTY

moves_list[60][13].append(Move(62, CSLK)) # ! White King
moves_list[60][13][-1][63] = MUST_BE_SPL_WHITE_ROOK
moves_list[60][13][-1][61] = MUST_BE_EMPTY
moves_list[60][13][-1][62] = MUST_BE_EMPTY

moves_list[60][13].append(Move(58, CSLQ)) # ! White King
moves_list[60][13][-1][56] = MUST_BE_SPL_WHITE_ROOK
moves_list[60][13][-1][57] = MUST_BE_EMPTY
moves_list[60][13][-1][59] = MUST_BE_EMPTY
moves_list[60][13][-1][58] = MUST_BE_EMPTY

# In order to be of any practical use, moves_list must be converted into
# an understandable manner for C

move_options = ''

move_options_entry_template = '''%s\t\t{ /* Type: %s */
\t\t\t.options_map = {
\t\t\t\t%s,
\t\t\t\t%s,
\t\t\t\t%s,
\t\t\t\t%s,
\t\t\t\t%s,
\t\t\t\t%s,
\t\t\t\t%s,
\t\t\t\t%s},
\t\t\t.num_options = %d,
\t\t\t.options = {%s
\t\t\t},
\t\t},
'''

move_no_options_entry_template = '''%s\t\t{ /* Type: %s */
\t\t\t.options_map = {
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1,
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1,
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1,
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1,
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1,
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1,
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1,
\t\t\t\t-1, -1, -1, -1, -1, -1, -1, -1},
\t\t\t.num_options = 0,
\t\t},
'''

move_data_entry_template = '''%s
\t\t\t\t{ /* Move Num: %02d */
\t\t\t\t\t.end_pos    = %2d,
\t\t\t\t\t.flag       = %s,
\t\t\t\t\t.num_checks = %d,
\t\t\t\t\t.squares    = {%s},
\t\t\t\t\t.inv_masks  = {
\t\t\t\t\t\t%s}
\t\t\t\t},'''

for start in range(64):
    move_options = '%s\t{ /* Pos: %02d */\n' % (move_options, start)

    for i in range(14):
        squares = ['-1' for pos in range(64)]
        move_data = ''

        # squares converts to a hashmap defined by
        # `MoveOptions.options_map`. It validates whether a piece can
        # potentially land on a certain square, pos:
        #
        #   MoveOptions.options_map[pos] != -1
        #
        # and if so, what index a move to such a position is associated
        # with within `MoveOptions.options`
        #
        # This loop serves dual purpose as both filling `squares` with
        # the correct indicies and filling `MoveOptions.options` with
        # all possible move options for the associated start pos/piece

        for j, move in enumerate(moves_list[start][i]):
            if move.flag != EPSN: # EnPassant
                squares[move.end] = '%2d' % (j)
            
            check_squares = list()
            check_masks   = list()

            for pair in move.items():
                check_squares.append('%2d'   % (pair[0]))
                check_masks.append('%#06x' % (pair[1]))

            move_data = move_data_entry_template % (
                    move_data,
                    j,
                    move.end,
                    move.print_flags(),
                    len(move),
                    ', '.join(check_squares),
                    ',\n\t\t\t\t\t\t'.join(check_masks))
        
        if(len(moves_list[start][i]) > 0):
            move_options = move_options_entry_template % (
                    move_options,
                    index_names[i],
                    ', '.join(squares[ 0: 8]),
                    ', '.join(squares[ 8:16]),
                    ', '.join(squares[16:24]),
                    ', '.join(squares[24:32]),
                    ', '.join(squares[32:40]),
                    ', '.join(squares[40:48]),
                    ', '.join(squares[48:56]),
                    ', '.join(squares[56:64]),
                    len(moves_list[start][i]),
                    move_data)
        else:
            move_options = move_no_options_entry_template % (
                    move_options,
                    index_names[i])


    move_options  = '%s\t},\n' % (move_options)

print('''#include "MoveInfo.h"

const MoveOptions move_options[64][14] = {
%s};
''' % (move_options))

