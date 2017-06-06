/******************************************************************************
* Implementation of the ZigBee Test Client.
* ZTC task public functions, and various internals.
*
* Copyright (c) 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
******************************************************************************/

#include "EmbeddedTypes.h"

#include "MsgSystem.h"
#include "FunctionLib.h"
#include "PublicConst.h"
#include "TS_Interface.h"
#include "NVM_Interface.h"
#include "NV_Flash.h"
#include "NV_FlashHAL.h"
#include "NV_Data.h"

#include "PWRLib.h"
#include "PublicConst.h"

#include "Uart_Interface.h"

#include "ZtcInterface.h"
#include "ZtcPrivate.h"
#include "ZtcClientCommunication.h"
#include "ZtcMsgTypeInfo.h"
#include "ZtcSAPHandlerInfo.h"
#include "ZtcCopyData.h"

#include "SPI_Interface.h"
#include "IIC_Interface.h"

#include "ApplicationConf.h"

#ifdef gZtcOtapSupport_d
  #include "OtapSupport.h"
  #include "Bootloader.h"
  #include "Eeprom.h"
#endif

#if gMacStandAlone_d
#include "Mac_Globals.h"
#endif

#if (gLpmIncluded_d == 1)
  #include "PWR_Interface.h"
#endif

#ifndef PROCESSOR_MC1323X
  #include "Phy.h"
#endif

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/

/* Format of the messages echanged between Ztc and the SAP Handlers.
 *
 * *warning* All of the SAP Handlers receive a sapHandlerMsg_t, containing a
 * msgType.
 * The AFDE -> APP and APP -> AFDE SAP Handler send back only raw data,
 * without a msgType, to ZTC_TaskEventMonitor().
 * All other SAP Handlers send back a sapHandlerMsg_t *with* a msgType.
 */
 
typedef struct sapHandlerMsgHdr_tag {
  ztcMsgType_t msgType;
} sapHandlerMsgHdr_t;

#ifndef gMAC2006_d
 #define mSapHandlerMsgTotalLen_c    144
 #define mMaxPacketSize_c            144
#else
 #define mSapHandlerMsgTotalLen_c    155  // Security material added (securityLevel, keyIdMode, keySource, keyIndex)
 #define mMaxPacketSize_c            155
#endif //gMAC2006_d

typedef union sapHandlerMsg_tag {
  uint8_t raw[mSapHandlerMsgTotalLen_c];
  struct {
    sapHandlerMsgHdr_t header;
    uint8_t payload[mSapHandlerMsgTotalLen_c - sizeof(sapHandlerMsgHdr_t)];
  } structured;
} sapHandlerMsg_t;

/* Permissions Configuration Table. */
#if (gZtcIncluded_d && (gStandardSecurity_d || gHighSecurity_d) && gApsMaxEntriesForPermissionsTable_c)
typedef struct ztcPermissionsTable_tag {
  index_t count;                                /* Number of entries in table. */
  permissionsTable_t table[(sizeof(clientPacket_t) - sizeof(clientPacketHdr_t))
                    / sizeof(permissionsTable_t)];     /* First entry of variable-length table. */
} ztcPermissionsTable_t;
#endif  /* (gZtcIncluded_d && (gStandardSecurity_d || gHighSecurity_d) && gApsMaxEntriesForPermissionsTable_c) */

#ifndef PROCESSOR_MC1323X
typedef struct ztcAbelWrite_tag {
  uint8_t  abelReg;
  uint16_t value;
} ztcAbelWrite_t;
#endif /* PROCESSOR_MC1323X */

/* a single fragment as seen by ZTC */
typedef struct apsdeFragment_tag {
  uint8_t iBlockNumber;
  uint8_t iMoreBlocks;
  uint8_t iLength;
  uint8_t aData[1];
} apsdeFragment_t;

typedef struct ztcWakeUpConfig_tag
{
 uint8_t selectedKBPin; /* Mask for selecting the pin */
 uint8_t detectionEdge; /* The selected edge when MCU should wake up; 
                           0x00 - wake up MCU on falling edges;
                           0x01 - wake up MCU on rising edges */
 bool_t  signalWhenWakeUpFlag; /* Flag used to send or not a WakeUp.Ind message */
 uint8_t prevPinLevel; /* Keep track of previous pin level */
}ztcWakeUpConfig_t;

/* Only KB4-KB7 can wake up the MCU on both (rising or falling) edges.
   KB3-KB0 can only wake up the MCU on falling edges. */
#define mRisingEdgesPinsMask_c  0xF0
/* detectionEdge == 0x01, MCU is waking up on rising edges
   detectionEdge == 0x00, MCU is waking up on falling edges */
#define mFallingEdges_c  0x00
#define mRisingEdges_c   0x01 


/******************************************************************************
*******************************************************************************
* Private prototypes
*******************************************************************************
******************************************************************************/

static void pktFromMsgAsync(uint8_t const *pMsgFromSAPHandler,
                            ztcSAPHandlerInfo_t const *pSrcSAPInfo,
                            ztcMsgTypeInfo_t const *pSrcMsgTypeInfo);

static void pktFromMsgSync(uint8_t const *pMsgFromSAPHandler,
                           ztcSAPHandlerInfo_t const *pSrcSAPInfo,
                           ztcMsgTypeInfo_t const *pSrcMsgTypeInfo,
                           uint8_t const srcStatus);

static void ZtcReq_Handler(void);

static void CheckZtcBufferStatus(void); 

static void ZtcApsde_Handler(void);

#if (gLpmIncluded_d == 1)
static void ZtcMsgWakeUpInd(void);
#endif /* #if gLpmIncluded_d */ 

/******************************************************************************
*******************************************************************************
* Private memory declarations
*******************************************************************************
******************************************************************************/

/* fragmentation fields */
extern uint8_t giFragmentedPreAcked;
/* Variable to store the BlackBox configuration parameters */
extern const blackBoxCommConfig_t blackBoxCommConfig;
/* Used to recognize monitor copies of messages that Ztc sent to other code. */
#if gZtcIncluded_d
static bool_t mMsgOriginIsZtc;
extern uint8_t gZDOJoinMode;
#endif

#if gZtcErrorReporting_d
/* Set to TRUE when ZtcError() is called. */
static bool_t mZtcErrorReported;
#endif


/******************************************************************************
*******************************************************************************
* Public memory definitions
*******************************************************************************
******************************************************************************/

/* The rest of BeeStack can set this pointer to allow handling of messages
 * from the external client that have opcode groups not normally
 * recognized by Ztc.
 */
#if gZtcIncluded_d
gpfTestClientToApplication_t gpfTestClientToApplicationFuncPtr = NULL;
#endif

#if gZtcIncluded_d
tsTaskID_t gZTCTaskID;
#endif

extern char __SEG_START_SSTACK[];
extern char __SEG_END_SSTACK[];
extern char __SEG_END_NV_CODE_BUFFER[];

/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
******************************************************************************/

#ifdef PROCESSOR_MC1323X   
  extern  void MacPhyInit_WriteExtAddress(uint8_t *pExtendedAddress);
#endif //PROCESSOR_MC1323X   
/* Main routine for the ZTC task.
 *
 * Ztc only understands one event, which signals that the UART/SPI ISR has received
 * an octet from a local communication port (e.g. serial port or USB).
 *
 * Note that no messages are ever sent to the Ztc task.
 *
 * The event flags are cleared by the kernel.
 *
 * Note that the ISR declares an event for every octet, so this routine is
 * often called before there is enough of a packet received from the external
 * client to process.
 *
 * Once a complete and well formed packet has arrived, copy to a SAP Handler
 * message, with whatever changes are needed to the format of the data, and
 * forward it to the SAP Handler.
 *
 * The SAP Handler will eventually call ZTC_TaskEventMonitor() to record the
 * SAP Handler's receipt of a message.
 */
