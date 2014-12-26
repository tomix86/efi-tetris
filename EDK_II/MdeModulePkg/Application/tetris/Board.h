#pragma once

#include "Piece.h"

#define BOARD_HEIGHT 21
#define BOARD_WIDTH  10
#define BOARD_TOP_X  25
#define BOARD_TOP_Y   3

#define NEXT_PIECE_FRAME_HEIGHT 2
#define NEXT_PIECE_FRAME_WIDTH 8
#define NEXT_PIECE_FRAME_TOP_X BOARD_TOP_X + 2 * BOARD_WIDTH + 4
#define NEXT_PIECE_FRAME_TOP_Y BOARD_TOP_Y + 1

#define PIECE_STARTING_POS { 4, 0 };
#define EMPTY_FIELD EFI_BLACK

typedef struct _Board Board;

typedef void( *DRAW_BOARD )( IN Board* this );
typedef void( *ROTATE_ACTIVE_PIECE )( IN Board* this );
typedef void( *MOVE_PIECE_LEFT ) ( IN Board* this );
typedef void( *MOVE_PIECE_RIGHT ) ( IN Board* this );
typedef BOOLEAN( *MOVE_PIECE_DOWN ) ( IN Board* this );
typedef void( *DROP_PIECE ) ( IN Board* this );

struct _Board {
	Piece* nextPiece;
	Piece* activePiece;
	UINT8 fields[ BOARD_HEIGHT ][ BOARD_WIDTH ];

	DRAW_BOARD drawBoard;
	ROTATE_ACTIVE_PIECE rotatePiece;
	MOVE_PIECE_LEFT movePieceLeft;
	MOVE_PIECE_RIGHT movePieceRight;
	MOVE_PIECE_DOWN movePieceDown;
	DROP_PIECE dropPiece;
};

void ConstructBoard( OUT Board** this );
void DestructBoard( IN Board* this );