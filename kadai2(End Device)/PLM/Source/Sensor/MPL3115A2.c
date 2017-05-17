/***********************************************************************************************\
* Freescale MPL3115A2 Driver
 *
 * Filename: mma845x.c
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
#include "MPL3115A2.h"


static tmrTimerID_t mMPL3115A2TimerID = gTmrInvalidTimerID_c;

static dataFrom_MPL3115A2_t dataFrom_MPL3115A2 = 
{                                                                
  0x00, 
  0x00, 
  0x00,
  0x00
};

extern uint8_t MPL3115A2_start_flag;


/*********************************************************\
* MPL3115A2_SetCallBack
\*********************************************************/
void MPL3115A2_SetCallBack(void (*pfCallBack)(void)) {
	
	pfMPL3115A2CallBack = pfCallBack;
	  	  
}

/*********************************************************\
* Put MPL3115A2 into Active Mode
 \*********************************************************/
void MPL3115A2_Active(void) {
	uint8_t tmp_data;
	
	tmp_data = IIC_RegRead(MPL3115A2_SlaveAddressIIC, CTRL_REG1);
	tmp_data |= OST_MASK;
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG1,tmp_data);

}


/*********************************************************\
* void MPL3115A2_Init_Alt(void) 
 \*********************************************************/
void MPL3115A2_Init_Alt(void) {

	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG1,
			(ALT_MASK | OS2_MASK | OS1_MASK | OS0_MASK));
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG2, CLEAR_CTRLREG2);
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG3, (PP_OD1_MASK | PP_OD2_MASK));
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG4, INT_EN_CLEAR);
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG5, INT_CFG_CLEAR);
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, PT_DATA_CFG_REG,
			(DREM_MASK | PDEFE_MASK | TDEFE_MASK));

}

/*********************************************************\
* void MPL3115A2_Init_Bar(void) 
 \*********************************************************/
void MPL3115A2_Init_Bar(void) {
	
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG1, 0x00);  //stand-by mode
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, PT_DATA_CFG_REG,(DREM_MASK | PDEFE_MASK | TDEFE_MASK));	
	IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG1, OSR_128); //Barometer_mode, OSC 512ms
	MPL3115A2_Active();
	mMPL3115A2TimerID = TMR_AllocateTimer();

}

/*********************************************************\
* void MPL3115A2_Start_Periodical_data(void)
 \*********************************************************/
void MPL3115A2_Start_Periodical_data(void) {
	TMR_StartIntervalTimer(mMPL3115A2TimerID, mMPL3115A2Interval_c,MPL3115A2_Periodical_data);
}

/*********************************************************\
* void MPL3115A2_Periodical_data(uint8_t timerId)
 \*********************************************************/
void MPL3115A2_Periodical_data(uint8_t timerId) {

	volatile int32_t Read_data_long = 0;
	volatile uint8_t rxData[6];
	int32_t tmp;
	uint16_t wait_cnt =0;

	
	(void) timerId; /* prevent compiler warning */


    rxData[0] = IIC_RegRead(MPL3115A2_SlaveAddressIIC,STATUS_00_REG); // checking a STATUS-reg
 

	
	if(  rxData[0] & 0x08  ){
		 
		 pfMPL3115A2CallBack();  // Set event in order to notify application in callback function.
		 		 
		IIC_RegReadN(MPL3115A2_SlaveAddressIIC,OUT_P_MSB_REG, OUT_T_LSB_REG, (uint8_t *)&rxData[1]); // Read data from $0x01 to 0x06

		if (  rxData[0] & PDR_MASK  ) {		
			  tmp = (int32_t)(rxData[1]);
			  Read_data_long= tmp<<16;	  
			  Read_data_long = Read_data_long + (int32_t)(rxData[2]<<8);
			  dataFrom_MPL3115A2.Pressure_data=  ( Read_data_long + (int32_t)rxData[3] ) >>4;
		}

		if (   rxData[0] & TDR_MASK  ) {		
			  Read_data_long = (int32_t)(rxData[4]<<8);
			  dataFrom_MPL3115A2.Temparature_data=  ( Read_data_long + (int32_t)rxData[5] ) >>4;		
		}	
		
		IIC_RegWrite(MPL3115A2_SlaveAddressIIC, CTRL_REG1, (OSR_128|OST_MASK));  
	
	
	}

	
}

