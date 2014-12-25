#include "CommonHeader.h"

void putchar( int x, int y, CHAR16 ch ) {
	CHAR16 chartab[ 2 ] = { 0 };
	chartab[ 0 ] = ch;

	gST->ConOut->SetCursorPosition( gST->ConOut, x, y );
	Print( chartab );
}



BOOLEAN isAlphanumeric( CHAR16 ch ) {
	return ch >= 'a' && ch <= 'z' ||
		ch >= 'A' && ch <= 'Z' ||
		ch >= '0' && ch <= '9';
}
