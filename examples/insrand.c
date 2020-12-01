#include <stdio.h>
#include <stdlib.h>

#include <ChessEngine.h>

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

	struct Board game;
	initBoard(&game);

	int turn = 0;

	while(!checkStalemate(&game) && genMoves(&game) > 0) {
		printf ("%3d (%s): ", turn++, game.active ? "Black" : "White");

		uint8_t moveNum = rand() % (game.active ? game.black : game.white).moves.size;
		struct MoveDetails* move = (game.active ? game.black : game.white).moves.mv + moveNum;

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
