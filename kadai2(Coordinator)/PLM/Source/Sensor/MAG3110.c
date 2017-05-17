/***********************************************************************************************\
 Freescale MAG3110 Driver
*
* Filename: MAG3110.c
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
#include "MAG3110.h"


/***********************************************************************************************\
* Public functions
\***********************************************************************************************/



extern uint8_t MAG3110_start_flag;


static dataFrom_MAG3110_t mDataFrom_MAG3110 = 
{                                                                
  0x00, 
  0x00,
  0x00, 
  0x00,
  0x00
};


static tmrTimerID_t mMAG3110TimerID = gTmrInvalidTimerID_c;

/*********************************************************\
* Set MAG3110 call-back function
\*********************************************************/
void MAG3110_SetCallBack(void (*pfCallBack)(void)) {
	
	pfMAG3110_CallBack = pfCallBack;
	  	  
}

/*********************************************************\
* void MAG3110_int(void)
\*********************************************************/
void MAG3110_int(void){
  IIC_RegWrite(MAG3110_SlaveAddressIIC,MAG3110_CTRL_REG1,0x00);    // CTRL_REG1: Standby mode 
  IIC_RegWrite(MAG3110_SlaveAddressIIC,MAG3110_CTRL_REG2,MAG3110_CTRL_REG2_AUTO_MRST_EN_MASK);    // CTRL_REG2: RAW=0
  IIC_RegWrite(MAG3110_SlaveAddressIIC,MAG3110_CTRL_REG1,0x19);    // CTRL_REG1: Standby mode   
  mMAG3110TimerID = TMR_AllocateTimer();
}

void MAG3110_Start_Periodical_data(void) {
	TMR_StartIntervalTimer(mMAG3110TimerID, mMAG3110Interval_c,MAG3110_Periodical_data);
}

/*********************************************************\
* MAG3110 read data
\*********************************************************/
void MAG3110_Periodical_data(uint8_t timerId) 
{
		
  uint8_t rxData[7];
  volatile int16_t Read_data_16bit;
  
  (void) timerId; /* prevent compiler warning */

  
  rxData[0] = IIC_RegRead(MAG3110_SlaveAddressIIC,MAG3110_DR); // checking a STATUS-reg
  
  if( rxData[0] & 0x08){
      
	  IIC_RegReadN(MAG3110_SlaveAddressIIC,MAG3110_OUT_X_MSB,MAG3110_OUT_Z_LSB,&rxData[1]);      // Read data from $0x01 to 0x06
	  
	  pfMAG3110_CallBack();  // Set event in order to notify application in callback function.
	  
	  Read_data_16bit = (int16_t)(rxData[1]<<8);
	  mDataFrom_MAG3110.xOutReg =  ( Read_data_16bit + (int16_t)rxData[2] );
	 
	  Read_data_16bit = (int16_t)(rxData[3]<<8);
	  mDataFrom_MAG3110.yOutReg =  ( Read_data_16bit + (int16_t)rxData[4] ); 
	  
	  Read_data_16bit = (int16_t)(rxData[5]<<8);
	  mDataFrom_MAG3110.zOutReg =  ( Read_data_16bit + (int16_t)rxData[6] ); 
    

  }
  
}

/*********************************************************\
* int16_t MAG3110_CatchSensorData(uint8_t number)
\*********************************************************/
int16_t MAG3110_CatchSensorData(uint8_t number){
	
	int16_t catch_data;
	
	switch(number){
		case 1:
			catch_data = mDataFrom_MAG3110.xOutReg;
		break;
		case 2:
			catch_data = mDataFrom_MAG3110.yOutReg;
		break;
		case 3:
			catch_data = mDataFrom_MAG3110.zOutReg;
		break;
		default :
		break;
	}
	
    return catch_data;
    
}

/*********************************************************\
* void MAG3110_Init(void)
\*********************************************************/
void MAG3110_Init(void){
    MAG3110_SetCallBack(MAG3110_CallBack);  
    MAG3110_int();
    MAG3110_start_flag = 1;	
}

/*********************************************************\
* void MAG3110_CallBack(void) 
\*********************************************************/
void MAG3110_CallBack(void){
	
	TS_SendEvent(gAppTaskID_c, gAppEvt_FromMAG3110_c);
	
}

/*********************************************************\
* void MAG3110_dump(event_t events)
\*********************************************************/
void MAG3110_dump(event_t events){
	
	volatile int16_t tmp;
	uint8_t i;
	
	// Start to receive periodical data 
	if(MAG3110_start_flag){
		MAG3110_start_flag = 0;
		MAG3110_Start_Periodical_data();
	}
	
	if (events & gAppEvt_FromMAG3110_c)
		{   
			UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 
			
			for(i=1; i<4; i++){
				switch(i){			
					case 1:
						UartUtil_Print("MAG3110(16bit) \n\rX axis : 0x", gAllowToBlock_d); 
					break;
					case 2:
						UartUtil_Print(" *0.1 [uT]\n\rY axis : 0x", gAllowToBlock_d);  
					break;
					case 3:
						UartUtil_Print(" *0.1 [uT]\n\rZ axis : 0x", gAllowToBlock_d);  	
					break;
					default:
						UartUtil_Print("???????????????", gAllowToBlock_d);  		
					break;
				}//switch(i){	 	
				
				tmp = MAG3110_CatchSensorData(i); 	
				UartUtil_PrintHex((uint8_t *)&tmp, 2, 1); 	
				
				if(i==3){
					UartUtil_Print(" *0.1 [uT]", gAllowToBlock_d); 					
				}
				
			} //for(i=1; i<4; i++){
			
			
			UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 		
		
		}
	
}
	


