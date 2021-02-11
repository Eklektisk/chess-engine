#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include <ChessEngine.h>

#define QUIT_GAME    0
#define SELECT_CELL  1
#define OTHER_ACTION 2

uint8_t handleInput(
	WINDOW* win,
	int8_t* selected
) {
	static int8_t row = -1;
	uint16_t key = wgetch(win);

	switch(key) {
		case 'q':
		case 'Q':
			return QUIT_GAME;

		case 'H': // FALL THROUGH
		case KEY_LEFT:
			switch(*selected) {
				case -1: *selected = 0;
				default: if(*selected % 8 > 0) { *selected -= 1; } break;
			}

			break;

		case 'L': // FALL THROUGH
		case KEY_RIGHT:
			switch(*selected) {
				case -1: *selected = 0;
				default: if(*selected % 8 < 7) { *selected += 1; } break;
			}

			break;

		case 'K': // FALL THROUGH
		case KEY_UP:
			switch(*selected) {
				case -1: *selected = 0;
				default: if(*selected / 8 > 0) { *selected -= 8; } break;
			}

			break;

		case 'J': // FALL THROUGH
		case KEY_DOWN:
			switch(*selected) {
				case -1: *selected = 0;
				default: if(*selected / 8 < 7) { *selected += 8; } break;
			}

			break;

		case 'a': // FALL THROUGH
		case 'b': // FALL THROUGH
		case 'c': // FALL THROUGH
		case 'd': // FALL THROUGH
		case 'e': // FALL THROUGH
		case 'f': // FALL THROUGH
		case 'g': // FALL THROUGH
		case 'h':
			row = key - 97;
			return SELECT_CELL;

		case '1': // FALL THROUGH
		case '2': // FALL THROUGH
		case '3': // FALL THROUGH
		case '4': // FALL THROUGH
		case '5': // FALL THROUGH
		case '6': // FALL THROUGH
		case '7': // FALL THROUGH
		case '8':
			switch(row) {
				case -1: break;
				default:
					*selected = 8 * (key - 49) + row;
					row = -1;
					break;
			}

			return SELECT_CELL;
	}

	row = -1;
	return OTHER_ACTION;
}

