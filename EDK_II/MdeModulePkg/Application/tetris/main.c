/*
Tetris w EFI

Projekt na przedmiot Oprogramowanie Systemowe

Autorzy:
Krzysztof £ukasz Necel, 143301
Tomasz Gajger, 143218
*/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiApplicationEntryPoint.h>

// returns false if error occured
BOOLEAN readSingleKey( EFI_SIMPLE_TEXT_INPUT_PROTOCOL* cin, EFI_INPUT_KEY* key ) {
	EFI_STATUS status;


	__debugbreak();
	do {
		status = cin->ReadKeyStroke( cin, key );
	} while ( !( status == EFI_SUCCESS || status == EFI_NOT_READY ) );

	ASSERT_EFI_ERROR( status );
	return !EFI_ERROR( status );
}

EFI_STATUS EFIAPI UefiMain( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable ) {
	//	CHAR16 chartab[] = { 0, 0 };
	///	EFI_INPUT_KEY key;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* cin;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* cout;

	cin = SystemTable->ConIn;
	cout = SystemTable->ConOut;

	Print( L"Projekt OS 2014.\n:)" );
	/*	do {
	if ( readSingleKey( cin, &key ) ) {
	return EFI_PROTOCOL_ERROR;
	}

	chartab[ 0 ] = key.UnicodeChar;

	Print( chartab );

	} while ( key.ScanCode != SCAN_ESC );
	*/

	return EFI_SUCCESS;
}
