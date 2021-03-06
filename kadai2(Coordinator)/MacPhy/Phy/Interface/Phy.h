/************************************************************************************
* Header file physical layer
*
* (c) Copyright 2007, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/

#ifndef _PHY_H_
#define _PHY_H_

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#ifdef PROCESSOR_MC1323X
  #include "PhyMC1323X.h"
#else  

#include "MacPhy.h"
#include "Mc1319xReg.h"
#include "Mc1319xDrv.h"




/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

typedef enum ePhyEnums_tag phyTxRxState_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
void IrqStreamingRx(void);
void IrqStreamingRxFirst(void);
void IrqStreamingTx(void);
void DoFastEdEof(void);
void DoFastCcaEof(void);

void InitRxStream(void);
void IsrTx2prime(void);

void SetupPendingProtected(void);
void SetupImmediateRx(void);
void SetupPendingCca(void);
void SetupPendingEd(void);
void SetupPendingNop(void);
void DoFastTxEof(void);
void DoFastRxEofCrcValid(void);
void DoFastRxEofCrcInvalid(void);

void RxTimeout(void);
void ConvertLinkQuality(void);
uint8_t GetEnergyLevel(void);
uint8_t PhyGetLastRxLqiValue(void);

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gAbelWarmupTimeSym_c    (9)
#define gPhyMinRestartRxTimeSym_c (12)

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern uint8_t *gpPhyPacketHead;
extern int8_t gTxRemainingLength;
extern uint16_t gIsrMask;
extern phyTxRxState_t mPhyTxRxState;
extern uint8_t gIsrEntryCounter;

#ifdef MEMORY_MODEL_BANKED
extern void __near(* __near mpfPendingSetup)(void);
extern void __near(* __near gpTimer2Isr)(void);
#else
extern void (* __near mpfPendingSetup)(void);
extern void (* __near gpTimer2Isr)(void);
#endif


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/




/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/***********************************************************************************/
#pragma CODE_SEG DEFAULT

#endif /* PROCESSOR_MC1323X */

#endif /* _PHY_H_ */
