#ifndef GAME_H
#define GAME_H

#include "DataTypes.h"

struct Player {
	int8_t kingPos;
	int8_t kingCheck;
	Set opposingAlongKingRay;
}

struct {
	ChessPieceMap chessPieceMap;

	int8_t whiteKingPos;
	int8_t blackKingPos;

	Set blackAlongWhiteKingRay;
	Set whiteAlongBlackKingRay;

	int8_t whiteKingCheck;
	int8_t blackKingCheck;

	bool isCastle;
	uint8_t turn;
} Game;

MoveDetails genMoves(int8_t pos);
void postMoveActions();
void updateCheck();
void updateTurn(Game* game);

#endif
