#include "ChessEngine.h"

bool inLineOfSight(
	int8_t start,
	int8_t end,
	struct Player* attacker,
	struct Player* defender,
	bool clear
) {
	int8_t index = 0;
	int8_t pos;

	switch(rayContains[start][end]) {
		case CP_TOP:      // FALL THROUGH
		case CP_TOPRIGHT: // FALL THROUGH
		case CP_RIGHT:    // FALL THROUGH
		case CP_BOTRIGHT: // FALL THROUGH
		case CP_BOTTOM:   // FALL THROUGH
		case CP_BOTLEFT:  // FALL THROUGH
		case CP_LEFT:     // FALL THROUGH
		case CP_TOPLEFT:
			while((pos = rayStep(&allRays[start][rayContains[start][end]], &index)) != end) {
				if(attacker->pieces.pos[pos] || defender->pieces.pos[pos]) { return false; }
			}

			return !(attacker->pieces.pos[pos]) && (clear ? !(defender->pieces.pos[pos]) : true);

		case CP_NONE:
			return false;

	}
}

int8_t getCheckStatus(struct Player* atk, struct Player* dfn) {
	int8_t checkStatus = CP_NONE;
	// Point moves
	if(
		capturePiece[atk->knight_Q.pos][CP_KNIGHT][atk->color][dfn->kingPos] &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }

	if(
		capturePiece[atk->king.pos][CP_KING][atk->color][dfn->kingPos] &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }

	if(
		capturePiece[atk->knight_K.pos][CP_KNIGHT][atk->color][dfn->kingPos] &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }

	// Ray moves
	if(
		capturePiece[atk->rook_Q.pos][CP_ROOK][atk->color][dfn->kingPos] &&
		inLineOfSight(atk->rook_Q.pos, dfn->kingPos, atk, dfn, false) &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }
	
	if(
		capturePiece[atk->bishop_Q.pos][CP_BISHOP][atk->color][dfn->kingPos] &&
		inLineOfSight(atk->bishop_Q.pos, dfn->kingPos, atk, dfn, false) &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }
	
	if(
		capturePiece[atk->queen.pos][CP_QUEEN][atk->color][dfn->kingPos] &&
		inLineOfSight(atk->queen.pos, dfn->kingPos, atk, dfn, false) &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }
	
	if(
		capturePiece[atk->bishop_K.pos][CP_BISHOP][atk->color][dfn->kingPos] &&
		inLineOfSight(atk->bishop_K.pos, dfn->kingPos, atk, dfn, false) &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }
	
	if(
		capturePiece[atk->rook_K.pos][CP_ROOK][atk->color][dfn->kingPos] &&
		inLineOfSight(atk->rook_K.pos, dfn->kingPos, atk, dfn, false) &&
		++checkStatus == CP_DOUBLE
	) { return checkStatus; }
	
	return checkStatus;
}

void movePiece(struct Player* player, struct ChessPiece* piece, int8_t newPos) {
	player->pieces.pos[piece->pos] = false;
	player->pieces.pos[newPos]     = true;
	player->pieces.piece[newPos]   = piece;

	piece->pos = newPos;

	if(piece->type == CP_KING) {
		player->kingPos = newPos;
	}
}

void findLegalOnRay(
	struct ChessPiece* piece,
	struct Ray* moveList,
	int8_t ray,
	struct CPVector* validList,
	struct Player* active,
	struct Player* other
) {
	const int8_t oldPos = piece->pos;

	for(int8_t* move = moveList[ray].pos; move < moveList[ray].pos + moveList[ray].size; ++move) {
		if(active->pieces.pos[*move]) { return; }

		movePiece(active, piece, *move);

		if(getCheckStatus(other, active) == CP_NONE) {
			validList->validMoves[validList->size++] = (struct MoveDetails) {
				cp: piece,
				newPos: *move,
				castle: CP_NONE
			};
		}

		movePiece(active, piece, oldPos);

		if(other->pieces.pos[*move]) { return; }
	}
}

void findLegalMoves(
	struct ChessPiece* piece,
	struct Vector8* moveList,
	struct CPVector* validList,
	struct Player* active,
	struct Player* other
) {
	const int8_t oldPos = piece->pos;

	for(int8_t* move = moveList->pos; move < moveList->pos + moveList->size; ++move) {
		movePiece(active, piece, *move);

		if(getCheckStatus(other, active) == CP_NONE) {
			validList->validMoves[validList->size++] = (struct MoveDetails) {
				cp: piece,
				newPos: *move,
				castle: CP_NONE
			};
		}

		movePiece(active, piece, oldPos);
	}
}

