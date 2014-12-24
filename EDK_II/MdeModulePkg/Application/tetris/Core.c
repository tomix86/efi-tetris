#include <Uefi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include "Core.h"


// SetTimer do czasu?

/*
BOOLEAN isAlphanumeric( CHAR16 ch ) {
return ch >= 'a' && ch <= 'z' ||
ch >= 'A' && ch <= 'Z' ||
ch >= '0' && ch <= '9';
}
*/

void handleInput( Core* this ) {
	EFI_INPUT_KEY key;
	EFI_STATUS status;

	status = gST->ConIn->ReadKeyStroke( gST->ConIn, &key );

	if ( status != EFI_NOT_READY ) {
		ASSERT_EFI_ERROR( status );

		if ( key.UnicodeChar == 'p' || key.UnicodeChar == 'P' ) {
			this->gameWindow->paused ^= 1;
		}
		else if ( key.UnicodeChar == ' ' ) {

		}
		else {
			switch ( key.ScanCode ) {
				case SCAN_UP:

					break;
				case SCAN_DOWN:

					break;
				case SCAN_LEFT:

					break;
				case SCAN_RIGHT:

					break;
				case SCAN_ESC:
					this->gameState = GAME_STATE_EXIT;
					break;
				case SCAN_PAGE_UP:
					if ( this->gameWindow->level < 5 ) {
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

void timerCallback( EFI_EVENT event, void* context ) {
	Core* core = context;
	core->gameState;
	/*
	if ( Game->GameTimer )
	Game->GameTimer--;
	*/
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

	// Start the Periodic Timer 
	ASSERT_EFI_ERROR( gBS->SetTimer( core->timerEvent, TimerPeriodic, TIMER_PERIOD ) );

	*this = core;
}

void DestructCore( Core* this ) {
	ASSERT_EFI_ERROR( gBS->CloseEvent( this->timerEvent ) );
	DestructGameWindow( this->gameWindow );
	FreePool( this );
}