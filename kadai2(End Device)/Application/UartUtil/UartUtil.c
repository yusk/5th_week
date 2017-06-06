/************************************************************************************
* Includes the UART Utils.
*
* (c) Copyright 2007, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/
#include "EmbeddedTypes.h"
#include "UartUtil.h"
#include "UART_Interface.h"
#include "Uart.h"

#ifndef mSCIxS1_TDRE_c
#define mSCIxS1_TDRE_c      0x80        /* Transmit Data Register Empty Flag. */
#endif

/******************************************************************************/
/******************************************************************************/

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************/
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
void UartUtil_TxCallbackPaged(unsigned char const *pBuf);
#pragma CODE_SEG DEFAULT
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
 
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

#if gUart1_Enabled_d || gUart2_Enabled_d

void UartUtil_TxCallback(unsigned char const *pBuf)
{
  #ifdef MEMORY_MODEL_BANKED
    UartUtil_TxCallbackPaged(pBuf);
  #else
    (void) pBuf;
  #endif
}

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
void UartUtil_TxCallbackPaged(unsigned char const *pBuf)
{
  (void) pBuf;
}
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
void UartUtil_Tx(uint8_t * pData, uint8_t length)
{
  /* wait until module can insert message into uart buffers */
  while(!UartX_Transmit((unsigned char const*)pData,(index_t)length,UartUtil_TxCallback));
  /* wait until tx complete */
  while (UartX_IsTxActive());      
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
void UartUtil_Print(uint8_t* pString, uartUtilBlock_t allowToBlock)
{
  uint16_t i = 0;
  uint8_t*  pTmpString = pString ;
  
  while (*pTmpString != '\0') 
  {
    pTmpString++;
    i++;
  } 
  if(allowToBlock)
  {
    /* wait until module can insert message into uart buffers */
    while(!UartX_Transmit((unsigned char const*)pString,(index_t)i,UartUtil_TxCallback));
    /* wait until tx complete */
    while (UartX_IsTxActive());  
  }
  else
  {
    (void)UartX_Transmit((unsigned char const*)pString,(index_t)i,UartUtil_TxCallback);
  }
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

/* Trasform from hex to ascii */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
static uint8_t HexToAscii(uint8_t hex)
{
  hex &= 0x0F;
  return hex + ((hex <= 9) ? '0' : ('A'-10));
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
void UartUtil_PrintHex(uint8_t *hex, uint8_t len, uint8_t flags)
{
  uint8_t i=0; 
  uint8_t hexString[5]; /* 2 bytes  - hexadecimal display 
                           1 byte   - separator ( comma, space)
                           2 bytes  - new line (\n\r)  */
  
  if(! (flags & gPrtHexBigEndian_c))
   {
    hex = hex + (len-1);
   }
   
  while(len)
  {
    /* wait until previous hex print transmission is over */  
    while (UartX_IsTxActive());  
      
    /* start preparing the print of a new byte */
    i=0;
    hexString[i++] = HexToAscii((*hex)>>4);
    hexString[i++] = HexToAscii( *hex);

    if(flags & gPrtHexCommas_c)
     {
      hexString[i++] = ',';
     }
    if(flags & gPrtHexSpaces_c) 
     { 
      hexString[i++] = ' ';
     }
    hex = hex + (flags & gPrtHexBigEndian_c ? 1 : -1);
    len--;
    
    if((len == 0) && (flags & gPrtHexNewLine_c))
     {
      hexString[i++] = '\n';
      hexString[i++] = '\r';
     }
     /* transmit formatted byte */      
    (void)UartX_Transmit((unsigned char const*)hexString,(index_t)i,UartUtil_TxCallback);   
  }
  /* wait to complete the last transmission */
  while (UartX_IsTxActive());   
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
bool_t UartUtil_CheckIdle(void)
{   
    return FALSE;  
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
uint8_t UartUtil_Poll(uint8_t * pBuffer){
  (void)pBuffer;
  return FALSE; 
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
uint8_t UartUtil_PollMessage(uint8_t * pBuffer){
  (void)pBuffer;
  return FALSE;   
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
void UartUtil_ConfigureStopMode(bool_t enterStopMode){
(void)enterStopMode;  
}

#else /* gUart1_Enabled_d || gUart2_Enabled_d */

/* Create  dummy functions  */
void UartUtil_Print(char * pString, uartUtilBlock_t allowToBlock)
{
  /* Avoid compiler warning */
  (void)pString;
  (void)allowToBlock;
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
void UartUtil_PrintHex(uint8_t *hex, uint8_t len, uint8_t flags)
{
  /* Avoid compiler warning */
  (void)hex;
  (void)len;
  (void)flags;
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
void UartUtil_Tx(uint8_t * pData, uint8_t length){
  /* Avoid compiler warning */
  (void)pData;
  (void)length;
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
bool_t UartUtil_CheckIdle(void)
{
   return FALSE;
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
uint8_t UartUtil_Poll(uint8_t * pBuffer){
  (void)pBuffer;
  return FALSE; 
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
uint8_t UartUtil_PollMessage(uint8_t * pBuffer){
  (void)pBuffer;
  return FALSE;   
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

/************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __FAR_SEG PAGED_ROM
#endif
void UartUtil_ConfigureStopMode(bool_t enterStopMode){
(void)enterStopMode;  
}
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG DEFAULT
#endif

#endif  /* gUart1_Enabled_d || gUart2_Enabled_d */