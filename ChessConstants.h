#ifndef CHESS_CONSTANTS_H
#define CHESS_CONSTANTS_H

/* PROJECT HEADERS */
#include "DataTypes.h"

/* General ****************/
#define CP_NONE    -1
/* Colors *****************/
#define CP_WHITE    0
#define CP_BLACK    1
/* Types ******************/
#define CP_BISHOP   0
#define CP_KING     1
#define CP_KNIGHT   2
#define CP_PAWN     3
#define CP_QUEEN    4
#define CP_ROOK     5
/* Check ******************/
#define CP_SINGLE   0
#define CP_DOUBLE   1
#define CP_MATE     2
/* Rays *******************/
#define CP_TOP      0
#define CP_TOPRIGHT 1
#define CP_RIGHT    2
#define CP_BOTRIGHT 3
#define CP_BOTTOM   4
#define CP_BOTLEFT  5
#define CP_LEFT     6
#define CP_TOPLEFT  7
/* Castle Direction *******/
#define CP_CASTLE_K 0
#define CP_CASTLE_Q 1
/**************************/

extern const struct Ray allRays[64][8];
/*
 * initPos
 * ray
 */

extern const bool rayMoves[64][6][8][64];
/* initPos
 * type
 * ray
 * endPos
 */

extern const bool pointMoves[64][6][64];
/* initPos
 * type
 * endPos
 */

extern const bool castleMoves[64][2][2][2][2][64];
/* initPos
 * kingHasMoved
 * castleDir
 * rookHasMoved
 * isRook
 * endPos
 */

extern const bool pawnMoves[64][2][2][2][64];
/* initPos
 * color
 * hasMoved
 * endEmpty
 * endPos
 */

#endif