void printBoard(struct Board* board, bool* isSpaceHighlighted, int8_t selected) {
	int8_t bgOffset = 0;

	clear();

	mvaddstr( 0,  1, "+--------------------------------------------+");
	mvaddch(  1,  1, '|');
	mvaddch(  1, 46, '|');
	mvaddch( 26,  1, '|');
	mvaddch( 26, 46, '|');
	mvaddstr(27,  1, "+--------------------------------------------+");

	mvaddch( 3,  3, '1');
	mvaddch( 6,  3, '2');
	mvaddch( 9,  3, '3');
	mvaddch(12,  3, '4');
	mvaddch(15,  3, '5');
	mvaddch(18,  3, '6');
	mvaddch(21,  3, '7');
	mvaddch(24,  3, '8');

	mvaddch(26,  6, 'A');
	mvaddch(26, 11, 'B');
	mvaddch(26, 16, 'C');
	mvaddch(26, 21, 'D');
	mvaddch(26, 26, 'E');
	mvaddch(26, 31, 'F');
	mvaddch(26, 36, 'G');
	mvaddch(26, 41, 'H');

	for(uint8_t y = 0; y < 8; ++y) {
		mvaddch(3 * y + 2,  1, '|');
		mvaddch(3 * y + 2, 46, '|');
		mvaddch(3 * y + 3,  1, '|');
		mvaddch(3 * y + 3, 46, '|');
		mvaddch(3 * y + 4,  1, '|');
		mvaddch(3 * y + 4, 46, '|');

		for(uint8_t x = 0; x < 8; ++x) {
			int8_t pos = 8 * y + x;
			int8_t fgOffset = bgOffset;
			bool check = false;
			char letter;
			int8_t color;

			if(board->white.pieces.pos[pos]) {
				fgOffset = CP_WHITE;
				switch(board->white.pieces.piece[pos]->type) {
					case CP_BISHOP: letter = 'B'; break;
					case CP_KING:   letter = 'K'; check = board->white.kingCheck != CP_NONE; break;
					case CP_KNIGHT: letter = 'N'; break;
					case CP_PAWN:   letter = 'P'; break;
					case CP_QUEEN:  letter = 'Q'; break;
					case CP_ROOK:   letter = 'R'; break;
				}

			} else if(board->black.pieces.pos[pos]) {
				fgOffset = CP_BLACK;
				switch(board->black.pieces.piece[pos]->type) {
					case CP_BISHOP: letter = 'B'; break;
					case CP_KING:   letter = 'K'; check = board->black.kingCheck != CP_NONE; break;
					case CP_KNIGHT: letter = 'N'; break;
					case CP_PAWN:   letter = 'P'; break;
					case CP_QUEEN:  letter = 'Q'; break;
					case CP_ROOK:   letter = 'R'; break;
				}

			} else {
				letter = ' ';
			}

			if(isSpaceHighlighted[pos]) {
				color =  5 + (fgOffset * 2) + bgOffset;
			} else if(check) {
				color = 11 + fgOffset;
			} else if(pos == selected) {
				color =  9 + fgOffset;
			} else {
				color =  1 + (fgOffset * 2) + bgOffset;
			}

			attron(A_BOLD | COLOR_PAIR(color));

			mvaddch(3 * y + 2, 5 * x + 4, ' ');
			mvaddch(3 * y + 2, 5 * x + 5, ' ');
			mvaddch(3 * y + 2, 5 * x + 6, ' ');
			mvaddch(3 * y + 2, 5 * x + 7, ' ');
			mvaddch(3 * y + 2, 5 * x + 8, ' ');
			mvaddch(3 * y + 3, 5 * x + 4, ' ');
			mvaddch(3 * y + 3, 5 * x + 5, ' ');
			mvaddch(3 * y + 3, 5 * x + 6, letter);
			mvaddch(3 * y + 3, 5 * x + 7, ' ');
			mvaddch(3 * y + 3, 5 * x + 8, ' ');
			mvaddch(3 * y + 4, 5 * x + 4, ' ');
			mvaddch(3 * y + 4, 5 * x + 5, ' ');
			mvaddch(3 * y + 4, 5 * x + 6, ' ');
			mvaddch(3 * y + 4, 5 * x + 7, ' ');
			mvaddch(3 * y + 4, 5 * x + 8, ' ');

			attroff(A_BOLD | COLOR_PAIR(color));
			attron(COLOR_PAIR(5 + bgOffset));

			if(isSpaceHighlighted[pos]) {
				mvaddch(3 * y + 4, 5 * x + 4, "ABCDEFGH"[x]);
				mvaddch(3 * y + 4, 5 * x + 5, "12345678"[y]);
			}

			attroff(COLOR_PAIR(5 + bgOffset));

			bgOffset = bgOffset ? 0 : 1;
		}

		bgOffset = bgOffset ? 0 : 1;
	}

	attron(A_BOLD);
	mvaddstr(0, 3, "BOARD");
	attroff(A_BOLD);
}

void printStats(struct Board* board, uint16_t turn) {
	mvaddstr(0, 48, "+--------------------------+");
	mvaddstr(1, 48, "|                          |");
	mvaddstr(2, 48, "|  Turn:                   |");
	mvaddstr(3, 48, "|  Active Player:          |");
	mvaddstr(4, 48, "|  Available Moves:        |");
	mvaddstr(5, 48, "|  In Check:               |");
	mvaddstr(6, 48, "|                          |");
	mvaddstr(7, 48, "+--------------------------+");

	char turnStr[6];
	char active[6];
	char movesStr[4];
	char inCheck[4];

	snprintf(turnStr,  6, "%d", turn);
	
	switch(board->active) {
		case CP_WHITE:
			snprintf(movesStr, 4, "%zu", board->white.moves.size);
			snprintf(active,   6, "White");
			snprintf(inCheck,   4, board->white.kingCheck == CP_NONE ? "No" : "Yes");
			break;

		case CP_BLACK:
			snprintf(movesStr, 4, "%zu", board->black.moves.size);
			snprintf(active,   6, "Black");
			snprintf(inCheck,   4, board->black.kingCheck == CP_NONE ? "No" : "Yes");
			break;
	}

	mvaddstr(2, 68, turnStr);
	mvaddstr(3, 68, active);
	mvaddstr(4, 68, movesStr);
	mvaddstr(5, 68, inCheck);

	attron(A_BOLD);
	mvaddstr(0, 50, "STATS");
	attroff(A_BOLD);
}