void Ztc_Task(event_t events) {
#if !gZtcIncluded_d
  (void) events;
#else
  sapHandlerMsg_t *pMsgToSAPHandler;
  ztcMsgTypeInfo_t const *pMsgTypeInfo;
  ztcOpcodeGroup_t opcodeGroup;
  ztcSAPHandlerInfo_t const *pSAPInfo;
  ztcFmtInfo_t const *pFmtInfo;
  index_t dstDataLen;
  pZtcMsgFromPkt_t pCopyFunc;
  uint8_t status;

  (void) events;

  /* Do we have a complete packet? */
  if (!ZtcReadPacketFromClient()) {
    CheckZtcBufferStatus();
    return;
  }

#if gZtcErrorReporting_d
  mZtcErrorReported = FALSE;
#endif

  opcodeGroup = gZtcPacketFromClient.structured.header.opcodeGroup;

  /* *warning* ZtcReq_Handler() depends on it's input data being in the */
  /* global gZtcPacketFromClient, and it modifies gZtcPacketToClient. */
  if (opcodeGroup == gZtcReqOpcodeGroup_c) {
    ZtcReq_Handler();
    CheckZtcBufferStatus();
    return;
  }

  /* special fragmented APSDE commands */
#if (gFragmentationCapability_d)
  if (opcodeGroup == gAfApsdeOpcodeGroup_c && 
      gZtcPacketFromClient.structured.header.msgType != gApsdeDataReqMsgType_d)
  {
    ZtcApsde_Handler();
    CheckZtcBufferStatus();
    return;
  }
#endif

  pSAPInfo = pZtcSAPInfoFromOpcodeGroup(opcodeGroup);

  /* If this is a recognized opcode group, but one which has been disabled */
  /* by an option setting, let the user know. Otherwise, Test Tool users */
  /* may wonder why they are not getting any responses to valid requests. */
#if gZtcErrorReporting_d
  if (pSAPInfo == gZtcIsDisabledOpcodeGroup_c) {
    ZtcError(gZtcOpcodeGroupIsDisabled_c);
    CheckZtcBufferStatus();
    return;
  }
#endif

  /* If the opcode group is not familiar, call a general purpose hook. */
  if (!pSAPInfo) {
    if (gpfTestClientToApplicationFuncPtr) {
      (*gpfTestClientToApplicationFuncPtr)((void *) gZtcPacketFromClient.raw);
    } else {
      ZtcError(gZtcUnknownOpcodeGroup_c);
    }
    CheckZtcBufferStatus();
    return;
  }

  /* The client has sent a well formed packet, with a known opcode group. */
  pMsgTypeInfo = pZtcMsgTypeInfoFromMsgType(pSAPInfo->pMsgTypeTable,
                                            *(pSAPInfo->pMsgTypeTableLen),
                                            gZtcPacketFromClient.structured.header.msgType);
  if (!pMsgTypeInfo) {
    ZtcError(gZtcUnknownOpcode_c);
    CheckZtcBufferStatus();
    return;
  }

  /* The size of the destination message isn't known yet, so allocate */
  /* a large message. */
  pMsgToSAPHandler = (sapHandlerMsg_t *) MSG_Alloc(sizeof(sapHandlerMsg_t));

  if (!pMsgToSAPHandler) {
    ZtcError(gZtcOutOfMessages_c);
    CheckZtcBufferStatus();
    return;
  }

  /* Copy the content of the packet to the SAP Handler message, changing */
  /* formats as necessary. */
  pFmtInfo = &(maZtcFmtInfoTable[ZtcMsgGetFmt(pMsgTypeInfo->flagAndFormat)]);
  pCopyFunc = maMsgFromPktFunctionTable[ZtcGetMsgFromPktFuncIndex(pFmtInfo->copyFunctionIndexes)];

  dstDataLen = (*pCopyFunc)(pMsgToSAPHandler->structured.payload,
                            sizeof(pMsgToSAPHandler->structured.payload),
                            gZtcPacketFromClient.structured.payload,
                            gZtcPacketFromClient.structured.header.len,
                            pFmtInfo->msgFromPktParametersIndex);

  if (dstDataLen == gTooBig_c) {
    MSG_Free(pMsgToSAPHandler);
    ZtcError(gZtcTooBig_c);
    CheckZtcBufferStatus();
    return;
  }

  /* mMsgOriginIsZtc lets ZTC_TaskEventMonitor() know that this message */
  /* originated with Ztc. */
  pMsgToSAPHandler->structured.header.msgType = gZtcPacketFromClient.structured.header.msgType;
  mMsgOriginIsZtc = TRUE;
  status = (pSAPInfo->pSAPHandler)(pMsgToSAPHandler);
  mMsgOriginIsZtc = FALSE;

  /* SAP Handlers don't free buffers that contain synchronous messages. */
  if (ZtcMsgGetFlg(pMsgTypeInfo->flagAndFormat) == gZtcMsgFlagSync_c) {
    #if gSAPMessagesEnableMlme_d
      /* For a MLME.Reset request synchronous message, the memory pool is re-initiated and
         all allocated messages are deallocated. Because of this, we don't need 
         to free the allocated message. */
      if ((opcodeGroup != gNwkMlmeOpcodeGroup_c)||(pMsgTypeInfo->msgType != gMlmeResetReq_c)) {
    #endif
        MSG_Free(pMsgToSAPHandler);
    #if gSAPMessagesEnableMlme_d
      }
    #endif
  } else if (status != gZbSuccess_c) {
    /* This is an async message, and the SAP Handler is unhappy. Synthesize */
    /* a confirmation message containing the status, and send it back to the */
    /* external client. This is the SAP Handler's issue, not a ZtcError(). */
    
    ZTCQueue_QueueToTestClient(&status, pSAPInfo->converseOpcodeGroup, pMsgTypeInfo->cnfType, sizeof(status));
  }
  CheckZtcBufferStatus();
#endif                                  /* #if !gZtcIncluded_d #else */
}                                       /* Ztc_Task() */

/****************************************************************************/

/* Initialization routine for the ZTC task. Called once by the kernel at */
/* startup. */
#if gZtcIncluded_d
void ZtcRxCallBack(void) {
  SendEventToZTC();
}
#endif

void Ztc_TaskInit(void) {
#if gZtcIncluded_d
  
  #if defined(gSPIInterface_d)
  
    spiConfig_t mSpiConfig;
       
    /* Initialize the SPI module */
    SPI_Init();
    
    /* Prepare the SPI module in slave mode */
	  /* SPI_SCI bridge has the same configuration in master mode*/
    mSpiConfig.devMode      = gSPI_SlaveMode_c;
    mSpiConfig.baudRate     = blackBoxCommConfig.spiBaudRate;
    mSpiConfig.clockPol     = gSPI_ActiveHighPolarity_c;
    mSpiConfig.clockPhase   = gSPI_EvenEdgeShifting_c;
    mSpiConfig.bitwiseShift = gSPI_MsbFirst_c;
    mSpiConfig.s3Wire       = gSPI_FullDuplex_c;
    
    /* Apply configuration to the SPI module */
    SPI_SetConfig(mSpiConfig);
	
	  /* Set call back */  
    SPI_SetSlaveRxCallBack(ZtcRxCallBack);
         
  #elif defined(gIICInterface_d)
  
    IIC_ModuleInit();
    
    (void)IIC_SetBaudRate(blackBoxCommConfig.iicBaudRate);
    
    (void)IIC_SetSlaveAddress(blackBoxCommConfig.iicSlaveAddress);
    
    IIC_SetSlaveRxCallBack(ZtcRxCallBack);
   
  #else
  
    UartX_SetBaud(blackBoxCommConfig.uartBaudRate);
    
    UartX_SetRxCallBack(ZtcRxCallBack);
    
  #endif

  gZTCTaskID = TS_CreateTask(gTsZTCTaskPriority_c, Ztc_Task);
#endif
}                                       /* Ztc_TaskInit() */

/****************************************************************************/

/* Send an arbitrary event from an application to the external client. The
 * data, opcode group and message type can be anything.
 */
void ZTCQueue_QueueToTestClient
  (
  uint8_t const *const pData,
  uint8_t const opcodeGroup,
  uint8_t const msgType,
  uint8_t const dataLen
  )
{
#if !gZtcIncluded_d
  (void) pData;
  (void) opcodeGroup;
  (void) msgType;
  (void) dataLen;
#else

#if gZtcErrorReporting_d
  mZtcErrorReported = FALSE;
#endif

  if (dataLen > sizeof(gZtcPacketToClient.structured.payload)) {
    ZtcError(gZtcAppMsgTooBig_c);
    return;
  }

  if (mZtcPacketToClientIsBusy) {
    return;
  }

  gZtcPacketToClient.structured.header.opcodeGroup = opcodeGroup;
  gZtcPacketToClient.structured.header.msgType = msgType;
  gZtcPacketToClient.structured.header.len = dataLen;

  FLib_MemCpy(gZtcPacketToClient.structured.payload, (void *) pData, dataLen);
  ZtcWritePacketToClient(sizeof(gZtcPacketToClient.structured.header) + dataLen);
#endif                                  /* #if !gZtcIncluded_d #else */
}                                       /* ZTCQueue_QueueToTestClient() */

/****************************************************************************/

/* Called by other functions in BeeStack, generally SAP Handlers.
 * A message is passing through the SAP Handler, and it is letting us know.
 * A SAP Handler can receive a message from a BeeStack layer, or from Ztc.
 * The SAP Handler doesn't know the origin, and calls this function
 * regardless.
 *
 * If the message is async and it was sent to the SAP Handler by Ztc,
 * ignore it.
 *
 * If the message is async and it was not sent to the SAP Handler by Ztc,
 * forward a copy to the external client.
 *
 * Sync messages are always responses to requests. The origin of the request
 * (Ztc or other BeeStack code) does not matter; this routine will only see
 * the response, not the request.
 * Alter the opcode group to make the message look like it is coming from
 * the converse of the SAP Handler that actually sent it here, and change
 * the message type to the type of the corresponding response. Forward the
 * edited message to the external client.
 *
 * Note that the message from the SAP Handler is sent to the external client
 * directly from here, eliminating the need for logic to queue more than
 * one packet.
 */
