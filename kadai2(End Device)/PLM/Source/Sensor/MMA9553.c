/***********************************************************************************************\
* Freescale MMA9553 Driver
*
* Filename: MMA9553.c
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
#include "MMA9553.h"


/***********************************************************************************************\
*
\***********************************************************************************************/
static tmrTimerID_t mMMA9553TimerID = gTmrInvalidTimerID_c;

extern uint8_t MMA9553_start_flag;



// *********************************************************
//  void MMA9553_SetCallBack(void (*pfCallBack)(void))
// *********************************************************
void MMA9553_SetCallBack(void (*pfCallBack)(void)) {
	
	pfMMA9553CallBack = pfCallBack;
	  	  
}

// ================================================
// void MMA9553_Init(void)
// ================================================
void MMA9553_Init(void){
  uint8_t err_cntr;
	
  MMA9553_SetCallBack(MMA9553_CallBack);  
  err_cntr = 0;
  
  while( 0x00 != MMA9553_int() ){
	   err_cntr++;
  }
  
  MMA9553_start_flag = 1;	
  mMMA9553TimerID = TMR_AllocateTimer();
}


// ==================================
// MMA9553_int
// ==================================
static dataFrom_MMA9553_t dataFrom_MMA9553 = {                                                                
  0x00, 
  0x00, 
  0x00,
  0x00,
  0x00,
  0x00
};

/*********************************************************\
* uint8_t MMA9553_int(void)
\*********************************************************/
uint8_t MMA9553_int(void){  
  uint8_t error = 0;
  
  MMA9553_WakeUp();       // Wake up from Deep Sleep
  MMA9553_Disable();      // Disable the Pedometer application
  error = MMA9553_Run();
   
  if(error==0){
    MMA9553_Enable();     // Enable the Pedometer application
  }
  
  return error;
}

// ================================================
// uint8_t MMA9553_Run(void)
// ================================================
uint8_t MMA9553_Run(void){
	
  uint8_t error=1;
  uint8_t i;
  
  for( i=0; i<20; i++ ){
    if(error > 0){
      error = MMA9553_SetConfig();
    }//if(res==1)
    else{
      error = 0;
      break;        // config done.
    }
  }
  return error;       // 0=good
}

// ================================================
// void MMA9553_CallBack(void) 
// ================================================
void MMA9553_CallBack(void){	
	TS_SendEvent(gAppTaskID_c, gAppEvt_FromMMA9553_c);	
}


// ================================================
// void MMA9553_Start_Periodical_data(void)
// ================================================
void MMA9553_Start_Periodical_data(void) {
	TMR_StartIntervalTimer( mMMA9553TimerID, mMMA9553Interval_c,MMA9553_Periodical_data );
}

// ================================================
// int16_t MMA9553_CatchSensorData(uint8_t number)
// ================================================
int16_t MMA9553_CatchSensorData(uint8_t number){
	
	volatile int16_t catch_data;
	  	  
	switch(number){
		case 1:
			catch_data = dataFrom_MMA9553.Act_data;
		break;
		case 2:
			catch_data = dataFrom_MMA9553.Step_data;
		break;
		case 3:
			catch_data = dataFrom_MMA9553.Dist_data;
		break;
		case 4:
			catch_data = dataFrom_MMA9553.Speed_data;
		break;
		case 5:
			catch_data = dataFrom_MMA9553.Cal_data;
		break;
		case 6:
			catch_data = dataFrom_MMA9553.Sleep_cnt_data;
		break;
		default :
		break;
	}
	
    return catch_data;
    
}

// ================================================
// void MMA9553_Periodical_data(uint8_t timerId)
// ================================================
void MMA9553_Periodical_data(uint8_t timerId)
{

	volatile int16_t Read_data_16bit = 0;
	volatile uint8_t dumpstatus_data[4]={0x15, 0x30, 0x00, 0x0C};
	volatile uint8_t rxData[20];
	volatile uint8_t check_status = 0;
	uint8_t  i;
	
	(void) timerId; /* prevent compiler warning */
	

	for(i=0; i<20; i++){  // retry
		IIC_RegWriteN(MMA9553_IIC_ADDRESS,MMA9553_Mailbox,4,(uint8_t *)&dumpstatus_data);
		IIC_RegReadN(MMA9553_IIC_ADDRESS,0x00,16,(uint8_t *)&rxData); // Read data 
	    check_status =((rxData[1]&0x80)==0x80) & (rxData[2]==0x0C);   // check COCO and num of read bytes 		 
	    if(check_status){
	      break;          // if no-error 
	    }	    	
	}
	
	while(check_status == 0x00){
		// if it stays, error occurred....
	}
	

	pfMMA9553CallBack();  // Set event in order to notify application in callback function.

	Read_data_16bit =  (int16_t)( rxData[4] & 0x07 );
	dataFrom_MMA9553.Act_data=  Read_data_16bit;
	
	Read_data_16bit = (int16_t)(rxData[6]<<8);
	dataFrom_MMA9553.Step_data = ( Read_data_16bit + (int16_t)rxData[7] );	

	Read_data_16bit = (int16_t)(rxData[8]<<8);
	dataFrom_MMA9553.Dist_data = ( Read_data_16bit + (int16_t)rxData[9] );	
	
	Read_data_16bit = (int16_t)(rxData[10]<<8);
	dataFrom_MMA9553.Speed_data = ( Read_data_16bit + (int16_t)rxData[11] );	

	Read_data_16bit = (int16_t)(rxData[12]<<8);
	dataFrom_MMA9553.Cal_data = ( Read_data_16bit + (int16_t)rxData[13] );	

	Read_data_16bit = (int16_t)(rxData[14]<<8);
	dataFrom_MMA9553.Sleep_cnt_data = ( Read_data_16bit + (int16_t)rxData[15] );	
	
}

