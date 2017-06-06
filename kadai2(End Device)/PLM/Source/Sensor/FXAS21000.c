/***********************************************************************************************\
* Freescale FXAS21000 Driver
*
* Filename: FXAS21000.c
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
\***********************************************************************************************/

#include "IIC_Interface.h"
#include "IIC.h"
#include "IoConfig.h"
#include "IrqControlLib.h"
#include "TS_Interface.h"
#include "FunctionLib.h"
#include "PublicConst.h"
#include "MApp_init.h"
#include "FXAS21000.h"


/***********************************************************************************************\
* Public functions
\***********************************************************************************************/

static tmrTimerID_t mFXAS21000TimerID = gTmrInvalidTimerID_c;
extern uint8_t FXAS21000_start_flag;
static start_print = 1;

// *********************************************************
//  FXAS21000
// *********************************************************

static dataFrom_FXAS21000_t mDataFrom_FXAS21000 = 
{                                                                
  0x00, 
  0x00,
  0x00, 
  0x00,
  0x00
};

/*********************************************************\
* Set MAG3110 call-back function
\*********************************************************/
void FXAS21000_SetCallBack(void (*pfCallBack)(void)){
	
	pfFXAS21000_CallBack = pfCallBack;
	  	  
}

// ==================================
// FXAS21000_int
// ==================================
void FXAS21000_int(void){
  IIC_RegWrite(FXAS21000_SlaveAddressIIC,0x13,0x00);    // CTRL_REG1: Standby mode
  IIC_RegWrite(FXAS21000_SlaveAddressIIC,0x0D,0x00);    // CTRL_REG0: HPF=Off, 1600dps,
  IIC_RegWrite(FXAS21000_SlaveAddressIIC,0x14,0x00);    // CTRL_REG2: Interrupt=Off
  IIC_RegWrite(FXAS21000_SlaveAddressIIC,0x13,0x02);    // CTRL_REG1: ODR=200Hz, Active
  mFXAS21000TimerID = TMR_AllocateTimer();
}

/*********************************************************\
* void FXAS21000_Start_Periodical_data(void) 
\*********************************************************/
void FXAS21000_Start_Periodical_data(void){
	TMR_StartIntervalTimer(mFXAS21000TimerID, mFXAS21000Interval_c,FXAS21000_Periodical_data);
}

/*********************************************************\
* void FXAS21000_Periodical_data(uint8_t timerId) 
\*********************************************************/
void FXAS21000_Periodical_data(uint8_t timerId){
		
  uint8_t rxData[7];
  volatile int16_t Read_data_16bit;
  
  (void) timerId; /* prevent compiler warning */

  
  rxData[0] = IIC_RegRead(FXAS21000_SlaveAddressIIC,0x00); // checking a STATUS-reg
  
  if( rxData[0] & 0x08 ){
      
	  IIC_RegReadN(FXAS21000_SlaveAddressIIC,0x01,0x06,&rxData[1]);      // Read data from $0x01 to 0x06
	  
	  pfFXAS21000_CallBack();  // Set event in order to notify application in callback function.
	  
	  Read_data_16bit = (int16_t)(rxData[1]<<8);
	  mDataFrom_FXAS21000.xOutReg =  ( Read_data_16bit + (int16_t)rxData[2] )>>2;
	 
	  Read_data_16bit = (int16_t)(rxData[3]<<8);
	  mDataFrom_FXAS21000.yOutReg =  ( Read_data_16bit + (int16_t)rxData[4] )>>2; 
	  
	  Read_data_16bit = (int16_t)(rxData[5]<<8);
	  mDataFrom_FXAS21000.zOutReg =  ( Read_data_16bit + (int16_t)rxData[6] )>>2; 
    

  }
  
}


/*********************************************************\
* int16_t FXAS21000_CatchSensorData(uint8_t number)
\*********************************************************/
int16_t FXAS21000_CatchSensorData(uint8_t number){
	
	int16_t catch_data;
	
	switch(number){
		case 1:
			catch_data = mDataFrom_FXAS21000.xOutReg;
		break;
		case 2:
			catch_data = mDataFrom_FXAS21000.yOutReg;
		break;
		case 3:
			catch_data = mDataFrom_FXAS21000.zOutReg;
		break;
		default :
		break;
	}
	
    return catch_data;
    
}


/*********************************************************\
* void FXAS21000_Init(void)
\*********************************************************/
void FXAS21000_Init(void){
	  FXAS21000_SetCallBack(FXAS21000_CallBack);  
	  FXAS21000_int();
	  FXAS21000_start_flag = 1;
}

/*********************************************************\
* void FXAS21000_CallBack(void) 
\*********************************************************/
void FXAS21000_CallBack(void){
	
	TS_SendEvent(gAppTaskID_c, gAppEvt_FromFXAS21000_c);
	
}

/*********************************************************\
* void FXAS21000_dump(event_t events)
\*********************************************************/
void FXAS21000_dump(event_t events){

	volatile int16_t tmp;
	uint8_t  i;
	
	// Start to receive periodical data 
	if(FXAS21000_start_flag){
		FXAS21000_start_flag = 0;
		FXAS21000_Start_Periodical_data();
	}
	
	if (events & gAppEvt_FromFXAS21000_c)
	{   
	    if(start_print){
	    	start_print = 0;
			UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 
			UartUtil_Print("FXAS21000(12bit)------------------------------------", gAllowToBlock_d); 	
			UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 		
	    }
		for(i=1; i<4; i++){
			switch(i){			
				case 1:
					
					UartUtil_Print("\n\rX axis : 0x", gAllowToBlock_d); 	
				break;
				case 2:
					UartUtil_Print(" *0.2 [dps]  Y axis : 0x", gAllowToBlock_d);  
				break;
				case 3:
					UartUtil_Print(" *0.2 [dps]  Z axis : 0x", gAllowToBlock_d);  	
				break;
				
			}//switch(i){	 	
			
		    tmp = (uint16_t)FXAS21000_CatchSensorData(i);
			UartUtil_PrintHex((uint8_t *)&tmp, 2, 1); 	
			
			if(i==3){
				UartUtil_Print(" *0.2 [dps]", gAllowToBlock_d);  					
			}
			
		} //for(i=1; i<4; i++){
		
		
	//	UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 		
	
	}
	
}

