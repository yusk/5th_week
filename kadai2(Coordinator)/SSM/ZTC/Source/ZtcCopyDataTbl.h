/*****************************************************************************
* Tables of information about Ztc data formats.
*
* Copyright (c) 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from  Freescale Semiconductor.
*
******************************************************************************/

/* The purpose of these tables is to keep various declarations and defintions
 * in perfect sync, without requiring maintenence of different declarations.
 *
 * The code should never be aware of which data format it is working with.
 * There should never be logic of the form:
 *
 *      "if this is SAP Handler X (or message type X)
 *      then do Y"
 *
 * All of the knobs that can be tweaked to describe data formats should be
 * in this file. The code should instead contain logic of the form:
 *
 *      "if the data format info table entry for this data format says X,
 *      then do Y".
 *
 * Making a change to the way the code deals with different data formats
 * should require only changes in this file, unless a completely new kind
 * of handling is needed. In that case, this file should be changed to provide
 * a means of triggering the new behavior of the code.
 */

/* IMPORTANT NOTE: The order of the entries in these tables is not significant.
 * Entries can be added, removed or moved freely. The code must work if the
 * order of the entries in the tables is changed.
 */

/* All SAP Handlers receive a pointer to a message, which the SAP Handler
 * interprets as a pointer to a structure type. The first byte of the
 * message is always the message type. All of the structure types contain
 * a union of other structure types for access to the rest of the data in
 * the message.
 * The APP_ZDP_SapHandler(), unlike all other SAP Handlers, has an additional
 * data item between the message type and the union.
 * These macros compenstate for that extra field.
 */
#define AppZdpSize(type) \
  (sizeof(type) + MbrOfs(appToZdpMessage_t, msgData) - MbrSizeof(appToZdpMessage_t, msgType))

#define AppZdpOfs(type, member) \
  ((MbrOfs(appToZdpMessage_t, msgData) - MbrSizeof(appToZdpMessage_t, msgType)) + MbrOfs(type, member))

/****************************************************************************/

/* Before defining the main table, define the format parameter subtables.
 * Some of the format copy routines can handle a number of specific formats,
 * with parameters describing the details. Each format copy routine knows
 * about its parameter table; all that is needed here is to enter the actual
 * paremeters into the right tables, and provide a table index that the copy
 * routine can use to find the appropriate row.
 *
 * Not all copy functions need (or have) parameter tables.
 */

/* Parameter table for packets and messages that contain a header plus a
 * variable number of data items. The number of items is specified in a
 * field in the header.
 *
 * For packet-from-message and message-from-packet, copy the entire source
 * (header plus data) to the destination.
 *
 * The macro format is:
 *  Column 1    name: Name of index, for use in the Fmt() table.
 *  Column 2    hdrLen: Length of the header.
 *  Column 3    offsetToCount: Offset in the header to the number of
 *                items of data.
 *  Column 4    itemLen: length of each data item.
 *
 * Used to define ztcFmtHdrAndData[].
 *
 * This format is similar to FmtPtrToArray, but there is no pointer.
 */

#undef FmtHdrAndData

/****************************************************************************/

/* Tables of length-index-value-to-length, used by the FmtLenCodeAndPtr()
 * macros below.
 *
 * The macro format is:
 *  Column 1    code: The content of the code value specified by
 *                offsetToLenCode in the FmtLenCodeAndPtr() table below.
 *  Column 2    len: The length of the data associated with this
 *                index value.
 *
 * *warning* This format is only used for get/set messages, but the entire
 * get/set mechanism in BeeStack is probably going to be redesigned.
 */

#if mZtcLenCodeAndPtrFormat_d

#ifndef CodeAndLenMacSetReq
#define CodeAndLenMacSetReq(index, len)
#endif

