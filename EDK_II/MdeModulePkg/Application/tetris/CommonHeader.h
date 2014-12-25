#pragma once

#include <Uefi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>

void putchar( IN int x, IN int y, IN CHAR16 ch );
BOOLEAN isAlphanumeric( IN CHAR16 ch );