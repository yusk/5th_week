/*****************************************************************************
* IIC Serial Port implementation.
*
* (c) Copyright 2008, Freescale, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#include "IIC.h"
#include "IIC_Interface.h"
#include "IoConfig.h"
#include "IrqControlLib.h"


#ifndef gMacStandAlone_d
#define gMacStandAlone_d 0
#endif

#if gMacStandAlone_d
  #include "Mac_Globals.h"
#endif

/*Set the IIC task Priority  */
#ifndef gTsI2CTaskPriority_c
#define gTsI2CTaskPriority_c             0x05
#endif 

/*****************************************************************************
******************************************************************************
* Private macros
******************************************************************************
*****************************************************************************/

#if gIIC_Slave_TxDataAvailableSignal_Enable_c

/* Verify if there is data to be received from the Slave */
#define IIC_IsTxDataAvailable()  ( (gIIC_TxDataAvailablePortDataReg_c & gIIC_TxDataAvailablePinMask_c) == 0)
#endif
/*****************************************************************************
******************************************************************************
* Private type definitions
******************************************************************************
*****************************************************************************/
/* The transmit code keeps a circular list of buffers to be sent */
typedef struct IIcTxBufRef_tag {
  unsigned char const *pBuf;
  void (*pfCallBack)(unsigned char const *pBuf);
} IIcTxBufRef_t;

typedef enum
  {
    mIIC_OpType_Tx_c,
    mIIC_OpType_Rx_c
  }iicOpType_t;

typedef struct iicMasterOp_tag {
  iicOpType_t iicOpType;
  uint8_t *pBuf;
  uint8_t bufLen;
} iicMasterOp_t;

/*****************************************************************************
******************************************************************************
* Public memory definitions
******************************************************************************
*****************************************************************************/
#if gIIC_Enabled_d
/* ID for IIC task */
         tsTaskID_t   gIIcTaskId;
volatile index_t      mIIcRxBufferByteCount;

#endif

//extern void  (*pfIIcMasterCallBack)(void);



/*****************************************************************************
******************************************************************************
* Private memory definitions
******************************************************************************
*****************************************************************************/
#if gIIC_Enabled_d
/* The leading index is the next position to store a buffer reference.
    The trailing index is the buffer currently being transmitted.
    Using a separate table for the buffer lengths reduces code size */    
static IIcTxBufRef_t  maIIcTxBufRefTable[gIIC_SlaveTransmitBuffersNo_c];
static index_t        maIIcTxBufLenTable[gIIC_SlaveTransmitBuffersNo_c];
static index_t        mIIcTxBufRefLeadingIndex;    /* Post-increment. */
static index_t        mIIcTxBufRefTrailingIndex;   /* Post-increment. */

/* Callback table and index pointers */
static IIcTxBufRef_t  maIIcTxCallbackTable[gIIC_SlaveTransmitBuffersNo_c];
static index_t        maIIcTxCallbackLeadingIndex   = 0; 
static index_t        maIIcTxCallbackTrailingIndex  = 0; 

/* I2C module has a small local circular Rx buffer to store data until the
    application can retrieve it. In addition to the leading and trailing
    indexes, the Rx code keeps a count of the number of bytes in the buffer. */
static uint8_t        maIIcRxBuf[gIIC_SlaveReceiveBufferSize_c];
static index_t        mIIcRxBufLeadingIndex;       /* Post-increment. */
static index_t        mIIcRxBufTrailingIndex;      /* Post-increment. */
/* Local variable to keep the I2C Rx callback provided by the application */
static void           (*pfIIcSlaveRxCallBack)(void);
static void           (*pfIIcMasterRxCallBack)(void (*pfCallBack)(void));
static void           (*pfIIcMasterTxCallBack)(bool_t status);
static iicMasterOp_t   mIICMasterOp;

#endif

#if 1 
void IIC_CycleWrite(uint8_t bout);
void IIC_RegWrite(uint8_t address, uint8_t reg, uint8_t val);
uint8_t IIC_RegRead(uint8_t address, uint8_t reg);
void IIC_CycleWrite(uint8_t bout);
uint8_t IIC_CycleRead(uint8_t byteLeft);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_RepeatStart(void);
uint8_t IIC_StopRead(void);
void IIC_RegWriteN(uint8_t address, uint8_t reg1,uint8_t N,uint8_t *array);
void IIC_RegReadN(uint8_t address, uint8_t reg1, uint8_t N,uint8_t *array);
void IIC_Bus_Reset(void);

#endif

static uint16_t timeout;
  

