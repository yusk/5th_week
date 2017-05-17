/******************************************************************************
* ZtcSapHandlerInfoTbl.h
* Ztc is a replacement for the existing ZigBee Text Client (ZTC) module.
*
* Copyright (c) 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
******************************************************************************/

/* Tables of information about SAP Handlers. */

/* The purpose of these tables is to keep various declarations and defintions
 * in perfect sync, without requiring maintenence of different declarations,
 * and without requiring changes in more than one file.
 * All information needed by Ztc about the SAP Handlers is kept in this single
 * place.
 *
 * The code should never be aware of which SAP Handler it is working with.
 * There should never be logic of the form:
 *
 *      "if this is SAP Handler X,
 *      then do Y"
 *
 * All of the knobs that can be tweaked to describe SAP Handlers should be
 * in this file. The code should instead contain logic of the form:
 *
 *      "if the SAP Handler info table entry for this SAP Handler says X,
 *      then do Y".
 *
 * Making a change to the way the code deals with different SAP Handlers
 * should require only changes in this file, unless a completely new kind
 * of handling is needed. In that case, this file should be changed to provide
 * a means of triggering the new behavior of the code.
 */

/* Some of the symbols and values in this table must remain as they are
 * for compabilility with external tools and with the rest of the Beestack
 * code. Note that the MAC library source code is not normally available;
 * some values must agree with the existing MAC library binary.
 */

/* IMPORTANT NOTE: The order of the entries in these tables is not significant.
 * Entries can be added, removed or moved freely. The code must work if the
 * order of the entries in the tables is changed.
 */

/****************************************************************************/

/* Table of SAP Handler mode table indexes.
 *
 * Ztc can set any SAP handler to one of three modes:
 *  disable:    Ztc ignores messages between BeeStack and the SAP Handler.
 *  hook:       Messages between BeeStack and the SAP Handler are intercepted
 *                by Ztc and sent only to the Test Tool. The SAP Handler never
 *                sees the intercepted message.
 *  monitor:    Ztc forwards copies of messages between BeeStack and the SAP
 *                Handler to the Test Client.
 *
 * The Test Tool thinks both SAP Handlers between any two layers (one in each
 * direction) as a single entity, and configures both at the same time. Instead
 * of depending on the specific integer values of the opcode groups or internal
 * SAP Handler Ids, the main SAP Handler info table contains a field that
 * specifieswhich mode value that SAP Handler uses.
 *
 * The correspondance between SAP Handlers and mode table index values is not
 * one-to-one, or even a simple two-to-one.
 *
 * These macros are used to create the table of SAP Handler mode values.
 *
 * Note: Since synchronous messages are never actually queued, they cannot be
 * hooked.
 *
 * The SapMode() macro format is:
 *  Column 1    name: The name is used by the Sap() macros to refer to an index
 *                in ztcSAPModeTable[].
 *  Column 2    index: The index into ztcSAPModeTable[]. This must agree with
 *                the Test Tool.
 *  Column 3    defaultMode: The mode that the SAP Handlers using this index
 *                are in at startup.
 *
 * The canonical declaration of this macro is:
 *  SapMode( name, index, defaultMode )
 *
 * These macros are used to define maZtcSAPModeTable[].
 */

#ifndef SapMode
#define SapMode( name, index, defaultMode )
#endif

#ifdef gZtcDebug_d
#define gZtcSAPModeDefault_c    gZtcSAPModeMonitor_c
#else
#define gZtcSAPModeDefault_c    gZtcSAPModeDisable_c
#endif

