#ifndef CE__MOVE_INFO__H
#define CE__MOVE_INFO__H

enum MaskFlags {
	AllowEmpty = 0x0000, /* May match empty square to MoveSignature */
	NoEmpty    = 0x0200, /* Must match a piece to MoveSignature */
};

enum MoveFlags {
	NormalMove  = 0x00,
	RegKingMove = 0x01,
	CastleQueen = 0x02,
	CastleKing  = 0x03,
	EnPassant   = 0x04,
	PawnJump    = 0x05,
	Transform   = 0x06,
};

typedef struct MoveSignature {
	enum MoveFlags flag;
	char end_pos;

	unsigned char num_checks;
	unsigned char squares[7];
	unsigned short int inv_masks[7];
} MoveSignature;

typedef struct MoveOptions {
	unsigned char num_options;
	char options_map[64];

	MoveSignature options[27];
} MoveOptions;

extern const MoveOptions move_options[64][14];

#endif
