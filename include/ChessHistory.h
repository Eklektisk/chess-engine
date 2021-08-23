#ifndef CE__CHESS_HISTORY__H
#define CE__CHESS_HISTORY__H

#include "ChessEngine.h"

#include <stdlib.h>

typedef struct HistItem {
	ChessPiece* piece_start;
	char start_pos;
	unsigned short int start_code;

	ChessPiece* piece_end;
	char end_pos;
} HistItem;

typedef struct HistEntry {
	unsigned char old_hm_clock;
	HistItem items[2];
} HistEntry;

typedef struct ChessHistory {
	HistEntry* log;
	size_t size;
	size_t capacity;
} ChessHistory;

void
do_move_and_record(
		ChessGame* game,
		MovesList* moves_list,
		unsigned char index,
		ChessHistory* history);

void
free_history(ChessHistory* history);

size_t
init_history(ChessHistory* history, size_t capacity);

size_t undo_moves(
		ChessGame* game,
		ChessHistory* history,
		size_t count_undo);

#endif
