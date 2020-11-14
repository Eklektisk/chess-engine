#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include "DataTypes.h"
#include "Constants.h"

struct ChessPiece {
	int8_t pos;
	int8_t color;
	int8_t type;
	bool   taken;
	bool   hasMoved;
	bool   jumped;
};

struct ChessPieceMap {
	bool pos[64];
	struct ChessPiece* piece[64];
};

struct MoveDetails {
	struct ChessPiece* cp;
	int8_t newPos;
	int8_t castle;
	bool jump;
	bool enPassant;
	int8_t transform;
};

struct CPVector {
	struct MoveDetails mv[84];
	size_t size;
};


struct Player {
	int8_t kingPos;
	int8_t kingCheck;
	int8_t color;

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

	struct ChessPieceMap pieces;

	struct CPVector moves;
};

struct Board {
	struct Player white;
	struct Player black;

	uint8_t turn;
	bool isOver;
};

struct Board initBoard();
void setPiece(
	struct Player* player,
	struct ChessPiece* piece,
	int8_t pos,
	bool hasMoved,
	bool jumped
);
size_t genMoves(struct Board* board);
void doMove(struct Board* board, int8_t moveNum);
void postMoveActions(struct Board* board, int8_t moveNum);
void switchTurn(struct Board* board);

#endif