/* Used for MacGetPIBAttribute.Request (85 03), aka MLME-GET.request */
/*      and MacSetPIBAttribute.Request (85 09), aka MLME-SET.request */
/* Defines maZtcMacSetDataLengthsTable[]. */
CodeAndLenMacSetReq(gMPibAckWaitDuration_c,                 MbrSizeof(macPib_t, mPIBimacAckWaitDuration))
CodeAndLenMacSetReq(gMPibAssociationPermit_c,               MbrSizeof(macPib_t, mPIBimacAssociationPermit))
CodeAndLenMacSetReq(gMPibAutoRequest_c,                     MbrSizeof(macPib_t, mPIBimacAutoRequest))
CodeAndLenMacSetReq(gMPibBattLifeExt_c,                     MbrSizeof(macPib_t, mPIBimacBattLifeExt))
CodeAndLenMacSetReq(gMPibBattLifeExtPeriods_c,              MbrSizeof(macPib_t, mPIBimacBattLifeExtPeriods))
CodeAndLenMacSetReq(gMPibBeaconPayload_c,                   MbrSizeof(macPib_t, mPIBnmacBeaconPayload))
CodeAndLenMacSetReq(gMPibBeaconPayloadLength_c,             MbrSizeof(macPib_t, mPIBimacBeaconPayloadLength))
CodeAndLenMacSetReq(gMPibBeaconOrder_c,                     MbrSizeof(macPib_t, mPIBimacBeaconOrder))
CodeAndLenMacSetReq(gMPibBeaconTxTime_c,                    MbrSizeof(macPib_t, mPIBlmacBeaconTxTime))
CodeAndLenMacSetReq(gMPibBsn_c,                             MbrSizeof(macPib_t, mPIBimacBsn))
CodeAndLenMacSetReq(gMPibCoordExtendedAddress_c,            MbrSizeof(macPib_t, mPIBnmacCoordExtendedAddress))
CodeAndLenMacSetReq(gMPibCoordShortAddress_c,               MbrSizeof(macPib_t, mPIBsmacCoordShortAddress))
CodeAndLenMacSetReq(gMPibDsn_c,                             MbrSizeof(macPib_t, mPIBimacDsn))
CodeAndLenMacSetReq(gMPibGtsPermit_c,                       MbrSizeof(macPib_t, mPIBimacGtsPermit))
CodeAndLenMacSetReq(gMPibMaxCsmaBackoffs_c,                 MbrSizeof(macPib_t, mPIBimacMaxCsmaBackoffs))
CodeAndLenMacSetReq(gMPibMinBe_c,                           MbrSizeof(macPib_t, mPIBimacMinBe))
CodeAndLenMacSetReq(gMPibPanId_c,                           MbrSizeof(macPib_t, mPIBsmacPanId))
CodeAndLenMacSetReq(gMPibPromiscuousMode_c,                 MbrSizeof(macPib_t, mPIBimacPromiscuousMode))
CodeAndLenMacSetReq(gMPibRxOnWhenIdle_c,                    MbrSizeof(macPib_t, mPIBimacRxOnWhenIdle))
CodeAndLenMacSetReq(gMPibShortAddress_c,                    MbrSizeof(macPib_t, mPIBsmacShortAddress))
CodeAndLenMacSetReq(gMPibSuperFrameOrder_c,                 MbrSizeof(macPib_t, mPIBimacSuperFrameOrder))
CodeAndLenMacSetReq(gMPibTransactionPersistenceTime_c,      MbrSizeof(macPib_t, mPIBsmacTransactionPersistenceTime))
#ifdef gMAC2006_d
CodeAndLenMacSetReq(gMPibAssociatedPANCoord_c,              MbrSizeof(macPib_t, mPIBimacAssociatedPANCoord))
CodeAndLenMacSetReq(gMPibMaxBe_c,                           MbrSizeof(macPib_t, mPIBimacMaxBE))
CodeAndLenMacSetReq(gMPibMaxFrameRetries_c,                 MbrSizeof(macPib_t, mPIBimacMaxFrameRetries))
CodeAndLenMacSetReq(gMPibResponseWaitTime_c,                MbrSizeof(macPib_t, mPIBimacResponseWaitTime))
CodeAndLenMacSetReq(gMPibSyncSymbolOffset_c,                MbrSizeof(macPib_t, mPIBsmacSyncSymbolOffset))
CodeAndLenMacSetReq(gMPibTimestampSupported_c,              MbrSizeof(macPib_t, mPIBimacTimestampSupported))
CodeAndLenMacSetReq(gMPibSecurityEnabled_c,                 MbrSizeof(macPib_t, mPIBimacSecurityEnabled))
#endif //gMAC2006_d

#if gSecurityCapability_d

