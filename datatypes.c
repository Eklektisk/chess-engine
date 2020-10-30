#include "DataTypes.h"

/* Ray */
int8_t step(Ray* ray, int8_t* index) {
	if(*index >= ray->size || *index < 0) {
		return -1;
	}

	return ray->pos[(*index)++];
}

/* ChessPieceMap */
bool addPiece(ChessPieceMap* map, int8_t pos, ChessPiece* cp) {
	if(map->pos[pos]) {
		return false;
	}

	map->pos[pos] = true;
	map->chessPiece[pos] = cp;

	return true;
}

bool movePiece(ChessPieceMap* map, int8_t oldPos) {
	if(!map->pos[oldPos] || map->pos[newPos]) {
		return false;
	}

	map->pos[newPos] = true;
	map->pos[oldPos] = false;
	map->chessPiece[newPos] = map->chessPiece[oldPos];
	map->chessPiece[oldPos] = NULL;

	return true;
}

bool removePiece(ChessPieceMap* map, int8_t pos) {
	if(!map->pos[pos]) {
		return false;
	}

	map->pos[pos] = false;
	map->chessPiece[pos] = NULL; /* Technically not necessary */

	return true;
}

ChessPiece* at(ChessPieceMap* map, int8_t pos) {
	return (map->pos[pos]) ? map->chessPiece[pos] : NULL;
}
