/******************************************************************************
* ZTC message type table definitions and related code.
*
* Copyright (c) 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from  Freescale Semiconductor.
*
******************************************************************************/

/* Define the tables that describe the Ztc-relevant properties associated
 * with the various Message Ids, and the utility functions used to find
 * things in those tables.
 *
 * There is one table per SAP Handler; eachtable contains information about
 * all of the Message Ids recognized by that SAP Handler.
 *
 * All Message Id infomation (well, almost all) is actually collected in
 * ZtcMsgTypeInfoTbl.h, with the information about a given Message Id on a
 * single line, within a macro. This file defines those macros to select
 * the desired part of the Message Id information, and then #include's
 * the tbl file.
 *
 * The tables here are never directly accesed by their names. The SAP Handler
 * info table contains a pointer to the appropriate Message Id information
 * table for each SAP Handler.
 */

#include "EmbeddedTypes.h"

#include "ZtcInterface.h"

#if gZtcIncluded_d

#include "AppAspInterface.h"
#include "ZtcSAPHandlerInfo.h"
#include "AspZtc.h"

#include "ZtcPrivate.h"
#include "ZtcClientCommunication.h"
#include "ZtcMsgTypeInfo.h"
#include "ZtcCopyData.h"

/******************************************************************************
*******************************************************************************
* Public memory definitions
*******************************************************************************
******************************************************************************/

/* Note that for each table, there is also a table length index_t. The code
 * needs some way to know how many rows are in each table, so the main SAP
 * Handler table contains a pointer to this length variable. This costs
 * one index_t for the length variable plus one (index_t *) for the pointer
 * to it, per SAP Handler. Fortunately, there aren't very many SAP Handlers.
 *
 * There are other ways to make the lengths of the message info tables
 * available for use in the SAP Handler table that would not require the
 * pointers, but they involve some C language and/or pre-processor tricks
 * that are not obvious, and may not work in all compilers.
 */

/* opcode group 0x87, NWK_MCPS_SapHandler() */
#if gSAPMessagesEnableMcps_d
#define NwkMcpsTbl(msgType, cnfType, flags, len, format) \
    {msgType, cnfType, len, (flags | format)},
ztcMsgTypeInfo_t const gaZtcNwkMcpsMsgTypeTable[] = {
#include "ZtcMsgTypeInfoTbl.h"
};
index_t const gZtcNwkMcpsMsgTypeTableLen = NumberOfElements(gaZtcNwkMcpsMsgTypeTable);
#endif

/* opcode group 0x86, MCPS_NWK_SapHandler() */
#if gSAPMessagesEnableMcps_d
#define McpsNwkTbl(msgType, cnfType, flags, len, format) \
    {msgType, cnfType, len, (flags | format)},
ztcMsgTypeInfo_t const gaZtcMcpsNwkMsgTypeTable[] = {
#include "ZtcMsgTypeInfoTbl.h"
};
index_t const gZtcMcpsNwkMsgTypeTableLen = NumberOfElements(gaZtcMcpsNwkMsgTypeTable);
#endif

/* opcode group_c = 0x85, NWK_MLME_SapHandler() */
#if gSAPMessagesEnableMlme_d
#define NwkMlmeTbl(msgType, cnfType, flags, len, format) \
    {msgType, cnfType, len, (flags | format)},
ztcMsgTypeInfo_t const gaZtcNwkMlmeMsgTypeTable[] = {
#include "ZtcMsgTypeInfoTbl.h"
};
index_t const gZtcNwkMlmeMsgTypeTableLen = NumberOfElements(gaZtcNwkMlmeMsgTypeTable);
#endif

/* opcode group 0x84, MLME_NWK_SapHandler() */
#if gSAPMessagesEnableMlme_d
#define MlmeNwkTbl(msgType, cnfType, flags, len, format) \
    {msgType, cnfType, len, (flags | format)},
ztcMsgTypeInfo_t const gaZtcMlmeNwkMsgTypeTable[] = {
#include "ZtcMsgTypeInfoTbl.h"
};
index_t const gZtcMlmeNwkMsgTypeTableLen = NumberOfElements(gaZtcMlmeNwkMsgTypeTable);
#endif

/* opcode group_c = 0x95, APP_ASP_SapHandler() */
#if gSAPMessagesEnableAsp_d
#define NwkAspTbl(msgType, cnfType, flags, len, format) \
    {msgType, cnfType, len, (flags | format)},
ztcMsgTypeInfo_t const gaZtcNwkAspMsgTypeTable[] = {
#include "ZtcMsgTypeInfoTbl.h"
};
index_t const gZtcNwkAspMsgTypeTableLen = NumberOfElements(gaZtcNwkAspMsgTypeTable);
#endif

/* opcode group 0x94, ASP_APP_SapHandler() */
#if gSAPMessagesEnableAsp_d
#define AspNwkTbl(msgType, cnfType, flags, len, format) \
    {msgType, cnfType, len, (flags | format)},
ztcMsgTypeInfo_t const gaZtcAspNwkMsgTypeTable[] = {
#include "ZtcMsgTypeInfoTbl.h"
};
index_t const gZtcAspNwkMsgTypeTableLen = NumberOfElements(gaZtcAspNwkMsgTypeTable);
#endif


/* Ztc handles all requests internally, and needs less information about
 * message types than the SAP Handlers need.
 */
#define ZtcMsgTbl(msgType, configFunc) \
  {msgType, configFunc},
ztcInternalMsgTypeInfo_t const gaZtcInternalMsgTypeInfoTable[] = {
#include "ZtcMsgTypeInfoTbl.h"
};
index_t const gZtcInternalMsgTypeInfoTableLen = NumberOfElements(gaZtcInternalMsgTypeInfoTable);

/******************************************************************************
*******************************************************************************
* Private memory declarations
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
******************************************************************************/

/* Given a pointer to a message info table and a message type, return a */
/* pointer to the entry describing the message type. */
ztcMsgTypeInfo_t const *pZtcMsgTypeInfoFromMsgType
  (
  ztcMsgTypeInfo_t const *pMsgTypeTable,
  index_t const msgTypeInfoLen,
  ztcMsgType_t const msgType
  )
{
  index_t i;

  for (i = 0; i < msgTypeInfoLen; ++i) {
    if (pMsgTypeTable[i].msgType == msgType) {
      return &(pMsgTypeTable[i]);
    }
  }

  return NULL;
}                                       /* pZtcPtrToMsgTypeInfo() */

#endif                                  /* #if gZtcIncluded_d == 1 */