#ifndef gMAC2006_d
CodeAndLenMacSetReq(gMPibAclEntryDescriptorSet_c,           MbrSizeof(macPib_t, gpPIBaclEntryDescriptorSet))
CodeAndLenMacSetReq(gMPibAclEntryDescriptorSetSize_c,       MbrSizeof(macPib_t, mPIBimacAclEntryDescriptorSetSize))
CodeAndLenMacSetReq(gMPibDefaultSecurity_c,                 MbrSizeof(macPib_t, mPIBimacDefaultSecurity))
CodeAndLenMacSetReq(gMPibDefaultSecurityMaterialLength_c,   MbrSizeof(macPib_t, mPIBimacDefaultSecurityMaterialLength))
CodeAndLenMacSetReq(gMPibDefaultSecurityMaterial_c,         MbrSizeof(macPib_t, mPIBnmacDefaultSecurityMaterial))
CodeAndLenMacSetReq(gMPibDefaultSecuritySuite_c,            MbrSizeof(macPib_t, mPIBimacDefaultSecuritySuite))
CodeAndLenMacSetReq(gMPibSecurityMode_c,                    MbrSizeof(macPib_t, mPIBimacSecurityMode))
/* Vendor specific. Used for selecting current */
/* entry in ACL descriptor table.              */
CodeAndLenMacSetReq(gMPibAclEntryCurrent_c,                 MbrSizeof(macPib_t, mPIBimacAclEntryCurrent))
/* The following attributes are mapped to elements */
/* in the gMPibAclEntryDescriptorSet_c attribute.  */
CodeAndLenMacSetReq(gMPibAclEntryExtAddress_c,              MbrSizeof(aclEntryDescriptor_t, aclExtendedAddress))
CodeAndLenMacSetReq(gMPibAclEntryShortAddress_c,            MbrSizeof(aclEntryDescriptor_t, aclShortAddress))
CodeAndLenMacSetReq(gMPibAclEntryPanId_c,                   MbrSizeof(aclEntryDescriptor_t, aclPanId))
CodeAndLenMacSetReq(gMPibAclEntrySecurityMaterialLength_c,  MbrSizeof(aclEntryDescriptor_t, aclSecurityMaterialLength))
CodeAndLenMacSetReq(gMPibAclEntrySecurityMaterial_c,        MbrSizeof(aclEntryDescriptor_t, aclSecurityMaterial))
CodeAndLenMacSetReq(gMPibAclEntrySecuritySuite_c,           MbrSizeof(aclEntryDescriptor_t, aclSecuritySuite))
#else
//CodeAndLenMacSetReq(gMPibKeyTable_c,                        MbrSizeof(macPib_t, gpPIBmacKeyTable))
CodeAndLenMacSetReq(gMPibKeyTableEntries_c,                 MbrSizeof(macPib_t, mPIBimacKeyTableEntries))
//CodeAndLenMacSetReq(gMPibDeviceTable_c,                     MbrSizeof(macPib_t, gpPIBmacDeviceTable))
CodeAndLenMacSetReq(gMPibDeviceTableEntries_c,              MbrSizeof(macPib_t, mPIBimacDeviceTableEntries))
//CodeAndLenMacSetReq(gMPibSecurityLevelTable_c,              MbrSizeof(macPib_t, gpPIBmacSecurityLevelTable))
CodeAndLenMacSetReq(gMPibSecurityLevelTableEntries_c,       MbrSizeof(macPib_t, mPIBimacSecurityLevelTableEntries))
CodeAndLenMacSetReq(gMPibFrameCounter_c,                    MbrSizeof(macPib_t, mPIBnmacFrameCounter))
CodeAndLenMacSetReq(gMPibAutoRequestSecurityLevel_c,        MbrSizeof(macPib_t, mPIBimacAutoRequestSecurityLevel))
CodeAndLenMacSetReq(gMPibAutoRequestKeyIdMode_c,            MbrSizeof(macPib_t, mPIBimacAutoRequestKeyIdMode))
CodeAndLenMacSetReq(gMPibAutoRequestKeySource_c,            MbrSizeof(macPib_t, mPIBnmacAutoRequestKeySource))
CodeAndLenMacSetReq(gMPibAutoRequestKeyIndex_c,             MbrSizeof(macPib_t, mPIBimacAutoRequestKeyIndex))
CodeAndLenMacSetReq(gMPibDefaultKeySource_c,                MbrSizeof(macPib_t, mPIBnmacDefaultKeySource))
CodeAndLenMacSetReq(gMPibPANCoordExtendedAddress_c,         MbrSizeof(macPib_t, mPIBnmacPANCoordExtendedAddress))
CodeAndLenMacSetReq(gMPibPANCoordShortAddress_c,            MbrSizeof(macPib_t, mPIBsmacPANCoordShortAddress))

CodeAndLenMacSetReq(gMPibKeyIdLookupEntries_c,              MbrSizeof(KeyDescriptor_t, KeyIdLookupEntries))
CodeAndLenMacSetReq(gMPibKeyDeviceListEntries_c,            MbrSizeof(KeyDescriptor_t, KeyDeviceListEntries))
CodeAndLenMacSetReq(gMPibKeyUsageListEntries_c,             MbrSizeof(KeyDescriptor_t, KeyUsageListEntries))
CodeAndLenMacSetReq(gMPibKey_c,                             MbrSizeof(KeyDescriptor_t, Key))

CodeAndLenMacSetReq(gMPibKeyUsageFrameType_c,               MbrSizeof(KeyUsageDescriptor_t, FrameType))
CodeAndLenMacSetReq(gMPibKeyUsageCmdFrameId_c,              MbrSizeof(KeyUsageDescriptor_t, CommandFrameIdentifier))

CodeAndLenMacSetReq(gMPibKeyDeviceDescriptorHandle_c,       MbrSizeof(KeyDeviceDescriptor_t, DeviceDescriptorHandle))
CodeAndLenMacSetReq(gMPibUniqueDevice_c,                    MbrSizeof(KeyDeviceDescriptor_t, UniqueDevice))
CodeAndLenMacSetReq(gMPibBlackListed_c,                     MbrSizeof(KeyDeviceDescriptor_t, BlackListed))

