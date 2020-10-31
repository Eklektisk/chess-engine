#include "ChessEngine.h"

size_t genMoves(Game* game, int8_t color) {
	Player* player = color ? &(game->black) : &(game->white);
	size_t countMoves = 0;

	case(player->kingCheck) {
		case CP_NONE:
			break;
		case CP_SINGLE:
			break;
		case CP_DOUBLE:
			// TODO: Might change this to a vector

			if(pointMoves[player->kingPos][CP_KING][player->kingPos - 9]) {
				player->possibleMoves[countMoves].newPos   = player->kingPos - 9;
				player->possibleMoves[countMoves++].castle = CP_NONE;
			}

			(pointMoves[player->kingPos][CP_KING][player->kingPos - 8])
			(pointMoves[player->kingPos][CP_KING][player->kingPos - 7])
			(pointMoves[player->kingPos][CP_KING][player->kingPos - 1])
			(pointMoves[player->kingPos][CP_KING][player->kingPos + 1])
			(pointMoves[player->kingPos][CP_KING][player->kingPos + 7])
			(pointMoves[player->kingPos][CP_KING][player->kingPos + 8])
			(pointMoves[player->kingPos][CP_KING][player->kingPos + 9])

			break;
	}
	struct ChessPiece rook_Q;
	struct ChessPiece knight_Q;
	struct ChessPiece bishop_Q;
	struct ChessPiece queen;
	struct ChessPiece king;
	struct ChessPiece bishop_K;
	struct ChessPiece knight_K;
	struct ChessPiece rook_K;
	struct ChessPiece pawn_RQ;
	struct ChessPiece pawn_NQ;
	struct ChessPiece pawn_BQ;
	struct ChessPiece pawn_Q;
	struct ChessPiece pawn_K;
	struct ChessPiece pawn_BK;
	struct ChessPiece pawn_NK;
	struct ChessPiece pawn_RK;
}

bool vaidateMove(Game* game, int8_t color, int8_t moveNum) {
	return (moveNum >= 0) && (moveNum < (color ? &(game->black) : &(game->white)));
}

void doMove(Game* game, int8_t color, int8_t moveNum) {
	// This assumes that a valid move number was passed!
	Player* player = color ? &(game->black) : &(game->white);
}

void postMoveActions(Game* game, int8_t color, int8_t moveNum) {
	// This assumes that a valid move number was passed!
	Player* player = color ? &(game->black) : &(game->white);
}

void updateCheck(Game* game, int8_t color) {
	Player* activePlayer = color ? &(game->black) : &(game->white);
	Player* otherPlayer  = color ? &(game->white) : &(game->black);
}

void updateTurn(Game* game) {
	game->turn = game->turn ? CP_WHITE : CP_BLACK;
}

