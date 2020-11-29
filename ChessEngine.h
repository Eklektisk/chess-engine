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
	struct MoveDetails mv[139];
	size_t size;
};

struct Player {
	int8_t kingPos;
	int8_t kingCheck;
	int8_t color;

	struct ChessPiece rook_Q;    // 14 moves
	struct ChessPiece knight_Q;  //  8 moves
	struct ChessPiece bishop_Q;  // 13 moves
	struct ChessPiece queen;     // 27 moves
	struct ChessPiece king;      // 10 moves
	struct ChessPiece bishop_K;  // 13 moves
	struct ChessPiece knight_K;  //  8 moves
	struct ChessPiece rook_K;    // 14 moves
	struct ChessPiece pawn_RQ;   //  4 moves
	struct ChessPiece pawn_NQ;   //  4 moves
	struct ChessPiece pawn_BQ;   //  4 moves
	struct ChessPiece pawn_Q;    //  4 moves
	struct ChessPiece pawn_K;    //  4 moves
	struct ChessPiece pawn_BK;   //  4 moves
	struct ChessPiece pawn_NK;   //  4 moves
	struct ChessPiece pawn_RK;   //  4 moves

	struct ChessPieceMap pieces; // 139 moves

	struct CPVector moves;
};

struct Board {
	struct Player white;
	struct Player black;

	uint8_t turn;
	// TODO: Use isOver boolean to note when the game is over
	// May need to modify interface to do so
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
bool checkStalemate(struct Board* board);
void doMove(struct Board* board, int8_t moveNum);
void postMoveActions(struct Board* board, int8_t moveNum);
void switchTurn(struct Board* board);

#endif
