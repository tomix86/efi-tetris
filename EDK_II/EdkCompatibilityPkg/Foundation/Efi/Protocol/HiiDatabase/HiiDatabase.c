/*++

Copyright (c) 2007, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

    HiiDatabase.c
    
Abstract:

    EFI_HII_DATABASE_PROTOCOL

Revision History

--*/

#include "EfiSpec.h"
#include EFI_PROTOCOL_DEFINITION (HiiDatabase)

EFI_GUID  gEfiHiiDatabaseProtocolGuid = EFI_HII_DATABASE_PROTOCOL_GUID;

EFI_GUID_STRING(&gEfiHiiDatabaseProtocolGuid, "EFI HII DATABASE Protocol", "UEFI 2.1 HII DATABASE Protocol");
