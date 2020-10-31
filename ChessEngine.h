#ifndef GAME_H
#define GAME_H

#include "DataTypes.h"

struct MoveInfo {
	struct ChessPiece* piece;
	int8_t newPos;
	int8_t castleType;
};

struct Player {
	int8_t kingPos;
	int8_t kingCheck;
	Set opposingAlongKingRay;

	struct ChessPiece rook_Q;
	struct ChessPiece knight_Q;
	struct ChessPiece bishop_Q;
	struct ChessPiece queen;
	struct ChessPiece king;
	struct ChessPiece bishop_K;
	struct ChessPiece knight_K;
	struct ChessPiece rook_K;
	struct ChessPiece pawn_RQ;
	struct ChessPiece pawn_NQ;
	struct ChessPiece pawn_BQ;
	struct ChessPiece pawn_Q;
	struct ChessPiece pawn_K;
	struct ChessPiece pawn_BK;
	struct ChessPiece pawn_NK;
	struct ChessPiece pawn_RK;

	Set piecePos;

	struct MoveInfo possibleMoves[84];
	size_t countMoves;
};

struct {
	struct ChessPieceMap chessPieceMap;

	int8_t whiteKingPos;
	int8_t blackKingPos;

	Set blackAlongWhiteKingRay;
	Set whiteAlongBlackKingRay;

	int8_t whiteKingCheck;
	int8_t blackKingCheck;

	bool isCastle;
	uint8_t turn;
} Game;

size_t genMoves(Game* game, int8_t color);
bool vaidateMove(Game* game, int8_t color, int8_t moveNum);
void doMove(Game* game, int8_t color, int8_t moveNum);
void postMoveActions(Game* game, int8_t color, int8_t moveNum);
void updateCheck(Game* game, int8_t color);
void updateTurn(Game* game);

#endif
