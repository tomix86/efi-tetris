#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Uefi.h>
#include "Board.h"

void drawboard( Board* this ) {
	
}

void ConstructBoard( Board** this ) {
	Board* board = AllocatePool( sizeof( Board ) );
	ZeroMem( board, sizeof( Board ) );

	board->drawBoard = drawboard;

	*this = board;
}

void DestructBoard( Board* this ) {
	FreePool( this );
}