#if 0
void IIC_SetCallBack(void (*pfCallBack)(void)) {
	
	pfIIcMasterCallBack = pfCallBack;
	  	  
}
#endif
/*****************************************************************************
*   IIC_ModuleInit 
*
*   Initializes the I2C module 
******************************************************************************/
void IIC_ModuleInit(void)
{
#if gIIC_Enabled_d  
  /* Configure the I2C hardware peripheral */
  mIIC_C_c = mIICC_Reset_c;
  /* Clear the I2C Rx software buffer */
  mIIcRxBufLeadingIndex = mIIcRxBufTrailingIndex = mIIcRxBufferByteCount = 0;
  pfIIcSlaveRxCallBack = NULL;
  pfIIcMasterTxCallBack = NULL;
  
  #if  gIIC_Slave_TxDataAvailableSignal_Enable_c
/* Configure as output the GPIO that will be used to signal to the host that
     the blackBox I2C slave device has data to be transmitted */
/* Signal to the host that there are no data available to be read */     
  gIIC_TxDataAvailablePortDataReg_c |= gIIC_TxDataAvailablePinMask_c; 
  gIIC_TxDataAvailablePortDDirReg_c |= gIIC_TxDataAvailablePinMask_c;
#endif

  mIIC_S_c = mIICS_Init_c;   
  mIIC_F_c = gIIC_DefaultBaudRate_c;

  #if defined(PROCESSOR_MC1323X)  
  IIC1C2 = mIICxC2_Init_c; 
#endif
 /* Create I2C module main task */
  //IIC_TaskInit();

  mIIC_C_c = mIICC_IICEN_c; // Polling base

#endif  
}



/*********************************************************\
* Initiate IIC Bus Reset
\*********************************************************/

void IIC_Bus_Reset(void)
{
      int loop;
        
      // Disable the I2C block on the Host Controller 
      IICC1 &= ~(init_IICC1);            

      PAUSE;
             
      /* Create START condition (SDA goes low while SCL is high) */
      I2C_SDA_DD_L;   // SDA = 0   //PTA5 
      PAUSE;
      I2C_SCL_DD_H;   // SCL = 1   //PTA6
      PAUSE;

      /* Release SDA back high */
      I2C_SDA_DD_H;   // SDA = 1
      PAUSE;
    
      /* Clock SCL for at least 9 clocks until SDA goes high */
      loop = 0;

      while (loop < 90)
      {
            loop++;
            /* Apply one SCL clock pulse */
            I2C_SCL_DD_H; // SCL = 1
            PAUSE;
            I2C_SCL_DD_L; // SCL = 0
            PAUSE;
            /* If SDA is high and a complete byte was sent then exit the loop */ 
            if( ( PTAD & 0x20 ) && ((loop % 9) == 0) )
                  break;
      }
    
      /* Create STOP condition (SDA goes high while SCL is high) */
      I2C_SDA_DD_L;          // SDA = 0
      PAUSE;
      I2C_SCL_DD_H;          // SCL = 1
      PAUSE;
      I2C_SDA_DD_H;          // SDA = 1
      PAUSE;

      //Set operation back to default for all pins on PTADD and Enable I2C
      PTADD = init_PTADD;
      
      IICC1 = init_IICC1;
      
}



/*********************************************************\
* IIC Write Register
\*********************************************************/
void IIC_RegWrite(uint8_t address, uint8_t reg,uint8_t val)
{                      
  mIIC_C_c |= mIICC_TX_c;                       // Transmit Mode
  IIC_Start();                                  // Send Start
  IIC_CycleWrite(address);                      // Send IIC "Write" Address
  IIC_CycleWrite(reg);                          // Send Register
  IIC_CycleWrite(val);                          // Send Value
  IIC_Stop();                                   // Send Stop
}

/*********************************************************\
* IIC Read Register
\*********************************************************/
uint8_t IIC_RegRead(uint8_t address, uint8_t reg)
{
  volatile uint8_t b;
                         
  mIIC_C_c |= mIICC_TX_c;                       // Transmit Mode
  IIC_Start();                                  // Send Start
  IIC_CycleWrite(address);                      // Send IIC "Write" Address
  IIC_CycleWrite(reg);                          // Send Register
  IIC_RepeatStart();                            // Send Repeat Start
  IIC_CycleWrite(address+1);                    // Send IIC "Read" Address
  b = IIC_CycleRead(1);                         // *** Dummy read: reads "IIC_ReadAddress" value ***
  b = IIC_StopRead();                           // Send Stop Read command
  return b;
}

