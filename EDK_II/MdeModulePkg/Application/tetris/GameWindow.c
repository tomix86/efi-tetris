#include "GameWindow.h"

void drawStaticElements( void );



void drawGameWindow( GameWindow* this ) {
	this->board->drawBoard( this->board );

	setTextColor( EFI_LIGHTGRAY );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X + 8, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 4 );
	Print( L"%d", this->level );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X + 8, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 5 );
	Print( L"%d", this->lines );
	setCursorPos( NEXT_PIECE_FRAME_TOP_X + 8, NEXT_PIECE_FRAME_TOP_Y + NEXT_PIECE_FRAME_HEIGHT + 6 );
	Print( L"%d", this->score );

	setTextColor( EFI_RED );
}



void ConstructGameWindow( GameWindow** this ) {
	GameWindow* gameWindow = AllocatePool( sizeof( GameWindow ) );
	ZeroMem( gameWindow, sizeof( GameWindow ) );

	ConstructBoard( &gameWindow->board );

	gameWindow->drawGameWindow = drawGameWindow;
	gameWindow->level = 1;
	gameWindow->lines = 0;
	gameWindow->score = 0;

	drawStaticElements();

	*this = gameWindow;
}



void DestructGameWindow( GameWindow* this ) {
	DestructBoard( this->board );
	FreePool( this );
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