void printSelection(
	int8_t selected,
	struct Board* board
) {
	mvaddstr( 8, 48, "+--------------------------+");
	mvaddstr( 9, 48, "|                          |");
	mvaddstr(10, 48, "|  Selected Cell:          |");
	mvaddstr(11, 48, "|  Piece Type:             |");
	mvaddstr(12, 48, "|  Piece Color:            |");
	mvaddstr(13, 48, "|  Resulting Moves:        |");
	mvaddstr(14, 48, "|                          |");
	mvaddstr(15, 48, "+--------------------------+");
	
	char cellStr[5];
	char pieceType[7];
	char pieceCol[6];
	char movesStr[3];

	if(selected == -1) {
		snprintf(cellStr, 5, "None");

		pieceType[0] = '\0';
		pieceCol[0]  = '\0';
		movesStr[0]  = '\0';

	} else {
		snprintf(
			cellStr, 5,
			"%c%c",
			"ABCDEFGH"[selected % 8],
			"12345678"[selected / 8]
		);

		struct Player* active = board->active ? &(board->black) : &(board->white);
		struct Player* other  = board->active ? &(board->white) : &(board->black);

		if(active->pieces.pos[selected]) {
			struct ChessPiece* piece = active->pieces.piece[selected];

			switch(piece->type) {
				case CP_BISHOP: snprintf(pieceType, 7, "Bishop"); break;
				case CP_KING:   snprintf(pieceType, 7, "King");   break;
				case CP_KNIGHT: snprintf(pieceType, 7, "Knight"); break;
				case CP_PAWN:   snprintf(pieceType, 7, "Pawn");   break;
				case CP_QUEEN:  snprintf(pieceType, 7, "Queen");  break;
				case CP_ROOK:   snprintf(pieceType, 7, "Rook");   break;
			}

			switch(piece->color) {
				case CP_WHITE: snprintf(pieceCol, 6, "White"); break;
				case CP_BLACK: snprintf(pieceCol, 6, "Black"); break;
			}

			uint8_t countMoves;
			for(uint8_t i = 0; i < active->moves.size; ++i) {
				if(active->moves.mv[i].cp == piece) {
					++countMoves;
				}
			}

			snprintf(movesStr, 4, "%d", countMoves);

		} else if(other->pieces.pos[selected]) {
			struct ChessPiece* piece = other->pieces.piece[selected];

			switch(piece->type) {
				case CP_BISHOP: snprintf(pieceType, 7, "Bishop"); break;
				case CP_KING:   snprintf(pieceType, 7, "King");   break;
				case CP_KNIGHT: snprintf(pieceType, 7, "Knight"); break;
				case CP_PAWN:   snprintf(pieceType, 7, "Pawn");   break;
				case CP_QUEEN:  snprintf(pieceType, 7, "Queen");  break;
				case CP_ROOK:   snprintf(pieceType, 7, "Rook");   break;
			}

			switch(piece->color) {
				case CP_WHITE: snprintf(pieceCol, 6, "White"); break;
				case CP_BLACK: snprintf(pieceCol, 6, "Black"); break;
			}

			movesStr[0] = '0';
			movesStr[1] = '\0';

		} else {
			pieceType[0] = '\0';
			pieceCol[0]  = '\0';
			movesStr[0]  = '\0';
		}

	}

	mvaddstr(10, 68, cellStr);
	mvaddstr(11, 68, pieceType);
	mvaddstr(12, 68, pieceCol);
	mvaddstr(13, 68, movesStr);

	attron(A_BOLD);
	mvaddstr( 8, 50, "SELECTION");
	attroff(A_BOLD);
}

void printInstructions() {
	mvaddstr(16, 48, "+--------------------------+");
	mvaddstr(17, 48, "|                          |");
	mvaddstr(18, 48, "|  Move using:             |");
	mvaddstr(19, 48, "|  * Shift + H,J,K,L       |");
	mvaddstr(20, 48, "|  * Arrow keys            |");
	mvaddstr(21, 48, "|                          |");
	mvaddstr(22, 48, "|  Select a cell by        |");
	mvaddstr(23, 48, "|  typing its code         |");
	mvaddstr(24, 48, "|                          |");
	mvaddstr(25, 48, "|  Press q to quit         |");
	mvaddstr(26, 48, "|                          |");
	mvaddstr(27, 48, "+--------------------------+");

	attron(A_BOLD);
	mvaddstr(16, 50, "INSTRUCTIONS");
	attroff(A_BOLD);
}

uint8_t handlePawnEnd(uint8_t firstMove) {
	// This will only be called when a pawn tries to move to the final square
	// The player will be presented with four options to transform into a:
	// * Bishop
	// * Knight
	// * Queen
	// * Rook
	
	mvaddstr( 9, 14, "+-------------------+");
	mvaddstr(10, 14, "|                   |");
	mvaddstr(11, 14, "|  Transform into:  |");
	mvaddstr(12, 14, "|                   |");
	mvaddstr(13, 14, "|     1. Bishop     |");
	mvaddstr(14, 14, "|     2. Knight     |");
	mvaddstr(15, 14, "|     3. Queen      |");
	mvaddstr(16, 14, "|     4. Rook       |");
	mvaddstr(17, 14, "|                   |");
	mvaddstr(18, 14, "+-------------------+");

	refresh();

	while(true) {
		char key = getchar();

		switch(key) {
			case '1': // FALL THROUGH
			case '2': // FALL THROUGH
			case '3': // FALL THROUGH
			case '4':
				return firstMove + key - 49;
		}
	}
}

