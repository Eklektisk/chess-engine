#include <stdio.h>
#include <stdlib.h>

#include "ChessEngine.h"

void printMove(struct MoveDetails* move) {
	printf("Old pos: %d\nNew Pos: %d\n", move->cp->pos, move->newPos);
}

void printBoard(struct Board* game) {
	char letter[64]  = "                                                                ";
	int8_t color[64] = {CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE};

	int8_t checkPos = -1;

	if(game->white.kingCheck != CP_NONE) {
		checkPos = game->white.kingPos;
	}
	if(game->black.kingCheck != CP_NONE) {
		checkPos = game->black.kingPos;
	}

	letter[game->white.rook_Q.pos] = 'R'; color[game->white.rook_Q.pos] = CP_WHITE;
	letter[game->white.knight_Q.pos] = 'N'; color[game->white.knight_Q.pos] = CP_WHITE;
	letter[game->white.bishop_Q.pos] = 'B'; color[game->white.bishop_Q.pos] = CP_WHITE;
	letter[game->white.queen.pos] = 'Q'; color[game->white.queen.pos] = CP_WHITE;
	letter[game->white.king.pos] = 'K'; color[game->white.king.pos] = CP_WHITE;
	letter[game->white.bishop_K.pos] = 'B'; color[game->white.bishop_K.pos] = CP_WHITE;
	letter[game->white.knight_K.pos] = 'N'; color[game->white.knight_K.pos] = CP_WHITE;
	letter[game->white.rook_K.pos] = 'R'; color[game->white.rook_K.pos] = CP_WHITE;
	letter[game->white.pawn_RQ.pos] = 'P'; color[game->white.pawn_RQ.pos] = CP_WHITE;
	letter[game->white.pawn_NQ.pos] = 'P'; color[game->white.pawn_NQ.pos] = CP_WHITE;
	letter[game->white.pawn_BQ.pos] = 'P'; color[game->white.pawn_BQ.pos] = CP_WHITE;
	letter[game->white.pawn_Q.pos] = 'P'; color[game->white.pawn_Q.pos] = CP_WHITE;
	letter[game->white.pawn_K.pos] = 'P'; color[game->white.pawn_K.pos] = CP_WHITE;
	letter[game->white.pawn_BK.pos] = 'P'; color[game->white.pawn_BK.pos] = CP_WHITE;
	letter[game->white.pawn_NK.pos] = 'P'; color[game->white.pawn_NK.pos] = CP_WHITE;
	letter[game->white.pawn_RK.pos] = 'P'; color[game->white.pawn_RK.pos] = CP_WHITE;

	letter[game->black.rook_Q.pos] = 'R'; color[game->black.rook_Q.pos] = CP_BLACK;
	letter[game->black.knight_Q.pos] = 'N'; color[game->black.knight_Q.pos] = CP_BLACK;
	letter[game->black.bishop_Q.pos] = 'B'; color[game->black.bishop_Q.pos] = CP_BLACK;
	letter[game->black.queen.pos] = 'Q'; color[game->black.queen.pos] = CP_BLACK;
	letter[game->black.king.pos] = 'K'; color[game->black.king.pos] = CP_BLACK;
	letter[game->black.bishop_K.pos] = 'B'; color[game->black.bishop_K.pos] = CP_BLACK;
	letter[game->black.knight_K.pos] = 'N'; color[game->black.knight_K.pos] = CP_BLACK;
	letter[game->black.rook_K.pos] = 'R'; color[game->black.rook_K.pos] = CP_BLACK;
	letter[game->black.pawn_RQ.pos] = 'P'; color[game->black.pawn_RQ.pos] = CP_BLACK;
	letter[game->black.pawn_NQ.pos] = 'P'; color[game->black.pawn_NQ.pos] = CP_BLACK;
	letter[game->black.pawn_BQ.pos] = 'P'; color[game->black.pawn_BQ.pos] = CP_BLACK;
	letter[game->black.pawn_Q.pos] = 'P'; color[game->black.pawn_Q.pos] = CP_BLACK;
	letter[game->black.pawn_K.pos] = 'P'; color[game->black.pawn_K.pos] = CP_BLACK;
	letter[game->black.pawn_BK.pos] = 'P'; color[game->black.pawn_BK.pos] = CP_BLACK;
	letter[game->black.pawn_NK.pos] = 'P'; color[game->black.pawn_NK.pos] = CP_BLACK;
	letter[game->black.pawn_RK.pos] = 'P'; color[game->black.pawn_RK.pos] = CP_BLACK;

	int8_t background = CP_WHITE;
	for(int8_t y = 0; y < 8; ++y) {
		for(int8_t x = 0; x < 8; ++x) {
			if(8 * y + x == checkPos) {
				printf("\e[41m");
			} else {
				switch(background) {
					case CP_WHITE: printf("\e[43m"); break;
					case CP_BLACK: printf("\e[40m"); break;
				}
			}

			switch(color[8 * y + x]) {
				case CP_WHITE: printf("\e[93m"); break;
				case CP_BLACK: printf("\e[90m"); break;
			}

			printf("%c\e[0m", letter[8 * y + x]);
			background = background ? CP_WHITE : CP_BLACK;
		}

		printf("\n");
		background = background ? CP_WHITE : CP_BLACK;
	}
}

int main(int argc, char** argv) {
	if(argc >= 2) {
		srand(atoi(argv[1]));
		printf("Using seed %d\n", atoi(argv[1]));

	} else {
		printf("Using seed 0\n");

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

	printBoard(&game);
	getchar();

	while(genMoves(&game) > 0) {
		printf(game.turn ? "Black\n" : "White\n");
		printf("%d moves available\n", (game.turn ? game.black : game.white).moves.size);

		switch((game.turn ? game.black : game.white).kingCheck) {
			case CP_NONE:   printf("Not in check\n"); break;
			case CP_SINGLE: printf("In check (S)\n"); break;
			case CP_DOUBLE: printf("In check (D)\n"); break;
		}

		uint8_t moveNum = rand() % (game.turn ? game.black : game.white).moves.size;

		printMove((game.turn ? game.black : game.white).moves.mv + moveNum);
		doMove(&game, moveNum);
		postMoveActions(&game, moveNum);

		printBoard(&game);
		getchar();
		switchTurn(&game);
	}

	return EXIT_SUCCESS;
}
