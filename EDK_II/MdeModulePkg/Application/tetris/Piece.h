#pragma once

#include "CommonHeader.h"

// y
// 1 . . . .
// 0 . . . .
//   0 1 2 3 x
// { {x,y}, {x,y}, {x,y}, {x,y} }

// XXXX
#define TETRIMINO_I_BODY { {0,0}, {1,0}, {2,0}, {3,0} }
#define TETRIMINO_I_COLOR EFI_LIGHTCYAN

// XXX
//   X
#define TETRIMINO_J_BODY { {0,1}, {1,1}, {2,1}, {2,0} }
#define TETRIMINO_J_COLOR EFI_LIGHTBLUE

// XXX
// X
#define TETRIMINO_L_BODY { {0,0}, {0,1}, {1,1}, {2,1} }
#define TETRIMINO_L_COLOR EFI_GREEN 

// XX
// XX
#define TETRIMINO_O_BODY { {1,0}, {1,1}, {2,0}, {2,1} }
#define TETRIMINO_O_COLOR EFI_YELLOW

//  XX 
// XX
#define TETRIMINO_S_BODY { {0,0}, {1,0}, {1,1}, {2,1} }
#define TETRIMINO_S_COLOR EFI_LIGHTGREEN 

// XXX
//  X
#define TETRIMINO_T_BODY { {0,1}, {1,1}, {2,1}, {1,0} }
#define TETRIMINO_T_COLOR EFI_MAGENTA

// XX
//  XX
#define TETRIMINO_Z_BODY { {0,1}, {1,0}, {1,1}, {2,0} }       
#define TETRIMINO_Z_COLOR EFI_LIGHTMAGENTA

typedef	Point Body[ 4 ];

typedef struct _Piece Piece;

typedef void( *RANDOMIZE_PIECE )( IN Piece* this );
typedef void( *ROTATE_PIECE )( IN Piece* this );

struct _Piece {
	Body body;
	UINT8 color; // must be unique for each piece type
	Point pos;

	RANDOMIZE_PIECE randomize;
	ROTATE_PIECE rotate;
};

void ConstructPiece( OUT Piece** this );
void DestructPiece( IN Piece* this );