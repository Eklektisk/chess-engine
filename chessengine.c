#include "ChessEngine.h"
#include "Lookups.h"

void movePiece(struct Player* player, struct ChessPiece* piece, int8_t newPos) {
	player->pieces.pos[piece->pos] = false;
	player->pieces.pos[newPos]     = true;
	player->pieces.piece[newPos]   = piece;

	piece->pos = newPos;

	if(piece->type == CP_KING) {
		player->kingPos = newPos;
	}
}

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

			if(clear) {
				return !(attacker->pieces.pos[pos] || defender->pieces.pos[pos]);
			} else {
				return !attacker->pieces.pos[pos];
			}

		case CP_NONE:
			return false;

	}
}

bool canPieceCapture(
	struct ChessPiece* pieceOne,
	struct ChessPiece* pieceTwo,
	struct Player* attacker,
	struct Player* defender
) {
	if(pieceOne->color == pieceTwo->color) { return false; }

	switch(pieceOne->type) {
		case CP_BISHOP: // FALL THROUGH
		case CP_QUEEN:  // FALL THROUGH
		case CP_ROOK:
			return (
				capturePiece[pieceOne->pos][pieceOne->type][pieceOne->color][pieceTwo->pos] &&
				inLineOfSight(pieceOne->pos, pieceTwo->pos, attacker, defender, false)
			);

		case CP_KING:   // FALL THROUGH
		case CP_KNIGHT: // FALL THROUGH
		case CP_PAWN:
			return capturePiece[pieceOne->pos][pieceOne->type][pieceOne->color][pieceTwo->pos];
	}
}

bool ignorePiece(struct ChessPiece* piece, int8_t ignorePos) {
	switch(ignorePos) {
		case -1:
			return piece->taken;
		default:
			return piece->taken || piece->pos == ignorePos;
	}
}