void ZTC_TaskEventMonitor
  (
  ztcIntSAPId_t const srcIntSAPId,
  uint8_t *pMsgFromSAPHandler,
  clientPacketStatus_t const srcStatus
  )
{
#if (!gZtcIncluded_d || defined(gZtcOtapSupport_d))
  (void) srcIntSAPId;
  (void) pMsgFromSAPHandler;
  (void) srcStatus;
#else
  ztcMsgTypeInfo_t const *pSrcMsgTypeInfo;
  ztcSAPHandlerInfo_t const *pSrcSAPInfo;

#if gZtcErrorReporting_d
  /* Sync messages might be originated by Ztc_Task(), which already set this */
  /* once. Setting it again here could lead to reporting two errors. Since */
  /* are likely to be errors in different contexts, this isn't generally a */
  /* problem. */
  mZtcErrorReported = FALSE;
#endif

  pSrcSAPInfo = pZtcSAPInfoFromIntSAPId(srcIntSAPId);
  if (!pSrcSAPInfo) {
    /* Calling ZtcError() here might just annoy the user, repeatedly */
    /* sending an error message about something they already know. */
    return;
  }

  ZtcPrint(FALSE, &srcStatus, sizeof(srcStatus));
  ZtcPrint(FALSE, &pMsgFromSAPHandler, sizeof(pMsgFromSAPHandler));
  ZtcPrint(FALSE, &pSrcSAPInfo->opcodeGroup, sizeof(pSrcSAPInfo->opcodeGroup));
  ZtcPrint(TRUE,  pMsgFromSAPHandler, 32);

  if (maZtcSAPModeTable[pSrcSAPInfo->modeIndex] == gZtcSAPModeDisable_c) {
    return;
  }

  pSrcMsgTypeInfo = pZtcMsgTypeInfoFromMsgType(pSrcSAPInfo->pMsgTypeTable,
                                               *pSrcSAPInfo->pMsgTypeTableLen,
                                               ((sapHandlerMsg_t *) pMsgFromSAPHandler)->structured.header.msgType);
  if (!pSrcMsgTypeInfo) {
    /* Calling ZtcError() here might just annoy the user, repeatedly */
    /* sending an error message about something they already know. */
    return;
  }

  if (mZtcPacketToClientIsBusy) {
    return;
  }

  if (ZtcMsgGetFlg(pSrcMsgTypeInfo->flagAndFormat) == gZtcMsgFlagAsync_c) {
    pktFromMsgAsync(pMsgFromSAPHandler, pSrcSAPInfo, pSrcMsgTypeInfo);
  } else {
    pktFromMsgSync(pMsgFromSAPHandler, pSrcSAPInfo, pSrcMsgTypeInfo, srcStatus);
  }
#endif  /* #if !gZtcIncluded_d #else */
}  /* ZTC_TaskEventMonitor() */

/****************************************************************************/

#if gZtcIncluded_d
void pktFromMsgAsync
(
	uint8_t const *pMsgFromSAPHandler,
	ztcSAPHandlerInfo_t const *pSrcSAPInfo,
	ztcMsgTypeInfo_t const *pSrcMsgTypeInfo
)
{
	index_t dstDataLen;
	pZtcPktFromMsg_t pCopyFunc;
	ztcFmtInfo_t const *pFmtInfo;
	nwkMessage_t *pMsg=NULL; /* MAC Confirm/Indication data type */

	/* If this message is originated in Ztc, ignore it. */
	if (mMsgOriginIsZtc)
		return;

 	(void)pMsg; /* Set stub the pointer used for the Queue */
  	pFmtInfo = &(maZtcFmtInfoTable[ZtcMsgGetFmt(pSrcMsgTypeInfo->flagAndFormat)]);
	  pCopyFunc = maPktFromMsgFunctionTable[ZtcGetPktFromMsgFuncIndex(pFmtInfo->copyFunctionIndexes)];

	  /* Copy the payload from the message to the client packet. */
	  dstDataLen = (*pCopyFunc)(gZtcPacketToClient.structured.payload,
		  												sizeof(gZtcPacketToClient.structured.payload),
			  											((sapHandlerMsg_t *) pMsgFromSAPHandler)->structured.payload,
				  										pSrcMsgTypeInfo->len,
					  									pFmtInfo->pktFromMsgParametersIndex);  

	/* If the SAP Handler that sent the message here is in hook mode, dequeue */
	/* the message and clear the event to prevent the message from reaching the */
	/* destination SAP Handler. */
	if (maZtcSAPModeTable[pSrcSAPInfo->modeIndex] == gZtcSAPModeHook_c)
	{
		if (pSrcSAPInfo->pMsgQueue)
		{
			/* If there is a Message in the Queue, then pint to it */
			pMsg = MSG_DeQueue(pSrcSAPInfo->pMsgQueue);
 
			/* The MAC Layer has some exceptions in some message that need to free some internal memory */
			if (pSrcSAPInfo->opcodeGroup == gMlmeNwkOpcodeGroup_c) /* If the message came from MAC layer */
			{
				if (pSrcMsgTypeInfo->msgType == gNwkScanCnf_c) /* If the messaage is a ScanRequest */
				{
					if (pMsg->msgData.scanCnf.resList.pPanDescriptorBlocks->descriptorList) /* If the internal list of the scan has somem data or point to some place */
						/* Free the internal list comming in the the Scan from the MAC (second block) */
						if(pMsg->msgData.scanCnf.resList.pPanDescriptorBlocks->pNext)
						  MSG_Free(pMsg->msgData.scanCnf.resList.pPanDescriptorBlocks->pNext);
						/* Free the internal list comming in the the Scan from the MAC (first block) */
						MSG_Free(pMsg->msgData.scanCnf.resList.pPanDescriptorBlocks);
				}
				if (pSrcMsgTypeInfo->msgType == gNwkBeaconNotifyInd_c) /* If The messge is a Beacon Notify Indiction it has  messge allocated inside the message already allocted */
				{
					if (pMsg->msgData.beaconNotifyInd.pBufferRoot)
						MSG_Free(pMsg->msgData.beaconNotifyInd.pBufferRoot);
				}
			}
		}

		if (pSrcSAPInfo->pTaskID[0]) /* Take the message out of the Queue for no further processing */
		{
			TS_ClearEvent(pSrcSAPInfo->pTaskID[0], pSrcSAPInfo->msgEvent);
		}
	}

	if (dstDataLen == gTooBig_c)
	{
		ZtcError(gZtcTooBig_c);
		return;
	}

	gZtcPacketToClient.structured.header.opcodeGroup = pSrcSAPInfo->opcodeGroup;
	gZtcPacketToClient.structured.header.msgType = ((sapHandlerMsg_t *) pMsgFromSAPHandler)->structured.header.msgType;
	gZtcPacketToClient.structured.header.len = dstDataLen;

	ZtcWritePacketToClient(sizeof(gZtcPacketToClient.structured.header) + dstDataLen);
	if (pMsg && (maZtcSAPModeTable[pSrcSAPInfo->modeIndex] == gZtcSAPModeHook_c))
		MSG_Free(pMsg); /* Free the whole message comming from the Queue */
}  /* pktFromMsgAsync() */
#endif

/****************************************************************************/

/* Handle a synchronous message from a SAP Handler.
 * Sync messages are always requests. Translate this message into the
 * corresponding response.
 *
 * By convention, the request message type info contains the format
 * information.
 */
#if gZtcIncluded_d
void pktFromMsgSync
(
	uint8_t const *pMsgFromSAPHandler,
	ztcSAPHandlerInfo_t const *pSrcSAPInfo,
	ztcMsgTypeInfo_t const *pSrcMsgTypeInfo,
	uint8_t const srcStatus
)
{
	index_t dstDataLen;
	pZtcPktFromMsg_t pCopyFunc;
	ztcFmtInfo_t const *pFmtInfo;
	uint8_t *pSrcData;

	/* For sync messages, a status code was passed here as a parameter, */
	/* and must be stored in the first byte of the packet payload. */
	gZtcPacketToClient.structured.payload[0] = srcStatus;
	pSrcData = ((sapHandlerMsg_t *) pMsgFromSAPHandler)->structured.payload;

	pFmtInfo = &(maZtcFmtInfoTable[ZtcMsgGetFmt(pSrcMsgTypeInfo->flagAndFormat)]);
	pCopyFunc = maPktFromMsgFunctionTable[ZtcGetPktFromMsgFuncIndex(pFmtInfo->copyFunctionIndexes)];

	/* Copy the payload from the message to the client packet. */
	dstDataLen = (*pCopyFunc)(&gZtcPacketToClient.structured.payload[sizeof(clientPacketStatus_t)],
														sizeof(gZtcPacketToClient.structured.payload) - sizeof(clientPacketStatus_t),
														pSrcData,
														pSrcMsgTypeInfo->len,
														pFmtInfo->pktFromMsgParametersIndex);

	if (dstDataLen == gTooBig_c)
	{
		ZtcError(gZtcTooBig_c);
		return;
	}

	gZtcPacketToClient.structured.header.opcodeGroup = pSrcSAPInfo->converseOpcodeGroup;
	gZtcPacketToClient.structured.header.msgType = pSrcMsgTypeInfo->cnfType;
	gZtcPacketToClient.structured.header.len = dstDataLen + sizeof(clientPacketStatus_t);

	ZtcWritePacketToClient(dstDataLen
												+ sizeof(gZtcPacketToClient.structured.header)
												+ sizeof(clientPacketStatus_t));
}  /* pktFromMsgSync() */
#endif

/******************************************************************************
*******************************************************************************
* Private functions
*******************************************************************************
******************************************************************************/
/****************************************************************************
This function checks if there is any bytes in the uart/spi buffer, and 
if there is any left, the ZTC task is signaled.
****************************************************************************/
#if gZtcIncluded_d
static void CheckZtcBufferStatus(void) {
  
  #if defined(gSPIInterface_d)
    if (mSpiRxBufferByteCount != 0)
  #elif defined(gIICInterface_d)
    if (mIIcRxBufferByteCount != 0)
  #else
    if (UartX_RxBufferByteCount != 0)
  #endif
  
  {  
    SendEventToZTC();    
  }
}
#endif 