CodeAndLenMacSetReq(gMPibSecLevFrameType_c,                      MbrSizeof(SecurityLevelDescriptor_t, FrameType))
CodeAndLenMacSetReq(gMPibSecLevCommnadFrameIdentifier_c,         MbrSizeof(SecurityLevelDescriptor_t, CommandFrameIdentifier))
CodeAndLenMacSetReq(gMPibSecLevSecurityMinimum_c,                MbrSizeof(SecurityLevelDescriptor_t, SecurityMinimum))
CodeAndLenMacSetReq(gMPibSecLevDeviceOverrideSecurityMinimum_c,  MbrSizeof(SecurityLevelDescriptor_t, DeviceOverrideSecurityMinimum))

CodeAndLenMacSetReq(gMPibDeviceDescriptorPanId_c,           MbrSizeof(DeviceDescriptor_t, PANId))
CodeAndLenMacSetReq(gMPibDeviceDescriptorShortAddress_c,    MbrSizeof(DeviceDescriptor_t, ShortAddress))
CodeAndLenMacSetReq(gMPibDeviceDescriptorExtAddress_c,      MbrSizeof(DeviceDescriptor_t, ExtAddress))
CodeAndLenMacSetReq(gMPibDeviceDescriptorFrameCounter_c,    MbrSizeof(DeviceDescriptor_t, FrameCounter))
CodeAndLenMacSetReq(gMPibDeviceDescriptorExempt,            MbrSizeof(DeviceDescriptor_t, Exempt))

CodeAndLenMacSetReq(gMPibKeyIdLookupData_c,                 MbrSizeof(KeyIdLookupDescriptor_t, KeyIdLookupData))
CodeAndLenMacSetReq(gMPibKeyIdLookupDataSize_c,             MbrSizeof(KeyIdLookupDescriptor_t, LookupDataSize))

CodeAndLenMacSetReq(gMPibKeyTableCrtEntry_c,                MbrSizeof(macPib_t, mPIBimacKeyTableCrtEntry))
CodeAndLenMacSetReq(gMPibDeviceTableCrtEntry_c,             MbrSizeof(macPib_t, mPIBimacDeviceTableCrtEntry))
CodeAndLenMacSetReq(gMPibSecurityLevelTableCrtEntry_c,      MbrSizeof(macPib_t, mPIBimacSecurityLevelTableCrtEntry))
CodeAndLenMacSetReq(gMPibKeyIdLookupListCrtEntry_c,         MbrSizeof(macPib_t, mPIBimacKeyIdlLookuplistCrtEntry))
CodeAndLenMacSetReq(gMPibKeyUsageListCrtEntry_c,            MbrSizeof(macPib_t, mPIBimacKeyUsageListCrtEntry))
CodeAndLenMacSetReq(gMPibKeyDeviceListCrtEntry_c,           MbrSizeof(macPib_t, mPIBimacKeyDeviceListCrtEntry))

#endif //gMAC2006_d
#endif                                  /* #if gSecurityCapability_d */
CodeAndLenMacSetReq(gMPibRole_c,                            MbrSizeof(macPib_t, mPIBimacRole))
CodeAndLenMacSetReq(gMPibLogicalChannel_c,                  MbrSizeof(macPib_t, mPIBimacLogicalChannel))
CodeAndLenMacSetReq(gMPibTreemodeStartTime_c,               MbrSizeof(macPib_t, mPIBlmacTreemodeStartTime))
CodeAndLenMacSetReq(gMPibPanIdConflictDetection_c,          MbrSizeof(macPib_t, mPIBimacPanIdConflictDetection))
CodeAndLenMacSetReq(gMPibBeaconResponseDenied_c,            MbrSizeof(macPib_t, mPIBimacBeaconResponseDenied))
CodeAndLenMacSetReq(gMPibNBSuperFrameInterval_c,            MbrSizeof(macPib_t, mPIBimacNBSuperFrameInterval))
CodeAndLenMacSetReq(gMPibBeaconPayloadLength_c,             MbrSizeof(macPib_t, mPIBimacBeaconPayloadLengthVendor))
CodeAndLenMacSetReq(gMPibBeaconResponseLQIThreshold_c,      MbrSizeof(macPib_t, mPIBimacBeaconResponseLQIThreshold))

#undef CodeAndLenMacSetReq
#endif                                  /* #if mZtcLenCodeAndPtrFormat_d */

