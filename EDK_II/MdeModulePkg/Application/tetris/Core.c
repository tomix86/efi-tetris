#include "Core.h"

void timerCallback( EFI_EVENT event, void* context );
void togglePause( Core* this );
void gameOver( Core* this );
void startGame( Core* this );

void handleInput( Core* this ) {
	EFI_INPUT_KEY key;
	EFI_STATUS status;

	status = gST->ConIn->ReadKeyStroke( gST->ConIn, &key );

	if ( status != EFI_NOT_READY ) {
		ASSERT_EFI_ERROR( status );

		if ( this->gameState == GAME_STATE_WAITING_FOR_START ) {
			startGame( this );
		}
		else if ( key.UnicodeChar == 'p' || key.UnicodeChar == 'P' ) {
			if ( this->gameState == GAME_STATE_RUNNING || this->gameState == GAME_STATE_PAUSED ) {
				togglePause( this );
			}
		}
		else if ( key.UnicodeChar == ' ' ) {
			if ( this->gameState == GAME_STATE_RUNNING ) {
				this->board->dropPiece( this->board );
			}
		}
		else {
			switch ( key.ScanCode ) {
				case SCAN_UP:
					if ( this->gameState == GAME_STATE_RUNNING ) {
						this->board->rotatePiece( this->board );
					}
					break;
				case SCAN_DOWN:
					if ( this->gameState == GAME_STATE_RUNNING ) {
						this->board->movePieceDown( this->board );
					}
					break;
				case SCAN_LEFT:
					if ( this->gameState == GAME_STATE_RUNNING ) {
						this->board->movePieceLeft( this->board );
					}
					break;
				case SCAN_RIGHT:
					if ( this->gameState == GAME_STATE_RUNNING ) {
						this->board->movePieceRight( this->board );
					}
					break;
				case SCAN_ESC:
					this->gameState = GAME_STATE_EXIT;
					break;
				case SCAN_PAGE_UP:
					if ( this->board->level < MAX_LEVEL ) {
						this->board->level++;
					}
					break;
				case SCAN_PAGE_DOWN:
					if ( this->board->level > 1 ) {
						this->board->level--;
					}
					break;
				default:
					break;
			}
		}
	}
}



void drawWindow( Core* this ) {
	if ( this->gameState == GAME_STATE_RUNNING && this->board->blocked ) {
		gameOver( this );
	}

	if ( this->tickCounter == 0 ) {
		this->board->movePieceDown( this->board );

		//   lvl   1    2    3    4    5    6
		//period 100%  90%  80%  70%  60%  50%
		this->tickCounter = ( GAME_REFRESH_PERIOD * ( 11 - this->board->level ) ) / 10;
	}

	this->board->drawBoard( this->board );
}



void ConstructCore( Core** this ) {
	Core* core = AllocatePool( sizeof( Core ) );
	ZeroMem( core, sizeof( Core ) );

	ConstructBoard( &core->board );

	core->gameState = GAME_STATE_WAITING_FOR_START;
	core->handleInput = handleInput;
	core->drawWindow = drawWindow;
	core->tickCounter = 0;

	// set up timer event
	ASSERT_EFI_ERROR( gBS->CreateEventEx( EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, timerCallback, core, NULL, &core->timerEvent ) );

	// start the timer 
	ASSERT_EFI_ERROR( gBS->SetTimer( core->timerEvent, TimerPeriodic, TIMER_PERIOD ) );

	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_WHITE, EFI_RED ) );
	setCursorPos( BOARD_TOP_X, 1 );
	Print( L" PRESS ANY KEY TO START " );

	*this = core;
}



void DestructCore( Core* this ) {
	ASSERT_EFI_ERROR( gBS->CloseEvent( this->timerEvent ) );
	DestructBoard( this->board );
	FreePool( this );
}



void timerCallback( EFI_EVENT event, void* context ) {
	Core* core = context;

	if ( core->gameState == GAME_STATE_RUNNING && core->tickCounter != 0 )
		core->tickCounter--;
}



void togglePause( Core* this ) {
	this->gameState = this->gameState == GAME_STATE_PAUSED ? GAME_STATE_RUNNING : GAME_STATE_PAUSED;


	setCursorPos( BOARD_TOP_X + BOARD_WIDTH - 6, 1 );
	if ( this->gameState == GAME_STATE_PAUSED ) {
		gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_WHITE, EFI_GREEN ) );
		Print( L" GAME PAUSED " );
	}
	else {
		gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_BLACK, EFI_BLACK ) );
		Print( L"             " );
	}
}



void gameOver( Core* this ) {
	this->gameState = GAME_STATE_OVER;
	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_WHITE, EFI_RED ) );
	setCursorPos( BOARD_TOP_X + BOARD_WIDTH - 6, 1 );
	Print( L" GAME OVER " );
}



void startGame( Core* this ) {
	gST->ConOut->SetAttribute( gST->ConOut, EFI_TEXT_ATTR( EFI_BLACK, EFI_BLACK ) );
	setCursorPos( BOARD_TOP_X, 1 );
	Print( L"                        " );

	this->gameState = GAME_STATE_RUNNING;
}