/* Send an error message back to the external client. */
#if gZtcErrorReporting_d
void ZtcError(uint8_t errorCode)
{
  uint8_t buf[MbrSizeof(clientPacket_t, headerAndStatus)];

  if (mZtcErrorReported)
    return;                             /* Don't cascade error messages. */


  ((clientPacket_t *) buf)->headerAndStatus.header.opcodeGroup = gZtcCnfOpcodeGroup_c;
  ((clientPacket_t *) buf)->headerAndStatus.header.msgType = mZtcMsgError_c;
  ((clientPacket_t *) buf)->headerAndStatus.header.len = sizeof(((clientPacket_t *) buf)->headerAndStatus.status);
  ((clientPacket_t *) buf)->headerAndStatus.status = errorCode;

  ZtcTransmitBufferBlock(buf, sizeof(buf));
  mZtcErrorReported = TRUE;
}
#endif                                  /* #if gZtcErrorReporting_d */

/****************************************************************************/

/* Ztc's version of printf() debugging.
 *
 * To save RAM, use a dynamically allocated message buffer to construct
 * the message. If no buffer is available, send an error buffer and give up.
 *
 * Write as much of the requested data as possible to the buffer, but there
 * is no guarantee there the buffer will be big enough.
 *
 * Concatenate the source data to the print buffer, then
 *  If readyToSend is false, do nothing.
 *  If readyToSend is true,  send the buffer to the external client.
 */
#if gZtcDebug_d
void ZtcPrint
  (
  bool_t readyToSend,
  void const *pSrc,
  index_t len
  )
{
  static index_t bufLen = 0;
  static index_t dataInBuf = 0;
  static uint8_t *pBuf = NULL;

  /* Search for a valid and available message buffer size, instead of */
  /* depending on knowing the possible buffer sizes. */
#define ztcPrintDecrement       8       /* Ensure decrementing to zero. */
#define ztcPrintNumberOfTries   8
  if (!bufLen) {
    for (pBuf = 0, bufLen = ztcPrintDecrement * ztcPrintNumberOfTries;
         !pBuf && bufLen;
         bufLen -= ztcPrintDecrement) {
      pBuf = MSG_Alloc(bufLen);
      dataInBuf = sizeof(clientPacketHdr_t);
    }

    if (!bufLen) {
      ZtcError(gZtcDebugPrintFailed_c);
      return;
    }
  }

  /* Concatenate the source data to the print buffer, if there's room. */
  if (dataInBuf + len < bufLen) {
    FLib_MemCpy(pBuf + dataInBuf, pSrc, len);
    dataInBuf += len;
  }

  if (readyToSend) {
    ((clientPacketHdr_t *) pBuf)->opcodeGroup = gZtcCnfOpcodeGroup_c;
    ((clientPacketHdr_t *) pBuf)->msgType = mZtcMsgDebugPrint_c;
    ((clientPacketHdr_t *) pBuf)->len = dataInBuf - sizeof(clientPacketHdr_t);
    ZtcTransmitBufferBlock(pBuf, dataInBuf);
    MSG_Free(pBuf);
    bufLen = 0;
  }
}
#endif                                  /* #if gZtcDebug_d */


/*
  Called from both AF_APSDE_SapHandler() and APSDE_AF_SapHandler(), this function will 
  display all fragmented blocks to ZTC if both fragmentation and ZTC are enabled. 
*/
void ZTC_FragEventMonitor
  (
  ztcIntSAPId_t const srcIntSAPId,
  uint8_t *pMsgFromSAPHandler
  )
{
#if (!gZtcIncluded_d)
  /* avoid compiler warning on unused parameters */
  (void)srcIntSAPId;
  (void)pMsgFromSAPHandler;

/* ZTC is included */
#else

 #if (gFragmentationCapability_d)
  zbTxFragmentedHdr_t *pFragHdr;
  apsdeFragment_t sZtcFragment;
  uint8_t *pData;
  uint8_t msgType;
  uint8_t dataSize;
  uint8_t opCodeGroup;
  uint8_t iBlockNumber;
 #endif

  /* first, send the main message to ZTC */
  ZTC_TaskEventMonitor(srcIntSAPId, (uint8_t *) pMsgFromSAPHandler, gZbSuccess_c);

 #if (gFragmentationCapability_d)

  /* find fragHdr for this message */
  /* the message will either come from the gAF_APSDE_SAPHandlerId_c or the gAPSDE_AF_SAPHandlerId_c */
  /* note: this code assumes both zbTxFragmentedHdr_t and zbRxFragmentedHdr_t begin with same 2 fields */
  msgType = *pMsgFromSAPHandler;
  if(srcIntSAPId == gAF_APSDE_SAPHandlerId_c)
  {
    /* make sure message is a data request */
    if(msgType != gApsdeDataReqMsgType_d)
      return;

    /* find first fragment header */
    pFragHdr = &(((afToApsdeMessage_t *)pMsgFromSAPHandler)->msgData.dataReq.fragmentHdr);
    opCodeGroup = 0x9c;
  }

  /* assumes srcIntSAPId == gAPSDE_AF_SAPHandlerId_c */
  else
  {
    /* make sure message is a data indication */
    if(msgType != gApsdeDataIndMsgType_d)
      return;

    /* find first fragment header */
    pFragHdr = (zbTxFragmentedHdr_t *)(&(((apsdeToAfMessage_t *)pMsgFromSAPHandler)->msgData.dataIndication.fragmentHdr));
    opCodeGroup = 0x9d;
  }

  /* skip first block (with dataReq or dataInd), as that's already been sent to ZTC */
  pFragHdr = pFragHdr->pNextDataBlock;

  /* send all subsequent blocks to ZTC */
  iBlockNumber = 1;
  while(pFragHdr)
  {
    /* copy data into ZTC Tx buffer */
    dataSize = pFragHdr->iDataSize;
    gZtcPacketToClient.structured.header.opcodeGroup = opCodeGroup;
    gZtcPacketToClient.structured.header.msgType = pFragHdr->iMsgType;
    gZtcPacketToClient.structured.header.len = (sizeof(apsdeFragment_t) - 1) + dataSize;
    sZtcFragment.iLength = dataSize;
    sZtcFragment.iBlockNumber = iBlockNumber++;
    sZtcFragment.iMoreBlocks = pFragHdr->pNextDataBlock ? TRUE : FALSE;
    gZtcPacketToClient.structured.payload[0] = dataSize;
    if(srcIntSAPId == gAF_APSDE_SAPHandlerId_c)
      pData = (uint8_t *)(pFragHdr + 1);    /* on Tx, data immediately follows TxFragHdr */
    else
      pData = ((zbRxFragmentedHdr_t *)pFragHdr)->pData; /* on Rx, it's pointed to by RxFragHdr */
    FLib_MemCpy(gZtcPacketToClient.structured.payload, &sZtcFragment, sizeof(apsdeFragment_t) - 1);
    FLib_MemCpy(&gZtcPacketToClient.structured.payload[sizeof(apsdeFragment_t) - 1], pData, dataSize);

    /* send through ZTC */
	  ZtcWritePacketToClient(sizeof(gZtcPacketToClient.structured.header) + 
	    (sizeof(apsdeFragment_t) - 1) + dataSize);

    /* go on to next block */
    pFragHdr = pFragHdr->pNextDataBlock;
  }
 #endif /* gFragmentationCapability_d */
#endif  /* gZtcIncluded_d */
}

#if (gZtcIncluded_d && gFragmentationCapability_d)

/* for loading fragments prior to a data request */
static zbTxFragmentedHdr_t *mpFragHead;  /* if NULL, then no fragments loaded */

