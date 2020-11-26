#include <stdio.h>
#include <stdlib.h>

#include <ChessEngine.h>

char getLetter(int8_t type) {
	switch(type) {
		case CP_BISHOP: return 'B';
		case CP_KING:   return 'K';
		case CP_KNIGHT: return 'N';
		case CP_PAWN:   return 'P';
		case CP_QUEEN:  return 'Q';
		case CP_ROOK:   return 'R';
	}
}

void printMove(struct MoveDetails* move) {
	char oldRow = "ABCDEFGH"[move->cp->pos % 8];
	char oldCol = "12345678"[move->cp->pos / 8];

	char newRow = "ABCDEFGH"[move->newPos % 8];
	char newCol = "12345678"[move->newPos / 8];

	printf("%c%c %c%c\n", oldRow, oldCol, newRow, newCol);
}

int main(int argc, char** argv) {
	unsigned int max = -1;

	switch(argc) {
		case 0: // FALL THROUGH
		case 1:
			printf("Using seed 0\n");
			break;

		case 2:
			srand(atoi(argv[1]));
			printf("Using seed %d\n", atoi(argv[1]));
			break;

		case 3: // FALL THROUGH
		default:
			srand(atoi(argv[1]));
			printf("Using seed %d\n", atoi(argv[1]));

			max = atoi(argv[2]);

			break;
	}

	struct Board game = initBoard();

	setPiece(&game.black, &game.black.rook_Q,    0, false, false);
	setPiece(&game.black, &game.black.knight_Q,  1, false, false);
	setPiece(&game.black, &game.black.bishop_Q,  2, false, false);
	setPiece(&game.black, &game.black.queen,     3, false, false);
	setPiece(&game.black, &game.black.king,      4, false, false);
	setPiece(&game.black, &game.black.bishop_K,  5, false, false);
	setPiece(&game.black, &game.black.knight_K,  6, false, false);
	setPiece(&game.black, &game.black.rook_K,    7, false, false);
	setPiece(&game.black, &game.black.pawn_RQ,   8, false, false);
	setPiece(&game.black, &game.black.pawn_NQ,   9, false, false);
	setPiece(&game.black, &game.black.pawn_BQ,  10, false, false);
	setPiece(&game.black, &game.black.pawn_Q,   11, false, false);
	setPiece(&game.black, &game.black.pawn_K,   12, false, false);
	setPiece(&game.black, &game.black.pawn_BK,  13, false, false);
	setPiece(&game.black, &game.black.pawn_NK,  14, false, false);
	setPiece(&game.black, &game.black.pawn_RK,  15, false, false);

	setPiece(&game.white, &game.white.pawn_RQ,  48, false, false);
	setPiece(&game.white, &game.white.pawn_NQ,  49, false, false);
	setPiece(&game.white, &game.white.pawn_BQ,  50, false, false);
	setPiece(&game.white, &game.white.pawn_Q,   51, false, false);
	setPiece(&game.white, &game.white.pawn_K,   52, false, false);
	setPiece(&game.white, &game.white.pawn_BK,  53, false, false);
	setPiece(&game.white, &game.white.pawn_NK,  54, false, false);
	setPiece(&game.white, &game.white.pawn_RK,  55, false, false);
	setPiece(&game.white, &game.white.rook_Q,   56, false, false);
	setPiece(&game.white, &game.white.knight_Q, 57, false, false);
	setPiece(&game.white, &game.white.bishop_Q, 58, false, false);
	setPiece(&game.white, &game.white.queen,    59, false, false);
	setPiece(&game.white, &game.white.king,     60, false, false);
	setPiece(&game.white, &game.white.bishop_K, 61, false, false);
	setPiece(&game.white, &game.white.knight_K, 62, false, false);
	setPiece(&game.white, &game.white.rook_K,   63, false, false);

	int turn = 0;

	while(genMoves(&game) > 0) {
		printf ("%3d (%s): ", turn++, game.turn ? "Black" : "White");

		uint8_t moveNum = rand() % (game.turn ? game.black : game.white).moves.size;
		struct MoveDetails* move = (game.turn ? game.black : game.white).moves.mv + moveNum;
		uint8_t oldPos = move->cp->pos;
		uint8_t newPos = move->newPos;

		printMove(move);
		doMove(&game, moveNum);
		postMoveActions(&game, moveNum);

		switchTurn(&game);

		if(turn >= max) {
			printf("HIT INSTRUCTION LIMIT\n");
			return EXIT_SUCCESS;
		}
	}

	printf("GAME OVER\n");
	return EXIT_SUCCESS;
}
