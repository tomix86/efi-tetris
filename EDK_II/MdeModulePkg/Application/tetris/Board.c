#include "CommonHeader.h"
#include "Board.h"

void drawNextUp( Board* this ) {
	int i, j, margin;
	static UINT8 previousPieceColor = 0;

	if ( this->nextPiece->color != previousPieceColor ) {
		previousPieceColor = this->nextPiece->color;

		for ( i = 1; i <= NEXT_PIECE_FRAME_WIDTH; ++i ) {
			for ( j = 1; j <= NEXT_PIECE_FRAME_HEIGHT; ++j ) {
				putchar( NEXT_PIECE_FRAME_TOP_X + i, NEXT_PIECE_FRAME_TOP_Y + j, L' ' );
			}
		}

		gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( this->nextPiece->color, EFI_BLACK ) );

		if ( this->nextPiece->color == TETRIMINO_I_COLOR || this->nextPiece->color == TETRIMINO_O_COLOR ) { // identyfikacja klockow po kolorze nie jest chyba zbyt elegancka...
			margin = 1;
		}
		else {
			margin = 2;
		}

		for ( i = 0; i < 4; ++i ) {
			putchar( NEXT_PIECE_FRAME_TOP_X + margin + 2 * this->nextPiece->body[ i ].x, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT - this->nextPiece->body[ i ].y, BLOCKELEMENT_FULL_BLOCK );
			putchar( NEXT_PIECE_FRAME_TOP_X + margin + 2 * this->nextPiece->body[ i ].x + 1, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT - this->nextPiece->body[ i ].y, BLOCKELEMENT_FULL_BLOCK );
		}
	}
}



void drawBoard( Board* this ) {
	drawNextUp( this );
}



void rotatePiece( Board* this ) {
	this->activePiece->rotate( this->activePiece );
}



void movePieceLeft( Board* this ) {

}



void movePieceRight( Board* this ) {

}



void movePieceDown( Board* this ) {

}



void dropPiece( Board* this ) {

}



void ConstructBoard( Board** this ) {
	Board* board = AllocatePool( sizeof( Board ) );
	ZeroMem( board, sizeof( Board ) );

	board->drawBoard = drawBoard;
	board->rotatePiece = rotatePiece;
	board->movePieceDown = movePieceDown;
	board->movePieceLeft = movePieceLeft;
	board->movePieceRight = movePieceRight;
	board->dropPiece = dropPiece;
	ConstructPiece( &board->nextPiece );
	ConstructPiece( &board->activePiece );

	*this = board;
}



void DestructBoard( Board* this ) {
	DestructPiece( this->nextPiece );
	DestructPiece( this->activePiece );
	FreePool( this );
}