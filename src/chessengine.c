#include "ChessEngine.h"
#include "MoveInfo.h"
#include "Utility.h"

#include <stdlib.h>

static void update_check_status(ChessGame* game);

void
do_move(ChessGame* game, MoveOp* move_obj)
{
	int start = move_obj->old_pos;
	int end   = move_obj->new_pos;

	switch(move_obj->_flag) {
		case NormalMove:  /* FALL THROUGH */
		case RegKingMove:
			if(game->board[end]) {
				/* Capturing piece */

				game->board[end]->pos = -1;
			}

			game->board[end]   = game->board[start];
			game->board[start] = NULL;

			game->board[end]->code |= Normal;
			game->board[end]->pos   = end;

			break;

		case CastleQueen:
			game->board[end]   = game->board[start];
			game->board[start] = NULL;

			game->board[end]->code |= Normal;
			game->board[end]->pos   = end;

			game->board[end + 1]   = game->board[start - 4];
			game->board[start - 4] = NULL;

			game->board[end + 1]->code |= Normal;
			game->board[end + 1]->pos   = end + 1;

			break;

		case CastleKing:
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
			game->board[end]   = game->board[start];
			game->board[start] = NULL;

			game->board[end]->code &= 0x00ff; /* Mark as special */
			game->board[end]->pos   = end;

			break;

		case Transform:
			if(game->board[end]) {
				/* Capturing piece - Remove taken piece from other
				 * player's list */

				game->board[end]->pos = -1;
			}

			game->board[end] = game->board[start];
			game->board[start] = NULL;

			/* Switch piece type by clearing old piece type and then
			 * updating with new one */

			game->board[end]->code &= Normal | Black | White;
			game->board[end]->code |= move_obj->new_type;

			game->board[end]->pos   = end;

			break;
	}
}

unsigned char
generate_moves_list(ChessGame* game, MovesList* moves_list)
{
	unsigned int i, j, start, end, idx;
	ChessPlayer* active;
	unsigned char size = 0;

	active = game->players + player_to_index(game->active_player);

	/* Only the king may move in order to escape double check */
	if(game->check_status == DoubleCheck) {
		start = active->king->pos;
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
					RegKingMove,
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
							Transform,
							Bishop };

						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							Transform,
							Knight };

						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							Transform,
							Queen };

						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							Transform,
							Rook };

						break;

					default:
						moves_list->moves[size++] = (MoveOp) {
							start,
							end,
							move_options[start][idx].options[j].flag,
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
	 * rows A/B and the white player starting on rows G/H
	 *
	 * Pieces are sorted to by the type of moves that arise from those
	 * pieces first, and by alphabetical order second */

	unsigned char i;

	*game = (ChessGame) {
		.players = {
			{
				.pieces = {
					{ Normal  | Black | Bishop,  2 },
					{ Normal  | Black | Bishop,  5 },
					{ Normal  | Black | Knight,  1 },
					{ Normal  | Black | Knight,  6 },
					{ Normal  | Black | Queen,   3 },
					{ Special | Black | Rook,    0 },
					{ Special | Black | Rook,    7 },
					{ Normal  | Black | Pawn,    8 },
					{ Normal  | Black | Pawn,    9 },
					{ Normal  | Black | Pawn,   10 },
					{ Normal  | Black | Pawn,   11 },
					{ Normal  | Black | Pawn,   12 },
					{ Normal  | Black | Pawn,   13 },
					{ Normal  | Black | Pawn,   14 },
					{ Normal  | Black | Pawn,   15 },
					{ Special | Black | King,    4 }},
			},
			{
				.pieces = {
					{ Normal  | White | Bishop, 58 },
					{ Normal  | White | Bishop, 61 },
					{ Normal  | White | Knight, 57 },
					{ Normal  | White | Knight, 62 },
					{ Normal  | White | Queen,  59 },
					{ Special | White | Rook,   56 },
					{ Special | White | Rook,   63 },
					{ Normal  | White | Pawn,   48 },
					{ Normal  | White | Pawn,   49 },
					{ Normal  | White | Pawn,   50 },
					{ Normal  | White | Pawn,   51 },
					{ Normal  | White | Pawn,   52 },
					{ Normal  | White | Pawn,   53 },
					{ Normal  | White | Pawn,   54 },
					{ Normal  | White | Pawn,   55 },
					{ Special | White | King,   60 }},
			},
		},

		.en_passant    = -1,
		.active_player = White,
		.check_status  = NotInCheck,

		.hm_clock      =  0,
		.turn_counter  =  0,
	};

	/* Set pointer to king piece */
	game->players[0].king = game->players[0].pieces + 15;
	game->players[1].king = game->players[1].pieces + 15;

	/* Update board */
	for(i = 0; i < 16; ++i) {
		unsigned char pos;

		pos = game->players[0].pieces[i].pos; /* Black */
		game->board[pos] = game->players[0].pieces + i;

		pos = game->players[1].pieces[i].pos; /* White */
		game->board[pos] = game->players[1].pieces + i;
	}
}

