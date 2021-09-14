#include "MoveInfo.h"
#include "Utility.h"

#include <stdlib.h>

static int can_check_after_move(
		ChessGame* game,
		unsigned char start,
		unsigned char king_pos,
		unsigned char move_start,
		unsigned char move_end,
		char ignore_pos);

int
can_check_after_move(
		ChessGame* game,
		unsigned char start,
		unsigned char king_pos,
		unsigned char move_start,
		unsigned char move_end,
		char ignore_pos)
{
	unsigned int i, pos, piece_idx;
	int move_idx;
	ChessPiece* piece_ptr;
	const MoveSignature* mov_sig;

	if(start == move_end || start == ignore_pos) {
		return 0;
	}

	piece_idx = pieceset_to_index(game->board[start]->code);
	move_idx  = move_options[start][piece_idx].options_map[king_pos];

	if(move_idx == -1) {
		/* Piece cannot reach king no matter what, so it cannot put the
		 * king in check */

		return 0;
	}

	mov_sig = move_options[start][piece_idx].options + move_idx;

	switch(mov_sig->flag) {
		case CastleQueen: /* FALL THROUGH */
		case CastleKing:  /* FALL THROUGH */
		case EnPassant:
			/* Pieces cannot be taken with castling, so don't bother
			 * checking the move if so
			 *
			 * Likewise, en passant only captures pawns, not kings
			 *
			 * Note that en passant moves are not indexed in options_map
			 * in favor of regular capture moves, so this is technically
			 * not a concern */

			return 0;

		default:
			break;
	}

	/* This follows relatively the same logic used for
	 * is_move_pseudolegal, with a couple differences:
	 * * The last piece will definitely be the king if it is reached, so
	 *   check the final square against the king's square than whatever
	 *   square is given
	 * * Both move_start or ignore_pos would be empty after a move, so
	 *   treat those squares as empty
	 * * Reference the piece at move_start if the position move_end is
	 *   needed
	 *
	 * Additionally, support for castling or en passant is not included
	 * since they should not appear here */

	for(i = 0; i < mov_sig->num_checks; ++i) {
		pos = mov_sig->squares[i];

		if(pos == ignore_pos || pos == move_start) {
			piece_ptr = NULL;

		} else if(pos == move_end) {
			piece_ptr = game->board[move_start];

		} else if(pos == mov_sig->end_pos) {
			piece_ptr = game->board[king_pos];

		} else {
			piece_ptr = game->board[pos];
		}

		if((mov_sig->inv_masks[i] & NoEmpty) == NoEmpty) {
			if(
					piece_ptr != NULL &&
					(piece_ptr->code & mov_sig->inv_masks[i]) == 0
			) {
				continue;
			}

		} else { /* AllowEmpty */
			if(
					piece_ptr == NULL ||
					(piece_ptr->code & mov_sig->inv_masks[i]) == 0
			) {
				continue;
			}
		}

		return 0;
	}

	return 1;
}

int
is_move_legal(
		ChessGame* game,
		unsigned char start,
		const MoveSignature* mov_sig)
{
	unsigned int i;
	unsigned char king_pos;
	char ignore_pos;

	ChessPlayer* active;
	ChessPlayer* other;

	/* A king cannot castle out of check, so automatically disallow such
	 * behaviour
	 *
	 * Additionally, the only way to escape double check is for the king
	 * to move - making all other moves invalid */

	static const int invalid_check_status[7] = {
		DoubleCheck,               /* NormalMove  */
		0,                         /* RegKingMove */
		SingleCheck | DoubleCheck, /* CastleQueen */
		SingleCheck | DoubleCheck, /* CastleKing  */
		DoubleCheck,               /* EnPassant   */
		DoubleCheck,               /* PawnJump    */
		DoubleCheck,               /* Transform   */
	};

	if(game->check_status & invalid_check_status[mov_sig->flag]) {
		return 0;
	}

	if(!is_move_pseudolegal(game->board, mov_sig)) {
		return 0;
	}

	/* Since a move might be possible, it is necessary to test whether
	 * such a move would put the king in check or not */

	active = game->players + player_to_index(game->active_player);
	other  = game->players + opp_player_to_index(game->active_player);

	/* Get the king's position after the move, should it take place
	 *
	 * Note that if the king moved, checks for legality should use the
	 * king's new position rather than its old one
	 *
	 * If necessary, also note any pieces of importance to ignore */

	switch(mov_sig->flag) {
		case RegKingMove: /* FALL THROUGH */
		case CastleQueen: /* FALL THROUGH */
		case CastleKing:
			ignore_pos = -1;
			king_pos   = mov_sig->end_pos;

			break;

		case EnPassant:
			/* Get the location of the pawn to be taken by en passant */
			ignore_pos = mov_sig->end_pos + (
					game->active_player == Black ? -8 : 8);

			king_pos = active->king->pos;

			break;

		default:
			ignore_pos = -1;
			king_pos   = active->king->pos;
	}

	/* Loop through all of the opponent's pieces, and determine whether
	 * any of them can now put the king in check */

	for(i = 0; i < 16; ++i) {
		if(other->pieces[i].pos == -1) {
			continue;
		}

		if(
				can_check_after_move(
					game, other->pieces[i].pos, king_pos,
					start, mov_sig->end_pos, ignore_pos)
		) {
			return 0;
		}
	}

	/* In addition to the aforementioned rules, a king cannot
	 * castle through check, so also check against a dummy move as if
	 * the king were to only move one square in the desired direction
	 * rather than two */

	switch(mov_sig->flag) {
		case CastleQueen:
			++king_pos;
			break;

		case CastleKing:
			--king_pos;
			break;

		default:
			return 1;
	}

	for(i = 0; i < 16; ++i) {
		if(other->pieces[i].pos == -1) {
			continue;
		}

		if(
				can_check_after_move(
					game, other->pieces[i].pos, king_pos,
					start, king_pos, ignore_pos)
		) {
			return 0;
		}
	}

	return 1;
}