/*
  For handling the "special" APSDE commands for fragmentation. These do not go to the SAP 
  handler.

  ResetFragments   Free any memory allocated by LoadFragment. Only required if aborting a
                   Data request, or if
  LoadFragment     Load a fragment into memory through the ZTC interface.
  SetFragAckMask   Used for passing ZCP tests. Pretend a packet has already been ACKed so
                   it's not sent first time.
    
  Normal sequence for initiating an APSDE-DATA.request over ZTC is:
  1. Call LoadFragment (for all fragments but the first one. These fragments may be any length, 
     but should be the max size that will fit in a message (128 bytes) 
  2. Call APSDE-DATA.request for the first fragment. AsduLen should be the length of the first
     fragment only. ZTC will add the total size of all fragments.

  Note: the above sequence is different than from an application. The AF_DataRequestFragmented()
  and AF_DataRequestFragmentedNoCopy() should be called instead.

  Note: ZtcApsde_Handler() receives data in the global gZtcPacketFromClient, and modifies 
  gZtcPacketToClient.
*/
static void ZtcApsde_Handler(void)
{

  /* these commands do not actually go through the SAP, but are valid only in ZTC */
  #define gZtcMsgTypeResetFragments_c       0x10
  #define gZtcMsgTypeLoadFragment_c         0x11
  #define gZtcMsgTypeSetFragAckMask_c       0x12
  #define gZtcMsgTypeSetMaxFragmentLength_c 0x13
  #define gZtcMsgTypeSetWindowSize_c        0x14
  #define gZtcMsgTypeSetInterframeDelay_c   0x15

  zbTxFragmentedHdr_t *pFragBlock;    /* allocated message */
  zbTxFragmentedHdr_t *pFragNext;     /* next fragmented buffer */
  uint8_t fragLen;
  ztcMsgType_t msgType;
  clientPacketStatus_t ztcError = gZtcSuccess_c;

  msgType = gZtcPacketFromClient.structured.header.msgType;
  switch(msgType)
  {
    /* ResetFragments command. Will free any memory allocated with LoadFragment command. */
    case gZtcMsgTypeResetFragments_c:

      /* free every buffer in the linked list */
      while(mpFragHead)
      {
        pFragNext = mpFragHead->pNextDataBlock;
        MSG_Free(mpFragHead);
        mpFragHead = pFragNext;
      }
      /* note: mpFragHead will end up NULL */
    break;

    /* LoadFragment command */
    case gZtcMsgTypeLoadFragment_c:

      /* allocate a message buffer for the fragment */
      fragLen = gZtcPacketFromClient.structured.header.len - 1;
      pFragBlock = MSG_Alloc(sizeof(zbTxFragmentedHdr_t) + fragLen);
      if(!pFragBlock)
      {
        ztcError = gZtcOutOfMessages_c;
        break;
      }

      /* find tail, and link in allocated buffer at end */
      if(!mpFragHead)
        mpFragHead = pFragBlock;
      else
      {
        /* find tail */
        pFragNext = mpFragHead;
        while(pFragNext->pNextDataBlock)
          pFragNext = pFragNext->pNextDataBlock;
        pFragNext->pNextDataBlock = pFragBlock;
      }

      /* set up allocated buffer */
      pFragBlock->iDataSize = fragLen;
      pFragBlock->iMsgType = gApsdeDataReqFragMsgType_d;
      pFragBlock->pNextDataBlock = NULL;

      /* copy actual octets (they immediately follow the header) */
      FLib_MemCpy((uint8_t *)pFragBlock + sizeof(zbTxFragmentedHdr_t), 
          &(gZtcPacketFromClient.structured.payload[1]), fragLen);
    break;

    /* set the fragmentation mask (for debugging and ZCP testing) */
    case gZtcMsgTypeSetFragAckMask_c:
      giFragmentedPreAcked = *gZtcPacketFromClient.structured.payload;
    break;

    /* set the maximum OTA fragment length */
    case gZtcMsgTypeSetMaxFragmentLength_c:
      ApsmeSetRequest(gApsMaxFragmentLength_c, *gZtcPacketFromClient.structured.payload);
    break;

    /* set the window size (1-8) */
    case gZtcMsgTypeSetWindowSize_c:
      ApsmeSetRequest(gApsMaxWindowSize_c, *gZtcPacketFromClient.structured.payload);
    break;

    /* set the interframe delay 1-255 milliseconds */
    case gZtcMsgTypeSetInterframeDelay_c:
      ApsmeSetRequest(gApsInterframeDelay_c, *gZtcPacketFromClient.structured.payload);
    break;

    /* unknown opcode (aka msgType) */
    default:
      ztcError = gZtcUnknownOpcode_c;
    break;
  }

  /* send response */
  gZtcPacketToClient.structured.header.opcodeGroup = gApsdeAfOpcodeGroup_c;
  gZtcPacketToClient.structured.header.msgType = msgType;
  gZtcPacketToClient.structured.header.len = sizeof(clientPacketStatus_t);
  gZtcPacketToClient.structured.payload[0] = ztcError;
  ZtcWritePacketToClient(sizeof(gZtcPacketToClient.structured.header)
                               + gZtcPacketToClient.structured.header.len);

}
#endif // #if (gZtcIncluded_d && gFragmentationCapability_d)


#if gZtcIncluded_d

#if gBeeStackIncluded_d
/*
  Called on inbound APSDE data requests

  This function both services APSDE-DATA.requests
*/
index_t ZtcMsgFromPktApsde
  (
  uint8_t *pMsg,                        /* Pointer to message payload (just after msgType byte) */
  index_t msgLen,                       /* Length  of message payload */
  uint8_t *pPkt,                        /* Pointer to packet  payload */
  index_t pktLen,                       /* Length  of packet  payload */
  index_t fmtParametersIndex
  )
{
  zbApsdeDataReq_t *pDataReq;
  uint8_t asduLen;

  /* unused parameters, mark so no compiler warnings */
  (void)msgLen;
  (void)fmtParametersIndex;

  /* copy in 1st part header (up to asdu) */
  pDataReq = (void *)pMsg;
  FLib_MemCpy(pDataReq, pPkt, MbrOfs(zbApsdeDataReq_t, pAsdu));

  /* asdu too large? we're done */
  asduLen = pktLen - (sizeof(zbApsdeDataReq_t) - sizeof(zbTxFragmentedHdr_t) - sizeof(uint8_t *));
  pDataReq->asduLength = asduLen;

  /* copy in 2nd part of header */
  FLib_MemCpy(&pDataReq->txOptions, pPkt + MbrOfs(zbApsdeDataReq_t, pAsdu) + asduLen, 
    MbrOfs(zbApsdeDataReq_t, fragmentHdr) - MbrOfs(zbApsdeDataReq_t, txOptions));
  BeeUtilZeroMemory(&pDataReq->fragmentHdr, sizeof(pDataReq->fragmentHdr));

  /* copy in ASDU */
  pDataReq->pAsdu = (void *)((pMsg - 1) + ApsmeGetAsduOffset());
  FLib_MemCpy(pDataReq->pAsdu, pPkt + MbrOfs(zbApsdeDataReq_t, pAsdu), asduLen);

  /* set up fragmented part */
#if (gFragmentationCapability_d)
  pDataReq->fragmentHdr.pNextDataBlock = mpFragHead;
  pDataReq->fragmentHdr.iDataSize = asduLen;
  pDataReq->fragmentHdr.iMsgType = gApsdeDataReqMsgType_d;

  /* mark frag head as NULL for next data request */
  /* all buffers in linked list will be freed by APSDE-DATA.request itself */
  mpFragHead = NULL;
#else
  pDataReq->fragmentHdr.pNextDataBlock = NULL;
  pDataReq->fragmentHdr.iDataSize = asduLen;
  pDataReq->fragmentHdr.iMsgType = gApsdeDataReqMsgType_d;
#endif

  return 1; // everything is OK
}
#endif /* #if gBeeStackIncluded_d */
#endif

/****************************************************************************/

/* Handle Ztc configuration commands/requests from the external client.
 * This code would be easier to read, but slower and larger, if each
 * case called a function instead of processing in-line.
 *
 * On arrival, the opcodeGroup in the pMsg has been changed to the opcode
 * group used for Ztc responses.
 *
 * The functions that actually handle the packets are responsible for the
 * contents of the return packet, except for the return packet's opcode
 * group. The message type of the return packet will be unchanged from the
 * request packet.
 *
 * *warning* ZtcReq_Handler() receives data in the global gZtcPacketFromClient,
 * and modifies gZtcPacketToClient.
 */
#if gZtcIncluded_d
void ZtcReq_Handler(void) {
  ztcMsgType_t msgType = gZtcPacketFromClient.structured.header.msgType;
  index_t i;

  if (mZtcPacketToClientIsBusy) {
    return;
  }

  for (i = 0; i < gZtcInternalMsgTypeInfoTableLen; ++i) {
    if (gaZtcInternalMsgTypeInfoTable[i].msgType == msgType) {

      /* Set default values. */
      gZtcPacketToClient.structured.header.opcodeGroup = gZtcCnfOpcodeGroup_c;
      gZtcPacketToClient.structured.header.msgType = msgType;
      gZtcPacketToClient.structured.header.len = sizeof(clientPacketStatus_t);
      gZtcPacketToClient.structured.payload[0] = gZtcSuccess_c;

      (*gaZtcInternalMsgTypeInfoTable[i].pConfigFunc)();

#if gZtcErrorReporting_d
      if (!mZtcErrorReported)
#endif
        ZtcWritePacketToClient(sizeof(gZtcPacketToClient.structured.header)
                               + gZtcPacketToClient.structured.header.len);

      return;
    }
  }

  ZtcError(gZtcUnknownOpcode_c);
}                                       /* ZtcReq_Handler() */
#endif

/****************************************************************************/

#if gZtcIncluded_d
void ZtcMsgGetModeReqFunc(void) {
  uint8_t i;
  index_t payloadIndex = sizeof(clientPacketStatus_t);

  gZtcPacketToClient.structured.payload[payloadIndex++] = (uint8_t) gZtcTxBlocking;

  for (i = 0; i < mZtcSAPModeTableLen; ++i) {
    gZtcPacketToClient.structured.payload[payloadIndex++] = (uint8_t) maZtcSAPModeTable[i];
  }

  gZtcPacketToClient.structured.header.len = payloadIndex;
}                                       /* ZtcMsgGetModeReqFunc() */
#endif


/****************************************************************************/

#if gZtcIncluded_d
void ZtcMsgGetNVDataSetDescReqFunc(void) {
  index_t payloadIndex = sizeof(clientPacketStatus_t);
#if !gNvStorageIncluded_d
  gZtcPacketToClient.structured.payload[payloadIndex++] = 0;
#else
  index_t dataSetIndex;
  NvDataItemDescription_t const *pDesc;
  uint16_t size;

  gZtcPacketToClient.structured.payload[payloadIndex++] = NumberOfElements(NvDataSetTable);

  for (dataSetIndex = 0; dataSetIndex < NumberOfElements(NvDataSetTable); ++dataSetIndex) {
    gZtcPacketToClient.structured.payload[payloadIndex++] = NvDataSetTable[dataSetIndex].dataSetID;

    pDesc = NvDataSetTable[dataSetIndex].pItemDescriptions;
    size = 0;
    while (pDesc->length) {
      size += pDesc++->length;
    }

    gZtcPacketToClient.structured.payload[payloadIndex++] = (size & 0xFF);
    gZtcPacketToClient.structured.payload[payloadIndex++] = (size >> 8);
  }

#endif
  gZtcPacketToClient.structured.header.len = payloadIndex;
}                                       /* ZtcMsgGetNVDataSetDescReqFunc() */
#endif