int8_t getCheckStatus(struct Player* attacker, struct Player* defender, int8_t ignorePos) {
	int8_t checkStatus = CP_NONE;

	if(
		!ignorePiece(&attacker->rook_Q, ignorePos) &&
		canPieceCapture(&(attacker->rook_Q),  &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->knight_Q, ignorePos) &&
		canPieceCapture(&(attacker->knight_Q), &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->bishop_Q, ignorePos) &&
		canPieceCapture(&(attacker->bishop_Q), &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->queen, ignorePos) &&
		canPieceCapture(&(attacker->queen),    &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->king, ignorePos) &&
		canPieceCapture(&(attacker->king),     &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->bishop_K, ignorePos) &&
		canPieceCapture(&(attacker->bishop_K), &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->knight_K, ignorePos) &&
		canPieceCapture(&(attacker->knight_K), &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->rook_K, ignorePos) &&
		canPieceCapture(&(attacker->rook_K),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_RQ, ignorePos) &&
		canPieceCapture(&(attacker->pawn_RQ),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_NQ, ignorePos) &&
		canPieceCapture(&(attacker->pawn_NQ),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_BQ, ignorePos) &&
		canPieceCapture(&(attacker->pawn_BQ),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_Q, ignorePos) &&
		canPieceCapture(&(attacker->pawn_Q),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_K, ignorePos) &&
		canPieceCapture(&(attacker->pawn_K),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_BK, ignorePos) &&
		canPieceCapture(&(attacker->pawn_BK),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_NK, ignorePos) &&
		canPieceCapture(&(attacker->pawn_NK),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	if(
		!ignorePiece(&attacker->pawn_RK, ignorePos) &&
		canPieceCapture(&(attacker->pawn_RK),   &(defender->king), attacker, defender) &&
		++checkStatus == CP_DOUBLE
	) { return CP_DOUBLE; }

	return checkStatus;
}

void findLegalOnRay(
	struct ChessPiece* piece,
	struct Ray* moveList,
	struct CPVector* validList,
	struct Player* active,
	struct Player* other
) {
	const int8_t initPos = piece->pos;

	for(
		int8_t* move = moveList->pos;
		move < moveList->pos + moveList->size;
		++move
	) {
		if(active->pieces.pos[*move]) { break; }

		movePiece(active, piece, *move);

		if(getCheckStatus(other, active, *move) == CP_NONE) {
			validList->mv[validList->size++] = (struct MoveDetails) {
				cp: piece,
				newPos: *move,
				castle: CP_NONE,
				jump: false,
				enPassant: false,
				transform: CP_NONE
			};
		}

		if(other->pieces.pos[*move]) { break; }
	}

	movePiece(active, piece, initPos);
}

void findLegalMoves(
	struct ChessPiece* piece,
	struct Vector8* moveList,
	struct CPVector* validList,
	struct Player* active,
	struct Player* other
) {
	const int8_t initPos = piece->pos;

	for(
		int8_t* move = moveList->pos;
		move < moveList->pos + moveList->size;
		++move
	) {
		if(active->pieces.pos[*move]) { continue; }

		movePiece(active, piece, *move);

		if(getCheckStatus(other, active, *move) == CP_NONE) {
			validList->mv[validList->size++] = (struct MoveDetails) {
				cp: piece,
				newPos: *move,
				castle: CP_NONE,
				jump: false,
				enPassant: false,
				transform: CP_NONE
			};
		}
	}

	movePiece(active, piece, initPos);
}

void addMoves(struct ChessPiece* piece, struct CPVector* validList, struct Player* active, struct Player* other) {
	if(piece->pos == -1) { return; }

	switch(piece->type) {
		case CP_BISHOP:
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_BISHOP][1], validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_BISHOP][3], validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_BISHOP][5], validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_BISHOP][7], validList, active, other);
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
					if(!(active->pieces.pos[piece->pos - 8] || other->pieces.pos[piece->pos - 8])) {
						int8_t initPos = piece->pos;
						movePiece(active, piece, piece->pos - 8);

						switch(piece->pos) {
							case 0: // FALL THROUGH
							case 1: // FALL THROUGH
							case 2: // FALL THROUGH
							case 3: // FALL THROUGH
							case 4: // FALL THROUGH
							case 5: // FALL THROUGH
							case 6: // FALL THROUGH
							case 7:
								if(getCheckStatus(other, active, -1) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_BISHOP
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_KNIGHT
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_QUEEN
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_ROOK
									};
								}

								break;

							default: // FALL THROUGH
								if(getCheckStatus(other, active, -1) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_NONE
									};
								}

							case 48: // FALL THROUGH
							case 49: // FALL THROUGH
							case 50: // FALL THROUGH
							case 51: // FALL THROUGH
							case 52: // FALL THROUGH
							case 53: // FALL THROUGH
							case 54: // FALL THROUGH
							case 55:
								if(
									!(
										piece->hasMoved ||
										active->pieces.pos[piece->pos - 8] ||
										other->pieces.pos[piece->pos - 8]
									)
								) {
									movePiece(active, piece, piece->pos - 8);

									if(getCheckStatus(other, active, -1) == CP_NONE) {
										validList->mv[validList->size++] = (struct MoveDetails) {
											cp: piece,
											newPos: piece->pos,
											castle: CP_NONE,
											jump: true,
											enPassant: false,
											transform: CP_NONE
										};
									}
								}

								break;
						}

						movePiece(active, piece, initPos);
					}

					switch(piece->pos) {
						case  8: // FALL THROUGH
						case  9: // FALL THROUGH
						case 10: // FALL THROUGH
						case 11: // FALL THROUGH
						case 12: // FALL THROUGH
						case 13: // FALL THROUGH
						case 14: // FALL THROUGH
						case 15:
							if(other->pieces.pos[piece->pos - 7]) {
								movePiece(active, piece, piece->pos - 7);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_BISHOP
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_KNIGHT
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_QUEEN
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_ROOK
									};
								}

								movePiece(active, piece, piece->pos + 7);
							}

							if(other->pieces.pos[piece->pos - 9]) {
								movePiece(active, piece, piece->pos - 9);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_BISHOP
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_KNIGHT
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_QUEEN
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_ROOK
									};
								}

								movePiece(active, piece, piece->pos + 9);
							}

							break;

						default:
							if(other->pieces.pos[piece->pos - 7]) {
								movePiece(active, piece, piece->pos - 7);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos + 7);
							}

							if(other->pieces.pos[piece->pos - 9]) {
								movePiece(active, piece, piece->pos - 9);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos + 9);
							}

							break;

						case 24: // FALL THROUGH
						case 25: // FALL THROUGH
						case 26: // FALL THROUGH
						case 27: // FALL THROUGH
						case 28: // FALL THROUGH
						case 29: // FALL THROUGH
						case 30: // FALL THROUGH
						case 31:
							// En passant
							if(
								other->pieces.pos[piece->pos + 1] &&
								other->pieces.piece[piece->pos + 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos + 1]->jumped
							) {
								movePiece(active, piece, piece->pos - 7);

								if(getCheckStatus(other, active, piece->pos + 8) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: true,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos + 7);
							}

							if(
								other->pieces.pos[piece->pos - 1] &&
								other->pieces.piece[piece->pos - 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos - 1]->jumped
							) {
								movePiece(active, piece, piece->pos - 9);

								if(getCheckStatus(other, active, piece->pos + 8) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: true,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos + 9);
							}

							break;
					}

					break;

				case CP_BLACK:
					if(!(active->pieces.pos[piece->pos + 8] || other->pieces.pos[piece->pos + 8])) {
						int8_t initPos = piece->pos;
						movePiece(active, piece, piece->pos + 8);

						switch(piece->pos) {
							case 63: // FALL THROUGH
							case 62: // FALL THROUGH
							case 61: // FALL THROUGH
							case 60: // FALL THROUGH
							case 59: // FALL THROUGH
							case 58: // FALL THROUGH
							case 57: // FALL THROUGH
							case 56:
								if(getCheckStatus(other, active, -1) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_BISHOP
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_KNIGHT
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_QUEEN
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_ROOK
									};
								}

								break;

							default: // FALL THROUGH
								if(getCheckStatus(other, active, -1) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_NONE
									};
								}

							case  8: // FALL THROUGH
							case  9: // FALL THROUGH
							case 10: // FALL THROUGH
							case 11: // FALL THROUGH
							case 12: // FALL THROUGH
							case 13: // FALL THROUGH
							case 14: // FALL THROUGH
							case 15:
								if(
									!(
										piece->hasMoved ||
										active->pieces.pos[piece->pos + 8] ||
										other->pieces.pos[piece->pos + 8]
									)
								) {
									movePiece(active, piece, piece->pos + 8);

									if(getCheckStatus(other, active, -1) == CP_NONE) {
										validList->mv[validList->size++] = (struct MoveDetails) {
											cp: piece,
											newPos: piece->pos,
											castle: CP_NONE,
											jump: true,
											enPassant: false,
											transform: CP_NONE
										};
									}
								}

								break;
						}

						movePiece(active, piece, initPos);
					}

					switch(piece->pos) {
						case 48: // FALL THROUGH
						case 49: // FALL THROUGH
						case 50: // FALL THROUGH
						case 51: // FALL THROUGH
						case 52: // FALL THROUGH
						case 53: // FALL THROUGH
						case 54: // FALL THROUGH
						case 55:
							if(other->pieces.pos[piece->pos + 7]) {
								movePiece(active, piece, piece->pos + 7);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_BISHOP
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_KNIGHT
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_QUEEN
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_ROOK
									};
								}

								movePiece(active, piece, piece->pos - 7);
							}

							if(other->pieces.pos[piece->pos + 9]) {
								movePiece(active, piece, piece->pos + 9);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_BISHOP
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_KNIGHT
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_QUEEN
									};
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_ROOK
									};
								}

								movePiece(active, piece, piece->pos - 9);
							}

							break;

						default:
							if(other->pieces.pos[piece->pos + 7]) {
								movePiece(active, piece, piece->pos + 7);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos - 7);
							}

							if(other->pieces.pos[piece->pos + 9]) {
								movePiece(active, piece, piece->pos + 9);

								if(getCheckStatus(other, active, piece->pos) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: false,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos - 9);
							}

							break;

						case 32: // FALL THROUGH
						case 33: // FALL THROUGH
						case 34: // FALL THROUGH
						case 35: // FALL THROUGH
						case 36: // FALL THROUGH
						case 37: // FALL THROUGH
						case 38: // FALL THROUGH
						case 39:
							// En passant
							if(
								other->pieces.pos[piece->pos - 1] &&
								other->pieces.piece[piece->pos - 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos - 1]->jumped
							) {
								movePiece(active, piece, piece->pos + 7);

								if(getCheckStatus(other, active, piece->pos - 8) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: true,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos - 7);
							}

							if(
								other->pieces.pos[piece->pos + 1] &&
								other->pieces.piece[piece->pos + 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos + 1]->jumped
							) {
								movePiece(active, piece, piece->pos + 9);

								if(getCheckStatus(other, active, piece->pos - 8) == CP_NONE) {
									validList->mv[validList->size++] = (struct MoveDetails) {
										cp: piece,
										newPos: piece->pos,
										castle: CP_NONE,
										jump: false,
										enPassant: true,
										transform: CP_NONE
									};
								}

								movePiece(active, piece, piece->pos - 9);
							}

							break;
					}

					break;
			}

			break;

		case CP_QUEEN:
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][0],  validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][1],  validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][2],  validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][3],  validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][4],  validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][5],  validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][6],  validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_QUEEN][7],  validList, active, other);
			break;

		case CP_ROOK:
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_ROOK][0],   validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_ROOK][2],   validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_ROOK][4],   validList, active, other);
			findLegalOnRay(piece, &rayMoves[piece->pos][CP_ROOK][6],   validList, active, other);
			break;
	}
}

