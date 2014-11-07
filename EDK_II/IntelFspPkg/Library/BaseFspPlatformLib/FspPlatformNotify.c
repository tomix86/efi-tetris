/** @file

  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Library/FspCommonLib.h>
#include <Guid/EventGroup.h>
#include <FspApi.h>
#include <Protocol/PciEnumerationComplete.h>

EFI_PEI_PPI_DESCRIPTOR      mPeiPostPciEnumerationPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPciEnumerationCompleteProtocolGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR      mPeiReadyToBootPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEventReadyToBootGuid,
  NULL
};


UINT32  mFspNotfifySequence[] = {
  EnumInitPhaseAfterPciEnumeration,
  EnumInitPhaseReadyToBoot
};

/**
  Install FSP notification.

  @param[in] NotificatonCode  FSP notification code

  @retval EFI_SUCCESS            Notify FSP successfully
  @retval EFI_INVALID_PARAMETER  NotificatonCode is invalid

**/
EFI_STATUS
EFIAPI
FspNotificationHandler (
  IN  UINT32     NotificatonCode
  )
{
  EFI_STATUS                Status;

  Status   = EFI_SUCCESS;

  switch (NotificatonCode) {
  case EnumInitPhaseAfterPciEnumeration:
    //
    // Do POST PCI initialization if needed
    //
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Post PCI Enumeration ...\n"));
    PeiServicesInstallPpi (&mPeiPostPciEnumerationPpi);
    break;

  case EnumInitPhaseReadyToBoot:
    //
    // Ready To Boot
    //
    DEBUG ((DEBUG_INFO| DEBUG_INIT, "FSP Ready To Boot ...\n"));
    PeiServicesInstallPpi (&mPeiReadyToBootPpi);
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  }

  return Status;
}

/**
  This function transfer control to the ContinuationFunc passed in by the
  bootloader.

**/
VOID
EFIAPI
FspInitDone (
  VOID
  )
{
  FSP_INIT_PARAMS           *FspInitParams;

  FspInitParams   = (FSP_INIT_PARAMS *)GetFspApiParameter ();

  //
  // Modify the parameters for ContinuationFunc
  //
  SetFspContinuationFuncParameter(EFI_SUCCESS, 0);
  SetFspContinuationFuncParameter((UINT32)GetHobList(), 1);

  //
  // Modify the return address to ContinuationFunc
  //
  SetFspApiReturnAddress((UINT32)FspInitParams->ContinuationFunc);

  //
  // Give control back to the boot loader framework caller after FspInit is done
  // It is done throught the continuation function
  //
  SetFspMeasurePoint (FSP_PERF_ID_API_FSPINIT_EXIT);
  Pei2LoaderSwitchStack();
}

/**
  This function handle NotifyPhase API call from the bootloader.
  It gives control back to the bootloader after it is handled. If the
  Notification code is a ReadyToBoot event, this function will return
  and FSP continues the remaining execution until it reaches the DxeIpl.

**/
VOID
FspWaitForNotify (
  VOID
  )
{
  EFI_STATUS                 Status;
  UINT32                     NotificatonValue;
  UINT32                     NotificatonCount;
  UINT8                      Count;

  NotificatonCount = 0;
  while (NotificatonCount < sizeof(mFspNotfifySequence) / sizeof(UINT32)) {

    Count = (UINT8)((NotificatonCount << 1) & 0x07);
    SetFspMeasurePoint (FSP_PERF_ID_API_NOTIFY_POSTPCI_ENTRY + Count);

    NotificatonValue = ((NOTIFY_PHASE_PARAMS *)(UINTN)GetFspApiParameter ())->Phase;
    DEBUG ((DEBUG_INFO, "FSP Got Notification. Notification Value : 0x%08X\n", NotificatonValue));

    if (mFspNotfifySequence[NotificatonCount] != NotificatonValue) {
      //
      // Notify code does not follow the predefined order
      //
      SetFspApiReturnStatus(EFI_UNSUPPORTED);
    } else {
      //
      // Process Notification and Give control back to the boot loader framework caller
      //
      Status = FspNotificationHandler (NotificatonValue);
      SetFspApiReturnStatus(Status);
      if (!EFI_ERROR(Status)) {
        NotificatonCount++;
        SetFspApiReturnStatus(EFI_SUCCESS);
        if (NotificatonValue == EnumInitPhaseReadyToBoot) {
          break;
        }
      }
    }
    SetFspMeasurePoint (FSP_PERF_ID_API_NOTIFY_POSTPCI_EXIT + Count);
    Pei2LoaderSwitchStack();
  }

  //
  // Control goes back to the PEI Core and it dispatches further PEIMs.
  // DXEIPL is the final one to transfer control back to the boot loader.
  //
}