int main(int argc, char** argv) {
	struct Board game;
	initBoard(&game);

	uint16_t turn = 0;

	WINDOW* win = initscr();
	noecho();
	curs_set(false);
	keypad(win, true);

	if(has_colors() == false) {
		endwin();
		printf("Your terminal does not support color\n");
		return EXIT_FAILURE;
	}

	start_color();
	
	// Normal cells
	init_pair( 1, COLOR_WHITE, COLOR_RED);   // White piece, white cell
	init_pair( 2, COLOR_WHITE, COLOR_BLACK); // White piece, black cell
	init_pair( 3, COLOR_CYAN,  COLOR_RED);   // Black piece, white cell
	init_pair( 4, COLOR_CYAN,  COLOR_BLACK); // Black piece, black cell

	// Highlighted cells
	init_pair( 5, COLOR_WHITE, COLOR_MAGENTA); // White piece, white cell
	init_pair( 6, COLOR_WHITE, COLOR_BLUE);    // White piece, black cell
	init_pair( 7, COLOR_CYAN,  COLOR_MAGENTA); // Black piece, white cell
	init_pair( 8, COLOR_CYAN,  COLOR_BLUE);    // Black piece, black cell

	// Selected cells
	init_pair( 9, COLOR_WHITE, COLOR_GREEN); // White piece
	init_pair(10, COLOR_CYAN,  COLOR_GREEN); // Black piece

	// Check cells
	init_pair(11, COLOR_WHITE, COLOR_YELLOW); // White piece
	init_pair(12, COLOR_CYAN,  COLOR_YELLOW); // Black piece

	while(!checkStalemate(&game) && genMoves(&game) > 0) {
		bool isSpaceHighlighted[64] = {
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
		};

		bool handleTransform[64] = {
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,
		};

		uint8_t highlightedMoves[64] = {
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
		};

		static int8_t selected = -1;
		struct ChessPiece* highlightedPiece = NULL;

		int8_t moveNum = -1;

		struct Player* player = game.active ? &game.black : &game.white;

		while(moveNum == -1) {
			printBoard(&game, isSpaceHighlighted, selected);
			printStats(&game, turn);
			printSelection(selected, &game);
			printInstructions();

			refresh();

			switch(handleInput(win, &selected)) {
				case QUIT_GAME:
					goto QUIT_PROGRAM;

				case SELECT_CELL: // FALL THROUGH
					if(highlightedPiece) {
						if(isSpaceHighlighted[selected]) {
							switch(handleTransform[selected]) {
								case true:
									moveNum = handlePawnEnd(highlightedMoves[selected]);
									break;

								case false:
									moveNum = highlightedMoves[selected];
									break;
							}

							break;
						}
					}

				default:
					if(highlightedPiece) {
						for(uint8_t i = 0; i < 64; ++i) {
							if(isSpaceHighlighted[i]) { isSpaceHighlighted[i] = false; }
						}

						highlightedPiece = NULL;
					}

					if(player->pieces.pos[selected]) {
						highlightedPiece = player->pieces.piece[selected];

						for(uint8_t i = 0; i < player->moves.size; ++i) {
							if(player->moves.mv[i].cp == highlightedPiece) {
								isSpaceHighlighted[player->moves.mv[i].newPos] = true;
								highlightedMoves[player->moves.mv[i].newPos]   = i;

								if(player->moves.mv[i].transform != CP_NONE) {
									// The only transformation occurs when a pawn reaches the end
									// If one is found, the next three moves will be different
									// transformations based on the same move.
									// 
									// Since the transform is handled elsewhere, we can just 
									// skip the repeat moves for now.

									handleTransform[player->moves.mv[i].newPos] = true;
									i += 3;

								} else {
									handleTransform[player->moves.mv[i].newPos] = false;

								}
							}
						}
					}

					break;
			}

		}

		doMove(&game, moveNum);
		postMoveActions(&game, moveNum);
		switchTurn(&game);

		++turn;
	}

QUIT_PROGRAM:
	endwin();

	return EXIT_SUCCESS;
}