void
load_game(ChessGame* game, char* fen_string)
{
	const static unsigned short int piece_codes[19] = {
		Empty,
		Empty,
		Normal | Bishop,
		Empty,
		Empty,
		Empty,
		Empty,
		Empty,
		Empty,
		Empty,
		Empty,
		Normal | King,
		Empty,
		Empty,
		Normal | Knight,
		Empty,
		Normal | Pawn,
		Normal | Queen,
		Normal | Rook,
	};

	ChessPiece* piece;

	int i, j;
	char process_char = 1;

	int pos = 0;

	/* Reset the game to a blank state */

	for(i = 0; i < 64; ++i) {
		game->board[i] = NULL;
	}

	for(i = 0; i < 16; ++i) {
		game->players[0].pieces[i].pos = -1;
		game->players[1].pieces[i].pos = -1;
	}

	game->players[0].king = NULL;
	game->players[1].king = NULL;

	game->hm_clock     = 0;
	game->turn_counter = 0;

	i = 0;
	j = 0;

	while(process_char) {
		switch(*fen_string) {
			case ' ':
				process_char = 0;
				break;

			case '/':
				break;

			case '1': pos += 1; break;
			case '2': pos += 2; break;
			case '3': pos += 3; break;
			case '4': pos += 4; break;
			case '5': pos += 5; break;
			case '6': pos += 6; break;
			case '7': pos += 7; break;
			case '8': pos += 8; break;

			case 'k':
				game->players[0].king = game->players[0].pieces + i;

			case 'b':
			case 'n':
			case 'p':
			case 'q':
			case 'r':
				piece = game->players[0].pieces + i++;

				piece->code = Black | piece_codes[*(fen_string) & 0x1f];
				piece->pos  = pos++;

				break;

			case 'K':
				game->players[1].king = game->players[1].pieces + j;

			case 'B':
			case 'N':
			case 'P':
			case 'Q':
			case 'R':
				piece = game->players[1].pieces + j++;

				piece->code = White | piece_codes[*(fen_string) & 0x1f];
				piece->pos  = pos++;

				break;
		}

		++fen_string;
	}

	for(i = 0; i < 16; ++i) {
		pos = game->players[0].pieces[i].pos; /* Black */
		if(pos != -1) {
			game->board[pos] = game->players[0].pieces + i;
		}

		pos = game->players[1].pieces[i].pos; /* White */
		if(pos != -1) {
			game->board[pos] = game->players[1].pieces + i;
		}
	}

	game->active_player = *(fen_string++) == 'b' ? Black : White;

	if(*(++fen_string) == '-') {
		fen_string += 2;

	} else {
		process_char = 1;

		while(process_char) {
			switch(*fen_string) {
				case ' ':
					process_char = 0;
					break;

				case 'K':
					game->board[60]->code = Special | White | King;
					game->board[63]->code = Special | White | Rook;

					break;

				case 'Q':
					game->board[60]->code = Special | White | King;
					game->board[56]->code = Special | White | Rook;

					break;

				case 'k':
					game->board[4]->code = Special | Black | King;
					game->board[7]->code = Special | Black | Rook;

					break;

				case 'q':
					game->board[4]->code = Special | Black | King;
					game->board[0]->code = Special | Black | Rook;

					break;
			}

			++fen_string;
		}
	}

	if(*(fen_string++) == '-') {
		++fen_string;

	} else {
		game->en_passant  = *(fen_string++) == 'c' ? 24 : 32;
		game->en_passant += *(fen_string++) - 49;

		game->board[(unsigned char) game->en_passant]->code &= 0x00ff;
	}

	if(*fen_string == '\0') {
		return;
	} else {
		game->hm_clock = (unsigned char) atoi(++fen_string);
	}

	process_char = 1;
	while(process_char) {
		switch(*fen_string) {
			case '\0':
			case '-':
				return;

			case ' ':
				process_char = 0;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
		}

		++fen_string;
	}

	game->hm_clock = (unsigned int) atoi(fen_string) - 1;

	update_check_status(game);
}

