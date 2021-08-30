#include "ChessEngine.h"
#include "MoveInfo.h"
#include "Utility.h"

#include <stdlib.h>

#define ANY_PIECE   Bishop|King|Knight|Pawn|Queen|Rook

void
do_move(ChessGame* game, MovesList* moves_list, unsigned char index)
{
	int start, end, piece_idx, move_idx, reset_hm_clock;
	enum MoveFlags flags;

	start     = moves_list->moves[index].old_pos;
	end       = moves_list->moves[index].new_pos;
	piece_idx = pieceset_to_index(game->board[start]->code);
	move_idx  = moves_list->moves[index]._ref_id;
	flags = move_options[start][piece_idx].options[move_idx].flag;

	reset_hm_clock   = 0;

	if(game->en_passant != -1) {
		game->board[(unsigned char) game->en_passant]->code |= Normal;
	}

	switch(flags) {
		case NormalMove:  /* FALL THROUGH */
			if(game->board[start]->code & Pawn) {
				reset_hm_clock = 1;
			}

		case RegKingMove:
			if(game->board[end]) {
				/* Capturing piece - Remove taken piece from other
				 * player's list */

				game->board[end]->pos = -1;

				reset_hm_clock = 1;
			}

			if(game->en_passant != -1) {
				game->en_passant = -1;
			}

			game->board[end] = game->board[start];
			game->board[start] = NULL;

			game->board[end]->code |= Normal;
			game->board[end]->pos   = end;

			break;

		case CastleQueen:
			if(game->en_passant != -1) {
				game->en_passant = -1;
			}

			game->board[end] = game->board[start];
			game->board[start] = NULL;

			game->board[end]->code |= Normal;
			game->board[end]->pos   = end;

			game->board[end + 1]   = game->board[start - 4];
			game->board[start - 4] = NULL;

			game->board[end + 1]->code |= Normal;
			game->board[end + 1]->pos   = end + 1;

			break;

		case CastleKing:
			if(game->en_passant != -1) {
				game->en_passant = -1;
			}

			game->board[end] = game->board[start];
			game->board[start] = NULL;

			game->board[end]->code |= Normal;
			game->board[end]->pos   = end;

			game->board[end - 1]   = game->board[start + 3];
			game->board[start + 3] = NULL;

			game->board[end - 1]->code |= Normal;
			game->board[end - 1]->pos   = end - 1;

			break;

		case EnPassant:
			reset_hm_clock = 1;

			if(game->en_passant != -1) {
				game->en_passant = -1;
			}

			switch(game->active_player) {
				case Black:
					game->board[end - 8]->pos = -1;
					game->board[end - 8]      = NULL;
					break;

				case White:
					game->board[end + 8]->pos = -1;
					game->board[end + 8]      = NULL;
					break;
			}

			game->board[end]   = game->board[start];
			game->board[start] = NULL;

			game->board[end]->pos = end;

			break;

		case PawnJump:
			reset_hm_clock  = 1;

			game->en_passant = end;

			game->board[end] = game->board[start];
			game->board[start] = NULL;

			game->board[end]->code &= 0x00ff; /* Mark as special */
			game->board[end]->pos   = end;

			break;

		case Transform:
			reset_hm_clock = 1;

			if(game->en_passant != -1) {
				game->en_passant = -1;
			}

			if(game->board[end]) {
				/* Capturing piece - Remove taken piece from other
				 * player's list */

				game->board[end]->pos = -1;
			}

			game->board[end] = game->board[start];
			game->board[start] = NULL;

			/* Switch piece type by clearing old piece type and then
			 * updating with new one */

			game->board[end]->code &= Normal | White | Black;
			game->board[end]->code |= moves_list->moves[index].new_type;

			game->board[end]->pos   = end;

			break;
	}

	if(reset_hm_clock) {
		game->hm_clock = 0;
	} else {
		++(game->hm_clock);
	}
}

