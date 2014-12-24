#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>
#include "GameWindow.h"

#define STATIC_TEXT_LEFT L"\
     CONTROLS\r\n\
      P - Pause game\r\n\
     <- - Move left\r\n\
     -> - Move right\r\n\
   Down - Move down\r\n\
     Up - Rotate\r\n\
  Space - Drop\r\n\
  Pg Up - level up\r\n\
Pg Down - level down\r\n\
    ESC - Exit game\r\n"

void putchar( int x, int y, CHAR16 ch );
void drawStaticElements( void );

void drawGameWindow( GameWindow* this ) {
	this->board->drawBoard( this->board );

	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_LIGHTGRAY, EFI_BLACK ) );
	gST->ConOut->SetCursorPosition( gST->ConOut, NEXT_PIECE_FRAME_LTOP_X + 8, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT + 3 );
	Print( L"%d", this->level );
	gST->ConOut->SetCursorPosition( gST->ConOut, NEXT_PIECE_FRAME_LTOP_X + 8, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT + 4 );
	Print( L"%d", this->lines );
	gST->ConOut->SetCursorPosition( gST->ConOut, NEXT_PIECE_FRAME_LTOP_X + 8, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT + 5 );
	Print( L"%d", this->score );


	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_GREEN, EFI_BLACK ) );
	gST->ConOut->SetCursorPosition( gST->ConOut, BOARD_LTOP_X + BOARD_WIDTH / 2 - 5, 1 );
	if ( this->paused ) {
		Print( L"GAME PAUSED" );
	}
	else {
		Print( L"           " );
	}

	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_RED, EFI_BLACK ) );
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

	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_LIGHTGRAY, EFI_BLACK ) );
	gST->ConOut->SetCursorPosition( gST->ConOut, 0, 8 );
	Print( STATIC_TEXT_LEFT );
	gST->ConOut->SetCursorPosition( gST->ConOut, NEXT_PIECE_FRAME_LTOP_X + NEXT_PIECE_FRAME_WIDTH / 2 - 3, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT + 1 );
	Print( L"Next Up" );
	gST->ConOut->SetCursorPosition( gST->ConOut, NEXT_PIECE_FRAME_LTOP_X, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT + 3 );
	Print( L"Level :" );
	gST->ConOut->SetCursorPosition( gST->ConOut, NEXT_PIECE_FRAME_LTOP_X, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT + 4 );
	Print( L"Lines : " );
	gST->ConOut->SetCursorPosition( gST->ConOut, NEXT_PIECE_FRAME_LTOP_X, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT + 5 );
	Print( L"Score : " );
	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_RED, EFI_BLACK ) );

	// next piece frame
	for ( i = 0; i < NEXT_PIECE_FRAME_WIDTH; i++ ) {
		putchar( NEXT_PIECE_FRAME_LTOP_X + i, NEXT_PIECE_FRAME_LTOP_Y, BOXDRAW_DOUBLE_HORIZONTAL );
		putchar( NEXT_PIECE_FRAME_LTOP_X + i, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT, BOXDRAW_DOUBLE_HORIZONTAL );
	}

	for ( i = 0; i < NEXT_PIECE_FRAME_HEIGHT; i++ ) {
		putchar( NEXT_PIECE_FRAME_LTOP_X, NEXT_PIECE_FRAME_LTOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
		putchar( NEXT_PIECE_FRAME_LTOP_X + NEXT_PIECE_FRAME_WIDTH, NEXT_PIECE_FRAME_LTOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
	}

	putchar( NEXT_PIECE_FRAME_LTOP_X, NEXT_PIECE_FRAME_LTOP_Y, BOXDRAW_DOUBLE_DOWN_RIGHT );
	putchar( NEXT_PIECE_FRAME_LTOP_X + NEXT_PIECE_FRAME_WIDTH, NEXT_PIECE_FRAME_LTOP_Y, BOXDRAW_DOUBLE_DOWN_LEFT );
	putchar( NEXT_PIECE_FRAME_LTOP_X + NEXT_PIECE_FRAME_WIDTH, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT, BOXDRAW_DOUBLE_UP_LEFT );
	putchar( NEXT_PIECE_FRAME_LTOP_X, NEXT_PIECE_FRAME_LTOP_Y + NEXT_PIECE_FRAME_HEIGHT, BOXDRAW_DOUBLE_UP_RIGHT );



	// board frame
	for ( i = 0; i < BOARD_WIDTH; i++ ) {
		putchar( BOARD_LTOP_X + i, BOARD_LTOP_Y + BOARD_HEIGHT, BOXDRAW_DOUBLE_HORIZONTAL );
	}

	for ( i = 0; i < BOARD_HEIGHT; i++ ) {
		putchar( BOARD_LTOP_X, BOARD_LTOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
		putchar( BOARD_LTOP_X + BOARD_WIDTH, BOARD_LTOP_Y + i, BOXDRAW_DOUBLE_VERTICAL );
	}

	putchar( BOARD_LTOP_X, BOARD_LTOP_Y + BOARD_HEIGHT, BOXDRAW_DOUBLE_UP_RIGHT );
	putchar( BOARD_LTOP_X + BOARD_WIDTH, BOARD_LTOP_Y + BOARD_HEIGHT, BOXDRAW_DOUBLE_UP_LEFT );
}



void putchar( int x, int y, CHAR16 ch ) {
	CHAR16 chartab[ 2 ] = { 0 };
	chartab[ 0 ] = ch;

	gST->ConOut->SetCursorPosition( gST->ConOut, x, y );
	Print( chartab );
}