void addMoves(struct ChessPiece* piece, struct CPVector* validList, struct Player* active, struct Player* other) {
	if(piece->pos == -1) { return; }

	switch(piece->type) {
		case CP_BISHOP:
			findLegalOnRay(piece, rayMoves[piece->pos][CP_BISHOP], 1, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_BISHOP], 3, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_BISHOP], 5, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_BISHOP], 7, validList, active, other);
			break;

		case CP_KING:
			findLegalMoves(piece, &pointMoves[piece->pos][CP_KING],    validList, active, other);
			break;

		case CP_KNIGHT:
			findLegalMoves(piece, &pointMoves[piece->pos][CP_KNIGHT],  validList, active, other);
			break;

		case CP_PAWN:
			switch(piece->color) {
				case CP_WHITE:
					if(inLineOfSight(piece->pos, piece->pos - 8, active, other, true)) {
						validList->validMoves[validList->size++] = (struct MoveDetails) {
							cp: piece,
							newPos: piece->pos - 8,
							castle: CP_NONE
						};

						if(inLineOfSight(piece->pos, piece->pos - 16, active, other, true)) {
							validList->validMoves[validList->size++] = (struct MoveDetails) {
								cp: piece,
								newPos: piece->pos - 16,
								castle: CP_NONE
							};
						}

						if(other->pieces.pos[piece->pos - 15]) {
							validList->validMoves[validList->size++] = (struct MoveDetails) {
								cp: piece,
								newPos: piece->pos - 15,
								castle: CP_NONE
							};
						}

						if(other->pieces.pos[piece->pos - 17]) {
							validList->validMoves[validList->size++] = (struct MoveDetails) {
								cp: piece,
								newPos: piece->pos - 17,
								castle: CP_NONE
							};
						}

					}

					if(other->pieces.pos[piece->pos - 7]) {
						validList->validMoves[validList->size++] = (struct MoveDetails) {
							cp: piece,
							newPos: piece->pos - 7,
							castle: CP_NONE
						};
					}

					if(other->pieces.pos[piece->pos - 9]) {
						validList->validMoves[validList->size++] = (struct MoveDetails) {
							cp: piece,
							newPos: piece->pos - 9,
							castle: CP_NONE
						};
					}

					break;

				case CP_BLACK:
					if(inLineOfSight(piece->pos, piece->pos + 8, active, other, true)) {
						validList->validMoves[validList->size++] = (struct MoveDetails) {
							cp: piece,
							newPos: piece->pos + 8,
							castle: CP_NONE
						};

						if(inLineOfSight(piece->pos, piece->pos + 16, active, other, true)) {
							validList->validMoves[validList->size++] = (struct MoveDetails) {
								cp: piece,
								newPos: piece->pos + 16,
								castle: CP_NONE
							};
						}

						if(other->pieces.pos[piece->pos + 15]) {
							validList->validMoves[validList->size++] = (struct MoveDetails) {
								cp: piece,
								newPos: piece->pos + 15,
								castle: CP_NONE
							};
						}

						if(other->pieces.pos[piece->pos + 17]) {
							validList->validMoves[validList->size++] = (struct MoveDetails) {
								cp: piece,
								newPos: piece->pos + 17,
								castle: CP_NONE
							};
						}

					}

					if(other->pieces.pos[piece->pos + 7]) {
						validList->validMoves[validList->size++] = (struct MoveDetails) {
							cp: piece,
							newPos: piece->pos + 7,
							castle: CP_NONE
						};
					}

					if(other->pieces.pos[piece->pos + 9]) {
						validList->validMoves[validList->size++] = (struct MoveDetails) {
							cp: piece,
							newPos: piece->pos + 9,
							castle: CP_NONE
						};
					}

					break;
			}

			break;

		case CP_QUEEN:
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  0, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  1, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  2, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  3, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  4, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  5, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  6, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_QUEEN],  7, validList, active, other);
			break;

		case CP_ROOK:
			findLegalOnRay(piece, rayMoves[piece->pos][CP_ROOK],   0, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_ROOK],   2, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_ROOK],   4, validList, active, other);
			findLegalOnRay(piece, rayMoves[piece->pos][CP_ROOK],   6, validList, active, other);
			break;

	}
}

void setPiece(struct Player* player, struct ChessPiece* piece, int8_t pos) {
	player->pieces.pos[pos]     = true;
	player->pieces.piece[pos]   = piece;

	piece->pos = pos;

	if(piece->type == CP_KING) {
		player->kingPos = pos;
	}
}

