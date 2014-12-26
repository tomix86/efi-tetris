#include <Library/UefiRuntimeServicesTableLib.h>
#include "Piece.h"

unsigned rand( void );



void randomize( Piece* this ) {
	Body tetrimino_I = TETRIMINO_I_BODY;
	Body tetrimino_J = TETRIMINO_J_BODY;
	Body tetrimino_L = TETRIMINO_L_BODY;
	Body tetrimino_O = TETRIMINO_O_BODY;
	Body tetrimino_S = TETRIMINO_S_BODY;
	Body tetrimino_T = TETRIMINO_T_BODY;
	Body tetrimino_Z = TETRIMINO_Z_BODY;
	Body* target = NULL;


	switch ( rand() % 7 ) {
		case 0:
			target = &tetrimino_I;
			this->color = TETRIMINO_I_COLOR;
			break;
		case 1:
			target = &tetrimino_J;
			this->color = TETRIMINO_J_COLOR;
			break;
		case 2:
			target = &tetrimino_L;
			this->color = TETRIMINO_L_COLOR;
			break;
		case 3:
			target = &tetrimino_O;
			this->color = TETRIMINO_O_COLOR;
			break;
		case 4:
			target = &tetrimino_S;
			this->color = TETRIMINO_S_COLOR;
			break;
		case 5:
			target = &tetrimino_T;
			this->color = TETRIMINO_T_COLOR;
			break;
		case 6:
			target = &tetrimino_Z;
			this->color = TETRIMINO_Z_COLOR;
			break;
		default:
			ASSERT( FALSE );
	}

	CopyMem( &this->body, target, sizeof( Body ) );
}



void rotate( Piece* this ) {
	int i, tmp;

	for ( i = 0; i < 4; i++ ) {
		tmp = this->body[ i ].x;
		this->body[ i ].x = ~( this->body[ i ].y ) & 3;
		this->body[ i ].y = tmp;
	}
}



void ConstructPiece( Piece** this ) {
	Piece* piece = AllocatePool( sizeof( Piece ) );
	ZeroMem( piece, sizeof( Piece ) );

	piece->randomize = randomize;
	piece->rotate = rotate;
	piece->pos.x = 0;
	piece->pos.y = 0;
	randomize( piece );

	*this = piece;
}



void DestructPiece( Piece* this ) {
	FreePool( this );
}



unsigned rand( void ) {
	int a = 69069;
	int c = 1;
	int M = 1 << 31;
	static int previousValue;
	static BOOLEAN initialized = FALSE;

	if ( initialized == FALSE ) { // init dziala tak, ze nie dziala...
		EFI_TIME time;
		gRT->GetTime( &time, NULL );
		previousValue = time.Second;
		initialized = TRUE;
	}

	return previousValue = ( a*previousValue + c ) % M;
}