void addCastle(struct Player* active, struct Player* other) {
	if(active->king.hasMoved) { return; }

	int8_t kingPos = active->kingPos;

	if(
		!active->rook_Q.hasMoved ||
		inLineOfSight(kingPos, active->rook_Q.pos, active, other, true)
	) {
		movePiece(active, &(active->king), active->kingPos - 1);

		if(getCheckStatus(other, active, -1) == CP_NONE) {
			movePiece(active, &(active->king), active->kingPos - 1);

			if(getCheckStatus(other, active, -1) == CP_NONE) {
				active->moves.mv[active->moves.size++] = (struct MoveDetails) {
					cp: &(active->king),
					newPos: kingPos - 2,
					castle: CP_CASTLE_Q,
					jump: false,
					enPassant: false,
					transform: CP_NONE
				};
			}
		}
	}

	movePiece(active, &(active->king), kingPos);

	if(
		!active->rook_K.hasMoved ||
		inLineOfSight(kingPos, active->rook_K.pos, active, other, true)
	) {
		movePiece(active, &(active->king), active->kingPos + 1);

		if(getCheckStatus(other, active, -1) == CP_NONE) {
			movePiece(active, &(active->king), active->kingPos + 1);

			if(getCheckStatus(other, active, -1) == CP_NONE) {
				active->moves.mv[active->moves.size++] = (struct MoveDetails) {
					cp: &(active->king),
					newPos: kingPos + 2,
					castle: CP_CASTLE_K,
					jump: false,
					enPassant: false,
					transform: CP_NONE
				};
			}
		}
	}

	movePiece(active, &(active->king), kingPos);
}

