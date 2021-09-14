#ifndef CE__CHESS_HISTORY__H
#define CE__CHESS_HISTORY__H

#include "ChessEngine.h"

#include <stdlib.h>

typedef struct HistItem {
	ChessPiece* piece_start;
	ChessPiece* piece_end;

	unsigned short int start_code;

	char start_pos;
	char end_pos;
} HistItem;

typedef struct HistEntry {
	HistItem items[2];

	unsigned int turn_counter;
	enum Color active_player;
	unsigned char hm_clock;
	char en_passant;
} HistEntry;

typedef struct ChessHistory {
	HistEntry* log;
	size_t size;
	size_t capacity;
} ChessHistory;

void free_history(ChessHistory* history);
size_t init_history(ChessHistory* history, size_t capacity);
void record_move(
		ChessGame* game,
		MoveOp* move_obj,
		ChessHistory* history);
size_t undo_move(
		ChessGame* game,
		ChessHistory* history);

#endif
