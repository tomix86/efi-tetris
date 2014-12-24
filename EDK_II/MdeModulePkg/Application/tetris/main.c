/*
Tetris w EFI

Projekt na przedmiot Oprogramowanie Systemowe

Autorzy:
Krzysztof £ukasz Necel, 143301
Tomasz Gajger, 143218
*/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DebugLib.h>
#include "Core.h"

void prepareConsole( IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* cout, OUT EFI_SIMPLE_TEXT_OUTPUT_MODE* modeToStore ) {
	EFI_STATUS status;
	CopyMem( modeToStore, cout->Mode, sizeof( EFI_SIMPLE_TEXT_OUTPUT_MODE ) );

	status = cout->EnableCursor( cout, FALSE ) ;
	if ( status != EFI_UNSUPPORTED ) { // workaround
		ASSERT_EFI_ERROR( status );
	}

	ASSERT_EFI_ERROR( cout->ClearScreen( cout ) );
	ASSERT_EFI_ERROR( cout->SetAttribute( cout, EFI_TEXT_ATTR( EFI_LIGHTGRAY, EFI_BLACK ) ) );
	ASSERT_EFI_ERROR( cout->SetCursorPosition( cout, 0, 0 ) );
}

void restoreInitialConsoleMode( IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* cout, IN EFI_SIMPLE_TEXT_OUTPUT_MODE* storedMode ) {
	ASSERT_EFI_ERROR( cout->EnableCursor( cout, storedMode->CursorVisible ));
	ASSERT_EFI_ERROR( cout->SetCursorPosition( cout, storedMode->CursorColumn, storedMode->CursorRow ));
	ASSERT_EFI_ERROR( cout->SetAttribute( cout, storedMode->Attribute ));
	ASSERT_EFI_ERROR( cout->ClearScreen( cout ));
}

EFI_STATUS EFIAPI UefiMain( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable ) {
	EFI_SIMPLE_TEXT_OUTPUT_MODE initialMode;
	Core* core = NULL;

	prepareConsole( SystemTable->ConOut, &initialMode );
	ConstructCore( &core );

	while ( core->gameState != GAME_STATE_EXIT ) {
		core->handleInput( core );

		core->drawWindow( core );
	}

	DestructCore( core );

	restoreInitialConsoleMode( SystemTable->ConOut, &initialMode );

	return EFI_SUCCESS;
}