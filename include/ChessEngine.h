#ifndef CE__CHESS_ENGINE__H
#define CE__CHESS_ENGINE__H

enum Piece {
	Empty    = 0x0000,
	Bishop   = 0x0001,
	King     = 0x0002,
	Knight   = 0x0004,
	Pawn     = 0x0008,
	Queen    = 0x0010,
	Rook     = 0x0020,
};

enum Color {
	Black    = 0x0040,
	White    = 0x0080,
};

enum Special {
	Special = 0x0000, /* Denotes castling or en passant */
	Normal  = 0x0100,
};

enum CheckStatus {
	NotInCheck  = 0x00,
	SingleCheck = 0x01, /* King is only in check due to one piece */
	DoubleCheck = 0x02, /* King is in check due to several pieces */
};

typedef struct ChessPiece {
	unsigned short int code;
	char pos;
} ChessPiece;

typedef struct ChessPlayer {
	ChessPiece* king;
	ChessPiece pieces[16];
} ChessPlayer;

typedef struct ChessGame {
	unsigned int turn_counter;
	unsigned char hm_clock;

	char en_passant;
	enum CheckStatus check_status;
	enum Color active_player;

	ChessPlayer players[2];
	ChessPiece* board[64];
} ChessGame;

typedef struct MoveOp {
	unsigned char old_pos;
	unsigned char new_pos;
	unsigned char _flag; /* For internal use only */
	enum Piece new_type;
} MoveOp;

typedef struct MovesList {
	MoveOp moves[139];
	unsigned char size;
} MovesList;

void do_move(ChessGame* game, MoveOp* move_obj);
unsigned char generate_moves_list(
		ChessGame* game,
		MovesList* moves_list);
void init_game(ChessGame* game);
void load_game(ChessGame* game, char* fen_string);
void update_antemove(ChessGame* game, MoveOp* move_obj);
void update_postmove(ChessGame* game);

#endif