// ==================================
// MMA9553_WakeUp
// ==================================
void MMA9553_WakeUp(void){
  const uint8_t wakeup_data[5]={0x12,0x20,0x06,0x01,0x00};
  IIC_RegWriteN(MMA9553_IIC_ADDRESS,MMA9553_Mailbox,5,(uint8_t *)&wakeup_data);
}


// ==================================
// MMA9553_Disable
// ==================================
void MMA9553_Disable(void){
	const uint8_t disable_data[5]={0x17,0x20,0x05,0x01,0x20};
	IIC_RegWriteN(MMA9553_IIC_ADDRESS,MMA9553_Mailbox,5,(uint8_t *)&disable_data);
}


// ==================================
// MMA9553_SetConfig
// ==================================
uint8_t MMA9553_SetConfig(void){
	
  volatile uint8_t i;
  volatile uint8_t error;
	
  const uint8_t config2_data[20]={
					0x15, 0x20, 0x00, 0x10,
					MMA9553_MB4 , MMA9553_MB5 , MMA9553_MB6 , MMA9553_MB7 , 
					MMA9553_MB8 , MMA9553_MB9 , MMA9553_MB10, MMA9553_MB11,
					MMA9553_MB12, MMA9553_MB13, MMA9553_MB14, MMA9553_MB15, 
					MMA9553_MB16, MMA9553_MB17, MMA9553_MB18, MMA9553_MB19
  	  	  	  	};  // config write data
	
  volatile uint8_t rxData[20]={ 0x15, 0x10, 0x00, 0x10 }; // config read data
 
  IIC_RegWriteN(MMA9553_IIC_ADDRESS,MMA9553_Mailbox,20,(uint8_t *)&config2_data);// config write
  IIC_RegWriteN(MMA9553_IIC_ADDRESS,MMA9553_Mailbox,4,(uint8_t *)&rxData); // config read,command
  IIC_RegReadN(MMA9553_IIC_ADDRESS,0x00,20,(uint8_t *)&rxData);            // config read,data 
  
  error = 0;
  
  for(i=4; i<19; i++){
    if(config2_data[i]!=rxData[i]){
        error++;	
    } // if(config2_data[i]!=rxData[i]){  
  } //for(i=4; i<19; i++){
  
  return error;  // 0=Good
  
  
}


// ==================================
// void MMA9553_Enable(void)
// ==================================
void MMA9553_Enable(void){
	 const uint8_t enable_data[5]={0x17,0x20,0x05,0x01,0x00};
	 IIC_RegWriteN(MMA9553_IIC_ADDRESS,MMA9553_Mailbox,5,(uint8_t *)&enable_data);
}


// ==================================
// void MMA9553_dump(event_t events)
// ==================================
void MMA9553_dump(event_t events){
	
	volatile int16_t tmp;
	uint8_t i;
	
	// Start to receive periodical data 
	if(MMA9553_start_flag){
		MMA9553_start_flag = 0;
		MMA9553_Start_Periodical_data();
	}
	
	if (events & gAppEvt_FromMMA9553_c)
	{          	


		UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 
		
		for(i=1; i<7; i++){
			switch(i){			
				case 1:
					UartUtil_Print("MMA9553 \n\rX Act_data : 0x", gAllowToBlock_d); 
				break;
				case 2:
					UartUtil_Print("\n\rX Step_data : 0x", gAllowToBlock_d); 
				break;	
				case 3:
					UartUtil_Print("\n\rX Dist_data : 0x", gAllowToBlock_d); 
				break;	
				case 4:
					UartUtil_Print("\n\rX Speed_data : 0x", gAllowToBlock_d); 
				break;	
				case 5:
					UartUtil_Print("\n\rX Cal_data : 0x", gAllowToBlock_d); 
				break;	
				case 6:
					UartUtil_Print("\n\rX Sleep_cnt_data : 0x", gAllowToBlock_d); 
				break;	
				default:
				break;						
			} // switch(i){	
			    tmp = (uint16_t)MMA9553_CatchSensorData(i);
				UartUtil_PrintHex((uint8_t *)&tmp, 2, 1); 				
			
		} // for(i=1; i<7; i++){
		
		UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 
				
		
	} // if (events & gAppEvt_FromMMA9553_c)
}


/***********************************************************************************************\
* Private functions
\***********************************************************************************************/