/****************************************************************************/

#if gZtcIncluded_d
void ZtcMsgGetNVPageHdrReqFunc(void) {
  index_t payloadIndex = sizeof(clientPacketStatus_t);
#if !gNvStorageIncluded_d
  gZtcPacketToClient.structured.payload[payloadIndex++] = 0;
#else
  index_t pageIndex;

  gZtcPacketToClient.structured.payload[payloadIndex++] = NumberOfElements(maNvRawPageAddressTable);

  for (pageIndex = 0; pageIndex < NumberOfElements(maNvRawPageAddressTable); ++pageIndex) {
    FLib_MemCpy((void *) &gZtcPacketToClient.structured.payload[payloadIndex],
                (void *) maNvRawPageAddressTable[pageIndex],
                sizeof(NvStructuredPageHeader_t));
    payloadIndex += sizeof(NvStructuredPageHeader_t);
  }
#endif
  gZtcPacketToClient.structured.header.len = payloadIndex;
}                                       /* ZtcMsgGetNVPageHdrReqFunc() */
#endif

/****************************************************************************/

/* Set various Ztc operating mode flags:
 *
 *  gZtcTxBlocking: if TRUE, the UART/SPI output routine will block until
 *  output is finished.
 *
 *  maZtcSAPModeTable[]: One byter per group of SAP Handlers, indicating
 *  what Ztc should do when a message from one of the given SAP Handlers
 *  arrives at ZTC_TaskEventMonitor():
 *      ignore messages from that SAP Handler, or
 *      forward monitor copies of messages from that SAP Handler to the
 *          external client, or
 *      intercept messages from that SAP Handle; forward them to the external
 *      client, and prevent them from being passed to their destination
 *      BeeStack layer.
 *  Note that these modes only affect messages that are sent from the SAP
 *  Handler to Ztc. These settings have no effect on packets sent from the
 *  external client to the SAP Handler.
 *
 * *warning* This is brittle. Ztc and the client must implicitly
 * agree on the order of the mode setting flags.
 */
#if gZtcIncluded_d
void ZtcMsgModeSelectReqFunc(void) {
  uint8_t byte;
  index_t count;
  index_t dataLen = gZtcPacketFromClient.structured.header.len;
  uint8_t i;
  index_t payloadIndex = 0;

  if (dataLen > 0) {
    gZtcTxBlocking = !!gZtcPacketFromClient.structured.payload[payloadIndex++];
  }

  dataLen -= sizeof(gZtcPacketFromClient.structured.payload[0]);

  count = (dataLen <= mZtcSAPModeTableLen ? dataLen : mZtcSAPModeTableLen);

  for (i = 0; i < count; ++i) {
    byte = gZtcPacketFromClient.structured.payload[payloadIndex + i];
    /* If support for these SAP Handlers hse by compiled out, ignore them. */
    if (maZtcSAPModeTable[i] != gZtcSAPModeInvalid_c) {
      maZtcSAPModeTable[i] = byte;
    }
  }
}                                       /* ZtcMsgModeSelectReqFunc() */
#endif

/****************************************************************************/

#if gZtcIncluded_d
void ZtcMsgNVSaveReqFunc(void) {
#if !gNvStorageIncluded_d
  ZtcError(gZtcRequestIsDisabled_c);
#else
  uint8_t dataSetIndex;

  dataSetIndex = gZtcPacketFromClient.structured.payload[0];
  if (dataSetIndex > NumberOfElements(NvDataSetTable)) {
    ZtcError(gZtcTooBig_c);
    return;
  }

  NvSaveOnIdle(NvDataSetTable[dataSetIndex].dataSetID);
#endif
}                                       /* ZtcMsgNVSaveReqFunc() */
#endif

/****************************************************************************/
#if gZtcIncluded_d
#if (gMsgInfo_d)
#include "msgsystem.h"
extern pools_t maMmPools[];

/******************************************************************************
* MM_DebugNumFreeBigBuffers
*
******************************************************************************/
uint8_t ZTC_DebugNumFreeBigBuffers
  (
  void
  )
{
  listHeader_t *pHead;
  uint8_t count = 0;
  pHead = maMmPools[1].anchor.pHead;
  while(pHead != NULL)   
  {
    pHead = pHead->pNext;
    count++;
  }
   
 return count;
}

/******************************************************************************
* MM_DebugNumFreeSmallBuffers
*
******************************************************************************/
uint8_t ZTC_DebugNumFreeSmallBuffers
  (
  void
  )
{
  listHeader_t *pHead;
  uint8_t count = 0;
  
  pHead = maMmPools[0].anchor.pHead;
  while(pHead != NULL)    
  {
    pHead = pHead->pNext;
    count++;
  }
  return count;
}

/******************************************************************************/
void ZtcMsgGetNumOfMsgsReqFunc(void) {
  gZtcPacketToClient.structured.payload[0] = ZTC_DebugNumFreeSmallBuffers();
  gZtcPacketToClient.structured.payload[1] = ZTC_DebugNumFreeBigBuffers();
  gZtcPacketToClient.structured.header.len = (sizeof(uint8_t) *2);
}
#endif //gMsgInfo_d

#endif
/****************************************************************************/

#if gZtcIncluded_d
void ZtcMsgReadExtendedAdrReqFunc(void) {
  FLib_MemCpy( &gZtcPacketToClient.structured.payload[sizeof(clientPacketStatus_t)],
       aExtendedAddress, 8 );
 
  gZtcPacketToClient.structured.header.len = sizeof(clientPacketStatus_t) + 8;
}                                         /* ZtcMsgReadExtendedAdrReqFunc() */

#endif

/****************************************************************************/

/* eliminate compiler warnings */
#if gZtcIncluded_d
uint8_t AF_AppToAfdeDataReq(void) {
  return gZbSuccess_c;
}

#endif

/****************************************************************************/

#if gZtcIncluded_d
void ZtcMsgResetCPUReqFunc(void) {

  /* We're about to reset the CPU. Turn off Uart/SPI */    
  Uart_ModuleUninit();
#if defined(gSPIInterface_d)

  SPI_Uninit();
  
#elif defined(gIICInterface_d)

  IIC_ModuleUninit();
  
#endif
  PWRLib_Reset();
}                                       /* ZtcMsgResetCPUReqFunc() */
#endif

/****************************************************************************/
#if gZtcIncluded_d
/* ZtcMsgWriteExtendedAdrReqFunc() */
void ZtcMsgWriteExtendedAdrReqFunc(void)
{
  FLib_MemCpy(aExtendedAddress, gZtcPacketFromClient.structured.payload,8);
#ifdef PROCESSOR_MC1323X
  MacPhyInit_WriteExtAddress(aExtendedAddress);
#endif //PROCESSOR_MC1323X   
}

#endif

/****************************************************************************/
#if (gSAPMessagesEnableNlme_d)
#if gZtcIncluded_d
void ZtcFreeDiscoveryTables(void) {
  NWK_FreeUpDiscoveryTables();
}                          

#endif
/****************************************************************************/

#if gZtcIncluded_d
void ZtcSetJoinFilterFlag(void) {
  gZDOJoinMode = gZtcPacketFromClient.structured.payload[0];
}                          
#endif
#endif
/****************************************************************************/

#if gUseHwValidation_c
/* Function used for writing to RAM memory.
 * gZtcPacketFromClient.structured.payload contains the packet received from TestTool:
 * bytes 0 to 1 --> start address for writing
 * byte    2    --> number of bytes to be written
 * bytes 3 to (3+ (2nd byte) - 1) --> data to be written starting with start address.  
 */
#if gZtcIncluded_d
void ZtcWriteMemoryBlock(void) 
{
  uint8_t *addr;
  uint8_t nBytes;
  uint8_t *pIn, *pOut;
  uint8_t nWritten, nRead;
  uint16_t auxAddr = 0;
  #define OffsetToSize   4  

  /* even if address is 4 bytes long, only the first 2 bytes received represent the effective address */
  FLib_MemCpy(&addr, gZtcPacketFromClient.structured.payload, sizeofPointer);
  
  #if gBigEndian_c
    auxAddr = Swap2Bytes((uint16_t)addr); 
    pOut = (uint8_t*)auxAddr;       
  #else
    pOut = addr; 
  #endif  

  /* number of bytes to write parameter is the 5th received byte */
  nBytes = gZtcPacketFromClient.structured.payload[OffsetToSize];
  /* data to write parameter starts with the 6th received byte */
  pIn = &gZtcPacketFromClient.structured.payload[OffsetToSize + 1];
  
  nRead = nWritten = 0;
  while (nRead++ < nBytes)
  {
  #ifndef PROCESSOR_QE128
  if (((uint16_t)pOut <= 0x107f)||(((uint16_t)pOut >= 0x1800)&&((uint16_t)pOut <= 0x182B)))/* Write to out of RAM range or I/O space attempted  */
  #else
  if ((uint16_t)pOut <= 0x207f)/* Write to out of RAM range or I/O space attempted  */ 
  #endif  
     {
        *pOut++ = *pIn++;
        nWritten++;
     }else{
        pOut++;
        pIn++;
     }
  }
  
  /* return the number of bytes written to RAM */
  gZtcPacketToClient.structured.payload[0] = nWritten;  
  
}
#endif
/****************************************************************************/

/* Function used for reading from memory.
 * gZtcPacketFromClient.structured.payload contains the packet received from TestTool:
 * bytes 0 to 1 --> start address for reading
 * byte    2    --> number of bytes to be read. 
 */
