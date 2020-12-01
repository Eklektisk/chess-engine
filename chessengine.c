#include "ChessEngine.h"
#include "Lookups.h"

/* LOS End Conditions *****/
#define CP_ATTACKER 0
#define CP_DEFENDER 1
#define CP_BOTH     2
/**************************/

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
	int8_t condition
) {
	int8_t selRay = rayContains[start][end];

	if(selRay == CP_NONE) {
		return false;
	}

	int8_t index = 0;
	int8_t pos   = allRays[start][selRay].pos[index++];

	while(pos != end) {
		if(attacker->pieces.pos[pos] || defender->pieces.pos[pos]) { return false; }
		pos = allRays[start][selRay].pos[index++];
	}

	switch(condition) {
		case CP_NONE:
			return !(attacker->pieces.pos[pos] || defender->pieces.pos[pos]);
		case CP_ATTACKER:
			return !defender->pieces.pos[pos];
		case CP_DEFENDER:
			return !attacker->pieces.pos[pos];
		case CP_BOTH:
			return true;
	}

	return false;
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
				inLineOfSight(pieceOne->pos, pieceTwo->pos, attacker, defender, CP_DEFENDER)
			);

		case CP_KING:   // FALL THROUGH
		case CP_KNIGHT: // FALL THROUGH
		case CP_PAWN:
			return capturePiece[pieceOne->pos][pieceOne->type][pieceOne->color][pieceTwo->pos];
	}

	return false;
}

bool validateMove(struct Player* attacker, struct Player* defender, int8_t ignorePos) {
	if(!(attacker->rook_Q.taken || attacker->rook_Q.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->rook_Q),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->knight_Q.taken || attacker->knight_Q.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->knight_Q), &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->bishop_Q.taken || attacker->bishop_Q.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->bishop_Q), &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->queen.taken || attacker->queen.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->queen),    &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->king.taken || attacker->king.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->king),     &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->bishop_K.taken || attacker->bishop_K.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->bishop_K), &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->knight_K.taken || attacker->knight_K.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->knight_K), &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->rook_K.taken || attacker->rook_K.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->rook_K),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_RQ.taken || attacker->pawn_RQ.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_RQ),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_NQ.taken || attacker->pawn_NQ.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_NQ),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_BQ.taken || attacker->pawn_BQ.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_BQ),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_Q.taken || attacker->pawn_Q.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_Q),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_K.taken || attacker->pawn_K.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_K),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_BK.taken || attacker->pawn_BK.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_BK),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_NK.taken || attacker->pawn_NK.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_NK),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	if(!(attacker->pawn_RK.taken || attacker->pawn_RK.pos == ignorePos)) {
		if(canPieceCapture(&(attacker->pawn_RK),   &(defender->king), attacker, defender)) {
			return false;
		}
	}

	return true;
}