unsigned char
generate_moves_list(ChessGame* game, MovesList* moves_list)
{
	int i, j, start, end, idx;
	ChessPlayer* active;
	unsigned char size = 0;

	active = game->players + player_to_index(game->active_player);

	/* Only the king may move in order to escape double check */
	if(game->check_status == DoubleCheck) {
		start = active->pieces[active->king_index].pos;
		idx   = pieceset_to_index(game->board[start]->code);

		for(i = 0; i < move_options[start][idx].num_options; ++i) {
			if(
					is_move_legal(
						game,
						start,
						move_options[start][idx].options + i)
			) {
				moves_list->moves[size++] = (MoveOp) {
					start,
					move_options[start][idx].options[i].end_pos,
					i,
					Empty };
			}
		}

		moves_list->size = size;

		return size;
	}

	for(i = 0; i < 16; ++i) {
		start = active->pieces[i].pos;

		if(start == -1) {
			continue;
		}

		idx = pieceset_to_index(game->board[start]->code);

		for(j = 0; j < move_options[start][idx].num_options; ++j) {
			if(
					is_move_legal(
						game,
						start,
						move_options[start][idx].options + j)
			) {
				end = move_options[start][idx].options[j].end_pos;
				switch(move_options[start][idx].options[j].flag) {
					case Transform:
						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							j,
							Bishop };

						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							j,
							Knight };

						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							j,
							Queen };

						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							j,
							Rook };

						break;

					default:
						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							j,
							Empty };

						break;
				}
			}
		}
	}

	moves_list->size = size;

	return size;
}

void
init_game(ChessGame* game)
{
	/* Initialize a new chess game with the black player starting on
	 * rows A/B and the white player starting on rows G/H */
	unsigned char i;

	*game = (ChessGame) {
		.players = {
			{
				.pieces = {
					{ Special | Black | Rook,    0 },
					{ Normal  | Black | Knight,  1 },
					{ Normal  | Black | Bishop,  2 },
					{ Normal  | Black | Queen,   3 },
					{ Special | Black | King,    4 },
					{ Normal  | Black | Bishop,  5 },
					{ Normal  | Black | Knight,  6 },
					{ Special | Black | Rook,    7 },
					{ Normal  | Black | Pawn,    8 },
					{ Normal  | Black | Pawn,    9 },
					{ Normal  | Black | Pawn,   10 },
					{ Normal  | Black | Pawn,   11 },
					{ Normal  | Black | Pawn,   12 },
					{ Normal  | Black | Pawn,   13 },
					{ Normal  | Black | Pawn,   14 },
					{ Normal  | Black | Pawn,   15 }},

				.king_index =  4,
			},
			{
				.pieces = {
					{ Normal  | White | Pawn,   48 },
					{ Normal  | White | Pawn,   49 },
					{ Normal  | White | Pawn,   50 },
					{ Normal  | White | Pawn,   51 },
					{ Normal  | White | Pawn,   52 },
					{ Normal  | White | Pawn,   53 },
					{ Normal  | White | Pawn,   54 },
					{ Normal  | White | Pawn,   55 },
					{ Special | White | Rook,   56 },
					{ Normal  | White | Knight, 57 },
					{ Normal  | White | Bishop, 58 },
					{ Normal  | White | Queen,  59 },
					{ Special | White | King,   60 },
					{ Normal  | White | Bishop, 61 },
					{ Normal  | White | Knight, 62 },
					{ Special | White | Rook,   63 }},

				.king_index = 12,
			},
		},

		.en_passant    = -1,
		.active_player = White,
		.check_status  = NotInCheck,

		.hm_clock      =  0,
		.turn_counter  =  0,
	};

	for(i = 0; i < 16; ++i) {
		game->board[i]      = game->players[0].pieces + i; /* Black */
		game->board[i + 48] = game->players[1].pieces + i; /* White */
	}
}

void
update(ChessGame* game)
{
	switch(game->active_player) {
		case Black:
			game->active_player = White;
			++(game->turn_counter);
			break;

		case White:
			game->active_player = Black;
			break;
	}

	game->check_status = verify_check_status(game, game->active_player);
}

