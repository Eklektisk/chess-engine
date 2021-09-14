#include "ChessHistory.h"
#include "MoveInfo.h"
#include "Utility.h"

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

void
record_move(ChessGame* game, MoveOp* move_obj, ChessHistory* history)
{
	int start, end;
	HistEntry* hist_obj;

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

	hist_obj = history->log + (history->size)++;

	start = move_obj->old_pos;
	end   = move_obj->new_pos;

	/* Copy game information */

	hist_obj->hm_clock      = game->hm_clock;
	hist_obj->turn_counter  = game->turn_counter;
	hist_obj->active_player = game->active_player;
	hist_obj->en_passant    = game->en_passant;

	/* Log move information and the associated piece information prior
	 * to afforementioned move
	 *
	 * As the piece code for the piece at the end position never changes
	 * as a result of the move itself, there is no need to log this
	 * change */

	hist_obj->items[0].piece_start = game->board[start];
	hist_obj->items[0].start_pos   = start;
	hist_obj->items[0].start_code  = game->board[start]->code;
	hist_obj->items[0].piece_end   = game->board[end];
	hist_obj->items[0].end_pos     = end;

	/* Some moves cause a second piece to move or be taken that would
	 * not have normally be taken or moved */

	switch(move_obj->_flag) {
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
			start = end + (game->active_player == Black ? -8 : 8);
			end   = -1;

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
}

size_t
undo_move(ChessGame* game, ChessHistory* history)
{
	int start, end;
	HistEntry* hist_obj;
	HistItem* hist_item;

	if(history->size == 0) {
		return 0;
	}

	hist_obj  = history->log + --(history->size);
	hist_item = hist_obj->items;

	/* Revert the last moved piece to its previous state */

	start = hist_item[0].start_pos; 
	end   = hist_item[0].end_pos;

	game->board[start] = hist_item[0].piece_start;
	game->board[end]   = hist_item[0].piece_end;

	hist_item->piece_start->code = hist_item[0].start_code;
	hist_item->piece_start->pos  = hist_item[0].start_pos;

	if(hist_item[0].piece_end) {
		hist_item[0].piece_end->pos = end;
	}

	/* If a second piece was moved or changed, also revert it to its
	 * previous state
	 *
	 * Note that in all cases, the "end position" for this piece will
	 * always be NULL before the move occurs, as explained in
	 * record_move */

	if(hist_item[1].piece_start) {
		start = hist_item[1].start_pos; 
		end   = hist_item[1].end_pos;

		game->board[start] = hist_item[1].piece_start;

		if(end != -1) {
			game->board[end] = NULL;
		}

		hist_item[1].piece_start->code = hist_item[1].start_code;
		hist_item[1].piece_start->pos  = hist_item[1].start_pos;
	}

	/* Reset game information */

	game->hm_clock      = hist_obj->hm_clock;
	game->turn_counter  = hist_obj->turn_counter;
	game->active_player = hist_obj->active_player;
	game->en_passant    = hist_obj->en_passant;

	/* If a piece may be taken en passant, make sure that it regains the
	 * special flag
	 *
	 * As en passant only lasts for a turn, any piece that should
	 * lose en passant already lost it when its move was undone */

	if(game->en_passant != -1) {
		game->board[(int) game->en_passant]->code &= 0x00ff;
	}

	return history->size;
}