int8_t getCheckStatus(struct Player* attacker, struct Player* defender) {
	int8_t checkStatus = CP_NONE;

	if(!attacker->rook_Q.taken) {
		if(canPieceCapture(&(attacker->rook_Q),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->knight_Q.taken) {
		if(canPieceCapture(&(attacker->knight_Q), &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->bishop_Q.taken) {
		if(canPieceCapture(&(attacker->bishop_Q), &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->queen.taken) {
		if(canPieceCapture(&(attacker->queen),    &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->king.taken) {
		if(canPieceCapture(&(attacker->king),     &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->bishop_K.taken) {
		if(canPieceCapture(&(attacker->bishop_K), &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->knight_K.taken) {
		if(canPieceCapture(&(attacker->knight_K), &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->rook_K.taken) {
		if(canPieceCapture(&(attacker->rook_K),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_RQ.taken) {
		if(canPieceCapture(&(attacker->pawn_RQ),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_NQ.taken) {
		if(canPieceCapture(&(attacker->pawn_NQ),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_BQ.taken) {
		if(canPieceCapture(&(attacker->pawn_BQ),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_Q.taken) {
		if(canPieceCapture(&(attacker->pawn_Q),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_K.taken) {
		if(canPieceCapture(&(attacker->pawn_K),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_BK.taken) {
		if(canPieceCapture(&(attacker->pawn_BK),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_NK.taken) {
		if(canPieceCapture(&(attacker->pawn_NK),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

	if(!attacker->pawn_RK.taken) {
		if(canPieceCapture(&(attacker->pawn_RK),   &(defender->king), attacker, defender)) {
			if(++checkStatus == CP_DOUBLE) {
				return CP_DOUBLE;
			}
		}
	}

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

	for(int8_t i = 0; i <  moveList->size; ++i) {
		if(active->pieces.pos[moveList->pos[i]]) { break; }

		movePiece(active, piece, moveList->pos[i]);

		if(validateMove(other, active, moveList->pos[i])) {
			validList->mv[(validList->size)++] = (struct MoveDetails) {
				cp: piece,
				newPos: moveList->pos[i],
				castle: CP_NONE,
				jump: false,
				enPassant: false,
				transform: CP_NONE
			};
		}

		if(other->pieces.pos[moveList->pos[i]]) { break; }
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

	for(int8_t i = 0; i < moveList->size; ++i) {
		if(active->pieces.pos[moveList->pos[i]]) { continue; }

		movePiece(active, piece, moveList->pos[i]);

		if(validateMove(other, active, moveList->pos[i])) {
			validList->mv[(validList->size)++] = (struct MoveDetails) {
				cp: piece,
				newPos: moveList->pos[i],
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
	if(piece->taken) { return; }

	switch(piece->type) {
		case CP_BISHOP:
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_BISHOP][1]), validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_BISHOP][3]), validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_BISHOP][5]), validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_BISHOP][7]), validList, active, other);
			break;

		case CP_KING:
			findLegalMoves(piece, &(pointMoves[piece->pos][CP_KING]),    validList, active, other);
			break;

		case CP_KNIGHT:
			findLegalMoves(piece, &(pointMoves[piece->pos][CP_KNIGHT]),  validList, active, other);
			break;

		case CP_PAWN:
			switch(piece->color) {
				case CP_WHITE:
					/*
					 * Moving to:                | Action:
					 * * 00 01 02 03 04 05 06 07 | Transform to B,N,Q,R
					 * * 48 49 50 51 52 53 54 55 | Check for jump
					 * * ----------------------- | Nothing
					 */

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
								if(validateMove(other, active, -1)) {
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
								if(validateMove(other, active, -1)) {
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

									if(validateMove(other, active, -1)) {
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

					/*
					 * Starting at:              | Action:
					 * * 08 09 10 11 12 13 14 15 | Transform to B,N,Q,R
					 * * 24 25 26 27 28 29 30 31 | Check for en passant
					 * * ----------------------- | Nothing
					 */

					switch(piece->pos) {
						case  8: // FALL THROUGH
						case  9: // FALL THROUGH
						case 10: // FALL THROUGH
						case 11: // FALL THROUGH
						case 12: // FALL THROUGH
						case 13: // FALL THROUGH
						case 14: // FALL THROUGH
						case 15:
							if(piece->pos != 15 && other->pieces.pos[piece->pos - 7]) {
								movePiece(active, piece, piece->pos - 7);

								if(validateMove(other, active, piece->pos)) {
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

							if(piece->pos != 8 && other->pieces.pos[piece->pos - 9]) {
								movePiece(active, piece, piece->pos - 9);

								if(validateMove(other, active, piece->pos)) {
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
							if(piece->pos % 8 != 7 && other->pieces.pos[piece->pos - 7]) {
								movePiece(active, piece, piece->pos - 7);

								if(validateMove(other, active, piece->pos)) {
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

							if(piece->pos % 8 != 0 && other->pieces.pos[piece->pos - 9]) {
								movePiece(active, piece, piece->pos - 9);

								if(validateMove(other, active, piece->pos)) {
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
							if(
								piece->pos != 31 &&
								other->pieces.pos[piece->pos + 1] &&
								other->pieces.piece[piece->pos + 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos + 1]->jumped
							) {
								movePiece(active, piece, piece->pos - 7);

								if(validateMove(other, active, piece->pos + 8)) {
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
								piece->pos  != 24 &&
								other->pieces.pos[piece->pos - 1] &&
								other->pieces.piece[piece->pos - 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos - 1]->jumped
							) {
								movePiece(active, piece, piece->pos - 9);

								if(validateMove(other, active, piece->pos + 8)) {
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
					/*
					 * Moving to:                | Action:
					 * * 56 57 58 59 60 61 62 63 | Transform to B,N,Q,R
					 * * 16 17 18 19 20 21 22 23 | Check for jump
					 * * ----------------------- | Nothing
					 */

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
								if(validateMove(other, active, -1)) {
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
								if(validateMove(other, active, -1)) {
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

									if(validateMove(other, active, -1)) {
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

					/*
					 * Starting at:              | Action:
					 * * 48 49 50 51 52 53 54 55 | Transform to B,N,Q,R
					 * * 32 33 34 35 36 37 38 39 | Check for en passant
					 * * ----------------------- | Nothing
					 */

					switch(piece->pos) {
						case 48: // FALL THROUGH
						case 49: // FALL THROUGH
						case 50: // FALL THROUGH
						case 51: // FALL THROUGH
						case 52: // FALL THROUGH
						case 53: // FALL THROUGH
						case 54: // FALL THROUGH
						case 55:
							if(piece->pos != 48 && other->pieces.pos[piece->pos + 7]) {
								movePiece(active, piece, piece->pos + 7);

								if(validateMove(other, active, piece->pos)) {
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

							if(piece->pos != 55 && other->pieces.pos[piece->pos + 9]) {
								movePiece(active, piece, piece->pos + 9);

								if(validateMove(other, active, piece->pos)) {
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
							if(piece->pos % 8 != 0 && other->pieces.pos[piece->pos + 7]) {
								movePiece(active, piece, piece->pos + 7);

								if(validateMove(other, active, piece->pos)) {
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

							if(piece->pos % 8 != 7 && other->pieces.pos[piece->pos + 9]) {
								movePiece(active, piece, piece->pos + 9);

								if(validateMove(other, active, piece->pos)) {
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
								piece->pos != 32 &&
								other->pieces.pos[piece->pos - 1] &&
								other->pieces.piece[piece->pos - 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos - 1]->jumped
							) {
								movePiece(active, piece, piece->pos + 7);

								if(validateMove(other, active, piece->pos - 8)) {
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
								piece->pos != 39 &&
								other->pieces.pos[piece->pos + 1] &&
								other->pieces.piece[piece->pos + 1]->type == CP_PAWN &&
								other->pieces.piece[piece->pos + 1]->jumped
							) {
								movePiece(active, piece, piece->pos + 9);

								if(validateMove(other, active, piece->pos - 8)) {
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
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][0]),  validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][1]),  validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][2]),  validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][3]),  validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][4]),  validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][5]),  validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][6]),  validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_QUEEN][7]),  validList, active, other);
			break;

		case CP_ROOK:
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_ROOK][0]),   validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_ROOK][2]),   validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_ROOK][4]),   validList, active, other);
			findLegalOnRay(piece, &(rayMoves[piece->pos][CP_ROOK][6]),   validList, active, other);
			break;
	}
}

void addCastle(struct Player* active, struct Player* other) {
	if(active->king.hasMoved) { return; }

	int8_t kingPos = active->kingPos;

	if(!active->rook_Q.taken) {
		if(
			!active->rook_Q.hasMoved &&
			inLineOfSight(kingPos, active->rook_Q.pos, active, other, CP_ATTACKER)
		) {
			movePiece(active, &(active->king), active->kingPos - 1);

			if(validateMove(other, active, -1)) {
				movePiece(active, &(active->king), active->kingPos - 1);

				if(validateMove(other, active, -1)) {
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

			movePiece(active, &(active->king), kingPos);
		}
	}


	if(!active->rook_K.taken) {
		if(
			!active->rook_K.hasMoved &&
			inLineOfSight(kingPos, active->rook_K.pos, active, other, CP_ATTACKER)
		) {
			movePiece(active, &(active->king), active->kingPos + 1);

			if(validateMove(other, active, -1)) {
				movePiece(active, &(active->king), active->kingPos + 1);

				if(validateMove(other, active, -1)) {
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

			movePiece(active, &(active->king), kingPos);
		}
	}
}

/// HEADER-DEFINED FUNCTIONS
void setPieceType(struct ChessPiece* piece, int8_t type) {
	piece->type = type;
}

void setPiecePos(struct Player* player, struct ChessPiece* piece, int8_t pos) {
	if(piece->pos >= 0 && piece->pos < 64) {
		player->pieces.pos[piece->pos] = false;
	}

	player->pieces.pos[pos]     = true;
	player->pieces.piece[pos]   = piece;

	piece->pos  = pos;

	if(piece->type == CP_KING) {
		player->kingPos = pos;
	}
}

void setPieceFlags(struct ChessPiece* piece, bool hasMoved, bool jumped) {
	piece->hasMoved = hasMoved;
	piece->jumped   = jumped;
}

void initBoard(struct Board* board) {
	*board = (struct Board) {
		white: (struct Player) {
			color: CP_WHITE,
			kingCheck: CP_NONE,
			kingPos: 60,
			rook_Q:   { pos: 56, color: CP_WHITE, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			knight_Q: { pos: 57, color: CP_WHITE, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			bishop_Q: { pos: 58, color: CP_WHITE, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			queen:    { pos: 59, color: CP_WHITE, type: CP_QUEEN,  taken: false, hasMoved: false, jumped: false },
			king:     { pos: 60, color: CP_WHITE, type: CP_KING,   taken: false, hasMoved: false, jumped: false },
			bishop_K: { pos: 61, color: CP_WHITE, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			knight_K: { pos: 62, color: CP_WHITE, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			rook_K:   { pos: 63, color: CP_WHITE, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			pawn_RQ:  { pos: 48, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NQ:  { pos: 49, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BQ:  { pos: 50, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_Q:   { pos: 51, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_K:   { pos: 52, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BK:  { pos: 53, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NK:  { pos: 54, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_RK:  { pos: 55, color: CP_WHITE, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		},
		black: (struct Player) {
			color: CP_BLACK,
			kingCheck: CP_NONE,
			kingPos: 4,
			rook_Q:   { pos:  0, color: CP_BLACK, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			knight_Q: { pos:  1, color: CP_BLACK, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			bishop_Q: { pos:  2, color: CP_BLACK, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			queen:    { pos:  3, color: CP_BLACK, type: CP_QUEEN,  taken: false, hasMoved: false, jumped: false },
			king:     { pos:  4, color: CP_BLACK, type: CP_KING,   taken: false, hasMoved: false, jumped: false },
			bishop_K: { pos:  5, color: CP_BLACK, type: CP_BISHOP, taken: false, hasMoved: false, jumped: false },
			knight_K: { pos:  6, color: CP_BLACK, type: CP_KNIGHT, taken: false, hasMoved: false, jumped: false },
			rook_K:   { pos:  7, color: CP_BLACK, type: CP_ROOK,   taken: false, hasMoved: false, jumped: false },
			pawn_RQ:  { pos:  8, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NQ:  { pos:  9, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BQ:  { pos: 10, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_Q:   { pos: 11, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_K:   { pos: 12, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_BK:  { pos: 13, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_NK:  { pos: 14, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
			pawn_RK:  { pos: 15, color: CP_BLACK, type: CP_PAWN,   taken: false, hasMoved: false, jumped: false },
		},
		active: CP_WHITE
	};

	board->white.pieces.pos[56] = true; board->white.pieces.piece[56] = &(board->white.rook_Q);
	board->white.pieces.pos[57] = true; board->white.pieces.piece[57] = &(board->white.knight_Q);
	board->white.pieces.pos[58] = true; board->white.pieces.piece[58] = &(board->white.bishop_Q);
	board->white.pieces.pos[59] = true; board->white.pieces.piece[59] = &(board->white.queen);
	board->white.pieces.pos[60] = true; board->white.pieces.piece[60] = &(board->white.king);
	board->white.pieces.pos[61] = true; board->white.pieces.piece[61] = &(board->white.bishop_K);
	board->white.pieces.pos[62] = true; board->white.pieces.piece[62] = &(board->white.knight_K);
	board->white.pieces.pos[63] = true; board->white.pieces.piece[63] = &(board->white.rook_K);
	board->white.pieces.pos[48] = true; board->white.pieces.piece[48] = &(board->white.pawn_RQ);
	board->white.pieces.pos[49] = true; board->white.pieces.piece[49] = &(board->white.pawn_NQ);
	board->white.pieces.pos[50] = true; board->white.pieces.piece[50] = &(board->white.pawn_BQ);
	board->white.pieces.pos[51] = true; board->white.pieces.piece[51] = &(board->white.pawn_Q);
	board->white.pieces.pos[52] = true; board->white.pieces.piece[52] = &(board->white.pawn_K);
	board->white.pieces.pos[53] = true; board->white.pieces.piece[53] = &(board->white.pawn_BK);
	board->white.pieces.pos[54] = true; board->white.pieces.piece[54] = &(board->white.pawn_NK);
	board->white.pieces.pos[55] = true; board->white.pieces.piece[55] = &(board->white.pawn_RK);

	board->black.pieces.pos[ 0] = true; board->black.pieces.piece[ 0] = &(board->black.rook_Q);
	board->black.pieces.pos[ 1] = true; board->black.pieces.piece[ 1] = &(board->black.knight_Q);
	board->black.pieces.pos[ 2] = true; board->black.pieces.piece[ 2] = &(board->black.bishop_Q);
	board->black.pieces.pos[ 3] = true; board->black.pieces.piece[ 3] = &(board->black.queen);
	board->black.pieces.pos[ 4] = true; board->black.pieces.piece[ 4] = &(board->black.king);
	board->black.pieces.pos[ 5] = true; board->black.pieces.piece[ 5] = &(board->black.bishop_K);
	board->black.pieces.pos[ 6] = true; board->black.pieces.piece[ 6] = &(board->black.knight_K);
	board->black.pieces.pos[ 7] = true; board->black.pieces.piece[ 7] = &(board->black.rook_K);
	board->black.pieces.pos[ 8] = true; board->black.pieces.piece[ 8] = &(board->black.pawn_RQ);
	board->black.pieces.pos[ 9] = true; board->black.pieces.piece[ 9] = &(board->black.pawn_NQ);
	board->black.pieces.pos[10] = true; board->black.pieces.piece[10] = &(board->black.pawn_BQ);
	board->black.pieces.pos[11] = true; board->black.pieces.piece[11] = &(board->black.pawn_Q);
	board->black.pieces.pos[12] = true; board->black.pieces.piece[12] = &(board->black.pawn_K);
	board->black.pieces.pos[13] = true; board->black.pieces.piece[13] = &(board->black.pawn_BK);
	board->black.pieces.pos[14] = true; board->black.pieces.piece[14] = &(board->black.pawn_NK);
	board->black.pieces.pos[15] = true; board->black.pieces.piece[15] = &(board->black.pawn_RK);
}

size_t genMoves(struct Board* board) {
	struct Player* active = board->active ? &board->black : &board->white;
	struct Player* other  = board->active ? &board->white : &board->black;

	active->moves.size = 0;

	switch(active->kingCheck) {
		case CP_NONE:   // FALL THROUGH
			addCastle(active, other);

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
	}
	
	return active->moves.size;
}

bool checkStalemate(struct Board* board) {
	if(
		board->white.rook_Q.taken &&
		board->white.knight_Q.taken &&
		board->white.bishop_Q.taken &&
		board->white.queen.taken &&
		board->white.bishop_K.taken &&
		board->white.knight_K.taken &&
		board->white.rook_K.taken &&
		board->white.pawn_RQ.taken &&
		board->white.pawn_NQ.taken &&
		board->white.pawn_BQ.taken &&
		board->white.pawn_Q.taken &&
		board->white.pawn_K.taken &&
		board->white.pawn_BK.taken &&
		board->white.pawn_NK.taken &&
		board->white.pawn_RK.taken
	) { return true; }

	if(
		board->black.rook_Q.taken &&
		board->black.knight_Q.taken &&
		board->black.bishop_Q.taken &&
		board->black.queen.taken &&
		board->black.bishop_K.taken &&
		board->black.knight_K.taken &&
		board->black.rook_K.taken &&
		board->black.pawn_RQ.taken &&
		board->black.pawn_NQ.taken &&
		board->black.pawn_BQ.taken &&
		board->black.pawn_Q.taken &&
		board->black.pawn_K.taken &&
		board->black.pawn_BK.taken &&
		board->black.pawn_NK.taken &&
		board->black.pawn_RK.taken
	) { return true; }

	return false;
}

void doMove(struct Board* board, int8_t moveNum) {
	struct Player* player  = board->active ? &board->black : &board->white;
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
	struct Player* active = board->active ? &board->black : &board->white;
	struct Player* other  = board->active ? &board->white : &board->black;

	struct MoveDetails* mv = active->moves.mv + moveNum;

	if(!mv->cp->hasMoved)           { mv->cp->hasMoved = true; }
	if(mv->cp->jumped != mv->jump ) { mv->cp->jumped   = mv->jump; }
	if(mv->transform != CP_NONE)    { mv->cp->type     = mv->transform; }

	switch(mv->castle) {
		case CP_NONE:
			// Castling cannot take pieces
			// So only check for overlapping pieces if not castling
			if(other->pieces.pos[mv->newPos]) {
				other->pieces.piece[mv->newPos]->pos   = -1;
				other->pieces.piece[mv->newPos]->taken = true;
				other->pieces.pos[mv->newPos]          = false;

			} else if(mv->enPassant) {
				int8_t rmPiecePos = mv->cp->color ? (mv->newPos - 8) : (mv->newPos + 8);

				other->pieces.piece[rmPiecePos]->pos   = -1;
				other->pieces.piece[rmPiecePos]->taken = true;
				other->pieces.pos[rmPiecePos]          = false;
			}

			break;

		case CP_CASTLE_Q:
			movePiece(active, &(active->rook_Q), board->active ? 3 : 59);
			active->rook_Q.hasMoved = true;
			break;

		case CP_CASTLE_K:
			movePiece(active, &(active->rook_K), board->active ? 5 : 61);
			active->rook_K.hasMoved = true;
			break;
	}

	active->kingCheck = CP_NONE;
	other->kingCheck  = getCheckStatus(active, other);
}

void switchTurn(struct Board* board) {
	board->active = board->active ? CP_WHITE : CP_BLACK;
}

