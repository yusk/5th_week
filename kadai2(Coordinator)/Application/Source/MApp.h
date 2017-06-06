/************************************************************************************
* This header file is for MyWirelessApp Demo Beacon Coordinator application.
*
* (c) Copyright 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/
#ifndef _MAPP_H_
#define _MAPP_H_

#include "MApp_init.h"
#include "ApplicationConf.h"
#include "802_15_4.h"      /* Include everything related to the 802.15.4 interface*/
#include "UartUtil.h"      /* Defines the interface of the demo UART. */
#include "SPI_Interface.h" /* Defines the interface of the SPI driver. */
#include "Display.h"       /* Defines the interface of the LCD driver. */
#include "Uart_Interface.h"/* Defines the interface of the Uart driver. */
#include "TMR_Interface.h" /* Defines the interface of the Timer driver. */
#include "TS_Interface.h"  /* Defines the interface of the TaskScheduler. */
#include "PWR_Interface.h" /* Defines the interface of the Low Power Module. */
#include "Led.h"           /* Defines the interface of the Led Module. */
#include "Keyboard.h"      /* Defines the interface of the Keyboard Module. */
#include "NV_Data.h"       /* Defines the interface of the Keyboard Module. */




/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/* The various states of the applications state machines. */
enum {
  stateInit,
  stateScanEdStart,
  stateScanEdWaitConfirm,
  stateStartCoordinator,
  stateStartCoordinatorWaitConfirm,
  stateListen
};


/* Events that are passed to the application task. 
   Are defined as byte masks to make possible 
   send multiple events to the task */

#define gAppEvtDummyEvent_c            (1 << 0)
#define gAppEvtRxFromUart_c            (1 << 1)
#define gAppEvtMessageFromMLME_c       (1 << 2)
#define gAppEvtMessageFromMCPS_c       (1 << 3)
#define gAppEvtStartCoordinator_c      (1 << 4)

#define gAppEvt_FromMPL3115A2_c        (1 << 7) // New for Freescale sensor board
#define gAppEvt_FromMMA8652_c          (1 << 9) // New
#define gAppEvt_FromMAG3110_c          (1 << 10)// New
#define gAppEvt_FromFXAS21000_c        (1 << 11)// New
#define gAppEvt_FromMMA9553_c          (1 << 12)// New

/* Error codes */
enum {
  errorNoError = 0,
  errorWrongConfirm,
  errorNotSuccessful,
  errorNoMessage,
  errorAllocFailed,
  errorInvalidParameter,
  errorNoScanResults
};

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
/*Set the Coordinator short address */ 
#define mDefaultValueOfShortAddress_c     0xCAFE

/*Set the Coordinator PanID */ 
#define mDefaultValueOfPanId_c            0xBEEF

/* Defines the beaconed network configuration. The values shown
   are suitable for transfer of 19200bps bidirectional UART data. */
#define mDefaultValueOfBeaconOrder_c      6
#define mDefaultValueOfSuperframeOrder_c  6

/* Maximum number of outstanding packets */
#define mDefaultValueOfMaxPendingDataPackets_c 2  
  
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/******************************************************************************
*******************************************************************************
* Public Prototypes
*******************************************************************************
******************************************************************************/
extern void MApp_init(void);
extern tsTaskID_t gAppTaskID_c;
/**********************************************************************************/

#endif /* _MAPP_H_ */