/*********************************************************\
* IIC Cycle Write
\*********************************************************/
void IIC_CycleWrite(uint8_t bout)
{
  volatile uint8_t error = 0x00;
  uint8_t read_status;
  
  timeout = 0;	  
  while ((mIICS_TCF_c != (mIIC_S_c & mIICS_TCF_c)) && (timeout<1000))
    timeout++;
  
  if (timeout >= 1000)
    error |= 0x08; 
  
  mIIC_D_c = bout; 
  
  timeout = 0;
  while ((mIICS_IICIF_c != (mIIC_S_c & mIICS_IICIF_c)) && (timeout<1000))
    timeout++;
  
  if (timeout >= 1000)
    error |= 0x10;
  
  mIIC_S_c |= mIICS_IICIF_c;
  
  read_status = mIIC_S_c & mIICS_RXAK_c;
  
  if( mIICS_RXAK_c == read_status )
    error |= 0x20;
}

/*********************************************************\
* IIC Cycle Read
\*********************************************************/
uint8_t IIC_CycleRead(uint8_t byteLeft)
{
  uint8_t bread; 
  volatile uint8_t error = 0x00;
  
  timeout = 0;
  while ((mIICS_TCF_c != (mIIC_S_c & mIICS_TCF_c)) && (timeout<1000))	  	  
    timeout++;
  
  if (timeout >= 1000)
    error|=0x08;
  
  mIIC_C_c &= ~(mIICC_TX_c);
    
  
  if( byteLeft <= 1 )
	  mIIC_C_c |= mIICC_TXAK_c;
  else
	  mIIC_C_c &= ~(mIICC_TXAK_c); 
  
  
  bread = mIIC_D_c;
  
  timeout = 0; 	  
  while ((mIICS_IICIF_c != (mIIC_S_c & mIICS_IICIF_c)) && (timeout<1000))	  	  
    timeout++;
  
  
  if (timeout >= 1000)
    error |= 0x10;
   
  mIIC_S_c |= mIICS_IICIF_c;
  
  
  return bread;
}

/*********************************************************\
* IIC Write Multiple Registers
\*********************************************************/
void IIC_RegWriteN(uint8_t address, uint8_t reg1, uint8_t N, uint8_t *array)
{                           
  mIIC_C_c |= mIICC_TX_c;                       // Transmit Mode
  IIC_Start();                                  // Send Start
  IIC_CycleWrite(address);                      // Send IIC "Write" Address
  IIC_CycleWrite(reg1);                         // Send Register
  while (N>0)                                   // Send N Values
  {
    IIC_CycleWrite(*array);
    array++;
    N--;
  }
  IIC_Stop();                                   // Send Stop
}

/*********************************************************\
* IIC Read Multiple Registers
\*********************************************************/
void IIC_RegReadN(uint8_t address, uint8_t reg1,uint8_t N,uint8_t *array)
{
  uint8_t read_data;
                                
  mIIC_C_c |= mIICC_TX_c;                       // Transmit Mode
  IIC_Start();                                  // Send Start
  IIC_CycleWrite(address);                      // Send IIC "Write" Address
  IIC_CycleWrite(reg1);                         // Send Register
  IIC_RepeatStart();                            // Send Repeat Start
  IIC_CycleWrite(address+1);                    // Send IIC "Read" Address
  read_data = IIC_CycleRead(N);                         // *** Dummy read: reads "IIC_ReadAddress" value ***
  while (N>1)                                   // Read N-1 Register Values
  {
    N--;
    read_data = IIC_CycleRead(N);
    *array = read_data;
    array++;
    
  }
  read_data = IIC_StopRead();
  *array = read_data;                                   // Read Last value
}


/*********************************************************\
* Initiate IIC Start Condition
\*********************************************************/
void IIC_Start(void)
{
  volatile uint8_t read_status;
  volatile uint8_t error = 0x00;
  
  read_status = mIIC_S_c;
    
  mIIC_C_c |= mIICC_MST_c;
  
  timeout = 0;   
  while ( (mIICS_BUSY_c != (mIIC_S_c & mIICS_BUSY_c) ) && (timeout<1000))
    timeout++;
  
  if (timeout >= 1000)
    error |= 0x01;
  
} //*** Wait until BUSY=1


/*********************************************************\
* Initiate IIC Stop Condition
\*********************************************************/
void IIC_Stop(void)
{
  volatile uint8_t error = 0x00;
	  
  mIIC_C_c &= ~(mIICC_MST_c);
	 
  timeout = 0;
  while ( (mIICS_BUSY_c == (mIIC_S_c & mIICS_BUSY_c)) && (timeout<1000))	  
    timeout++;
  
  if (timeout >= 1000)
    error |= 0x02;

} //*** Wait until BUSY=0

