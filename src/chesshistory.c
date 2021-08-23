#include "ChessHistory.h"
#include "MoveInfo.h"
#include "Utility.h"

#include <stdio.h>

void
do_move_and_record(
		ChessGame* game,
		MovesList* moves_list,
		unsigned char index,
		ChessHistory* history)
{
	int start, end, piece_idx, move_idx;

	if(history->size == history->capacity) {
		/* Not enough space - create more space to accomodate */

		int new_capacity = history->capacity == 0
			? 1
			: history->capacity * 2;

		history->log = realloc(
				history->log,
				new_capacity * sizeof(HistEntry));

		history->capacity = new_capacity;
	}

	HistEntry* hist_obj = history->log + (history->size)++;
	MoveOp* move_obj   = moves_list->moves + index;

	start = move_obj->old_pos;
	end   = move_obj->new_pos;

	/* Copy game information */

	hist_obj->old_hm_clock   = game->hm_clock;

	/* Log move information and the associated piece information prior
	 * to afforementioned move */

	hist_obj->items[0].piece_start = game->board[start];
	hist_obj->items[0].start_pos   = start;
	hist_obj->items[0].start_code  = game->board[start]->code;
	hist_obj->items[0].piece_end   = game->board[end];
	hist_obj->items[0].end_pos     = end;

	/* Some moves cause a second piece to move or be taken that would
	 * not have normally be taken or moved */

	piece_idx = pieceset_to_index(game->board[start]->code);
	move_idx  = move_obj->ref_id;

	switch(move_options[start][piece_idx].options[move_idx].flag) {
		case CastleQueen:
			/* Rook is moved */
			start -= 4;
			end   += 1;

			break;

		case CastleKing:
			/* Rook is moved */
			start += 3;
			end   -= 1;

			break;

		case EnPassant:
			/* Pawn is taken */
			switch(game->active_player) {
				case Black:
					start -= 8;
					break;

				case White:
					start += 8;
					break;
			}

			end = -1;

			break;

		default:
			/* No secondary piece is moved or taken */
			start = -1;
			end   = -1;

			break;
	}

	/* The end position for the secondary piece will always be empty
	 * prior to moving, either because:
	 * * A rook cannot take pieces when castling
	 * * The pawn taken with en passant does not land on a space
	 * * There is no secondary piece to move or take
	 */

	hist_obj->items[1].piece_start = start == -1
		? NULL
		: game->board[start];
	hist_obj->items[1].start_pos   = start;
	hist_obj->items[1].start_code  = start == -1
		? Empty
		: game->board[start]->code;
	hist_obj->items[1].piece_end   = NULL;
	hist_obj->items[1].end_pos     = end;

	do_move(game, moves_list, index);
}

void
free_history(ChessHistory* history)
{
	free(history->log);
}

size_t
init_history(ChessHistory* history, size_t capacity)
{
	history->log      = calloc(capacity, sizeof(HistEntry));
	history->size     = 0;

	history->capacity = history->log == NULL
		? 0         /* Failed to allocate enough space */
		: capacity;

	return history->capacity;
}

size_t
undo_moves(ChessGame* game, ChessHistory* history, size_t count_undo)
{
	int i;
	HistEntry* hist_obj = history->log + history->size - 1;

	if(history->size < count_undo) {
		count_undo = history->size;
	}

	for(i = 0; i < count_undo; ++i) {
		HistItem* hist_item = hist_obj->items;
		int start, end;

		game->hm_clock = hist_obj->old_hm_clock;

		/* Revert the last moved piece to its previous state */

		start = hist_item->start_pos; 
		end   = hist_item->end_pos;

		game->board[start] = hist_item->piece_start;
		game->board[end]   = hist_item->piece_end;

		hist_item->piece_start->code = hist_item->start_code;
		hist_item->piece_start->pos  = hist_item->start_pos;

		/* If a second piece was moved or changed, also revert it to its
		 * previous state */

		if((++hist_item)->piece_start) {
			start = hist_item->start_pos; 
			end   = hist_item->end_pos;

			game->board[start] = hist_item->piece_start;
			game->board[end]   = hist_item->piece_end;

			hist_item->piece_start->code = hist_item->start_code;
			hist_item->piece_start->pos  = hist_item->start_pos;
		}
	}

	history->size -= count_undo;

	return history->size;
}
