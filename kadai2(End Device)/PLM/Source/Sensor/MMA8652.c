/***********************************************************************************************\

* Freescale MMA8652 Driver
*
* Filename: MMA8652.c
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
#include "MMA8652.h"


static dataFrom_MMA8652_t mDataFrom_MMA8652 = 
{                                                                
  0x00, 
  0x00,
  0x00, 
  0x00,
  0x00
};

static tmrTimerID_t mMMA8652TimerID = gTmrInvalidTimerID_c;

extern uint8_t MMA8652_start_flag;


/*********************************************************\
* void MMA8652_SetCallBack(void (*pfCallBack)(void))
\*********************************************************/
void MMA8652_SetCallBack(void (*pfCallBack)(void)) {
	
	pfMMA8652_CallBack = pfCallBack;
	  	  
}

/*********************************************************\
* void MMA8652_int(void)
\*********************************************************/
void MMA8652_int(void){
	
	IIC_RegWrite(MMA8652_SlaveAddressIIC,CTRL_REG1,0x00);                   // CTRL_REG1:   Standby mode
	IIC_RegWrite(MMA8652_SlaveAddressIIC,XYZ_DATA_CFG_REG,FULL_SCALE_2G);   // XYZ_DATA_CFG:2g mode
	IIC_RegWrite(MMA8652_SlaveAddressIIC,CTRL_REG2,MODS1_MASK);             // CTRL_REG2:   High Resolution mode
	IIC_RegWrite(MMA8652_SlaveAddressIIC,CTRL_REG1,(DR1_MASK|ACTIVE_MASK)); // CTRL_REG1:   ODR=200Hz(=5ms),Active mode
	mMMA8652TimerID = TMR_AllocateTimer();
	
}

/*********************************************************\
* void MMA8652_Start_Periodical_data(void)
\*********************************************************/
void MMA8652_Start_Periodical_data(void) {
	TMR_StartIntervalTimer(mMMA8652TimerID, mMMA8652Interval_c,MMA8652_Periodical_data);
}


/*********************************************************\
* MMA8652_Data
\*********************************************************/
void MMA8652_Periodical_data(uint8_t timerId ) {
	
  uint8_t rxData[7];
  volatile int16_t Read_data_16bit;

  (void) timerId; /* prevent compiler warning */

  rxData[0] = (IIC_RegRead(MMA8652_SlaveAddressIIC,STATUS_00_REG)); // checking a STATUS-reg
	
  if( rxData[0] & 0x08 ){
	  
	  IIC_RegReadN(MMA8652_SlaveAddressIIC,OUT_X_MSB_REG, OUT_Z_LSB_REG, &rxData[1]); // Read data from $0x01 to 0x06
  
	  pfMMA8652_CallBack();  // Set event in order to notify application in callback function. 
	  
	  Read_data_16bit = (int16_t)(rxData[1]<<8);
	  mDataFrom_MMA8652.xOutReg =  ( Read_data_16bit + (int16_t)rxData[2] ) >>4;
	 
	  Read_data_16bit = (int16_t)(rxData[3]<<8);
	  mDataFrom_MMA8652.yOutReg =  ( Read_data_16bit + (int16_t)rxData[4] ) >>4; 
	  
	  Read_data_16bit = (int16_t)(rxData[5]<<8);
	  mDataFrom_MMA8652.zOutReg =  ( Read_data_16bit + (int16_t)rxData[6] ) >>4; 
	
		
  }
  
}

/*********************************************************\
* int16_t MMA8652_CatchSensorData(uint8_t number)
\*********************************************************/
int16_t MMA8652_CatchSensorData(uint8_t number){
	
	int16_t catch_data;
	
	switch(number){
		case 1:
			catch_data = mDataFrom_MMA8652.xOutReg;
		break;
		case 2:
			catch_data = mDataFrom_MMA8652.yOutReg;
		break;
		case 3:
			catch_data = mDataFrom_MMA8652.zOutReg;
		break;
		default :
		break;
	}
	
    return catch_data;
    
}

/*********************************************************\
* void MMA8652_Init(void)
\*********************************************************/
void MMA8652_Init(void){
	  MMA8652_SetCallBack(MMA8652_CallBack);  
	  MMA8652_int();
	  MMA8652_start_flag = 1;
}

/*********************************************************\
* void MMA8652_CallBack(void)
\*********************************************************/
void MMA8652_CallBack(void){
	
	TS_SendEvent(gAppTaskID_c, gAppEvt_FromMMA8652_c);
	
}

/*********************************************************\
* void MMA8652_dump(event_t events)
\*********************************************************/
void MMA8652_dump(event_t events){
	
	volatile int16_t tmp;	
	uint8_t i;
	
	// Start to receive periodical data 
	if(MMA8652_start_flag){
		MMA8652_start_flag = 0;
		MMA8652_Start_Periodical_data();
	}
	
	
	if (events & gAppEvt_FromMMA8652_c)
	{   
		UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 
		
		for(i=1; i<4; i++){
			switch(i){			
				case 1:
					UartUtil_Print("MMA8652(2g: Int2,Float10) \n\rX axis : 0x", gAllowToBlock_d); 
				break;
				case 2:
					UartUtil_Print(" /1024 [g]\n\rY axis : 0x", gAllowToBlock_d);  
				break;
				case 3:
					UartUtil_Print(" /1024 [g]\n\rZ axis : 0x", gAllowToBlock_d);  		
				break;
				default:
					UartUtil_Print("???????????????", gAllowToBlock_d);  		
				break;
			}//switch(i){	 	
			
			tmp = MMA8652_CatchSensorData(i); 	
			UartUtil_PrintHex((uint8_t *)&tmp, 2, 1); 	
			
			if(i==3){
				UartUtil_Print(" /1024 [g]", gAllowToBlock_d);  					
			}
			
		} //for(i=1; i<4; i++){
		
		
		UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 		
	
	}
	
	
}

/***********************************************************************************************\
* Private functions
\***********************************************************************************************/
