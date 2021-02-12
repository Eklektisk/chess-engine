#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DataTypes.h"
#include "Constants.h"

typedef struct ChessPiece {
	int8_t pos;
	int8_t color;
	int8_t type;
	bool   taken;
	bool   hasMoved;
	bool   jumped;
} ChessPiece;

typedef struct ChessPieceMap {
	bool pos[64];
	struct ChessPiece* piece[64];
} ChessPieceMap;

typedef struct MoveDetails {
	struct ChessPiece* cp;
	int8_t newPos;
	int8_t castle;
	bool jump;
	bool enPassant;
	int8_t transform;
} MoveDetails;

typedef struct CPVector {
	struct MoveDetails mv[139];
	size_t size;
} CPVector;

typedef struct Player {
	int8_t kingPos;
	int8_t kingCheck;
	int8_t color;

	ChessPiece rook_Q;    // 14 moves
	ChessPiece knight_Q;  //  8 moves
	ChessPiece bishop_Q;  // 13 moves
	ChessPiece queen;     // 27 moves
	ChessPiece king;      // 10 moves
	ChessPiece bishop_K;  // 13 moves
	ChessPiece knight_K;  //  8 moves
	ChessPiece rook_K;    // 14 moves
	ChessPiece pawn_RQ;   //  4 moves
	ChessPiece pawn_NQ;   //  4 moves
	ChessPiece pawn_BQ;   //  4 moves
	ChessPiece pawn_Q;    //  4 moves
	ChessPiece pawn_K;    //  4 moves
	ChessPiece pawn_BK;   //  4 moves
	ChessPiece pawn_NK;   //  4 moves
	ChessPiece pawn_RK;   //  4 moves

	ChessPieceMap pieces; // 139 moves

	CPVector moves;
} Player;

typedef struct Board {
	// TODO: Use isOver boolean to note when the game is over
	// May need to modify interface to do so

	Player white;
	Player black;

	uint8_t active; // Stores active player
} Board;

void setPieceType(struct ChessPiece* piece, int8_t type);
void setPiecePos(struct Player* player, struct ChessPiece* piece, int8_t pos);
void setPieceFlags(struct ChessPiece* piece, bool hasMoved, bool jumped);

void initBoard(struct Board* board);
bool checkStalemate(struct Board* board);
size_t genMoves(struct Board* board);
void doMove(struct Board* board, int8_t moveNum);
void postMoveActions(struct Board* board, int8_t moveNum);
void switchTurn(struct Board* board);

#ifdef __cplusplus
}
#endif

#endif
