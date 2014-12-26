#include "Board.h"

BOOLEAN isMovePossible( Board* this, Piece* piece, int x, int y );
void redrawField( Board* this, int x, int y, UINT8 color );
void clearFieldsOccupiedByPiece( Board* this );
void drawPiece( Board* this );
void drawNextUp( Board* this );
void placePiece( Board* this );
BOOLEAN isGameOver( Board* this );
void checkAndRemoveLines( Board* this );
void loadNextPiece( Board* this );
void drawStaticElements( void );



void drawBoard( Board* this ) {
	drawNextUp( this );

	setTextColor( EFI_LIGHTGRAY );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X + 8, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 4 );
	Print( L"%d", this->level );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X + 8, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 5 );
	Print( L"%d", this->lines );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X + 8, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 6 );
	Print( L"%d", this->score );
}



void rotatePiece( Board* this ) {
	Piece rotated = *this->activePiece;

	rotated.rotate( &rotated );

	clearFieldsOccupiedByPiece( this );
	if ( isMovePossible( this, &rotated, 0, 0 ) ) { // nazwa mylaca - zmienic
		*this->activePiece = rotated;
	}
	drawPiece( this );
}



void movePieceLeft( Board* this ) {
	clearFieldsOccupiedByPiece( this );
	if ( isMovePossible( this, this->activePiece, -1, 0 ) ) {
		this->activePiece->pos.x--;
	}
	drawPiece( this );
}



void movePieceRight( Board* this ) {
	clearFieldsOccupiedByPiece( this );
	if ( isMovePossible( this, this->activePiece, 1, 0 ) ) {
		this->activePiece->pos.x++;
	}
	drawPiece( this );
}



BOOLEAN movePieceDown( Board* this ) {
	clearFieldsOccupiedByPiece( this );
	if ( isMovePossible( this, this->activePiece, 0, 1 ) ) {
		this->activePiece->pos.y++;
		drawPiece( this );
		return TRUE;
	}
	else {
		drawPiece( this );
		placePiece( this );
		return FALSE;
	}
}



void dropPiece( Board* this ) {
	int height = 0;
	while ( movePieceDown( this ) ) {
		height++;
	};

	this->score += DROP_BONUS_SCORE_PER_LINE * height;
}



void ConstructBoard( Board** this ) {
	Board* board = AllocatePool( sizeof( Board ) );
	ZeroMem( board, sizeof( Board ) );

	board->blocked = FALSE;
	board->level = 1;
	board->lines = 0;
	board->score = 0;
	board->drawBoard = drawBoard;
	board->rotatePiece = rotatePiece;
	board->movePieceDown = movePieceDown;
	board->movePieceLeft = movePieceLeft;
	board->movePieceRight = movePieceRight;
	board->dropPiece = dropPiece;
	ConstructPiece( &board->nextPiece );
	ConstructPiece( &board->activePiece );

	loadNextPiece( board );

	drawStaticElements();

	*this = board;
}



void DestructBoard( Board* this ) {
	DestructPiece( this->nextPiece );
	DestructPiece( this->activePiece );
	FreePool( this );
}



BOOLEAN isMovePossible( Board* this, Piece* piece, int x, int y ) {
	int i, fx, fy;

	for ( i = 0; i < 4; ++i ) {
		fx = piece->body[ i ].x + piece->pos.x + x;
		fy = piece->body[ i ].y + piece->pos.y + y;

		if ( fx < 0 || fx >= BOARD_WIDTH ||
			fy < 0 || fy >= BOARD_HEIGHT ||
			this->fields[ fy ][ fx ] != EMPTY_FIELD ) {
			return FALSE;
		}
	}

	return TRUE;
}



void redrawField( Board* this, int x, int y, UINT8 color ) {
	ASSERT( x >= 0 && x < BOARD_WIDTH );
	ASSERT( y >= 0 && y < BOARD_HEIGHT );

	if ( y > 0 ) {
		this->fields[ y ][ x ] = color;
		setTextColor( color );
		putchar( BOARD_TOP_X + 1 + 2 * x, BOARD_TOP_Y + y, BLOCKELEMENT_FULL_BLOCK );
		putchar( BOARD_TOP_X + 1 + 2 * x + 1, BOARD_TOP_Y + y, BLOCKELEMENT_FULL_BLOCK );
	}
}



void clearFieldsOccupiedByPiece( Board* this ) {
	int i, x, y;

	for ( i = 0; i < 4; ++i ) {
		x = this->activePiece->body[ i ].x + this->activePiece->pos.x;
		y = this->activePiece->body[ i ].y + this->activePiece->pos.y;
		redrawField( this, x, y, EFI_BLACK );
	}
}


// copypaste z gory much XD moze by tak to zmienic...
void drawPiece( Board* this ) {
	int i, x, y;

	for ( i = 0; i < 4; ++i ) {
		x = this->activePiece->body[ i ].x + this->activePiece->pos.x;
		y = this->activePiece->body[ i ].y + this->activePiece->pos.y;
		redrawField( this, x, y, this->activePiece->color );
	}
}



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

		setTextColor( this->nextPiece->color );

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



