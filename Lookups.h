#ifndef LOOKUPS_H
#define LOOKUPS_H

#include "DataTypes.h"

// Using the honor code - these variables should never be modified!

extern struct Ray allRays[64][8];
/* initPos
 * ray
 */

extern int8_t rayContains[64][64];
/* initPos
 * ray
 * endPos
 */

extern struct Ray rayMoves[64][6][8];
/* initPos
 * type
 * ray
 */

extern struct Vector8 pointMoves[64][6];
/* initPos
 * type
 */

extern bool capturePiece[64][6][2][64];
/* piecePos
 * pieceType
 * pieceColor
 * capturePos
 */

#endif
