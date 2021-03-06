/** @file

  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "SecFsp.h"

UINT32  FspImageSizeOffset = FSP_INFO_HEADER_OFF + OFFSET_IN_FSP_INFO_HEADER(ImageSize);

/**

  Calculate the FSP IDT gate descriptor.

  @param[in] IdtEntryTemplate     IDT gate descriptor template.

  @return                     FSP specific IDT gate descriptor.

**/
UINT64
FspGetExceptionHandler(
  IN  UINT64  IdtEntryTemplate
  )
{
  UINT32                    Entry;
  UINT64                    ExceptionHandler;
  IA32_IDT_GATE_DESCRIPTOR *IdtGateDescriptor;
  FSP_INFO_HEADER          *FspInfoHeader;

  FspInfoHeader     = (FSP_INFO_HEADER *)(GetFspBaseAddress() + FSP_INFO_HEADER_OFF);
  ExceptionHandler  = IdtEntryTemplate;
  IdtGateDescriptor = (IA32_IDT_GATE_DESCRIPTOR *)&ExceptionHandler;
  Entry = (IdtGateDescriptor->Bits.OffsetHigh << 16) | IdtGateDescriptor->Bits.OffsetLow;
  Entry = FspInfoHeader->ImageBase + FspInfoHeader->ImageSize - (~Entry + 1);
  IdtGateDescriptor->Bits.OffsetHigh = (UINT16)(Entry >> 16);
  IdtGateDescriptor->Bits.OffsetLow  = (UINT16)Entry;

  return ExceptionHandler;
}

/**
  This function gets the FSP UPD region offset in flash.

  @return the offset of the UPD region.

**/
UINT32
EFIAPI
GetFspUpdRegionOffset (
  VOID
  )
{
  FSP_GLOBAL_DATA                   *FspData;
  UINT32                            *Offset;

  FspData       = GetFspGlobalDataPointer ();

  //
  // It is required to put PcdUpdRegionOffset at offset 0x000C
  // for all FSPs.
  // gPlatformFspPkgTokenSpaceGuid.PcdUpdRegionOffset       | 0x000C | 0x12345678
  //
  Offset        = (UINT32 *)(FspData->FspInfoHeader->ImageBase + \
                             FspData->FspInfoHeader->CfgRegionOffset + 0x0C);

  return  *Offset;
}

/**
  This interface fills platform specific data.

  @param[in,out]  FspData           Pointer to the FSP global data.

**/
VOID
EFIAPI
SecGetPlatformData (
  IN OUT  FSP_GLOBAL_DATA    *FspData
  )
{
  FSP_PLAT_DATA    *FspPlatformData;
  UINT32            TopOfCar;
  UINT32           *StackPtr;
  UINT32            DwordSize;

  FspPlatformData = &FspData->PlatformData;

  //
  // The entries of platform information, together with the number of them,
  // reside in the bottom of stack, left untouched by normal stack operation.
  //
  TopOfCar = PcdGet32 (PcdTemporaryRamBase) + PcdGet32 (PcdTemporaryRamSize);

  FspPlatformData->DataPtr   = NULL;
  FspPlatformData->CodeRegionSize      = 0;
  FspPlatformData->CodeRegionBase      = 0;
  FspPlatformData->MicorcodeRegionBase = 0;
  FspPlatformData->MicorcodeRegionSize = 0;

  //
  // Pointer to the size field
  //
  StackPtr  = (UINT32 *)(TopOfCar - sizeof(UINT32));

  while (*StackPtr != 0) {
    if (*(StackPtr - 1) == FSP_MCUD_SIGNATURE) {
      //
      // This following data was pushed onto stack after TempRamInit API
      //
      DwordSize = 4;
      StackPtr  = StackPtr - 1 - DwordSize;
      CopyMem (&(FspPlatformData->CodeRegionBase), StackPtr, (DwordSize << 2));
      StackPtr--;
    } else if (*(StackPtr - 1) == FSP_PER0_SIGNATURE) {
      //
      // This is the performance data for InitTempMemory API entry/exit
      //
      DwordSize = 4;
      StackPtr  = StackPtr - 1 - DwordSize;
      CopyMem (FspData->PerfData, StackPtr, (DwordSize << 2));
      ((UINT8 *)(&FspData->PerfData[0]))[7] = FSP_PERF_ID_API_TMPRAMINIT_ENTRY;
      ((UINT8 *)(&FspData->PerfData[1]))[7] = FSP_PERF_ID_API_TMPRAMINIT_EXIT;
      StackPtr--;
    } else {
      StackPtr -= (*StackPtr);
    }
  }
}

