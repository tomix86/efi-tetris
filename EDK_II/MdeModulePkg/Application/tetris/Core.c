#include "Core.h"

void timerCallback( EFI_EVENT event, void* context );
void togglePause( Core* this ); // w ktorym miejscu powinna byc zmienna paused... oraz jak obsluzyc koniec gry?


void handleInput( Core* this ) {
	EFI_INPUT_KEY key;
	EFI_STATUS status;

	status = gST->ConIn->ReadKeyStroke( gST->ConIn, &key );

	if ( status != EFI_NOT_READY ) {
		ASSERT_EFI_ERROR( status );

		if ( key.UnicodeChar == 'p' || key.UnicodeChar == 'P' ) {
			togglePause( this );
		}
		else if ( key.UnicodeChar == ' ' ) {
			this->gameWindow->board->dropPiece( this->gameWindow->board );
		}
		else {
			switch ( key.ScanCode ) {
				case SCAN_UP:
					this->gameWindow->board->rotatePiece( this->gameWindow->board );
					break;
				case SCAN_DOWN:
					this->gameWindow->board->movePieceDown( this->gameWindow->board );
					break;
				case SCAN_LEFT:
					this->gameWindow->board->movePieceLeft( this->gameWindow->board );
					break;
				case SCAN_RIGHT:
					this->gameWindow->board->movePieceRight( this->gameWindow->board );
					break;
				case SCAN_ESC:
					this->gameState = GAME_STATE_EXIT;
					break;
				case SCAN_PAGE_UP:
					if ( this->gameWindow->level < MAX_LEVEL ) {
						this->gameWindow->level++;
					}
					break;
				case SCAN_PAGE_DOWN:
					if ( this->gameWindow->level > 1 ) {
						this->gameWindow->level--;
					}
					break;
				default:
					break;
			}
		}
	}
}



void drawWindow( Core* this ) {
	this->gameWindow->drawGameWindow( this->gameWindow );
}



void ConstructCore( Core** this ) {
	Core* core = AllocatePool( sizeof( Core ) );
	ZeroMem( core, sizeof( Core ) );

	ConstructGameWindow( &core->gameWindow );

	core->gameState = GAME_STATE_MENU;
	core->handleInput = handleInput;
	core->drawWindow = drawWindow;

	// set up timer event
	ASSERT_EFI_ERROR( gBS->CreateEventEx( EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, timerCallback, core, NULL, &core->timerEvent ) );

	// start the timer 
	ASSERT_EFI_ERROR( gBS->SetTimer( core->timerEvent, TimerPeriodic, TIMER_PERIOD ) );

	*this = core;
}



void DestructCore( Core* this ) {
	ASSERT_EFI_ERROR( gBS->CloseEvent( this->timerEvent ) );
	DestructGameWindow( this->gameWindow );
	FreePool( this );
}



void timerCallback( EFI_EVENT event, void* context ) {
	Core* core = context;
	core->gameState;

	/*
	if ( Game->GameTimer )
	Game->GameTimer--;
	*/
}



void togglePause(Core* this) {
	this->gameWindow->paused ^= 1;

	setTextColor( EFI_GREEN );
	setCursorPos( BOARD_TOP_X + BOARD_WIDTH - 5, 1 );
	if ( this->gameWindow->paused ) {
		Print( L"GAME PAUSED" );
	}
	else {
		Print( L"           " );
	}
}