/***************************************
 * Parameter table for packets and messages that contain a pointer, a count
 * of array elements, and an index to a table of array element lengths.
 *
 * For packet-from-message, copy the message header to the packet. Use the
 * length code to find the length of each element in the array, multiply
 * by the number of elements, and store the length in the packet header
 * as a 16 bit value, replacing the message header's pointer. Copy the data
 * to immediately after the packet header. Note that the Test Tool is not
 * capable of doing the count * element-length arithmetic to find the length
 * of the data, so Ztc must do that for it. The Test Tool ignores the count
 * field in the packet header.
 *
 * For message-from-packet, copy the packet header to the message header. Use
 * the packet header's code field to look up the length of the array elements.
 * Copy the data from just after the end of the packet header to the offset
 * in the message specified by the offsetToMsgData field in this table. Store a
 * pointer to the data in the message header, at the offsetToPtr location.
 *
 * For both, the length of the array elements is found by using the value at
 * the offsetToLenCode as an index in the tableOfDataLengths.
 *
 * The macro format is:
 *  Column 1    name: Name of index, for use in the Fmt() table.
 *  Column 2    hdrLen: Total length of the header that contains the pointer.
 *  Column 3    offsetToPtr: For packet-from-message, the offset in the
 *                packet header of a 16 bit data length field. For
 *                message-from-packet, the offset in the message header
 *                of a pointer to the data. If this value is -1, there is
 *                no pointer; the data immediately follows the header.
 *  Column 4    offsetToCount: offset to a count of the number of elements
 *                in the data array. If this is == -1, then there is no
 *                count field; the count is 1.
 *  Column 5    offsetToLenCode: Offset in the header to the array element
 *                length code.
 *  Column 6    tableOfDataLengths: Pointer to the table of array element
 *                lengths.
 *  Column 7    offsetToMsgData: When copying an array message-from-packet,
 *                this is the offset to the destination array from the
 *                beginning of the destination message. Ignored for
 *                packet-from-message copies.
 *
 * This macro is used to create maZtcLenCodeAndPtrTable[].
 */

#if mZtcLenCodeAndPtrFormat_d

#ifndef FmtLenCodeAndPtr
#define FmtLenCodeAndPtr(name, hdrLen, offsetToPtr, offsetToCount, \
                         offsetToLenCode, tableOfDataLengths, offsetToMsgData)
#endif

/* Used for MacGetPIBAttribute.Request (85 03), aka MLME-GET.confirm.
 * This is a synchronous request, so the offsetToMsgData can be at the end of the
 * message header.
 * Note that MacGetPIBAttribute.Request (84 04) and
 *           MacSetPIBAttribute.Request (85 09)
 * use the same data lengths table.
 */
FmtLenCodeAndPtr(mFmtMacGetReq_c,                               /* name                 */ \
                 (sizeof(mlmeGetReq_t)),                        /* hdrLen               */ \
                 (MbrOfs(mlmeGetReq_t, pibAttributeValue)),     /* offsetToPtr          */ \
                 gMinusOne_c,                                   /* offsetToCount        */ \
                 (MbrOfs(mlmeGetReq_t, pibAttribute)),          /* offsetToLenCode      */ \
                 maZtcMacSetDataLengthsTable,                   /* tableOfDataLengths   */ \
                 (sizeof(mlmeGetReq_t)))                        /* offsetToMsgData      */

/* Used for MacSetPIBAttribute.Request (85 09), aka MLME-SET.request.
 * This is a synchronous request, so the offsetToMsgData can be at the end of the
 * message header.
 * Note that MacGetPIBAttribute.Request (84 04) and
 *           MacSetPIBAttribute.Request (85 09)
 * use the same data lengths table.
 */
FmtLenCodeAndPtr(mFmtMacSetReq_c,                               /* name                 */ \
                 (sizeof(mlmeSetReq_t)),                        /* hdrLen               */ \
                 (MbrOfs(mlmeSetReq_t, pibAttributeValue)),     /* offsetToPtr          */ \
                 gMinusOne_c,                                   /* offsetToCount        */ \
                 (MbrOfs(mlmeSetReq_t, pibAttribute)),          /* offsetToLenCode      */ \
                 maZtcMacSetDataLengthsTable,                   /* tableOfDataLengths   */ \
                 (sizeof(mlmeSetReq_t)))                        /* offsetToMsgData      */

#undef FmtLenCodeAndPtr
#endif                                  /* #if mZtcLenCodeAndPtrFormat_d */

/****************************************************************************/

/* Parameter table for packets and messages that contain two pointers, each
 * pointing to some number of fixed length data items, plus counts of the
 * numbers of items.
 *
 * This is the same as FmtPtrToArray(), but with two length-and-pointers to
 * two arrays.
 *
 * The message consists of a header that contains two pointers to arrays of
 * fixed length elements, plus two counts of the number of array elements.
 * The array data is somewhere else.
 *
 * The packet format is similar to the message, but the pointers are replaced
 * by the array data.
 *
 * For packet-from-message, copy the source message to the destination packet,
 * replacing each pointer with the data it points to.
 *
 * For message-from-packet, copy the data from the locations of the pointers
 * in the header to a location in the message specified by the offsetToMsgData.
 * fields in this table, then copy the source header to the destination
 * message, adding pointers to the data in the offsetToPtr[1|2} locations.
 *
 * For both directions, the length of the data == the number of array elements
 * specified in the count fields of the source times the fixed length of each
 * element from this table.
 *
 * The macro format is:
 *  Column  1   name: Name of index, for use in the Fmt() table.
 *  Column  2   hdrLen: Total length of the header that contains the pointer.
 *  Column  3   offsetToPtr1: Offset in message to the pointer to the first
 *                array.
 *  Column  4   offsetToCount1: Offset in message to the count of items in
 *                the first array.
 *  Column  5   arrayElementLen1: Length of each array item in the first
 *                array.
 *  Column  6   offsetToMsgData: When copying an array message-from-packet,
 *                this is the offset to the first destination array from the
 *                beginning of the destination message. Ignored for
 *                packet-from-message copies.
 *  Column  7   offsetToPtr2: Offset in message to the pointer to the second
 *                array.
 *  Column  8   offsetToCount2: Offset in message to the count of items in
 *                the second array.
 *  Column  9   arrayElementLen2: Length of each array item.
 *
 * The header offset values describe the typedef used by the message.
 *
 * Note that there is no offsetToMsgData2. In message-from-packet, the second
 * array is copied to immediately after the first.
 *
 * Used to define ztcFmtTwoArrayPtrs[].
 */

