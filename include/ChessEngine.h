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
	ChessPiece pieces[16];
	unsigned char king_index;
} ChessPlayer;

typedef struct ChessGame {
	ChessPiece* board[64];

	ChessPlayer players[2];

	enum CheckStatus check_status;

	enum Color active_player;

	unsigned char hm_clock;
	unsigned short int turn_counter;
} ChessGame;

typedef struct MoveOp {
	unsigned char old_pos;
	unsigned char new_pos;
	unsigned char ref_id; /* Move index - for internal use only */
	enum Piece new_type;
} MoveOp;

typedef struct MovesList {
	MoveOp moves[139];
	unsigned char size;
} MovesList;

void do_move(
		ChessGame* game,
		MovesList* moves_list,
		unsigned char index);
unsigned char generate_moves_list(
		ChessGame* game,
		MovesList* moves_list);
void init_game(ChessGame* game);
void update(ChessGame* game);

#endif
