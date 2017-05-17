/***********************************************************************************************\
* Freescale MMA9553 Driver
*
* Filename: MMA9553.h
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
\***********************************************************************************************/
#ifndef _MMA9553_H_
#define _MMA9553_H_



/***********************************************************************************************\
* Public prototypes
\***********************************************************************************************/
// prptptype
// MMA9553
uint8_t MMA9553_int(void);
void MMA9553_WakeUp(void);            // Wake up from Deep Sleep
void MMA9553_Disable(void);
void MMA9553_Enable(void);
void MMA9553_Periodical_data(uint8_t timerId);
int16_t MMA9553_CatchSensorData(uint8_t number);
void MMA9553_Start_Periodical_data(void);
void MMA9553_Init(void);
void MMA9553_dump(event_t events);
void MMA9553_SetCallBack(void (*pfCallBack)(void));
void MMA9553_CallBack(void);
uint8_t MMA9553_SetConfig(void);
uint8_t MMA9553_Run(void);
void  (*pfMMA9553CallBack)(void);


/***********************************************************************************************\
* Public macros
\***********************************************************************************************/

/* Data struct received from ACC */
typedef struct dataFrom_MMA9553_tag {
  int16_t Act_data;
  int16_t Step_data;
  int16_t Dist_data;
  int16_t Speed_data;
  int16_t Cal_data;  
  int16_t Sleep_cnt_data;    
}dataFrom_MMA9553_t;


#ifndef mMMA9553Interval_c
#define mMMA9553Interval_c  1000
#endif
	
#define MMA9553_IIC_ADDRESS   ( 0x4C<<1 )     /*MMA9553 slave Address*/

// MMA9553:Parameter for a pedometer 
#define MMA9553_Mailbox 0  // Destination            Mailbox Address
#define MMA9553_MB4  0x0E  // Sleep minimum          Hi 0x0E74(3700)*0.244= 903[mg]
#define MMA9553_MB5  0x74  //                        Lo
#define MMA9553_MB6  0x11  // Sleep Maximum          Hi 0x1194(4500)*0.244=1098[mg]
#define MMA9553_MB7  0x94  //                        Lo
#define MMA9553_MB8  0x00  // Sleep Theshold         Hi 0x007F=127[sample]
#define MMA9553_MB9  0x7F  //                        Lo
#define MMA9553_MB10 0xA0  // Config,Step            Hi Re-initialaize, suspend-counter=clear mode
#define MMA9553_MB11 0x00  //                        Lo
#define MMA9553_MB12 0xAF  // Height                 175[cm]
#define MMA9553_MB13 0x50  // Weight                 80[kg]
#define MMA9553_MB14 0x04  // Filter Steps           4[steps]               
#define MMA9553_MB15 0x03  // Gender,Filter Time     Female, 3[sec]
#define MMA9553_MB16 0x05  // Speed Perios           5[sec] Valid range: 0x02:0x05. 
#define MMA9553_MB17 0x10  // Coalesce Perios        16[steps] 
#define MMA9553_MB18 0x00  // Activity count         Hi 0x000A=10[steps]
#define MMA9553_MB19 0x0A  //                        Lo 




#endif  /* _MPL3115_A2 */