void placePiece( Board* this ) {
	if ( isGameOver( this ) ) {
		this->blocked = TRUE;
	}
	else {
		checkAndRemoveLines( this );
		loadNextPiece( this );
	}
}



BOOLEAN isGameOver( Board* this ) {
	int i;
	for ( i = 0; i < 4; ++i ) {
		if ( this->activePiece->body[ i ].y + this->activePiece->pos.y == 0 ) {
			return TRUE;
		}
	}
	return FALSE;
}



void checkAndRemoveLines( Board* this ) {
	int i, j, k;
	int counter = 0;
	BOOLEAN lineWasFull;

	// jak bedzie wolno dzialac mozna zrobic optymalizacje - przerywac w momencie wykrycia pustej linii
	for ( i = BOARD_HEIGHT - 1; i > 0; --i ) {
		lineWasFull = TRUE;

		for ( j = 0; j < BOARD_WIDTH; ++j ) {
			if ( this->fields[ i ][ j ] == EMPTY_FIELD ) {
				lineWasFull = FALSE;
				break;
			}
		}

		if ( lineWasFull ) {
			for ( j = 0; j < BOARD_WIDTH; ++j ) {
				redrawField( this, j, i, EFI_BLACK );
			}

			for ( j = i; j > 0; --j ) { // mozna zrobic optymalizacje - wypisywanie od razu calego wiersza
				for ( k = 0; k < BOARD_WIDTH; ++k ) {
					redrawField( this, k, j, this->fields[ j - 1 ][ k ] );
				}
			}

			i++;
			counter++;
		}
	}

	this->lines += counter;

	if ( counter != 0 ) {
		this->score += LINE_CLEAR_SCORE << ( counter - 1 ); // trolololo xD
		this->score += LINE_CLEAR_LEVEL_BONUS_SCORE * this->level; //zmienic wartosc?
	}
}



void loadNextPiece( Board* this ) {
	Point startingPos = PIECE_STARTING_POS;

	Piece* tmp = this->activePiece;
	this->activePiece = this->nextPiece;
	this->nextPiece = tmp;

	this->nextPiece->randomize( this->nextPiece );
	this->activePiece->pos = startingPos;
}



void drawStaticElements( void ) {
	int i;

	setTextColor( EFI_LIGHTGRAY );
	setCursorPos( 0, 8 );
	Print( STATIC_TEXT );

	setCursorPos( NEXT_PIECE_FRAME_TOP_X + NEXT_PIECE_FRAME_WIDTH / 2 - 3, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 2 );
	Print( L"Next Up" );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 4 );
	Print( L"Level :" );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 5 );
	Print( L"Lines : " );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 6 );
	Print( L"Score : " );

	setTextColor( EFI_RED );

	// next piece frame
	for ( i = 1; i <= NEXT_PIECE_FRAME_WIDTH; i++ ) {
		putchar( NEXT_PIECE_FRAME_TOP_X + i, NEXT_PIECE_FRAME_TOP_Y, BOXDRAW_DOUBLE_HORIZONTAL );
		putchar( NEXT_PIECE_FRAME_TOP_X + i, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 1, BOXDRAW_DOUBLE_HORIZONTAL );
	}

	for ( i = 1; i <= NEXT_PIECE_FRAME_HEIGHT; i++ ) {
		putchar( NEXT_PIECE_FRAME_TOP_X, NEXT_PIECE_FRAME_TOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
		putchar( NEXT_PIECE_FRAME_TOP_X + NEXT_PIECE_FRAME_WIDTH + 1, NEXT_PIECE_FRAME_TOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
	}

	putchar( NEXT_PIECE_FRAME_TOP_X, NEXT_PIECE_FRAME_TOP_Y, BOXDRAW_DOUBLE_DOWN_RIGHT );
	putchar( NEXT_PIECE_FRAME_TOP_X + NEXT_PIECE_FRAME_WIDTH + 1, NEXT_PIECE_FRAME_TOP_Y, BOXDRAW_DOUBLE_DOWN_LEFT );
	putchar( NEXT_PIECE_FRAME_TOP_X + NEXT_PIECE_FRAME_WIDTH + 1, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 1, BOXDRAW_DOUBLE_UP_LEFT );
	putchar( NEXT_PIECE_FRAME_TOP_X, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 1, BOXDRAW_DOUBLE_UP_RIGHT );



	// board frame
	for ( i = 0; i < 2 * BOARD_WIDTH + 2; i++ ) {
		putchar( BOARD_TOP_X + i, BOARD_TOP_Y + BOARD_HEIGHT, BOXDRAW_DOUBLE_HORIZONTAL );
	}

	for ( i = 1; i < BOARD_HEIGHT; i++ ) {
		putchar( BOARD_TOP_X, BOARD_TOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
		putchar( BOARD_TOP_X + 2 * BOARD_WIDTH + 1, BOARD_TOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
	}

	putchar( BOARD_TOP_X, BOARD_TOP_Y + BOARD_HEIGHT, BOXDRAW_DOUBLE_UP_RIGHT );
	putchar( BOARD_TOP_X + 2 * BOARD_WIDTH + 1, BOARD_TOP_Y + BOARD_HEIGHT, BOXDRAW_DOUBLE_UP_LEFT );
}