#pragma once

#include "Piece.h"

#define BOARD_HEIGHT 15
#define BOARD_WIDTH  20
#define BOARD_LTOP_X 25
#define BOARD_LTOP_Y  3

#define NEXT_PIECE_FRAME_HEIGHT 4
#define NEXT_PIECE_FRAME_WIDTH 10
#define NEXT_PIECE_FRAME_LTOP_X BOARD_LTOP_X + BOARD_WIDTH + 2
#define NEXT_PIECE_FRAME_LTOP_Y BOARD_LTOP_Y + 1

typedef struct _Board Board;

typedef void( *DRAW_BOARD )( IN Board* this );

struct _Board {
	DRAW_BOARD drawBoard;
};

void ConstructBoard( OUT Board** this );
void DestructBoard( IN Board* this );