/// HEADER-DEFINED FUNCTIONS

struct Board initBoard() {
	return (struct Board) {
		white: (struct Player) {
			color: CP_WHITE,
			kingCheck: CP_NONE,
			rook_Q:   { color: CP_WHITE, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			knight_Q: { color: CP_WHITE, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			bishop_Q: { color: CP_WHITE, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			queen:    { color: CP_WHITE, type: CP_QUEEN,  taken: false, hasMoved: false, jumped: false },
			king:     { color: CP_WHITE, type: CP_KING,   taken: false, hasMoved: false, jumped: false },
			bishop_K: { color: CP_WHITE, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			knight_K: { color: CP_WHITE, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			rook_K:   { color: CP_WHITE, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			pawn_RQ:  { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NQ:  { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BQ:  { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_Q:   { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_K:   { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BK:  { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NK:  { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_RK:  { color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		},
		black: (struct Player) {
			color: CP_BLACK,
			kingCheck: CP_NONE,
			rook_Q:   { color: CP_BLACK, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			knight_Q: { color: CP_BLACK, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			bishop_Q: { color: CP_BLACK, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			queen:    { color: CP_BLACK, type: CP_QUEEN,  taken: false, hasMoved: false, jumped: false },
			king:     { color: CP_BLACK, type: CP_KING,   taken: false, hasMoved: false, jumped: false },
			bishop_K: { color: CP_BLACK, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			knight_K: { color: CP_BLACK, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			rook_K:   { color: CP_BLACK, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			pawn_RQ:  { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NQ:  { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BQ:  { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_Q:   { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_K:   { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BK:  { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NK:  { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_RK:  { color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		},
		turn: CP_WHITE,
		isOver: false
	};
}

void setPiece(
	struct Player* player,
	struct ChessPiece* piece,
	int8_t pos,
	bool hasMoved,
	bool jumped
) {
	player->pieces.pos[pos]     = true;
	player->pieces.piece[pos]   = piece;

	piece->pos = pos;

	if(piece->type == CP_KING) {
		player->kingPos = pos;
	}

	piece->hasMoved = hasMoved;
	piece->jumped   = jumped;
}

size_t genMoves(struct Board* board) {
	struct Player* active = board->turn ? &board->black : &board->white;
	struct Player* other  = board->turn ? &board->white : &board->black;

	active->moves.size = 0;

	switch(active->kingCheck) {
		case CP_NONE:   // FALL THROUGH
			addCastle(active, other);

		case CP_SINGLE: // FALL THROUGH
			addMoves(&active->rook_Q,   &active->moves, active, other);
			addMoves(&active->knight_Q, &active->moves, active, other);
			addMoves(&active->bishop_Q, &active->moves, active, other);
			addMoves(&active->queen,    &active->moves, active, other);
			addMoves(&active->bishop_K, &active->moves, active, other);
			addMoves(&active->knight_K, &active->moves, active, other);
			addMoves(&active->rook_K,   &active->moves, active, other);
			addMoves(&active->pawn_RQ,  &active->moves, active, other);
			addMoves(&active->pawn_NQ,  &active->moves, active, other);
			addMoves(&active->pawn_BQ,  &active->moves, active, other);
			addMoves(&active->pawn_Q,   &active->moves, active, other);
			addMoves(&active->pawn_K,   &active->moves, active, other);
			addMoves(&active->pawn_BK,  &active->moves, active, other);
			addMoves(&active->pawn_NK,  &active->moves, active, other);
			addMoves(&active->pawn_RK,  &active->moves, active, other);

		case CP_DOUBLE:
			addMoves(&active->king,     &active->moves, active, other);
	}
	
	return active->moves.size;
}

void doMove(struct Board* board, int8_t moveNum) {
	struct Player* player  = board->turn ? &board->black : &board->white;
	struct MoveDetails* mv = player->moves.mv + moveNum;

	player->pieces.pos[mv->cp->pos]  = false;
	player->pieces.pos[mv->newPos]   = true;
	player->pieces.piece[mv->newPos] = mv->cp;

	mv->cp->pos = mv->newPos;

	if(mv->cp->type == CP_KING) {
		player->kingPos = mv->newPos;
	}
}

void postMoveActions(struct Board* board, int8_t moveNum) {
	struct Player* active = board->turn ? &board->black : &board->white;
	struct Player* other  = board->turn ? &board->white : &board->black;

	struct MoveDetails* mv = active->moves.mv + moveNum;

	if(!mv->cp->hasMoved)           { mv->cp->hasMoved = true; }
	if(mv->cp->jumped != mv->jump ) { mv->cp->jumped   = mv->jump; }
	if(mv->transform != CP_NONE)    { mv->cp->type     = mv->transform; }

	switch(mv->castle) {
		case CP_NONE:
			// Castling cannot take pieces
			// So only check for overlapping pieces if not castling
			if(other->pieces.pos[mv->newPos]) {
				other->pieces.piece[mv->newPos]->pos = -1;
				other->pieces.pos[mv->newPos]        = false;
			} else if(mv->enPassant) {
				int8_t rmPiecePos = mv->cp->color ? (mv->newPos - 8) : (mv->newPos + 8);
				other->pieces.piece[rmPiecePos]->pos = -1;
				other->pieces.pos[rmPiecePos]        = false;
			}

			break;

		case CP_CASTLE_Q:
			movePiece(active, &(active->rook_Q), board->turn ? 3: 59);
			active->rook_Q.hasMoved = true;
			break;

		case CP_CASTLE_K:
			movePiece(active, &(active->rook_K), board->turn ? 5: 61);
			active->rook_K.hasMoved = true;
			break;
	}

	active->kingCheck = CP_NONE;
	other->kingCheck  = getCheckStatus(active, other, -1);
}

void switchTurn(struct Board* board) {
	board->turn = board->turn ? CP_WHITE : CP_BLACK;
}