/*********************************************************\
* uint32_t MPL3115A2_CatchSensorData(uint8_t number)
\*********************************************************/
uint32_t MPL3115A2_CatchSensorData(uint8_t number){
	
	volatile uint32_t catch_data;
	
	switch(number){
		case 1:
			catch_data = dataFrom_MPL3115A2.Pressure_data;
		break;
		case 2:
			catch_data = dataFrom_MPL3115A2.Temparature_data;
		break;
		default :
		break;
	}
	
    return catch_data;
    
}

/*********************************************************\
* void MPL3115A2_PrintALT(void) 
\*********************************************************/
void MPL3115A2_PrintALT(void){

	volatile static uint8_t read_data;

	UartUtil_Print(" MPL_PrintALT\n\r", gAllowToBlock_d);
	read_data = IIC_RegRead(MPL3115A2_SlaveAddressIIC, OUT_P_MSB_REG);
	UartUtil_PrintHex((void *) &read_data, 1, FALSE);
	read_data = IIC_RegRead(MPL3115A2_SlaveAddressIIC, OUT_P_CSB_REG);
	read_data = IIC_RegRead(MPL3115A2_SlaveAddressIIC, OUT_P_LSB_REG);
}

/*********************************************************\
* void MPL3115A2_PrintTEMP(void) 
\*********************************************************/
void MPL3115A2_PrintTEMP(void) {
	volatile static uint8_t read_data;

	read_data = IIC_RegRead(MPL3115A2_SlaveAddressIIC, OUT_T_MSB_REG);
	read_data = IIC_RegRead(MPL3115A2_SlaveAddressIIC, OUT_T_LSB_REG);
}

/*********************************************************\
* void MPL3115A2_Init(void)
\*********************************************************/
void MPL3115A2_Init(void){
	MPL3115A2_SetCallBack(MPL3115A2_CallBack);  
	MPL3115A2_Init_Bar();
	MPL3115A2_start_flag = 1;
}

/*********************************************************\
* void MPL3115A2_CallBack(void) 
\*********************************************************/
void MPL3115A2_CallBack(void){	
	TS_SendEvent(gAppTaskID_c, gAppEvt_FromMPL3115A2_c);	
}

/*********************************************************\
* void MPL3115A2_dump(event_t events) 
\*********************************************************/
void MPL3115A2_dump(event_t events){

	volatile int32_t tmp;
	volatile uint32_t tmp_unsigned;
	
 //-----------------------------------------------------     
	// Start to receive periodical data 
	if(MPL3115A2_start_flag){
		MPL3115A2_start_flag = 0;
		MPL3115A2_Start_Periodical_data();
	}
	
	if (events & gAppEvt_FromMPL3115A2_c)
	{   
		UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 
		UartUtil_Print("MPL3115A2(P: Int18,Float2, T: Int8, Float4)  \n\rPress  : 0x", gAllowToBlock_d);  
		tmp = MPL3115A2_CatchSensorData(1);

		tmp_unsigned = (uint32_t)tmp;	
		UartUtil_PrintHex((uint8_t *)&tmp_unsigned, 4, 1);  	
		
#if 0		
		tmp_integer = tmp>>2;     // Integer bit
		tmp_float   = tmp & 0x03; // float bit
		
		UartUtil_PrintDec(tmp_integer, 4, 0);		
		UartUtil_Print(".", gAllowToBlock_d);	
		UartUtil_PrintDec(tmp_float, 4, 0);		
#endif		
		UartUtil_Print(" /4  [Pa]", gAllowToBlock_d);			
		
		tmp = MPL3115A2_CatchSensorData(2);
#if 0	
		tmp_integer = tmp>>4;     // Integer bit
		tmp_float   = tmp & 0x0F; // float bit
		UartUtil_PrintDec(tmp_integer, 4, 0);
		UartUtil_Print(".", gAllowToBlock_d);
		UartUtil_PrintDec(tmp_float, 4, 0);	
#endif		
		
		UartUtil_Print("\n\rTemp   : 0x", gAllowToBlock_d);  	
		tmp_unsigned = (uint32_t)tmp;	
		UartUtil_PrintHex((uint8_t *)&tmp_unsigned, 4, 1);  			
		UartUtil_Print(" /16 [C]", gAllowToBlock_d);
		UartUtil_Print("\n\r-----------------------------------------------\n\r", gAllowToBlock_d); 
		
		
	} 
	
	

	
}


/******************************************************
 * *****************************************\
* Private functions
 \***********************************************************************************************/
