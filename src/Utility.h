#ifndef CE__UTILITY__H
#define CE__UTILITY__H

#include "ChessEngine.h"

int can_check_after_move(
		ChessGame* game,
		unsigned char start,
		unsigned char king_pos,
		unsigned char move_start,
		unsigned char move_end,
		char ignore_pos);
int is_move_legal(
		ChessGame* game,
		unsigned char start,
		const MoveSignature* mov_sig);
int is_move_pseudolegal(
		ChessPiece** board,
		const MoveSignature* mov_sig);
int opp_player_to_index(enum Color color);
int pieceset_to_index(unsigned short int piece);
int player_to_index(enum Color color);
enum CheckStatus verify_check_status(
		ChessGame* game,
		enum Color player_color);

#endif