#if gSAPMessagesEnableMcps_d
SapMode( gSAPModeIndexMcps_c,  0, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexMcps_c,  0, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableMlme_d
SapMode( gSAPModeIndexMlme_c,  1, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexMlme_c,  1, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableAsp_d
SapMode( gSAPModeIndexAsp_c,   2, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexAsp_c,   2, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableNlde_d
SapMode( gSAPModeIndexNlde_c,  3, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexNlde_c,  3, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableNlme_d
SapMode( gSAPModeIndexNlme_c,  4, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexNlme_c,  4, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableApsde_d
SapMode( gSAPModeIndexApsde_c, 5, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexApsde_c, 5, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableAfde_d
SapMode( gSAPModeIndexAfde_c,  6, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexAfde_c,  6, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableApsme_d
SapMode( gSAPModeIndexApsme_c, 7, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexApsme_c, 7, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableZdp_d
SapMode( gSAPModeIndexZdp_c,   8, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexZdp_c,   8, gZtcSAPModeInvalid_c )
#endif

#if gSAPMessagesEnableInterPan_d
SapMode( gSAPModeIndexInterPan_c,   9, gZtcSAPModeDefault_c )
#else
SapMode( gSAPModeIndexInterPan_c,   9, gZtcSAPModeInvalid_c )
#endif


#undef  SapMode

/****************************************************************************/

/* Main SAP Handler information table.
 *
 * SAP Handlers generally come in pairs, one for each direction across
 * an interface between stack layers. There are two exceptions to this:
 * there is no ZDP-to-App SAP Handler, and there is no Ztc-to-external-
 * client SAP Handler.
 *
 * In the original ZTC code, the internal Sap Id (aka SAPId) to OpcodeGroup
 * map was not one-to-one. All of the OpcodeGroups mapped to unique internal
 * Ids, but two internal Sap Ids (gNwkASP_SAPHandlerId_c == 4 and
 * gASPNwkSAPHandlerId_c == 5) were both translated back to
 * OpCodeGrpForNwkToASP_SAP_c == 0x95.
 * This table makes the map symmetric (A -> B -> A), which might be
 * a problem.
 *
 * The Sap() macro format is:
 *  Column  1   opcodeGroup: The integer used by the Test Tool to identify this
 *                SAP Handler. Must agree with the Test Tool.
 *  Column  2   converse: The integer used by the Test Tool to identify the
 *                opposite direction SAP Handler.
 *  Column  3   intSAPIdName: The identifier used by the BeeStack code to refer
 *                to this SAP Handler. Must agree with the rest of BeeStack.
 *  Column  4   pSAPFunc: Pointer to the SAP Function.
 *  Column  5   modeTableIndex: This SAP Handler's index into the SAP Handler
 *                mode table (see SapMode() above.)
 *  Column  6   msgQueue: Pointer to the message queue used by this SAP
 *                Handler. Used to dequeue messages in hook mode.
 *  Column  7   msgEvent: Event flag passed to the SAP Handler's task to
 *                indicate a queued message.
 *  Column  8   pTaskID: Pointer to a global that contains the Task ID for
 *                this SAP Handler's task. Used to clear message events in
 *                hook mode. NULL indicates a SAP Handler that does not
 *                have an associated task.
 *  Column  9   msgTypeTable: Name of a table of information about the specific
 *                requests / commands / confirmations / indications that this
 *                SAP Handler knows how to handle.
 *  Column 10   msgTypeTableLen: Name of an index_t that contains the number
 *                of entries in the msgTypeTable.
 *
 * Used to define maZtcSAPHandlerInfo[].
 */

#ifndef Sap
#define Sap( opcodeGroup, converse, ztcSAPIdName, pSAPFunc, \
             modeTableIndex, msgQueue, msgEvent, pTaskID, msgTypeTable, \
             msgTypeTableLen )
#endif

#ifndef DisabledSap
#define DisabledSap( opcodeGroup )
#endif

/* MAC layer SAP Handlers */
/* MCPS is the 802.15.4 data tramission standard. */
#if gSAPMessagesEnableMcps_d
Sap( gNwkMcpsOpcodeGroup_c,                     /* opcodeGroup      */ \
     gMcpsNwkOpcodeGroup_c,                     /* converse         */ \
     gNwkMCPS_SAPHandlerId_c,                   /* ztcSAPIdName     */ \
     NWK_MCPS_SapHandler,                       /* pSAPFunc         */ \
     gSAPModeIndexMcps_c,                       /* modeTableIndex   */ \
     NULL,                                      /* msgQueue         */ \
     gTODO_c,                                   /* msgEvent         */ \
     &gMacTaskID_c,                             /* pTaskID          */ \
     gaZtcNwkMcpsMsgTypeTable,                  /* msgTypeTable     */ \
     gZtcNwkMcpsMsgTypeTableLen )               /* msgTypeTableLen  */
#else
DisabledSap( gNwkMcpsOpcodeGroup_c )
#endif
#if gMacStandAlone_d

#if gSAPMessagesEnableMcps_d
Sap( gMcpsNwkOpcodeGroup_c,                     /* opcodeGroup      */ \
     gNwkMcpsOpcodeGroup_c,                     /* converse         */ \
     gMCPSNwkSAPHandlerId_c,                    /* ztcSAPIdName     */ \
     MCPS_NWK_SapHandler,                       /* pSAPFunc         */ \
     gSAPModeIndexMcps_c,                       /* modeTableIndex   */ \
     &mMcpsNwkInputQueue,                       /* msgQueue         */ \
     gAppEvtMessageFromMCPS_c,                    /* msgEvent         */ \
     &gAppTaskID_c,                               /* pTaskID          */ \
     gaZtcMcpsNwkMsgTypeTable,                  /* msgTypeTable     */ \
     gZtcMcpsNwkMsgTypeTableLen )               /* msgTypeTableLen  */
#else
DisabledSap( gMcpsNwkOpcodeGroup_c )
#endif

#endif

#if gSAPMessagesEnableMlme_d
Sap( gNwkMlmeOpcodeGroup_c,                     /* opcodeGroup      */ \
     gMlmeNwkOpcodeGroup_c,                     /* converse         */ \
     gNwkMLME_SAPHandlerId_c,                   /* ztcSAPIdName     */ \
     NWK_MLME_SapHandler,                       /* pSAPFunc         */ \
     gSAPModeIndexMlme_c,                       /* modeTableIndex   */ \
     NULL,                                      /* msgQueue         */ \
     gTODO_c,                                   /* msgEvent         */ \
     &gMacTaskID_c,                             /* pTaskID          */ \
     gaZtcNwkMlmeMsgTypeTable,                  /* msgTypeTable     */ \
     gZtcNwkMlmeMsgTypeTableLen )               /* msgTypeTableLen  */
#else
DisabledSap( gNwkMlmeOpcodeGroup_c )
#endif

#if gMacStandAlone_d

#if gSAPMessagesEnableMlme_d
Sap( gMlmeNwkOpcodeGroup_c,                     /* opcodeGroup      */ \
     gNwkMlmeOpcodeGroup_c,                     /* converse         */ \
     gMLMENwkSAPHandlerId_c,                    /* ztcSAPIdName     */ \
     MLME_NWK_SapHandler,                       /* pSAPFunc         */ \
     gSAPModeIndexMlme_c,                       /* modeTableIndex   */ \
     &mMlmeNwkInputQueue,                       /* msgQueue         */ \
     gAppEvtMessageFromMLME_c,                    /* msgEvent         */ \
     &gAppTaskID_c,                               /* pTaskID          */ \
     gaZtcMlmeNwkMsgTypeTable,                  /* msgTypeTable     */ \
     gZtcMlmeNwkMsgTypeTableLen )               /* msgTypeTableLen  */
#else
DisabledSap( gMlmeNwkOpcodeGroup_c )
#endif

#endif 

#if gSAPMessagesEnableAsp_d
Sap( gAppAspOpcodeGroup_c,                      /* opcodeGroup      */ \
     gAspAppOpcodeGroup_c,                      /* converse         */ \
     gNwkASP_SAPHandlerId_c,                    /* ztcSAPIdName     */ \
     APP_ASP_SapHandler,                        /* pSAPFunc         */ \
     gSAPModeIndexAsp_c,                        /* modeTableIndex   */ \
     NULL,                                      /* msgQueue         */ \
     gTODO_c,                                   /* msgEvent         */ \
     &gMacTaskID_c,                             /* pTaskID          */ \
     gaZtcNwkAspMsgTypeTable,                   /* msgTypeTable     */ \
     gZtcNwkAspMsgTypeTableLen )                /* msgTypeTableLen  */
#else
DisabledSap( gAppAspOpcodeGroup_c )
#endif

#if gMacStandAlone_d

#if gSAPMessagesEnableAsp_d
Sap( gAspAppOpcodeGroup_c,                      /* opcodeGroup      */ \
     gAppAspOpcodeGroup_c,                      /* converse         */ \
     gASPNwkSAPHandlerId_c,                     /* ztcSAPIdName     */ \
     ASP_APP_SapHandler,                        /* pSAPFunc         */ \
     gSAPModeIndexAsp_c,                        /* modeTableIndex   */ \
     NULL,                                      /* msgQueue         */ \
     gTODO_c,                                   /* msgEvent         */ \
     &gAppTaskID_c,                               /* pTaskID          */ \
     gaZtcAspNwkMsgTypeTable,                   /* msgTypeTable     */ \
     gZtcAspNwkMsgTypeTableLen )                /* msgTypeTableLen  */
#else
DisabledSap( gAspAppOpcodeGroup_c )
#endif

#endif 


DisabledSap( gApsNldeOpcodeGroup_c )

DisabledSap( gNldeApsOpcodeGroup_c )

DisabledSap( gZdoNlmeOpcodeGroup_c )

DisabledSap( gNlmeZdoOpcodeGroup_c )

DisabledSap( gAfApsdeOpcodeGroup_c )

DisabledSap( gApsdeAfOpcodeGroup_c )

DisabledSap( gAppAfdeOpcodeGroup_c )

DisabledSap( gAppAfdeOpcodeGroup_c )

#if gStandardSecurity_d || gHighSecurity_d
#define mApsmeZdoInputQueue_d ( &mApsmeZdoInputQueue )
#else
#define mApsmeZdoInputQueue_d ( NULL )
#endif

DisabledSap( gZdoApsmeOpcodeGroup_c )

#if gStandardSecurity_d || gHighSecurity_d
#define mApsmeQueue_d ( &mApsmeQueue )
#else
#define mApsmeQueue_d ( NULL )
#endif


DisabledSap( gApsmeZdoOpcodeGroup_c )

DisabledSap( gAppZdpOpcodeGroup_c )

DisabledSap( gZdpAppOpcodeGroup_c )

DisabledSap( gAppZdpOpcodeGroup_c )

DisabledSap( gZdpAppOpcodeGroup_c )

#undef Sap
#undef DisabledSap
