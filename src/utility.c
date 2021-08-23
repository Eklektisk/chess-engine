#include "MoveInfo.h"
#include "Utility.h"

#include <stdlib.h>

int
can_check_after_move(
		ChessPiece** board,
		unsigned char start,
		unsigned char king_pos,
		unsigned char move_start,
		unsigned char move_end,
		char ignore_pos)
{
	int i, pos, piece_idx, move_idx;
	ChessPiece* piece_ptr;
	const MoveSignature* mov_sig;

	if(board[start] == NULL) {
		return 0;
	}

	piece_idx = pieceset_to_index(board[start]->code);
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
	 * Additionally, support for the afforementioned moves is not
	 * included since they should not appear here */

	for(i = 0; i < mov_sig->num_checks; ++i) {
		pos = mov_sig->squares[i];

		if(pos == ignore_pos || pos == move_start) {
			piece_ptr = NULL;

		} else if(pos == move_end) {
			piece_ptr = board[move_start];

		} else if(pos == mov_sig->end_pos) {
			piece_ptr = board[king_pos];

		} else {
			piece_ptr = board[pos];
		}

		if(piece_ptr == NULL) {
			switch(mov_sig->inv_masks[i] & NoEmpty) {
				case AllowEmpty: continue;
				case NoEmpty:    return 0;
			}
		}

		if((piece_ptr->code & mov_sig->inv_masks[i]) != 0) {
			return 0;
		}
	}

	return 1;
}

int
is_move_legal(
		ChessGame* game,
		unsigned char start,
		const MoveSignature* mov_sig)
{
	int i;
	unsigned char king_pos;
	char ignore_pos;

	ChessPlayer* active;
	ChessPlayer* other;

	/* A king cannot castle out of check, so automatically disallow such
	 * behaviour
	 *
	 * Additionally, the only way to escape double check is for the king
	 * to move - making all other moves invalid */

	switch(mov_sig->flag) {
		case CastleQueen: /* FALL THROUGH */
		case CastleKing:
			if(game->check_status != NotInCheck) {
				return 0;
			}

			break;

		case RegKingMove:
			break;

		default:
			if(game->check_status == DoubleCheck) {
				return 0;
			}

			break;
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
			switch(game->active_player) {
				case Black:
					ignore_pos = mov_sig->end_pos - 8;
					break;

				case White:
					ignore_pos = mov_sig->end_pos + 8;
					break;

				default: /* Invalid active player */
					return 0;
			}

			king_pos   = active->pieces[active->king_index].pos;

			break;

		default:
			ignore_pos = -1;
			king_pos   = active->pieces[active->king_index].pos;
	}

	/* Loop through all of the opponent's pieces, and determine whether
	 * any of them can now put the king in check */

	for(i = 0; i < 16; ++i) {
		if(other->pieces[i].pos == -1) {
			continue;
		}

		if(
				can_check_after_move(
					game->board, other->pieces[i].pos, king_pos,
					start, mov_sig->end_pos, ignore_pos)
		) {
			return 0;
		}
	}

	/* In addition to the aforementioned rules, a king cannot
	 * castle if such a move would move it through a square that
	 * would otherwise put it into check */

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
					game->board, other->pieces[i].pos, king_pos,
					start, mov_sig->end_pos, ignore_pos)
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
	unsigned char i, pos;

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

		if(board[pos] == NULL) {
			switch(mov_sig->inv_masks[i] & NoEmpty) {
				case AllowEmpty: continue;
				case NoEmpty:    return 0;
			}
		}

		if((board[pos]->code & mov_sig->inv_masks[i]) != 0) {
			return 0;
		}
	}

	return 1;
}

int
opp_player_to_index(enum Color color)
{
	switch(color) {
		case Black: return 1;
		case White: return 0;

		default: return -1; /* An invalid color was passed */
	}
}

int
pieceset_to_index(unsigned short int piece)
{
	/* Only the king has extra moves when marked as special.
	 *
	 * "Special" pawns and rooks enable other pieces to make special
	 * moves using them as opposed to being able to make special moves
	 * themselves. Therefore, they return the same index as their
	 * "normal" counterparts */

	switch(piece) {
		case Normal  | Black | Bishop: return  0;
		case Normal  | Black | King:   return  1;
		case Normal  | Black | Knight: return  2;
		case Normal  | Black | Pawn:   return  3;
		case Special | Black | Pawn:   return  3;
		case Normal  | Black | Queen:  return  4;
		case Normal  | Black | Rook:   return  5;
		case Special | Black | Rook:   return  5;
		case Normal  | White | Bishop: return  6;
		case Normal  | White | King:   return  7;
		case Normal  | White | Knight: return  8;
		case Normal  | White | Pawn:   return  9;
		case Special | White | Pawn:   return  9;
		case Normal  | White | Queen:  return 10;
		case Normal  | White | Rook:   return 11;
		case Special | White | Rook:   return 11;
		case Special | Black | King:   return 12;
		case Special | White | King:   return 13;

		default: return -1; /* An invalid bitset was passed to piece */
	}
}

int
player_to_index(enum Color color)
{
	switch(color) {
		case Black: return 0;
		case White: return 1;

		default: return -1; /* An invalid color was passed */
	}
}

enum CheckStatus
verify_check_status(ChessGame* game, enum Color player_color)
{
	int i, end;
	enum CheckStatus check_status = NotInCheck;

	ChessPlayer* check_player; /* Check if this player is in check */
	ChessPlayer* other_player; /* May be put in check by this player */

	check_player = game->players + player_to_index(player_color);
	other_player = game->players + opp_player_to_index(player_color);

	/* If a piece from other_player can reach the king, then
	 * check_player is in some form of check
	 *
	 * As such, check_player's king position is the target
	 * space for other_player's pieces */

	end = check_player->pieces[check_player->king_index].pos;

	for(i = 0; i < 16; ++i) {
		int piece_idx, move_idx;
		int start = other_player->pieces[i].pos;

		if(start == -1) {
			continue;
		}

		piece_idx = pieceset_to_index(game->board[start]->code);
		move_idx  = move_options[start][piece_idx].options_map[end];

		if(move_idx == -1) {
			continue;
		}

		if(
				is_move_pseudolegal(
					game->board,
					move_options[start][piece_idx].options + move_idx)
		) {
			switch(check_status) {
				case NotInCheck:
					/* There is a chance other pieces may put the king
					 * in check, so do not return yet */

					check_status = SingleCheck;
					break;

				default:
					/* At least one piece was previously found to put
					 * the king in check. Since the function is now
					 * guaranteed to return DoubleCheck, it is safe to
					 * stop looking here */

					return DoubleCheck;
			}
		}
	}

	return check_status;
}