/*********************************************************\
* Initiate IIC Repeat Start Condition
\*********************************************************/
void IIC_RepeatStart(void)
{
  volatile uint8_t read_status;
  volatile uint8_t error = 0x00;
  
  read_status = mIIC_S_c;
  
  mIIC_C_c |= mIICC_RSTA_c;
  
  timeout = 0; 
  while ( (mIICS_BUSY_c != (mIIC_S_c & mIICS_BUSY_c)) && (timeout<1000))
    timeout++;
  
  if (timeout >= 1000)
    error |= 0x04;
  
} //*** Wait until BUSY=1

/*********************************************************\
* Initiate IIC Stop Condition on Read
\*********************************************************/
uint8_t IIC_StopRead(void)
{
  volatile uint8_t error = 0x00;
  
  mIIC_C_c &= ~(mIICC_MST_c);
  
  timeout = 0;
  while ( (mIICS_BUSY_c == (mIIC_S_c & mIICS_BUSY_c) ) && (timeout<1000))	  
    timeout++;
  
  if (timeout >= 1000)
    error |= 0x02;
  
  return mIIC_D_c;
  
} //*** Wait until BUSY=0



/*****************************************************************************
*   IIC_ModuleUninit 
*
*   Resets the I2C module.
******************************************************************************/
void IIC_ModuleUninit(void)
{
#if gIIC_Enabled_d
  mIIC_C_c = mIICC_Reset_c;
  
  /* Destroy the IIC task */
  if(gIIcTaskId != gTsInvalidTaskID_c)
  {
    TS_DestroyTask(gIIcTaskId);  
  }
#endif
}
/*****************************************************************************
*   IIC_SetBaudRate 
*
*   Resets the I2C module.
******************************************************************************/
bool_t IIC_SetBaudRate(uint8_t baudRate)
{
#if gIIC_Enabled_d
   if(mIIC_S_c & mIICS_BUSY_c)
    {
     return FALSE; 
    }
   mIIC_F_c = baudRate;
   return TRUE;
#else
  {
   (void)baudRate;
   return FALSE; 
  }
#endif
}


/*****************************************************************************
*   IIC_SetSlaveAddress 
*
*   Sets the slave address of the I2C module.
******************************************************************************/
bool_t IIC_SetSlaveAddress(uint8_t slaveAddress)
{

#if 0	
  /* Check if the I2C address is valid */
  if((slaveAddress > 0x7f) || 
     (((slaveAddress & 0x78) == 0) && ((slaveAddress & 0x07) != 0)) || 
     ((slaveAddress & 0x78) == 0x78))
  {
    return FALSE;
  }
  
#endif
  
  {
    mIIC_A_c = (slaveAddress << 1);
    return TRUE;
  }
}


/*****************************************************************************
*   IIC_BusRecovery 
*
*   Resets the I2C module.
******************************************************************************/
void IIC_BusRecovery(void)
{
#if gIIC_Enabled_d
   uint8_t iicControlReg;
   iicControlReg = mIIC_C_c;
   mIIC_C_c = mIICC_Reset_c;
   mIIC_C_c = mIICC_MST_c;
   mIIC_C_c |= mIICC_IICEN_c;
   mIIC_S_c = mIICS_Init_c;
   mIIC_D_c;
   while((mIIC_S_c & mIICS_IICIF_c) == 0);
   mIIC_C_c &= ~mIICC_MST_c;
   mIIC_S_c = mIICS_Init_c;
   mIIC_C_c = iicControlReg;
   
#endif
}


/*****************************************************************************
*   IIC_SetRxCallBack 
*
*   Sets a pointer to the function to be called whenever a byte is received.
*   If the pointer is set to NULL clear the Rx buffer.
******************************************************************************/
void IIC_SetSlaveRxCallBack(void (*pfCallBack)(void))
{
#if !gIIC_Enabled_d
  (void) pfCallBack;
#else
  pfIIcSlaveRxCallBack = pfCallBack;
#endif
}