#ifndef FmtTwoArrayPtrs
#define FmtTwoArrayPtrs(name, hdrLen, \
                        offsetToPtr1, offsetToCount1, array1ElementLen, offsetToMsgData, \
                        offsetToPtr2, offsetToCount2, array2ElementLen)
#endif


/* Use for MacData.Indication (86 01). */
FmtTwoArrayPtrs(mFmtMacDataInd_c,                                                       /* name             */ \
                (sizeof(mcpsDataInd_t)),                                                /* hdrLen           */ \
                (MbrOfs(mcpsDataInd_t, pMsdu)),                                         /* offsetToPtr      */ \
                (MbrOfs(mcpsDataInd_t, msduLength)),                                    /* offsetToCount    */ \
                (MbrSizeof(mcpsDataInd_t, msduLength)),                              /* arrayElementLen  */ \
                (sizeof(mcpsDataInd_t)),                                                /* offsetToMsgData  */ \
                gZero_c,                                                                /* offsetToPtr2     */ \
                gZero_c,                                                                /* offsetToCount2   */ \
                gZero_c)                                                                /* array2ElementLen */

/* Used for Promiscuous.Ind (86 03). */
FmtTwoArrayPtrs(mFmtMcpsPromInd_c,                                                      /* name             */ \
                (sizeof(mcpsPromInd_t)),                                                /* hdrLen           */ \
                (MbrOfs(mcpsPromInd_t, pMsdu)),                                         /* offsetToPtr      */ \
                (MbrOfs(mcpsPromInd_t, msduLength)),                                    /* offsetToCount    */ \
                (IndirectMbrSizeof(mcpsPromInd_t, pMsdu)),                              /* arrayElementLen  */ \
                (sizeof(mcpsPromInd_t)),                                                /* offsetToMsgData  */ \
                gZero_c,                                                                /* offsetToPtr2     */ \
                gZero_c,                                                                /* offsetToCount2   */ \
                gZero_c)                                                                /* array2ElementLen */

/* Used for MacData.Request (87 00). */
FmtTwoArrayPtrs(mFmtMacDataReq_c,                                                       /* name             */ \
                (sizeof(mcpsDataReq_t)),                                                /* hdrLen           */ \
                (MbrOfs(mcpsDataReq_t, pMsdu)),                                         /* offsetToPtr      */ \
                (MbrOfs(mcpsDataReq_t, msduLength)),                                    /* offsetToCount    */ \
                (IndirectMbrSizeof(mcpsDataReq_t, pMsdu)),                              /* arrayElementLen  */ \
                (sizeof(mcpsDataReq_t)),                                                /* offsetToMsgData  */ \
                gZero_c,                                                                /* offsetToPtr2     */ \
                gZero_c,                                                                /* offsetToCount2   */ \
                gZero_c)                                                                /* array2ElementLen */
                
/* Used for Asp Raw Data Req (0x95 aspMsgTypeTelecSendRawData_c). */
FmtTwoArrayPtrs(mFmtAspTelecSendRawDataReq_c,                                           /* name             */ \
                (sizeof(aspTelecSendRawData_t)),                                        /* hdrLen           */ \
                (MbrOfs(aspTelecSendRawData_t, dataPtr)),                               /* offsetToPtr      */ \
                (MbrOfs(aspTelecSendRawData_t, length)),                                /* offsetToCount    */ \
                (IndirectMbrSizeof(aspTelecSendRawData_t, dataPtr)),                    /* arrayElementLen  */ \
                (sizeof(aspTelecSendRawData_t)),                                        /* offsetToMsgData  */ \
                gZero_c,                                                                /* offsetToPtr2     */ \
                gZero_c,                                                                /* offsetToCount2   */ \
                gZero_c)                 


#undef FmtTwoArrayPtrs

/****************************************************************************/