#if gZtcIncluded_d
void ZtcReadMemoryBlock(void) 
{
  uint8_t *addr;
  uint8_t nBytes, lcount;
  uint8_t *pIn, *pOut;
  uint16_t auxAddr = 0;
  #define OffsetToSize   4

  /* even if address is 4 bytes long, only the first 2 bytes received represent the effective address */
  FLib_MemCpy(&addr, gZtcPacketFromClient.structured.payload, sizeofPointer);
  
  #if gBigEndian_c
    auxAddr = Swap2Bytes((uint16_t)addr);
    pOut = (uint8_t*)auxAddr;        
  #else
    pOut = addr; 
  #endif  
  /* number of bytes to read parameter is the 5th received byte */
  nBytes = gZtcPacketFromClient.structured.payload[OffsetToSize];
  lcount = nBytes;
  /* Construct and send response */
  
  pIn = &gZtcPacketToClient.structured.payload[0];
  for (;lcount != 0; lcount--)
  {
    *pIn++ = *pOut++;
  }

  gZtcPacketToClient.structured.header.len = nBytes;   
  
}
#endif
/****************************************************************************/

/* Function used for writing 16 bit data to ABEL registers. 
 * gZtcPacketFromClient.structured.payload contains the packet received from TestTool:
 * byte 0       --> The address to be written (0x00-0x3F)
 * bytes 1 to 2 --> 16 bits value to be written 
 */
#if gZtcIncluded_d
#ifndef PROCESSOR_MC1323X
void ZtcAbelWrite(void) 
{
  uint16_t auxValue; 
  ztcAbelWrite_t* pAbelWrite = (ztcAbelWrite_t*)gZtcPacketFromClient.structured.payload;

  #if gBigEndian_c
    auxValue = Swap2Bytes((uint16_t)pAbelWrite->value);        
  #else
    auxValue = (uint16_t)pAbelWrite->value; 
  #endif
  MC1319xDrv_WriteSpiAsync(gZtcPacketFromClient.structured.payload[0], auxValue);
}
#endif /* PROCESSOR_MC1323X */
#endif
/****************************************************************************/

/* Function used for reading 16 bit data from ABEL registers. 
 * gZtcPacketFromClient.structured.payload contains the packet received from TestTool:
 * byte 0       --> The address to be read
 */
#if gZtcIncluded_d
#ifndef PROCESSOR_MC1323X
void ZtcAbelRead(void) 
{
  uint16_t auxValue, returnValue;
 
  MC1319xDrv_ReadSpiAsync(gZtcPacketFromClient.structured.payload[0], &auxValue);
  
  #if gBigEndian_c
    returnValue = Swap2Bytes(auxValue);        
  #else
    returnValue = auxValue; 
  #endif  

	gZtcPacketToClient.structured.header.len = sizeof(returnValue);
	/* return read value */
	FLib_MemCpy(gZtcPacketToClient.structured.payload, &returnValue, sizeof(returnValue));  
}
#endif
#endif
/****************************************************************************/

/*ZtcPing echoes back the payload*/
#if gZtcIncluded_d
void  ZtcPing(void)
{

  gZtcPacketToClient.structured.header.len = gZtcPacketFromClient.structured.header.len;
  FLib_MemCpy(gZtcPacketToClient.structured.payload, gZtcPacketFromClient.structured.payload,
              gZtcPacketFromClient.structured.header.len );

}
#endif

#endif /* gUseHwValidation_c */

/* Permissions Configuration Table */
#if (gZtcIncluded_d && (gStandardSecurity_d || gHighSecurity_d) && gApsMaxEntriesForPermissionsTable_c)

extern index_t GetPermissionsTable(permissionsTable_t * pDstTable);
extern void SetPermissionsTable(index_t entryCounter, uint8_t * payload);
extern uint8_t RemoveDeviceFromPermissionsTable(zbIeeeAddr_t aDevAddr);
extern void RemoveAllFromPermissionsTable(void);
extern uint8_t AddDeviceToPermissionsTable(uint8_t * aDevAddr, permissionsFlags_t permissionsCategory);

void ZtcMsgSetPermissionsTableFunc(void) {
  SetPermissionsTable(gZtcPacketFromClient.structured.payload[0], &(gZtcPacketFromClient.structured.payload[1]));
}                                       /*  */

void ZtcMsgGetPermissionsTableFunc(void) {
  ztcPermissionsTable_t *pTable = (ztcPermissionsTable_t *) &gZtcPacketToClient.structured.payload[sizeof(clientPacketStatus_t)];
  pTable->count = GetPermissionsTable(pTable->table);
  gZtcPacketToClient.structured.header.len = sizeof(clientPacketStatus_t)
                                           + sizeof(pTable->count)
                                           + (pTable->count * sizeof(permissionsTable_t));                                           
}                                       /*  */

void ZtcMsgRemoveFromPermissionsTableFunc(void)
{
  if (gZtcPacketFromClient.structured.payload[0])
    RemoveAllFromPermissionsTable();
  else
    gZtcPacketToClient.structured.payload[0] = RemoveDeviceFromPermissionsTable(&gZtcPacketFromClient.structured.payload[1]);
  gZtcPacketToClient.structured.header.len = sizeof(clientPacketStatus_t); 
}                                       /*  */

void ZtcMsgAddDeviceToPermissionsTableFunc(void)
{
  gZtcPacketToClient.structured.payload[0] = AddDeviceToPermissionsTable(&(gZtcPacketFromClient.structured.payload[0]), gZtcPacketFromClient.structured.payload[sizeof(zbIeeeAddr_t)]);
  gZtcPacketToClient.structured.header.len = sizeof(clientPacketStatus_t); 
}                                       /*  */

#endif /* (gZtcIncluded_d && (gStandardSecurity_d || gHighSecurity_d) && gApsMaxEntriesForPermissionsTable_c) */

/****************************************************************************/

#if (gZtcIncluded_d && defined(gZtcOtapSupport_d))

void (*pfOtapCallback)(void) = NULL;
void (*pfOtapCompute)(uint8_t*, uint8_t) = NULL;
static   bool_t   mFirstChunk, mLoadInProgress = FALSE;
static   uint32_t mRealImgSize, mRemainingSize, mEepromOffset;

void ZtcOtapSupportStartImageReqFunc(void) {
  
  gZtcPacketToClient.structured.header.len = (sizeof(uint8_t));
  gZtcPacketToClient.structured.payload[0] = gOtapSucess_c;
  
  if (mLoadInProgress){    
     gZtcPacketToClient.structured.payload[0] = gOtapInvalidOperation_c;
     return;
  }
  
  //Get image size (including Header and SublementTag)
  FLib_MemCpyReverseOrder(&mRemainingSize, gZtcPacketFromClient.structured.payload, 4);
  mRealImgSize = mRemainingSize - sizeof(ImageHeader_t) - sizeof(SubElement_t);
      
  if ((mRealImgSize == 0) ||
      (mRealImgSize > (gEepromParams_TotalSize_c - gBootData_Image_Offset_c))) { 
     gZtcPacketToClient.structured.payload[0] = gOtapInvalidParam_c;
     return; 
   }
   
  if(EEPROM_Init() != ee_ok) {
    gZtcPacketToClient.structured.payload[0] = gOtapEepromError_c;
    return;
  }
  
  mEepromOffset = gBootData_Image_Offset_c;
  mLoadInProgress = TRUE;
  mFirstChunk = TRUE;
}

void ZtcOtapSupportSetInternalFlashReqFunc(void) {}

/* Write data to eeprom*/
void ZtcOtapSupportPushImageChunkReqFunc(void) {
  uint8_t len, status;

  if (!mLoadInProgress){    
    gZtcPacketToClient.structured.payload[0] = gOtapInvalidOperation_c;
    return;
  }
  
  len = gZtcPacketFromClient.structured.payload[0];  
  mRemainingSize -=len;
  
  gZtcPacketToClient.structured.header.len = (sizeof(uint8_t));
  if (len == 0) {
    gZtcPacketToClient.structured.payload[0] = gOtapInvalidParam_c;
    return;
  }
  
  if (mFirstChunk) 
    {
     mFirstChunk = FALSE;
     status = EEPROM_WriteData(gImageDataFirstBlockSize_c, gBootData_Image_Offset_c + mRealImgSize, &gZtcPacketFromClient.structured.payload[1]);
     if (status == ee_ok)
       status = EEPROM_WriteData(len-gImageDataFirstBlockSize_c, mEepromOffset, &gZtcPacketFromClient.structured.payload[gImageDataFirstBlockSize_c +1]);
     mEepromOffset += len-gImageDataFirstBlockSize_c;
    } 
  else 
    {
      status = EEPROM_WriteData(len, mEepromOffset, &gZtcPacketFromClient.structured.payload[1]);
      mEepromOffset += len;
    }
  // Do other computing on the received buffer
  if (pfOtapCompute != NULL)
    pfOtapCompute(&gZtcPacketFromClient.structured.payload[1], len);

  gZtcPacketToClient.structured.payload[0] = status;    
}