/*****************************************************************************
*   IIC_Transmit_Slave 
*
*   Begin transmitting size bytes of data from *pBuffer.
*   Returns FALSE if there are no more slots in the buffer reference table.
******************************************************************************/
bool_t IIC_Transmit_Slave(uint8_t const *pBuf, index_t bufLen, void (*pfCallBack)(uint8_t const *pBuf)) 
{
#if !gIIC_Enabled_d
  (void) pBuf;
  (void) bufLen;
  (void) pfCallBack;
#else
  /* Handle empty buffers. */
  if (!bufLen) 
   {
    if(pfCallBack)
     {
      (*pfCallBack)(pBuf);  
     }
    return TRUE;
   }

  /* Room for one more? */
  if (maIIcTxBufLenTable[mIIcTxBufRefLeadingIndex]) 
  {
    return FALSE;
  }

  maIIcTxBufRefTable[mIIcTxBufRefLeadingIndex].pBuf = pBuf;
  maIIcTxBufRefTable[mIIcTxBufRefLeadingIndex].pfCallBack = pfCallBack;
  /* This has to be last, in case the Tx ISR finishes with the previous */
  /* buffer while this function is executing. */
  maIIcTxBufLenTable[mIIcTxBufRefLeadingIndex] = bufLen;

  if (++mIIcTxBufRefLeadingIndex >= NumberOfElements(maIIcTxBufRefTable)) 
  {
    mIIcTxBufRefLeadingIndex = 0;
  }
  
#if gIIC_Slave_TxDataAvailableSignal_Enable_c 
  /* Signal to host that there are data to receive */
  IIC_TxDataAvailable(TRUE);
#endif
  
#endif  
  
  return TRUE;
} 
/*****************************************************************************
*   IIC_Transmit_Master 
*
*   Begin transmitting size bytes of data from *pBuffer.
*   Returns FALSE if there are no more slots in the buffer reference table.
******************************************************************************/
bool_t IIC_Transmit_Master(uint8_t const *pBuf, index_t bufLen, uint8_t destAddress, void (*pfCallBack)(bool_t status)) 
{
#if !gIIC_Enabled_d
  (void) pBuf;
  (void) bufLen;
  (void) destAddress;
  (void) pfCallBack;
#else
  /* Handle empty buffers. */
  if (!bufLen) 
   {
    if(pfCallBack)
     {
      (*pfCallBack)(TRUE);  
     }
    return TRUE;
   }
   destAddress <<= 1;
   if(destAddress == mIIC_A_c)
    {
    return FALSE;
    }
   // pfIIcMasterTxCallBack is reset by IIC task after the callback is called
   //if pfIIcMasterTxCallBack is != 0 it means that the previous callback didn't run yet  
   if(pfIIcMasterTxCallBack) 
    {
      return FALSE;
    }
   if(mIIC_S_c & mIICS_BUSY_c)
    {
     return FALSE; 
    }
    mIICMasterOp.iicOpType = mIIC_OpType_Tx_c;
    mIICMasterOp.pBuf = (uint8_t*)pBuf;
    mIICMasterOp.bufLen = bufLen;
    pfIIcMasterTxCallBack = pfCallBack;
    
    mIIC_C_c |= (mIICC_MST_c | mIICC_TX_c);// start condition
    mIIC_D_c = destAddress; // address the slave for writting
  
  
#endif  
  
  return TRUE;
} 

/*****************************************************************************
*   IIC_Receive_Master 
*
*   Begin transmitting size bytes of data from *pBuffer.
*   Returns FALSE if there are no more slots in the buffer reference table.
******************************************************************************/
bool_t IIC_Receive_Master(uint8_t *pBuf, index_t bufLen, uint8_t destAddress, void (*pfCallBack)(bool_t status)) 
{
#if !gIIC_Enabled_d
  (void) pBuf;
  (void) bufLen;
  (void) destAddress;
  (void) pfCallBack;
#else
  /* Handle empty buffers. */
  if (!bufLen) 
   {
    if(pfCallBack)
     {
      (*pfCallBack)(TRUE);  
     }
    return TRUE;
   }
   destAddress <<= 1;
   if(destAddress == mIIC_A_c)
    {
    return FALSE;
    }
   // pfIIcMasterTxCallBack is reset by IIC task after the callback is called
   //if pfIIcMasterTxCallBack is != 0 it means that the previous callback didn't run yet  
   if(pfIIcMasterRxCallBack) 
    {
      return FALSE;
    }
   if(mIIC_S_c & mIICS_BUSY_c)
    {
     return FALSE; 
    }
    mIICMasterOp.iicOpType = mIIC_OpType_Rx_c;
    mIICMasterOp.pBuf = pBuf;
    mIICMasterOp.bufLen = bufLen;
    //Ori pfIIcMasterRxCallBack = pfCallBack;
    
    mIIC_C_c |= (mIICC_MST_c | mIICC_TX_c);// start condition
    mIIC_D_c = destAddress  | 0x1; // address the slave for reading
#endif  
  
  return TRUE;
} 

