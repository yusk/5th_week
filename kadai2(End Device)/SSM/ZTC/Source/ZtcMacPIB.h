/******************************************************************************
* ZtcMacPIB.h
* This module contains a fresh implementation of the ZigBee Test Client (ZTC).
*
* Copyright (c) 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from  Freescale Semiconductor.
*
******************************************************************************/

#ifndef _ZtcMacPIB_h
#define _ZtcMacPIB_h

#include "NwkMacInterface.h"
#include "FunctionalityDefines.h"

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
*******************************************************************************/
#if gSecurityCapability_d
#ifndef gDefaultSecurityMaterialMaxLength_c
#define gDefaultSecurityMaterialMaxLength_c sizeof(securityMaterial_t)
#endif  /* gDefaultSecurityMaterialMaxLength_c */
#endif /* gSecurityCapability_d */

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/

typedef struct macPib_tag {
  uint8_t mPIBimacAckWaitDuration;                // 54 or 120
  bool_t  mPIBimacAssociationPermit;              // T/F          Optional
  bool_t  mPIBimacAutoRequest;                    // T/F
  bool_t  mPIBimacBattLifeExt;                    // T/F
  uint8_t mPIBimacBattLifeExtPeriods;             // 6 or 8
  uint8_t mPIBnmacBeaconPayload[52];              // 0-52 bytes   Optional
  uint8_t mPIBimacBeaconPayloadLength;            // 0-52         Optional
  uint8_t mPIBimacBeaconOrder;                    // 0-15         Optional
  uint8_t mPIBlmacBeaconTxTime[3];                // 0-0xffffff   Optional
  uint8_t mPIBimacBsn;                            // 0-255        Optional
  uint8_t mPIBnmacCoordExtendedAddress[8];        // 64bit IEEE adr 
  uint8_t mPIBsmacCoordShortAddress[2];           // 0-0xffff
  uint8_t mPIBimacDsn;                            // 0-255
  bool_t  mPIBimacGtsPermit;                      // T/F          Optional
  uint8_t mPIBimacMaxCsmaBackoffs;                // 0-5
  uint8_t mPIBimacMinBe;                          // 0-3
  uint8_t mPIBsmacPanId[2];                       // 0-0xffff
  bool_t  mPIBimacPromiscuousMode;                // T/F          Optional
  uint8_t mPIBimacRxOnWhenIdle;                   // 0-3
  uint8_t mPIBsmacShortAddress[2];                // 0-0xffff
  uint8_t mPIBimacSuperFrameOrder;                // 0-15         Optional
  uint8_t mPIBsmacTransactionPersistenceTime[2];  // 0-0xffff     Optional
#ifdef gMAC2006_d
  bool_t  mPIBimacAssociatedPANCoord;
  uint8_t mPIBimacMaxBE;
  uint8_t mPIBimacMaxFrameRetries;
  uint8_t mPIBimacResponseWaitTime;
  uint8_t mPIBsmacSyncSymbolOffset[2];
  bool_t  mPIBimacTimestampSupported;
  bool_t  mPIBimacSecurityEnabled; 
  uint8_t mPIBimacMinLIFSPeriod;
  uint8_t mPIBimacMinSIFSPeriod; 
#endif //gMAC2006_d  

    // Security specific attributes from this point of.
#if gSecurityCapability_d
 #ifndef gMAC2006_d
  aclEntryDescriptor_t *gpPIBaclEntryDescriptorSet; // Size: (0-255)*40, access with MPIB_AclEntryDescIdxToPtr(), MPIB_SetAclEntry(), or MPIB_PutAclEntry()
  uint8_t mPIBimacAclEntryDescriptorSetSize;      // 0-255
  bool_t  mPIBimacDefaultSecurity;                // T/F
  uint8_t mPIBimacDefaultSecurityMaterialLength;  // 0x00-0x1A
  uint8_t mPIBnmacDefaultSecurityMaterial[gDefaultSecurityMaterialMaxLength_c]; //0-0x1A bytes 
  uint8_t mPIBimacDefaultSecuritySuite;           // 0-7
  uint8_t mPIBimacSecurityMode;                   // 0-2
  uint8_t mPIBimacAclEntryCurrent;                // 0-mPIBimacAclEntryDescriptorSetSize. Vendor specific. Used for selecting current entry in ACL descriptor table.
 #else  
  KeyDescriptor_t            *gpPIBmacKeyTable;
  uint8_t                    mPIBimacKeyTableEntries;
  DeviceDescriptor_t         *gpPIBmacDeviceTable;
  uint8_t                    mPIBimacDeviceTableEntries;
  SecurityLevelDescriptor_t  *gpPIBmacSecurityLevelTable;
  uint8_t                    mPIBimacSecurityLevelTableEntries;
  uint8_t                    mPIBnmacFrameCounter[4];
  uint8_t                    mPIBimacAutoRequestSecurityLevel;
  uint8_t                    mPIBimacAutoRequestKeyIdMode;
  uint8_t                    mPIBnmacAutoRequestKeySource[8];
  uint8_t                    mPIBimacAutoRequestKeyIndex;
  uint8_t                    mPIBnmacDefaultKeySource[8];  
  uint8_t                    mPIBnmacPANCoordExtendedAddress[8];
  uint8_t                    mPIBsmacPANCoordShortAddress[2];
  //Freescale specific 2006 security PIB
  uint8_t                    mPIBimacKeyTableCrtEntry;
  uint8_t                    mPIBimacDeviceTableCrtEntry;
  uint8_t                    mPIBimacSecurityLevelTableCrtEntry;
  uint8_t                    mPIBimacKeyIdlLookuplistCrtEntry;
  uint8_t                    mPIBimacKeyUsageListCrtEntry;
  uint8_t                    mPIBimacKeyDeviceListCrtEntry;  
 #endif //gMAC2006_d
#endif // gSecurityCapability_d

    // Vendor specific attributes from this point of.
  uint8_t mPIBimacRole;                           // 0-2          Optional
  uint8_t mPIBimacLogicalChannel;                 // 11-26        
  uint8_t mPIBlmacTreemodeStartTime[3];           // 0-0xffffff   Optional
  uint8_t mPIBimacPanIdConflictDetection;         // T/F
  uint8_t mPIBimacBeaconResponseDenied;           // T/F
  uint8_t mPIBimacNBSuperFrameInterval;           // 0-15
  uint8_t mPIBimacBeaconPayloadLengthVendor;      // 0-52         Optional
  uint8_t mPIBimacBeaconResponseLQIThreshold;     // 0-255   set to 0 to disable this functionality   
} macPib_t;

#endif                                  /* #ifndef _ZtcMacPIB_h */