int
is_move_pseudolegal(
		ChessPiece** board,
		const MoveSignature* mov_sig)
{
	unsigned int i, pos;

	for(i = 0; i < mov_sig->num_checks; ++i) {
		pos = mov_sig->squares[i];

		/* mov_sig->inv_masks inverts all legal bitmasks (0x01ff) for
		 * each listed square in mov_sig->squares. The 10th bit (0x0200)
		 * in inv_masks denotes whether the square may match an empty
		 * space or not. This affects which formula(s) may (in)validate
		 * a move for the checked square.
		 *
		 * AllowEmpty (0x0000):
		 *   As the square may be empty, it may match board[pos] if it
		 *   contains a null pointer. Alternatively, it may validate a
		 *   move by confirming that unwanted flags are not toggled on.
		 *
		 *   `inv_masks[pos]` denotes the inverse of all possible flags
		 *   (0x0000 - 0x01ff) for a given position (pos):
		 *     ~(Normal | Black | White | Bishop | Knight | ...)
		 *
		 *   `*board[pos]` denotes the bitset that makes up the piece
		 *   information (0x0000 - 0x01ff) for a given position (pos):
		 *     Normal | Black | Bishop
		 *
		 *   If any `(*board[pos] & inv_masks[pos]) != 0`, then there is
		 *   an invalid bitset at that position and the move is
		 *   invalid.
		 *
		 *   This applies for all moves except for those listed under
		 *   NoEmpty.
		 *
		 * NoEmpty (0x0200):
		 *   This may not match an empty square. As such, board[pos] may
		 *   not contain a null pointer. This only applies for:
		 *   * Pawns capturing a piece
		 *   * * Normally
		 *   * * En passant (specifically when checking for a pawn)
		 *   * Kings castling (specifically when checking for a rook)
		 */

		if((mov_sig->inv_masks[i] & NoEmpty) == NoEmpty) {
			if(
					board[pos] != NULL &&
					(board[pos]->code & mov_sig->inv_masks[i]) == 0
			) {
				continue;
			}

		} else { /* AllowEmpty */
			if(
					board[pos] == NULL ||
					(board[pos]->code & mov_sig->inv_masks[i]) == 0
			) {
				continue;
			}
		}

		return 0;
	}

	return 1;
}

unsigned int
opp_player_to_index(enum Color color)
{
	return color == Black ? 1 : 0;
}
unsigned int
pieceset_to_index(unsigned short int piece)
{
	/* Only the king has extra moves when marked as special.
	 *
	 * "Special" pawns and rooks enable other pieces to make special
	 * moves using them as opposed to being able to make special moves
	 * themselves. Therefore, they return the same index as their
	 * "normal" counterparts 
	 *
	 * When indexing along table, all pieces are bit-shifted right one
	 * in order to reduce the amount of space needed. This saves space
	 * where the table would otherwise need 416 elements to convey the
	 * same information.
	 *
	 * Normal  Black Bishop 0x0141 -> 0x00A0 = 160 ->  0 || [0x21] = 12
	 * Normal  Black King   0x0142 -> 0x00A1 = 161 ->  1 || [0x24] =  3
	 * Normal  Black Knight 0x0144 -> 0x00A2 = 162 ->  2 || [0x30] =  5
	 * Normal  Black Pawn   0x0148 -> 0x00A4 = 164 ->  3 || [0x41] = 13
	 * Special Black Pawn   0x0048 -> 0x0024 =  36 ->  3 || [0x44] =  9
	 * Normal  Black Queen  0x0150 -> 0x00A8 = 168 ->  4 || [0x50] = 11
	 * Normal  Black Rook   0x0160 -> 0x00B0 = 176 ->  5 || [0xA0] =  0
	 * Special Black Rook   0x0060 -> 0x0030 =  48 ->  5 || [0xA1] =  1
	 * Normal  White Bishop 0x0181 -> 0x00C0 = 192 ->  6 || [0xA2] =  2
	 * Normal  White King   0x0182 -> 0x00C1 = 193 ->  7 || [0xA4] =  3
	 * Normal  White Knight 0x0184 -> 0x00C2 = 194 ->  8 || [0xA8] =  4
	 * Normal  White Pawn   0x0188 -> 0x00C4 = 196 ->  9 || [0xB0] =  5
	 * Special White Pawn   0x0088 -> 0x0044 =  68 ->  9 || [0xC0] =  6
	 * Normal  White Queen  0x0190 -> 0x00C8 = 200 -> 10 || [0xC1] =  7
	 * Normal  White Rook   0x01A0 -> 0x00D0 = 208 -> 11 || [0xC2] =  8
	 * Special White Rook   0x00A0 -> 0x0050 =  80 -> 11 || [0xC4] =  9
	 * Special Black King   0x0042 -> 0x0021 =  33 -> 12 || [0xC8] = 10
	 * Special White King   0x0082 -> 0x0041 =  65 -> 13 || [0xD0] = 11
	 */

	static const int piece_indicies[209] = {
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 12, 14, 14,  3, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		 5, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 13, 14, 14,  9, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		11, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		 0,  1,  2, 14,  3, 14, 14, 14,  4, 14, 14, 14, 14, 14, 14, 14,
		 5, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		 6,  7,  8, 14,  9, 14, 14, 14, 10, 14, 14, 14, 14, 14, 14, 14,
		11
	};

	return piece_indicies[piece >> 1];
}

unsigned int
player_to_index(enum Color color)
{
	return color == Black ? 0 : 1;
}