/* Tables of indexes and copy functions, by copy direction. */
#ifndef PktFromMsgFunction
#define PktFromMsgFunction(index, pFunction)
#endif
#ifndef MsgFromPktFunction
#define MsgFromPktFunction(index, pFunction)
#endif

PktFromMsgFunction(mZtcPktFromMsgUnused_c,         ZtcPktFromMsgUnused)
PktFromMsgFunction(mZtcPktFromMsgSimple_c,         ZtcPktFromMsgSimple)

#if gSAPMessagesEnableMlme_d || gSAPMessagesEnableNlme_d
PktFromMsgFunction(mZtcPktFromMsgMacScanCnf_c,        ZtcPktFromMsgMacScanCnf)
#endif
#if gSAPMessagesEnableMlme_d 
PktFromMsgFunction(mZtcPktFromMsgMacBeaconNotifyInd_c,ZtcPktFromMsgMacBeaconNotifyInd)
#endif
#if mZtcLenCodeAndPtrFormat_d
PktFromMsgFunction(mZtcPktFromMsgLenCodeAndPtr_c,  ZtcPktFromMsgLenCodeAndPtr)
#endif
PktFromMsgFunction(mZtcPktFromMsgTwoArrayPtrs_c,   ZtcPktFromMsgTwoArrayPtrs)

MsgFromPktFunction(mZtcMsgFromPktUnused_c,         ZtcMsgFromPktUnused)
MsgFromPktFunction(mZtcMsgFromPktSimple_c,         ZtcMsgFromPktSimple)

#if gSAPMessagesEnableMlme_d 
MsgFromPktFunction(mZtcMsgFromPktMacScanReq_c,     ZtcMsgFromPktMacScanReq)
PktFromMsgFunction(mZtcPktFromMsgMacScanReq_c,     ZtcPktFromMsgMacScanReq)
#endif
#if mZtcLenCodeAndPtrFormat_d
MsgFromPktFunction(mZtcMsgFromPktLenCodeAndPtr_c,  ZtcMsgFromPktLenCodeAndPtr)
#endif
MsgFromPktFunction(mZtcMsgFromPktTwoArrayPtrs_c,   ZtcMsgFromPktTwoArrayPtrs)


#if gSAPMessagesEnableInterPan_d 
MsgFromPktFunction(mZtcMsgFromPktInterPanReq_c,     ZtcMsgFromPktInterPanDataReq)
PktFromMsgFunction(mZtcPktFromMsgInterPanReq_c,     ZtcPktFromMsgInterPanDataReq)
#endif




#undef PktFromMsgFunction
#undef MsgFromPktFunction

/****************************************************************************/

/* Main format table. Each entry in the message info tables includes an
 * enum that is used as an index into this table. Each row in this table
 * represents a named format conversion type.
 * Each table entry contains two (function pointer, index) pairs. The
 * function pointer points to the function to call to perform the copy,
 * packet-from-message or message-from-packet. The index is used by that
 * function in one of the tables above (each function knows which table
 * it uses) to find parameters for this particular copy operation.
 *
 * The macro format is:
 *  Column 1    name: Identifier naming this format.
 *  Column 2    pktFromMsgFunc: Function to use to copy client-packet-
 *                from-SAP-Handler-message.
 *  Column 3    pktFromMsgIndex: Index to the packet-from-msg copy
 *                function's parameters.
 *  Column 4    msgFromPktFunc: Function to use to copy message-from-packet.
 *  Column 3    msgFromPktIndex: Index to the msg-from-packet copy
 *                function's parameters.
 *
 * For both sync and async message types,
 *      message-from-packet copy functions are used to inject messages
 *      from the external client (the Test Tool) into the SAP Handlers.
 *
 * For async message types,
 *      packet-from-message copy functions are used for sending monitor
 *      copies of messsages that pass through SAP Handler out to the
 *      external client.
 *
 * For sync message types,
 *      packet-from-message copy function are used to synthesize
 *      confirmations or responses to requests. The request may
 *      originate from within BeeStack or from the external client
 *      via Ztc.
 *
 * Used to define ztcFmtInfoTable[].
 */

#ifndef Fmt
#define Fmt(name, pktFromMsgFuncIndex, pktFromMsgParamIndex, \
            msgFromPktFuncIndex, msgFromPktParamIndex)
#endif

/* Used for the majority of messages. The simple format does not need */
/* parameter table indexes. */
Fmt(gZtcMsgFmtSimple_c,                 /* name             */ \
    mZtcPktFromMsgSimple_c,             /* pktFromMsgFunc   */ \
    gUnused_c,                          /* pktFromMsgIndex  */ \
    mZtcMsgFromPktSimple_c,             /* msgFromPktFunc   */ \
    gUnused_c)                          /* msgFromPktIndex  */