void ZtcOtapSupportCommitImageReqFunc(void) {
  uint8_t status;

  gZtcPacketToClient.structured.header.len = (sizeof(uint8_t));
  if (!mLoadInProgress){    
    gZtcPacketToClient.structured.payload[0] = gOtapInvalidOperation_c;
    return;
  }
  
  mLoadInProgress = FALSE;
  if (mRemainingSize > 0) {
    gZtcPacketToClient.structured.payload[0] = gOtapInvalidOperation_c;
    return;
  }
  
  status = EEPROM_WriteData(gBootData_SectorsBitmap_Size_c, gBootData_SectorsBitmap_Offset_c, &gZtcPacketFromClient.structured.payload[2]);
  if (status == ee_ok)
    status = EEPROM_WriteData(gBootData_ImageLength_Size_c, gBootData_ImageLength_Offset_c, (uint8_t*)&mRealImgSize);  
  
  if (status == ee_ok) {
    gZtcPacketToClient.structured.payload[0] = gOtapSucess_c;
	if (pfOtapCallback!=NULL)
      pfOtapCallback(); // Inform AppTask
  }
  else
    gZtcPacketToClient.structured.payload[0] = gOtapEepromError_c;
}

void ZtcOtapSupportCancelImageReqFunc(void) {
  mLoadInProgress = FALSE;
}

#endif

#if gZtcIncluded_d
void ZtcGetLastReceivedPacketLqiValue(void)
{
  gZtcPacketToClient.structured.payload[0] = PhyGetLastRxLqiValue();
  gZtcPacketToClient.structured.header.len = 1;
}
#endif

#if gZtcIncluded_d
void ZtcMsgGetMaxProgramStackUsage(void)
{
  uint8_t *pStack;
  uint16_t stackSize =(uint16_t)__SEG_END_SSTACK , stackUsage = 0;
  stackSize -=  (uint16_t)__SEG_START_SSTACK;
#if (STACK_FILL)
  
  #if gNvStorageIncluded_d
    pStack = __SEG_END_NV_CODE_BUFFER;
  #else//gNvStorageIncluded_d
    pStack = __SEG_START_SSTACK;
  #endif//gNvStorageIncluded_d
  
  while ( pStack < __SEG_END_SSTACK )
  {
    if(*pStack != STACK_INIT_VALUE)
    {
      break;
    }
    else
    {
      stackUsage++;
    }
    pStack++;
  }
  #if gNvStorageIncluded_d
    stackUsage = (stackSize - stackUsage);
    stackUsage -= (uint16_t)((uint16_t) __SEG_END_NV_CODE_BUFFER - (uint16_t) __SEG_START_SSTACK);
    gZtcPacketToClient.structured.payload[0] = 2;//Err NvStorageIncluded_d=1 in file NVM_Interface
  #else//gNvStorageIncluded_d
    stackUsage = (stackSize - stackUsage);
    gZtcPacketToClient.structured.payload[0] = 0;//Success
  #endif//gNvStorageIncluded_d
  
  #if gBigEndian_c
    *((uint16_t *)&gZtcPacketToClient.structured.payload[1]) = Swap2Bytes(stackSize);
    *((uint16_t *)&gZtcPacketToClient.structured.payload[3]) = Swap2Bytes(stackUsage);
  #else//gBigEndian_c
    *((uint16_t *)&gZtcPacketToClient.structured.payload[1]) = (stackSize);
    *((uint16_t *)&gZtcPacketToClient.structured.payload[3]) = (stackUsage);
  #endif//gBigEndian_c
    gZtcPacketToClient.structured.header.len = 2 * sizeof(uint16_t) + 1;
#else// STACK_FILL
  gZtcPacketToClient.structured.payload[0] = 1;//Err STACK_FILL=0 in file crt0.h
  #if gBigEndian_c
    *((uint16_t *)&gZtcPacketToClient.structured.payload[1]) = Swap2Bytes(stackSize);
  #else//gBigEndian_c
    *((uint16_t *)&gZtcPacketToClient.structured.payload[1]) = (stackSize);
  #endif//gBigEndian_c
  *((uint16_t *)&gZtcPacketToClient.structured.payload[3]) = (0);
  gZtcPacketToClient.structured.header.len = 2 * sizeof(uint16_t) + 1;
#endif//STACK_FILL
}
#endif

#if (gLpmIncluded_d == 1)
/****************************************************************************/
#if gZtcIncluded_d
static void ZtcMsgWakeUpInd(void) 
{
  /* Set default values. */
  gZtcPacketToClient.structured.header.opcodeGroup = gZtcCnfOpcodeGroup_c;
  gZtcPacketToClient.structured.header.msgType = mZtcMsgWakeUpIndication_c;
  gZtcPacketToClient.structured.header.len = 0x00;
  ZtcWritePacketToClient(sizeof(gZtcPacketToClient.structured.header) + gZtcPacketToClient.structured.header.len);
}
#endif
/****************************************************************************/
/* This function selects and configures the PIN (KB0-KB7)
 * through which the MCU enters or not in sleep mode.
 */
#if gZtcIncluded_d 
void ZtcMsgSelectWakeUpPINReqFunc(void) 
{  
  /* Check if any pin is selected */
  if(gZtcPacketFromClient.structured.payload[0])
  {
  
#ifndef PROCESSOR_MC1323X  
  /* On S08GT60, if rising edges detection was selected, check if a right pin was chosen */
   if ((gZtcPacketFromClient.structured.payload[1] == 0x01) &&
      !(gZtcPacketFromClient.structured.payload[0] & mRisingEdgesPinsMask_c ))
   {
      /* The parameters are not correctly set */
      gZtcPacketToClient.structured.payload[0] = gZtcError_c; 
      return;
   }
#endif   
   /* Set the new configuration */   
   FLib_MemCpy(&ztcWakeUpConfig, &gZtcPacketFromClient.structured.payload[0], sizeof(ztcWakeUpConfig)-sizeof(ztcWakeUpConfig.prevPinLevel)); 
   KBISC |= 0x02; /* KBI status and control register initialized*/   
   if(ztcWakeUpConfig.detectionEdge == mRisingEdges_c) 
   {
#ifndef PROCESSOR_MC1323X   
     KBISC |= (ztcWakeUpConfig.selectedKBPin & mRisingEdgesPinsMask_c); /* Select to wake up on rising edges */
#else  
     KBIES |= ztcWakeUpConfig.selectedKBPin;
#endif 
   }
   else
   {
#ifndef PROCESSOR_MC1323X   
     KBISC &= ~(ztcWakeUpConfig.selectedKBPin & mRisingEdgesPinsMask_c);/* Select to wake up on falling edges */
#else
     KBIES &= ~ztcWakeUpConfig.selectedKBPin;
#endif   
   }
   KBIPE |= ztcWakeUpConfig.selectedKBPin; /* KBI pin enable controls initialized */  
   /* If prevPinLevel is 0xff, it's mean that we don't have any pin level previously detected */
   ztcWakeUpConfig.prevPinLevel = 0xff;
  }
  else
  {
    FLib_MemSet(&ztcWakeUpConfig, 0x00, sizeof(ztcWakeUpConfig)-sizeof(ztcWakeUpConfig.prevPinLevel));
    ztcWakeUpConfig.prevPinLevel = 0xff;
  }
  /* Get the level of the pin */
  Ztc_CheckWakeUpPIN();
}
#endif
/****************************************************************************/

/* Read the pin level to detect the level changes;
   Allow or not to enter in sleep mode (depend on wakeup pin configuration) */
void Ztc_CheckWakeUpPIN(void) 
{
 uint8_t currentPinLevel;
 /* By default, the Application allows device to sleep */
 static bool_t ztcDevCanGoToSleep_Flag = TRUE; 

 if(!ztcWakeUpConfig.selectedKBPin)
 {
 /* If the wake up pin is not configured, disallow device to sleep */ 
  if(ztcDevCanGoToSleep_Flag)
  {
    PWR_DisallowDeviceToSleep();  /* Disallow device to sleep */
    ztcDevCanGoToSleep_Flag = FALSE; 
  }    
  return;
 }

 /* Read the pin */
 currentPinLevel = SWITCH_PORT & ztcWakeUpConfig.selectedKBPin;
 /* Check if the level was changed since the previous read or this is the 
    first time we read it */
 if((currentPinLevel^ztcWakeUpConfig.prevPinLevel) || (ztcWakeUpConfig.prevPinLevel == 0xff))
 {
    /* If the MCU is waking up on rising edges and the level is low(or MCU is waking up on falling edges 
    and the level is high), allow device to sleep; else disallow to sleep;
    Send a Wakeup.Ind message if needed.
    */
    if( (!currentPinLevel && (ztcWakeUpConfig.detectionEdge == mRisingEdges_c)) ||
       (currentPinLevel && (ztcWakeUpConfig.detectionEdge == mFallingEdges_c)) )
    { /* Allow to sleep only if the device can NOT go to sleep */
     if(!ztcDevCanGoToSleep_Flag)
     {
       PWR_AllowDeviceToSleep(); /* Allow device to sleep */
       ztcDevCanGoToSleep_Flag = TRUE;
     }
    }
    else
    { /* Disallow to sleep only if the device can go to sleep */
     if(ztcDevCanGoToSleep_Flag)
     {
       PWR_DisallowDeviceToSleep();  /* Disallow device to sleep */
       ztcDevCanGoToSleep_Flag = FALSE;
       if(ztcWakeUpConfig.signalWhenWakeUpFlag && (ztcWakeUpConfig.prevPinLevel != 0xff))
          ZtcMsgWakeUpInd();
     }
    }
 }
 
  /* Update the previous pin level with the current one */
  ztcWakeUpConfig.prevPinLevel = currentPinLevel;
}

#else /*  else #if gLpmIncluded_d */
#if gZtcIncluded_d
void ZtcMsgSelectWakeUpPINReqFunc(void) 
{ 
}
#endif
#endif /* #if gLpmIncluded_d */ 



/****************************************************************************/