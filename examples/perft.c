#include <stdio.h>
#include <stdlib.h>

#include <ChessEngine.h>
#include <ChessHistory.h>

void
print(ChessPiece** board)
{
	unsigned char x, y;

	for(y = 0; y < 8; ++y) {
		for(x = 0; x < 8; ++x) {
			if(board[y * 8 + x] == NULL) {
				printf(".");
				continue;
			}

			switch(board[y * 8 + x]->code & 0x00ff) {
				case White | Bishop:
					printf("B");
					break;

				case White | King:
					printf("K");
					break;

				case White | Knight:
					printf("N");
					break;

				case White | Pawn:
					printf("P");
					break;

				case White | Queen:
					printf("Q");
					break;

				case White | Rook:
					printf("R");
					break;

				case Black | Bishop:
					printf("b");
					break;

				case Black | King:
					printf("k");
					break;

				case Black | Knight:
					printf("n");
					break;

				case Black | Pawn:
					printf("p");
					break;

				case Black | Queen:
					printf("q");
					break;

				case Black | Rook:
					printf("r");
					break;

				default:
					printf("?");
					break;
			}
		}

		printf("\n");
	}

	printf("\n");
}

int
main()
{
	ChessGame chess_game;
	MovesList possible_moves;
	ChessHistory history;

	init_game(&chess_game);
	init_history(&history, 10);

	switch(chess_game.check_status) {
		case NotInCheck:
			printf("White is not in check\n");
			break;

		case SingleCheck:
			printf("White is in check by one piece\n");
			break;

		case DoubleCheck:
			printf("White is in check by many pieces\n");
			break;
	}

	print(chess_game.board);

	printf("There are %d possible moves\n",
			generate_moves_list(&chess_game, &possible_moves));

	do_move_and_record(&chess_game, &possible_moves, 0, &history);
	update(&chess_game);

	switch(chess_game.check_status) {
		case NotInCheck:
			printf("Black is not in check\n");
			break;

		case SingleCheck:
			printf("Black is in check by one piece\n");
			break;

		case DoubleCheck:
			printf("Black is in check by many pieces\n");
			break;
	}

	print(chess_game.board);

	printf("There are %d possible moves\n",
			generate_moves_list(&chess_game, &possible_moves));

	undo_moves(&chess_game, &history, 1);

	switch(chess_game.check_status) {
		case NotInCheck:
			printf("White is not in check\n");
			break;

		case SingleCheck:
			printf("White is in check by one piece\n");
			break;

		case DoubleCheck:
			printf("White is in check by many pieces\n");
			break;
	}

	print(chess_game.board);

	printf("There are %d possible moves\n",
			generate_moves_list(&chess_game, &possible_moves));

	free_history(&history);

	return EXIT_SUCCESS;
}