#if gSAPMessagesEnableMlme_d    
/* Used for MacScan.Confirm (84 04).  */
Fmt(gZtcMsgFmtMacBeaconNotifyInd_c,     /* name             */ \
    mZtcPktFromMsgMacBeaconNotifyInd_c, /* pktFromMsgFunc   */ \
    gUnused_c,                          /* pktFromMsgIndex  */ \
    mZtcMsgFromPktSimple_c,             /* msgFromPktFunc   */ \
    gUnused_c)                          /* msgFromPktIndex  */

/* Used for MacScan.Request (85 08).  */
Fmt(gZtcMsgFmtMacScanReq_c,             /* name             */ \
    mZtcPktFromMsgMacScanReq_c,         /* pktFromMsgFunc   */ \
    gUnused_c,                          /* pktFromMsgIndex  */ \
    mZtcMsgFromPktMacScanReq_c,         /* msgFromPktFunc   */ \
    gUnused_c)                          /* msgFromPktIndex   */

#endif
#if gSAPMessagesEnableMlme_d || gSAPMessagesEnableNlme_d   
/* Used for MacScan.Confirm (84 0B). and for the NLME-ED-scan.Confirm    */
Fmt(gZtcMsgFmtMacScanCnf_c,             /* name             */ \
    mZtcPktFromMsgMacScanCnf_c,         /* pktFromMsgFunc   */ \
    gUnused_c,                          /* pktFromMsgIndex  */ \
    mZtcMsgFromPktSimple_c,             /* msgFromPktFunc   */ \
    gUnused_c)                          /* msgFromPktIndex  */
#endif    


/* Used for MacGetPIBAttribute.Request (85 03), aka MLME-GET.request. */
#if mZtcLenCodeAndPtrFormat_d
Fmt(gZtcMsgFmtMacGetReq_c,              /* name             */ \
    mZtcPktFromMsgLenCodeAndPtr_c,      /* pktFromMsgFunc   */ \
    mFmtMacGetReq_c,                    /* pktFromMsgIndex  */ \
    mZtcMsgFromPktLenCodeAndPtr_c,      /* msgFromPktFunc   */ \
    mFmtMacGetReq_c)                    /* msgFromPktIndex  */
#endif                                  /* #if mZtcLenCodeAndPtrFormat_d */

/* Used for MacSetPIBAttribute.Request (85 09), aka MLME-SET.request. */
#if mZtcLenCodeAndPtrFormat_d
Fmt(gZtcMsgFmtMacSetReq_c,              /* name             */ \
    mZtcPktFromMsgLenCodeAndPtr_c,      /* pktFromMsgFunc   */ \
    mFmtMacSetReq_c,                    /* pktFromMsgIndex  */ \
    mZtcMsgFromPktLenCodeAndPtr_c,      /* msgFromPktFunc   */ \
    mFmtMacSetReq_c)                    /* msgFromPktIndex  */
#endif                                  /* #if mZtcLenCodeAndPtrFormat_d */

/* Used for MacData.Indication (86 01). */
Fmt(gZtcMsgFmtMacDataInd_c,             /* name             */ \
    mZtcPktFromMsgTwoArrayPtrs_c,       /* pktFromMsgFunc   */ \
    mFmtMacDataInd_c,                   /* pktFromMsgIndex  */ \
    mZtcMsgFromPktTwoArrayPtrs_c,       /* msgFromPktFunc   */ \
    mFmtMacDataInd_c)                   /* msgFromPktIndex  */

/* Used for Promiscuous.Ind (86 03). */
Fmt(gZtcMsgFmtMcpsPromInd_c,            /* name             */ \
    mZtcPktFromMsgTwoArrayPtrs_c,       /* pktFromMsgFunc   */ \
    mFmtMcpsPromInd_c,                  /* pktFromMsgIndex  */ \
    mZtcMsgFromPktTwoArrayPtrs_c,       /* msgFromPktFunc   */ \
    mFmtMcpsPromInd_c)                  /* msgFromPktIndex  */

/* Used for MacData.Request (87 00). */
Fmt(gZtcMsgFmtMacDataReq_c,             /* name             */ \
    mZtcPktFromMsgTwoArrayPtrs_c,       /* pktFromMsgFunc   */ \
    mFmtMacDataReq_c,                   /* pktFromMsgIndex  */ \
    mZtcMsgFromPktTwoArrayPtrs_c,       /* msgFromPktFunc   */ \
    mFmtMacDataReq_c)                   /* msgFromPktIndex  */

/* Used for Asp Raw Data Req (0x95 aspMsgTypeTelecSendRawData_c). */
Fmt(gZtcMsgFmtAspTelecSendRawDataReq_c, /* name             */ \
    mZtcPktFromMsgTwoArrayPtrs_c,       /* pktFromMsgFunc   */ \
    mFmtAspTelecSendRawDataReq_c,       /* pktFromMsgIndex  */ \
    mZtcMsgFromPktTwoArrayPtrs_c,       /* msgFromPktFunc   */ \
    mFmtAspTelecSendRawDataReq_c)       /* msgFromPktIndex  */    

#undef Fmt
#undef AppZdpSize
#undef AppZdpOfs