/**

  Initialize the FSP global data region.
  It needs to be done as soon as possible after the stack is setup.

  @param[in,out] PeiFspData             Pointer of the FSP global data.
  @param[in]     BootFirmwareVolume     Point to the address of BootFirmwareVolume in stack.

**/
VOID
FspGlobalDataInit (
  IN OUT  FSP_GLOBAL_DATA    *PeiFspData,
  IN      VOID              **BootFirmwareVolume
  )
{
  VOID              *UpdDataRgnPtr;
  FSP_INIT_PARAMS   *FspInitParams;
  CHAR8              ImageId[9];
  UINTN              Idx;

  //
  // Init PCIE_BAR with value and set global FSP data pointer.
  // PciExpress Base should have been programmed by platform already.
  //
  SetFspGlobalDataPointer    (PeiFspData);
  ZeroMem  ((VOID *)PeiFspData, sizeof(FSP_GLOBAL_DATA));

  PeiFspData->Signature          = FSP_GLOBAL_DATA_SIGNATURE;
  PeiFspData->CoreStack          = *(UINTN *)(BootFirmwareVolume + 2);
  PeiFspData->PerfIdx            = 2;

  SetFspMeasurePoint (FSP_PERF_ID_API_FSPINIT_ENTRY);

  //
  // Get FSP Header offset
  // It may have multiple FVs, so look into the last one for FSP header
  //
  PeiFspData->FspInfoHeader      = (FSP_INFO_HEADER *)(GetFspBaseAddress() + FSP_INFO_HEADER_OFF);
  SecGetPlatformData (PeiFspData);

  //
  // Initialize UPD pointer.
  //
  FspInitParams = (FSP_INIT_PARAMS *)GetFspApiParameter ();
  UpdDataRgnPtr = ((FSP_INIT_RT_COMMON_BUFFER *)FspInitParams->RtBufferPtr)->UpdDataRgnPtr;
  if (UpdDataRgnPtr == NULL) {
    UpdDataRgnPtr = (VOID *)(PeiFspData->FspInfoHeader->ImageBase + GetFspUpdRegionOffset());
  }
  SetFspUpdDataPointer (UpdDataRgnPtr);

  //
  // Initialize serial port
  // It might have been done in ProcessLibraryConstructorList(), however,
  // the FSP global data is not initialized at that time. So do it again
  // for safe.
  //
  SerialPortInitialize ();

  //
  // Ensure the golbal data pointer is valid
  //
  ASSERT (GetFspGlobalDataPointer () == PeiFspData);

  for (Idx = 0; Idx < 8; Idx++) {
    ImageId[Idx] = PeiFspData->FspInfoHeader->ImageId[Idx];
  }
  ImageId[Idx] = 0;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "\n============= PEIM FSP  (%a 0x%08X) =============\n", \
         ImageId, PeiFspData->FspInfoHeader->ImageRevision));

}

/**

  Adjust the FSP data pointers after the stack is migrated to memory.

  @param[in] OffsetGap             The offset gap between the old stack and the new stack.

**/
VOID
FspDataPointerFixUp (
  IN UINT32   OffsetGap
  )
{
  FSP_GLOBAL_DATA  *NewFspData;

  NewFspData = (FSP_GLOBAL_DATA *)((UINTN)GetFspGlobalDataPointer() + (UINTN)OffsetGap);
  SetFspGlobalDataPointer (NewFspData);
}

/**
  This function check the FSP API calling condition.

  @param[in]  ApiIdx           Internal index of the FSP API.

**/
EFI_STATUS
EFIAPI
FspApiCallingCheck (
  UINT32   ApiIdx
  )
{
  EFI_STATUS       Status;
  FSP_GLOBAL_DATA *FspData;

  Status = EFI_SUCCESS;
  FspData  = GetFspGlobalDataPointer ();
  if (ApiIdx == 1) {
    //
    // FspInit check
    //
    if ((UINT32)FspData != 0xFFFFFFFF) {
      Status = EFI_UNSUPPORTED;
    }
  } else if (ApiIdx == 2) {
    //
    // NotifyPhase check
    //
    if ((FspData == NULL) || ((UINT32)FspData == 0xFFFFFFFF)) {
      Status = EFI_UNSUPPORTED;
    } else {
      if (FspData->Signature != FSP_GLOBAL_DATA_SIGNATURE) {
        Status = EFI_UNSUPPORTED;
      }
    }
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}
