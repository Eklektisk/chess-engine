#ifndef CE__UTILITY__H
#define CE__UTILITY__H

#include "ChessEngine.h"

int is_move_legal(
		ChessGame* game,
		unsigned char start,
		const MoveSignature* mov_sig);
int is_move_pseudolegal(
		ChessPiece** board,
		const MoveSignature* mov_sig);
unsigned int opp_player_to_index(enum Color color);
unsigned int pieceset_to_index(unsigned short int piece);
unsigned int player_to_index(enum Color color);

#endif
