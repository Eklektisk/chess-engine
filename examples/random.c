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

	return '?';
}

void printMove(struct MoveDetails* move) {
	printf("Old pos: %d\nNew Pos: %d\n", move->cp->pos, move->newPos);
}

void printBoard(struct Board* game, int8_t oldPos, int8_t newPos) {
	char letter[64]  = "                                                                ";
	int8_t color[64] = {CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE,CP_NONE};

	int8_t checkPos = -1;

	if(game->white.kingCheck != CP_NONE) {
		checkPos = game->white.kingPos;
	}
	if(game->black.kingCheck != CP_NONE) {
		checkPos = game->black.kingPos;
	}

	if(game->white.rook_Q.pos > -1)   { letter[game->white.rook_Q.pos] = getLetter(game->white.rook_Q.type); color[game->white.rook_Q.pos] = CP_WHITE; }
	if(game->white.knight_Q.pos > -1) { letter[game->white.knight_Q.pos] = getLetter(game->white.knight_Q.type); color[game->white.knight_Q.pos] = CP_WHITE; }
	if(game->white.bishop_Q.pos > -1) { letter[game->white.bishop_Q.pos] = getLetter(game->white.bishop_Q.type); color[game->white.bishop_Q.pos] = CP_WHITE; }
	if(game->white.queen.pos > -1)    { letter[game->white.queen.pos] = getLetter(game->white.queen.type); color[game->white.queen.pos] = CP_WHITE; }
	if(game->white.king.pos > -1)     { letter[game->white.king.pos] = getLetter(game->white.king.type); color[game->white.king.pos] = CP_WHITE; }
	if(game->white.bishop_K.pos > -1) { letter[game->white.bishop_K.pos] = getLetter(game->white.bishop_K.type); color[game->white.bishop_K.pos] = CP_WHITE; }
	if(game->white.knight_K.pos > -1) { letter[game->white.knight_K.pos] = getLetter(game->white.knight_K.type); color[game->white.knight_K.pos] = CP_WHITE; }
	if(game->white.rook_K.pos > -1)   { letter[game->white.rook_K.pos] = getLetter(game->white.rook_K.type); color[game->white.rook_K.pos] = CP_WHITE; }
	if(game->white.pawn_RQ.pos > -1)  {  letter[game->white.pawn_RQ.pos] = getLetter(game->white.pawn_RQ.type); color[game->white.pawn_RQ.pos] = CP_WHITE; }
	if(game->white.pawn_NQ.pos > -1)  {  letter[game->white.pawn_NQ.pos] = getLetter(game->white.pawn_NQ.type); color[game->white.pawn_NQ.pos] = CP_WHITE; }
	if(game->white.pawn_BQ.pos > -1)  {  letter[game->white.pawn_BQ.pos] = getLetter(game->white.pawn_BQ.type); color[game->white.pawn_BQ.pos] = CP_WHITE; }
	if(game->white.pawn_Q.pos > -1)   {  letter[game->white.pawn_Q.pos] = getLetter(game->white.pawn_Q.type); color[game->white.pawn_Q.pos] = CP_WHITE; }
	if(game->white.pawn_K.pos > -1)   {  letter[game->white.pawn_K.pos] = getLetter(game->white.pawn_K.type); color[game->white.pawn_K.pos] = CP_WHITE; }
	if(game->white.pawn_BK.pos > -1)  {  letter[game->white.pawn_BK.pos] = getLetter(game->white.pawn_BK.type); color[game->white.pawn_BK.pos] = CP_WHITE; }
	if(game->white.pawn_NK.pos > -1)  {  letter[game->white.pawn_NK.pos] = getLetter(game->white.pawn_NK.type); color[game->white.pawn_NK.pos] = CP_WHITE; }
	if(game->white.pawn_RK.pos > -1)  {  letter[game->white.pawn_RK.pos] = getLetter(game->white.pawn_RK.type); color[game->white.pawn_RK.pos] = CP_WHITE; }

	if(game->black.rook_Q.pos > -1)   { letter[game->black.rook_Q.pos] = getLetter(game->black.rook_Q.type); color[game->black.rook_Q.pos] = CP_BLACK; }
	if(game->black.knight_Q.pos > -1) { letter[game->black.knight_Q.pos] = getLetter(game->black.knight_Q.type); color[game->black.knight_Q.pos] = CP_BLACK; }
	if(game->black.bishop_Q.pos > -1) { letter[game->black.bishop_Q.pos] = getLetter(game->black.bishop_Q.type); color[game->black.bishop_Q.pos] = CP_BLACK; }
	if(game->black.queen.pos > -1)    { letter[game->black.queen.pos] = getLetter(game->black.queen.type); color[game->black.queen.pos] = CP_BLACK; }
	if(game->black.king.pos > -1)     { letter[game->black.king.pos] = getLetter(game->black.king.type); color[game->black.king.pos] = CP_BLACK; }
	if(game->black.bishop_K.pos > -1) { letter[game->black.bishop_K.pos] = getLetter(game->black.bishop_K.type); color[game->black.bishop_K.pos] = CP_BLACK; }
	if(game->black.knight_K.pos > -1) { letter[game->black.knight_K.pos] = getLetter(game->black.knight_K.type); color[game->black.knight_K.pos] = CP_BLACK; }
	if(game->black.rook_K.pos > -1)   { letter[game->black.rook_K.pos] = getLetter(game->black.rook_K.type); color[game->black.rook_K.pos] = CP_BLACK; }
	if(game->black.pawn_RQ.pos > -1)  { letter[game->black.pawn_RQ.pos] = getLetter(game->black.pawn_RQ.type); color[game->black.pawn_RQ.pos] = CP_BLACK; }
	if(game->black.pawn_NQ.pos > -1)  { letter[game->black.pawn_NQ.pos] = getLetter(game->black.pawn_NQ.type); color[game->black.pawn_NQ.pos] = CP_BLACK; }
	if(game->black.pawn_BQ.pos > -1)  { letter[game->black.pawn_BQ.pos] = getLetter(game->black.pawn_BQ.type); color[game->black.pawn_BQ.pos] = CP_BLACK; }
	if(game->black.pawn_Q.pos > -1)   { letter[game->black.pawn_Q.pos] = getLetter(game->black.pawn_Q.type); color[game->black.pawn_Q.pos] = CP_BLACK; }
	if(game->black.pawn_K.pos > -1)   { letter[game->black.pawn_K.pos] = getLetter(game->black.pawn_K.type); color[game->black.pawn_K.pos] = CP_BLACK; }
	if(game->black.pawn_BK.pos > -1)  { letter[game->black.pawn_BK.pos] = getLetter(game->black.pawn_BK.type); color[game->black.pawn_BK.pos] = CP_BLACK; }
	if(game->black.pawn_NK.pos > -1)  { letter[game->black.pawn_NK.pos] = getLetter(game->black.pawn_NK.type); color[game->black.pawn_NK.pos] = CP_BLACK; }
	if(game->black.pawn_RK.pos > -1)  { letter[game->black.pawn_RK.pos] = getLetter(game->black.pawn_RK.type); color[game->black.pawn_RK.pos] = CP_BLACK; }

	int8_t background = CP_WHITE;
	for(int8_t y = 0; y < 8; ++y) {
		for(int8_t x = 0; x < 8; ++x) {
			int8_t pos = 8 * y + x;

			if(pos == checkPos) {
				printf("\e[41m");
			} else if(pos == oldPos) {
				switch(color[newPos]) {
					case CP_WHITE: printf("\e[46m"); break;
					case CP_BLACK: printf("\e[44m"); break;
				}
			} else {
				switch(background) {
					case CP_WHITE: printf("\e[43m"); break;
					case CP_BLACK: printf("\e[40m"); break;
				}
			}

			if(pos == newPos) {
				switch(color[pos]) {
					case CP_WHITE: printf("\e[96m"); break;
					case CP_BLACK: printf("\e[94m"); break;
				}
			} else {
				switch(color[pos]) {
					case CP_WHITE: printf("\e[93m"); break;
					case CP_BLACK: printf("\e[90m"); break;
				}
			}

			printf("%c\e[0m", letter[pos]);
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

	struct Board game;
	initBoard(&game);

	printBoard(&game, -1, -1);
	getchar();

	int turn = 0;

	while(!checkStalemate(&game) && genMoves(&game) > 0) {
		printf("Turn %d - %s\n", turn++, game.active ? "Black" : "White");
		printf("%zu moves available\n", (game.active ? game.black : game.white).moves.size);

		switch((game.active ? game.black : game.white).kingCheck) {
			case CP_NONE:   printf("Not in check\n"); break;
			case CP_SINGLE: printf("In check (S)\n"); break;
			case CP_DOUBLE: printf("In check (D)\n"); break;
		}

		uint8_t moveNum = rand() % (game.active ? game.black : game.white).moves.size;
		struct MoveDetails* move = (game.active ? game.black : game.white).moves.mv + moveNum;
		uint8_t oldPos = move->cp->pos;
		uint8_t newPos = move->newPos;

		printMove(move);
		doMove(&game, moveNum);
		postMoveActions(&game, moveNum);

		printBoard(&game, oldPos, newPos);
		getchar();
		switchTurn(&game);
	}

	printf("GAME OVER\n");
	printf("Turn %d - %s\n", turn++, game.active ? "Black" : "White");
	switch((game.active ? game.black : game.white).kingCheck) {
		case CP_NONE:   printf("Not in check\n"); break;
		case CP_SINGLE: printf("In check (S)\n"); break;
		case CP_DOUBLE: printf("In check (D)\n"); break;
	}

	printBoard(&game, -1, -1);

	return EXIT_SUCCESS;
}