size_t genMoves(struct Board* board) {
	struct Player* active = board->turn ? &(board->black) : &(board->white);
	struct Player* other  = board->turn ? &(board->white) : &(board->black);
	size_t countMoves = 0;

	switch(active->kingCheck) {
		case CP_NONE:   // FALL THROUGH
			if(!active->king.hasMoved) {
				const int8_t initKingPos = active->kingPos;

				if(!active->rook_K.hasMoved) {
					movePiece(active, &(active->king), active->kingPos + 1);
					if(getCheckStatus(other, active) == CP_NONE) {
						movePiece(active, &(active->king), active->kingPos + 1);
						if(getCheckStatus(other, active) == CP_NONE) {
							active->moves.validMoves[active->moves.size++] = (struct MoveDetails) {
								cp: &(active->king),
								newPos: active->kingPos,
								castle: CP_CASTLE_K
							};
						}
					}

					movePiece(active, &(active->king), initKingPos);
				}

				if(!active->rook_Q.hasMoved) {
					movePiece(active, &(active->king), active->kingPos - 1);
					if(getCheckStatus(other, active) == CP_NONE) {
						movePiece(active, &(active->king), active->kingPos - 1);
						if(getCheckStatus(other, active) == CP_NONE) {
							active->moves.validMoves[active->moves.size++] = (struct MoveDetails) {
								cp: &(active->king),
								newPos: active->kingPos,
								castle: CP_CASTLE_Q
							};
						}
					}

					movePiece(active, &(active->king), initKingPos);
				}
			}

		case CP_SINGLE: // FALL THROUGH
			addMoves(&(active->rook_Q),   &(active->moves), active, other);
			addMoves(&(active->knight_Q), &(active->moves), active, other);
			addMoves(&(active->bishop_Q), &(active->moves), active, other);
			addMoves(&(active->queen),    &(active->moves), active, other);
			addMoves(&(active->bishop_K), &(active->moves), active, other);
			addMoves(&(active->knight_K), &(active->moves), active, other);
			addMoves(&(active->rook_K),   &(active->moves), active, other);
			addMoves(&(active->pawn_RQ),  &(active->moves), active, other);
			addMoves(&(active->pawn_NQ),  &(active->moves), active, other);
			addMoves(&(active->pawn_BQ),  &(active->moves), active, other);
			addMoves(&(active->pawn_Q),   &(active->moves), active, other);
			addMoves(&(active->pawn_K),   &(active->moves), active, other);
			addMoves(&(active->pawn_BK),  &(active->moves), active, other);
			addMoves(&(active->pawn_NK),  &(active->moves), active, other);
			addMoves(&(active->pawn_RK),  &(active->moves), active, other);

		case CP_DOUBLE:
			addMoves(&(active->king),     &(active->moves), active, other);
			break;
	}

	return active->moves.size;
}

bool vaidateMove(struct Board* board, int8_t moveNum) {
	return (moveNum >= 0) && (moveNum < (board->turn ? board->black.moves.size : board->white.moves.size));
}

void doMove(struct Board* board, int8_t moveNum) {
	// This assumes that a valid move number was passed!
	struct Player* active = board->turn ? &(board->black) : &(board->white);
	movePiece(active, active->moves.validMoves[moveNum].cp, active->moves.validMoves[moveNum].newPos);
}

void postMoveActions(struct Board* board, int8_t moveNum) {
	// This assumes that a valid move number was passed!
	struct Player* active = board->turn ? &(board->black) : &(board->white);
	struct Player* other  = board->turn ? &(board->white) : &(board->black);

	other->kingCheck = getCheckStatus(active, other);

	switch(active->moves.validMoves[moveNum].castle) {
		case CP_NONE:
			// Castling cannot take pieces
			// So only check for overlapping pieces if not castling
			if(other->pieces.pos[active->moves.validMoves[moveNum].newPos]) {
				other->pieces.piece[active->moves.validMoves[moveNum].newPos]->pos = -1;
				other->pieces.pos[active->moves.validMoves[moveNum].newPos] = false;
			}

			break;

		case CP_CASTLE_K:
			movePiece(active, &(active->rook_K), board->turn ? 5: 61);
			break;
		case CP_CASTLE_Q:
			movePiece(active, &(active->rook_K), board->turn ? 3: 59);
			break;
	}

	active->moves.size = 0;
}

void updateGameStatus(struct Board* board) {
	struct Player* player = board->turn ? &(board->black) : &(board->white);

	if(player->moves.size == 0) {
		/* These are the two possible conditions, as far as I am aware:
		 * Checkmate - King is in check and there are no valid moves
		 * Stalemate - King is not in check and there are no valid moves
		 */

		board->isOver = true;
	}
}

void switchTurn(struct Board* board) {
	board->turn = board->turn ? CP_WHITE : CP_BLACK;
}
