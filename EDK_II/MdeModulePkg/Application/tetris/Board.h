#pragma once

#include "Piece.h"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH  20
#define BOARD_TOP_X  25
#define BOARD_TOP_Y   3

#define NEXT_PIECE_FRAME_HEIGHT 2
#define NEXT_PIECE_FRAME_WIDTH 8
#define NEXT_PIECE_FRAME_TOP_X BOARD_TOP_X + BOARD_WIDTH + 2
#define NEXT_PIECE_FRAME_TOP_Y BOARD_TOP_Y + 1

typedef struct _Board Board;

typedef void( *DRAW_BOARD )( IN Board* this );
typedef void( *ROTATE_ACTIVE_PIECE )( IN Board* this );
typedef void( *MOVE_PIECE ) ( IN Board* this );
typedef void( *DROP_PIECE ) ( IN Board* this );

struct _Board {
	Piece* nextPiece;
	Piece* activePiece;
	UINT8 field[ BOARD_HEIGHT ][ BOARD_WIDTH / 2 ];

	DRAW_BOARD drawBoard;
	ROTATE_ACTIVE_PIECE rotatePiece;
	MOVE_PIECE movePieceLeft;
	MOVE_PIECE movePieceRight;
	MOVE_PIECE movePieceDown;
	DROP_PIECE dropPiece;
};

void ConstructBoard( OUT Board** this );
void DestructBoard( IN Board* this );