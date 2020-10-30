#!/usr/bin/python
# This script generates chessconstants.c
# Otherwise, writing five lookup tables would take forever

# chessconstants.c is a read-only file.
# If there is an issue with any of the lookup tables,
# please submit a pull request to fix the associated generator script.

from importlib import import_module

def main():
    castle_moves = import_module('castlemoves')
    pawn_moves   = import_module('pawnmoves')
    point_moves  = import_module('pointmoves')
    ray_moves    = import_module('raymoves')
    rays         = import_module('rays')

    with open('chessconstants.c', 'w') as constants:
        constants.write(f'''#include "ChessConstants.h"

{rays.format(rays.generate())}

{ray_moves.format(ray_moves.generate())}

{point_moves.format(point_moves.generate())}

{castle_moves.format(castle_moves.generate())}

{pawn_moves.format(pawn_moves.generate())}

''')

    return

if __name__ == '__main__':
    main()
