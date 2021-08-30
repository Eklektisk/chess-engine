#include <stdio.h>
#include <stdlib.h>

#include <ChessEngine.h>
#include <ChessHistory.h>

void
print(ChessGame* game)
{
	int row, col;

	for(row = 0; row < 8; ++row) {
		for(col = 0; col < 8; ++col) {
			int pos = row * 8 + col;
			if(game->board[pos]) {
				switch(game->board[pos]->code & 0x00ff) {
					case Black | Bishop: printf("b"); break;
					case Black | King:   printf("k"); break;
					case Black | Knight: printf("n"); break;
					case Black | Pawn:   printf("p"); break;
					case Black | Queen:  printf("q"); break;
					case Black | Rook:   printf("r"); break;
					case White | Bishop: printf("B"); break;
					case White | King:   printf("K"); break;
					case White | Knight: printf("N"); break;
					case White | Pawn:   printf("P"); break;
					case White | Queen:  printf("Q"); break;
					case White | Rook:   printf("R"); break;
				}
			} else {
				printf(".");
			}
		}

		printf("\n");
	}

	printf("\n");
}

char
get_char(unsigned short int code) {
	switch(code) {
		case Black | Bishop: return 'b';
		case Black | Knight: return 'k';
		case Black | King:   return 'n';
		case Black | Pawn:   return 'p';
		case Black | Queen:  return 'q';
		case Black | Rook:   return 'r';
		case White | Bishop: return 'B';
		case White | Knight: return 'K';
		case White | King:   return 'N';
		case White | Pawn:   return 'P';
		case White | Queen:  return 'Q';
		case White | Rook:   return 'R';
		default:  return '?';
	}
}

void
perft_test(
		ChessGame* game,
		ChessHistory* history,
		unsigned int curr_depth,
		unsigned int max_depth,
		size_t* results)
{
	int i;
	MovesList moves;

	generate_moves_list(game, &moves);

	results[curr_depth] += moves.size;

	if(curr_depth < max_depth - 1) {
		for(i = 0; i < moves.size; ++i) {
			do_move_and_record(game, &moves, i, history);
			update(game);

			perft_test(
					game,
					history,
					curr_depth + 1,
					max_depth,
					results);

			undo_moves(game, history, 1);
		}
	}
}

int
main()
{
	ChessGame chess_game;
	ChessHistory history;
	unsigned int max_depth, i;
	size_t* count_moves;

	max_depth = 7;

	count_moves = calloc(max_depth, sizeof(size_t));
	if(!count_moves) {
		return EXIT_FAILURE;
	}

	if(!init_history(&history, max_depth)) {
		free(count_moves);
		return EXIT_FAILURE;
	}

	init_game(&chess_game);

	perft_test(&chess_game, &history, 0, max_depth, count_moves);

	for(i = 0; i < max_depth; ++i) {
		printf("Depth %2d: %10zu moves\n", i + 1, count_moves[i]);
	}

	free_history(&history);
	free(count_moves);

	return EXIT_SUCCESS;
}