void
update_antemove(ChessGame* game, MoveOp* move_obj)
{
	/* Update the en passant status for the game, and remove the Special
	 * flag from the previous en passant piece if applicable */

	if(game->en_passant != -1) {
		game->board[(unsigned char) game->en_passant]->code |= Normal;
	}

	if(move_obj->_flag == PawnJump) {
		game->en_passant = move_obj->new_pos;

	} else {
		game->en_passant = -1;
	}

	/* Update the half-move clock based on whether a pawn was moved or a
	 * piece was captured */

	if(
			game->board[move_obj->new_pos] != NULL ||
			(game->board[move_obj->old_pos]->code & Pawn) != 0
	) {
		game->hm_clock = 0;

	} else {
		++(game->hm_clock);
	}
}

void
update_check_status(ChessGame* game)
{
	unsigned int i, end;
	enum Color active = game->active_player;

	ChessPlayer* other_player; /* May put other player in check */

	other_player = game->players + opp_player_to_index(active);

	game->check_status = NotInCheck;

	/* If a piece from other_player can reach the king, then
	 * check_player is in some form of check
	 *
	 * As such, check_player's king position is the target
	 * space for other_player's pieces */

	end = game->players[player_to_index(active)].king->pos;

	for(i = 0; i < 16; ++i) {
		unsigned int piece_idx;
		int move_idx;
		const MoveSignature* mov_sig;
		int start = other_player->pieces[i].pos;

		if(start == -1) {
			continue;
		}

		piece_idx = pieceset_to_index(game->board[start]->code);
		move_idx  = move_options[start][piece_idx].options_map[end];

		if(move_idx == -1) {
			continue;
		}

		mov_sig = move_options[start][piece_idx].options + move_idx;

		switch(mov_sig->flag) {
			case CastleQueen: /* FALL THROUGH */
			case CastleKing:  /* FALL THROUGH */
			case EnPassant:   /* FALL THROUGH */
				continue;

			default:
				break;
		}

		if(is_move_pseudolegal(game->board, mov_sig)) {
			switch(game->check_status) {
				case NotInCheck:
					/* There is a chance other pieces may put the king
					 * in check, so do not return yet */

					game->check_status = SingleCheck;
					break;

				default:
					/* At least one piece was previously found to put
					 * the king in check. Since the function is now
					 * guaranteed to return DoubleCheck, it is safe to
					 * stop looking here */

					game->check_status = DoubleCheck;
					return;
			}
		}
	}
}

void
update_postmove(ChessGame* game)
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

	update_check_status(game);
}

