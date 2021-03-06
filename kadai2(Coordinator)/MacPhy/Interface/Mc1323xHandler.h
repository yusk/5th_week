/************************************************************************************
* This header file is provided as part of the interface to the Freescale 802.15.4
* MAC and PHY layer.
*
* This file contains Platform specific implentations of functions.
*
* (c) Copyright 2007, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/
#ifndef _MC1323XHANDLER_H_
#define _MC1323XHANDLER_H_

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#include "IrqControlLib.h"

/************************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
************************************************************************************/
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED

#pragma CODE_SEG __NEAR_SEG NON_BANKED
INTERRUPT_KEYWORD void PhyHandlerTx_ISR(void);
INTERRUPT_KEYWORD void PhyHandlerRx_ISR(void);
//INTERRUPT_KEYWORD void PhyHandlerRxTx_ISR(void);
INTERRUPT_KEYWORD void PhyHandlerTmr_ISR(void);
INTERRUPT_KEYWORD void PhyHandlerRxWtrmrk_ISR(void);

#endif /* MEMORY_MODEL_BANKED */

#pragma CODE_SEG DEFAULT

#endif /* _MC1323XHANDLER_H_ */