/*****************************************************************************
*   IIC_IsSlaveTxActive 
*
*   Returns TRUE if there is still data to be transmitted to the master.
*   Returns FALSE if nothing left to transmit.
******************************************************************************/
bool_t IIC_IsSlaveTxActive(void) 
{
#if !gIIC_Enabled_d
  return FALSE;
#else  
  return (maIIcTxBufLenTable[mIIcTxBufRefTrailingIndex] != 0) || ((mIIC_S_c & mIICS_BUSY_c));
#endif  
}

/*****************************************************************************
*   IIC_TxDataAvailable 
*
*   Depending on bool parameter master will be signaled that need to read data from slave
******************************************************************************/
void IIC_TxDataAvailable(bool_t bIsAvailable)
{
#if gIIC_Slave_TxDataAvailableSignal_Enable_c
  if(bIsAvailable) 
    {
     gIIC_TxDataAvailablePortDataReg_c &= ~(gIIC_TxDataAvailablePinMask_c); 
    }
  else
    {
     gIIC_TxDataAvailablePortDataReg_c |= gIIC_TxDataAvailablePinMask_c; 
    }
#else
(void)bIsAvailable;
#endif    
}

/*****************************************************************************
*   IIC_GetByteFromRxBuffer 
*
*   Retrieves a byte from the driver's Rx buffer and store it at *pDst.
*   Return TRUE if a byte was retrieved; FALSE if the Rx buffer is empty.
******************************************************************************/
bool_t IIC_GetByteFromRxBuffer(unsigned char *pDst)
{
#if !gIIC_Enabled_d
  (void) pDst;
#else
  /* Temp storage for I2C control register */
  uint8_t iicControlReg;
  
  if (!mIIcRxBufferByteCount) 
    return FALSE;
  IrqControlLib_PushIrqStatus();
  IrqControlLib_DisableAllIrqs();
  iicControlReg = mIIC_C_c;
  mIIC_C_c &= ~mIICC_IICIE_c;
  IrqControlLib_PullIrqStatus();
  *pDst = maIIcRxBuf[mIIcRxBufTrailingIndex];
  if (++mIIcRxBufTrailingIndex >= sizeof(maIIcRxBuf)) {
    mIIcRxBufTrailingIndex = 0;
  }
  --mIIcRxBufferByteCount;
  mIIC_C_c = iicControlReg;
 #endif  

  return TRUE;
}




/*****************************************************************************
*   IIC_Task 
*
*   Main task of the I2C module 
******************************************************************************/
void IIC_Task(event_t events)
{
#if !gIIC_Enabled_d
  (void) events;
#else

  void (*pfCallBack)(uint8_t const *pBuf);

  if (events & gIIC_Event_SlaveRx_c)
  {
    pfIIcSlaveRxCallBack();
  }

  if (events & gIIC_Event_SlaveTx_c)
  {
    /* Tx call back event received - run through the callback table and execute any
         pending Callbacks */
    while (maIIcTxCallbackLeadingIndex != maIIcTxCallbackTrailingIndex)
    {
       pfCallBack = maIIcTxCallbackTable[maIIcTxCallbackTrailingIndex].pfCallBack;
      /* Call calback with buffer info as parameter */
      (*pfCallBack)(maIIcTxCallbackTable[maIIcTxCallbackTrailingIndex].pBuf);
      /* Increment and wrap around trailing index */
      if (++maIIcTxCallbackTrailingIndex >= NumberOfElements(maIIcTxCallbackTable))
      {
        maIIcTxCallbackTrailingIndex = 0;
      }
    }
  }
 
#if 0
  if (events & gIIC_Event_MasterRxFail_c)
    {
     (*pfIIcMasterRxCallBack)(FALSE); 
     pfIIcMasterRxCallBack = NULL;
    }
  
  
  if (events & gIIC_Event_MasterRxSuccess_c)
    {
     (*pfIIcMasterRxCallBack)(TRUE); 
     pfIIcMasterRxCallBack = NULL;
    }    
  
  
  if (events & gIIC_Event_MasterTxFail_c)  
    {
      (*pfIIcMasterTxCallBack)(FALSE);
      pfIIcMasterTxCallBack = NULL;
    }
  if (events & gIIC_Event_MasterTxSuccess_c)  
    {
      (*pfIIcMasterTxCallBack)(TRUE);
      pfIIcMasterTxCallBack = NULL;
    }    
  
#endif
  
#endif  
} 


