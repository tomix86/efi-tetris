#pragma once

#include <Base.h>

typedef struct _Piece Piece;

struct _Piece {
	int tmp;
};

void ConstructPiece( OUT Piece** this );
void DestructPiece( IN Piece* this );