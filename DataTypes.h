#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define Set bool[64]

struct {
	int8_t pos[7];
	size_t size;
} Ray;

struct {
	int8_t x;
	int8_t y;
	int8_t color;
	int8_t type;
	bool   hasMoved;
} ChessPiece;

struct {
	int8_t pieceType;
	Set moves;
	Set isCastle;
} MoveDetails;

struct {
	Set pos;
	ChessPiece* chessPiece[64];
} ChessPieceMap;

/* Ray */
int8_t step(Ray* ray, int8_t* index);

/* ChessPieceMap */
bool addPiece(ChessPieceMap* map, int8_t pos, ChessPiece* cp);
bool movePiece(ChessPieceMap* map, int8_t oldPos, int8_t newPos);
bool removePiece(ChessPieceMap* map, int8_t pos);
ChessPiece* at(ChessPieceMap* map, int8_t pos);

#endif