/*****************************************************************************
*  IIC_Isr
*
*  I2C Interrupt Service Routine.
******************************************************************************/
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

INTERRUPT_KEYWORD void IIC_Isr(void)
{

#if gIIC_Enabled_d

  /* Clear the interrupt request */
  mIIC_S_c |= mIICS_IICIF_c;
  /* Check arbitration  and slave addressing*/
  if (mIIC_S_c & (mIICS_ARBL_c | mIICS_IAAS_c ))
  {
    if (mIIC_S_c & mIICS_IAAS_c)
      {
      /* Check if I2C module was addressed for read or for write */
      if(mIIC_S_c & mIICS_SRW_c)
        {
         /* Configure I2C module for Tx operation.Writing to IICxC register also clears IAAS bit */
         mIIC_C_c |= mIICC_TX_c;
         /* Send next byte from the current Tx buffer */
         //IIC_SendNextByte();
        }
      else
        {
         /* Configure I2C module for Rx operation.Writing to IICxC register also clears IAAS bit */
         mIIC_C_c &= ~(mIICC_TX_c | mIICC_TXAK_c);
         mIIC_D_c;
        }

      }
    if(mIIC_S_c & mIICS_ARBL_c)
      {
      /* Arbitration lost */
       mIIC_S_c |= mIICS_ARBL_c;    
       
       if(mIICMasterOp.iicOpType == mIIC_OpType_Tx_c)
        {
         if(pfIIcMasterTxCallBack)
          {
           TS_SendEvent(gIIcTaskId, gIIC_Event_MasterTxFail_c); 
          }
          
        }
       else
        {
        if(pfIIcMasterRxCallBack)
          {
          TS_SendEvent(gIIcTaskId, gIIC_Event_MasterRxFail_c);    
          }
        
        }
      }
  }
  else
  {
    /* Arbitration okay
       Check addressing */
  if(mIIC_C_c & mIICC_MST_c)/* Master mode */
    {
     if(mIIC_C_c & mIICC_TX_c)
      {
        if(mIIC_S_c & mIICS_RXAK_c)// No ack received
          {
            mIIC_C_c &= ~(mIICC_MST_c | mIICC_TX_c);
            if(mIICMasterOp.iicOpType == mIIC_OpType_Tx_c)	// Tx
              {
              if(pfIIcMasterTxCallBack)
               {
                TS_SendEvent(gIIcTaskId, gIIC_Event_MasterTxFail_c); 
               }
              }
            else	// Rx
              {
              if(pfIIcMasterRxCallBack)
               {
                TS_SendEvent(gIIcTaskId, gIIC_Event_MasterRxFail_c);    
               }
              }
          }
        else	// Ack received
          {
            if(mIICMasterOp.iicOpType == mIIC_OpType_Rx_c)	// Rx
              {
              mIIC_C_c &= ~(mIICC_TX_c | mIICC_TXAK_c);
              if(mIICMasterOp.bufLen-- == 1)  
                {
                 mIIC_C_c |= mIICC_TXAK_c;
                }
              mIIC_D_c;  
              }
            else	// Tx
              {
               if(mIICMasterOp.bufLen)
                {
                 mIIC_D_c =  *mIICMasterOp.pBuf++ ;
                 mIICMasterOp.bufLen--;
                }
               else
                {
                 mIIC_C_c &= ~(mIICC_MST_c | mIICC_TX_c); 
                 if(pfIIcMasterTxCallBack)
                  {
                   TS_SendEvent(gIIcTaskId, gIIC_Event_MasterTxSuccess_c); 
                  }
                 
                }
              }
          }
      }
     else// (mIIC_C_c & mIICC_TX_c) == 0       
      {
       if(mIICMasterOp.bufLen == 0)   
        {
        mIIC_C_c &= ~mIICC_MST_c;   
        if(pfIIcMasterRxCallBack)
          {
           TS_SendEvent(gIIcTaskId, gIIC_Event_MasterRxSuccess_c); 
          }
        }
       else if(mIICMasterOp.bufLen-- == 1)  
        {
         mIIC_C_c |= mIICC_TXAK_c;
        }
       *mIICMasterOp.pBuf++ = mIIC_D_c;
        
      }
     
    }
    
  else   /* slave mode*/
    {
    if (mIIC_C_c & mIICC_TX_c)
      {
        /* IIC has Tx a byte to master. Check if ack was received */
        if (mIIC_S_c & mIICS_RXAK_c)
        {
          /* No ack received. Switch back to receive mode */
          mIIC_C_c &= ~mIICC_TX_c;
          mIIC_D_c;
        }
        else
        {
          /* Ack received. Send next byte */
          //IIC_SendNextByte();
        }
      }
    else
      {
        /* Put the received byte in the buffer */
        if(pfIIcSlaveRxCallBack)
          {
          maIIcRxBuf[mIIcRxBufLeadingIndex] = mIIC_D_c;

          if (++mIIcRxBufLeadingIndex >= sizeof(maIIcRxBuf)) 
            {
            mIIcRxBufLeadingIndex = 0;
            }

          if (mIIcRxBufferByteCount < sizeof(maIIcRxBuf)) 
            {
            ++mIIcRxBufferByteCount;
            }
          else
            {
             if (++mIIcRxBufTrailingIndex >= sizeof(maIIcRxBuf)) 
               {
                mIIcRxBufTrailingIndex = 0;
               }
            }
            
        /* Let the application know a byte has been received. */
          TS_SendEvent(gIIcTaskId, gIIC_Event_SlaveRx_c);
            
          }
        else
          {
           mIIC_D_c; 
          }
      }  
    }/* Data transfer.Check if it is a Tx or Rx operation */
      
    
  }
  
#endif  
}
#pragma CODE_SEG DEFAULT           


/*****************************************************************************
******************************************************************************
* Private functions
******************************************************************************
*****************************************************************************/

#if gIIC_Enabled_d

/*****************************************************************************
*   IIC_TaskInit 
*
*   Creates the I2C module task
******************************************************************************/
static void IIC_TaskInit(void)
{
  gIIcTaskId = TS_CreateTask(gTsI2CTaskPriority_c, IIC_Task);
}                                       

/*****************************************************************************
*   IIC_SendNextByte 
*
*   Send next byte of information from the Tx buffer
******************************************************************************/
#if 0

static void IIC_SendNextByte (void)
{
  void (*pfCallBack)(unsigned char const *pBuf);
  
  /* The HCS08 does not save H on interrupt. */
  __asm 
  {
    PSHH
  }

  
#if gIIC_Slave_TxDataAvailableSignal_Enable_c  
  if(IIC_IsTxDataAvailable())
#else
  if(maIIcTxBufLenTable[mIIcTxBufRefTrailingIndex])  
#endif  
  {
    /* Write a byte. */
    mIIC_D_c = maIIcTxBufRefTable[mIIcTxBufRefTrailingIndex].pBuf[mIIcTxCurIndex];

    /* Finished with this buffer? */
    if (++mIIcTxCurIndex >= maIIcTxBufLenTable[mIIcTxBufRefTrailingIndex]) 
    {

      /* Mark this one as done, and call the callback. */
      maIIcTxBufLenTable[mIIcTxBufRefTrailingIndex] = 0;
      pfCallBack = maIIcTxBufRefTable[mIIcTxBufRefTrailingIndex].pfCallBack;
      if (pfCallBack)
      {
        /* Signal the IIC task that we got a callback to be executed */
        TS_SendEvent(gIIcTaskId, gIIC_Event_SlaveTx_c);
        /* Add callback information to the callback table */
        maIIcTxCallbackTable[maIIcTxCallbackLeadingIndex].pfCallBack = pfCallBack;
        maIIcTxCallbackTable[maIIcTxCallbackLeadingIndex].pBuf = maIIcTxBufRefTable[mIIcTxBufRefTrailingIndex].pBuf;
        /* Increment and wrap around the leading index */
        if (++maIIcTxCallbackLeadingIndex >= NumberOfElements(maIIcTxCallbackTable)) {
          maIIcTxCallbackLeadingIndex = 0;
        }
      
      }
      /* Increment to the next buffer. */
      mIIcTxCurIndex = 0;
      if (++mIIcTxBufRefTrailingIndex >= NumberOfElements(maIIcTxBufRefTable)) 
      {
        mIIcTxBufRefTrailingIndex = 0;
      }

#if gIIC_Slave_TxDataAvailableSignal_Enable_c        
      /* If there is no more data to send, turn off the transmit interrupt. */
      if (!maIIcTxBufLenTable[mIIcTxBufRefTrailingIndex]) 
      {
        /* Signal to host that there are no more data to receive */
        IIC_TxDataAvailable(FALSE);
      }
#endif      
    }
  }
  else
  {
    /* Write a byte. */
    mIIC_D_c = 0;
  }
  __asm 
  {
    PULH
  }
}

#endif


#endif
