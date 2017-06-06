/***********************************************************************************************\
* Freescale MPL3115A2 Driver
*
* Filename: mpl3115A2.h
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
\***********************************************************************************************/
#ifndef _MMA845X_H_
#define _MMA845X_H_



/***********************************************************************************************\
* Public prototypes0
\***********************************************************************************************/
void MPL3115A2_Active(void);
void MPL3115A2_Init_Alt(void);
void MPL3115A2_Init_Bar(void);
void MPL3115A2_PrintTEMP(void);
void MPL3115A2_Periodical_data(uint8_t timerId);
void MPL3115A2_PrintALT(void);
void MPL3115A2_Start_Periodical_data(void);
uint32_t MPL3115A2_CatchSensorData(uint8_t number);
void MPL3115A2_Init(void);
void MPL3115A2_CallBack(void);
void MPL3115A2_dump(event_t events);
void  (*pfMPL3115A2CallBack)(void);



/* Data struct received from ACC */
typedef struct dataFrom_MPL3115A2_tag {
  int32_t Pressure_data;
  int32_t Temparature_data;
  uint8_t samplingRateStatustReg;
  uint8_t modeReg;
}dataFrom_MPL3115A2_t;


#define MPL3115A2_SlaveAddressIIC ( 0x60<<1 )

/***********************************************************************************************
**
**  MPL3115A2 Sensor Internal Registers
*/
enum
{
  MPL3115A2_STATUS_00 = 0,          // 0x00
  MPL3115A2_OUT_P_MSB,              // 0x01
  MPL3115A2_OUT_P_CSB,              // 0x02
  MPL3115A2_OUT_P_LSB,              // 0x03
  MPL3115A2_OUT_T_MSB,              // 0x04
  MPL3115A2_OUT_T_LSB,              // 0x05
  MPL3115A2_OUT_DR_STATUS,          // 0x06
  MPL3115A2_OUT_P_DELTA_MSB,        // 0x07
  MPL3115A2_OUT_P_DELTA_CSB,        // 0x08
  MPL3115A2_OUT_P_DELTA_LSB,        // 0x09
  MPL3115A2_OUT_T_DELTA_MSB,        // 0x0A
  MPL3115A2_OUT_T_DELTA_LSB,        // 0x0B
  MPL3115A2_WHO_AM_I,               // 0x0C
  MPL3115A2_F_STATUS,               // 0x0D
  MPL3115A2_F_DATA,                 // 0x0E
  MPL3115A2_F_SETUP,                // 0x0F
  MPL3115A2_TIME_DLY,               // 0x10
  MPL3115A2_SYSMOD,                 // 0x11
  MPL3115A2_INT_SOURCE,             // 0x12
  MPL3115A2_PT_DATA_CFG,            // 0x13
  MPL3115A2_BAR_IN_MSB,             // 0x14
  MPL3115A2_BAR_IN_LSB,             // 0x15
  MPL3115A2_P_ARM_MSB,              // 0x16
  MPL3115A2_P_ARM_LSB,              // 0x17
  MPL3115A2_T_ARM,                  // 0x18
  MPL3115A2_P_ARM_WND_MSB,          // 0x19
  MPL3115A2_P_ARM_WND_LSB,          // 0x1A
  MPL3115A2_T_ARM_WND,              // 0x1B
  MPL3115A2_P_MIN_MSB,              // 0x1C
  MPL3115A2_P_MIN_CSB,              // 0x1D
  MPL3115A2_P_MIN_LSB,              // 0x1E
  MPL3115A2_T_MIN_MSB,              // 0x1F
  MPL3115A2_T_MIN_LSB,              // 0x20
  MPL3115A2_P_MAX_MSB,              // 0x21
  MPL3115A2_P_MAX_CSB,              // 0x22
  MPL3115A2_P_MAX_LSB,              // 0x23
  MPL3115A2_T_MAX_MSB,              // 0x24
  MPL3115A2_T_MAX_LSB,              // 0x25
  MPL3115A2_CTRL_REG1,              // 0x26
  MPL3115A2_CTRL_REG2,              // 0x27
  MPL3115A2_CTRL_REG3,              // 0x28
  MPL3115A2_CTRL_REG4,              // 0x29
  MPL3115A2_CTRL_REG5,              // 0x2A
  MPL3115A2_OFF_P,                  // 0x2B
  MPL3115A2_OFF_T,                  // 0x2C
  MPL3115A2_OFF_H                   // 0x2D
};

/*
**  STATUS Registers
*/
#define STATUS_00_REG         0x00
#define DR_STATUS_00_REG      0x06
//
#define PTOW_BIT              Bit._7
#define POW_BIT               Bit._6
#define TOR_BIT               Bit._5

#define PTDR_BIT              Bit._3
#define PDR_BIT               Bit._2
#define TDR_BIT               Bit._1

#define PDR_MASK              0x04
#define TDR_MASK              0x02
//
#define ZYXOW_MASK            0x80
#define ZOW_MASK              0x40
#define YOR_MASK              0x20
#define XOR_MASK              0x10
#define ZYXDR_MASK            0x08
#define ZDR_MASK              0x04
#define YDR_MASK              0x02
#define XDR_MASK              0x01

/*
**  8-Bit OUT Data Registers
*/
#define OUT_P_MSB_REG        0x01
#define OUT_P_CSB_REG        0x02
#define OUT_P_LSB_REG        0x03
#define OUT_T_MSB_REG        0x04
#define OUT_T_LSB_REG        0x05
                             
#define OUT_P_DELTA_MSB  0x07
#define OUT_P_DELTA_CSB  0x08
#define OUT_P_DELTA_LSB  0x09
#define OUT_T_DELTA_MSB  0x0A
#define OUT_T_DELTA_LSB  0x0B
#define WHO_AM_I         0x0C

/*
**  F_STATUS FIFO Status Register
*/
#define F_STATUS_REG          0x0D
//
#define F_OVF_BIT             Bit._7
#define F_WMRK_FLAG_BIT       Bit._6
#define F_CNT5_BIT            Bit._5
#define F_CNT4_BIT            Bit._4
#define F_CNT3_BIT            Bit._3
#define F_CNT2_BIT            Bit._2
#define F_CNT1_BIT            Bit._1
#define F_CNT0_BIT            Bit._0
//
#define F_OVF_MASK            0x80
#define F_WMRK_FLAG_MASK      0x40
#define F_CNT5_MASK           0x20
#define F_CNT4_MASK           0x10
#define F_CNT3_MASK           0x08
#define F_CNT2_MASK           0x04
#define F_CNT1_MASK           0x02
#define F_CNT0_MASK           0x01
#define F_CNT_MASK            0x3F

/*
**  F_8DATA 8-Bit FIFO Data Register
*/
#define F_DATA_REG            0x0E


/*
**  F_SETUP FIFO Setup Register
*/
#define F_SETUP_REG           0x0F
//
#define F_MODE1_BIT           Bit._7
#define F_MODE0_BIT           Bit._6
#define F_WMRK5_BIT           Bit._5
#define F_WMRK4_BIT           Bit._4
#define F_WMRK3_BIT           Bit._3
#define F_WMRK2_BIT           Bit._2
#define F_WMRK1_BIT           Bit._1
#define F_WMRK0_BIT           Bit._0
//
#define F_MODE01_MASK         0x40
#define F_MODE10_MASK         0x80
#define F_WMRK5_MASK          0x20
#define F_WMRK4_MASK          0x10
#define F_WMRK3_MASK          0x08
#define F_WMRK2_MASK          0x04
#define F_WMRK1_MASK          0x02
#define F_WMRK0_MASK          0x01
#define F_MODE_MASK           0xC0
#define F_WMRK_MASK           0x3F
#define F_CLEAR_MASK          0x00

#define TIME_DELAY            0x10

/*
**  SYSMOD System Mode Register
*/
#define SYSMOD_REG            0x11
//

#define FGERR_BIT             Bit._6

#define SYSMOD0_BIT           Bit._0
//

#define SYSMOD_MASK           0x03

/*
**  INT_SOURCE System Interrupt Status Register
*/
#define INT_SOURCE_REG        0x12
//
#define SRC_DRDY_BIT      Bit._7
#define SRC_FIFO_BIT      Bit._6
#define SRC_PW            Bit._5
#define SRC_TW            Bit._4
#define SRC_PTH           Bit._3
#define SRC_TTH           Bit._2
#define SRC_PCHG          Bit._1
#define SRC_TCHG          Bit._0
//
#define SRC_DRDY_MASK     0x80
#define SRC_FIFO_MASK     0x40
#define SRC_P_MASK        0x20
#define SRC_T_MASK        0x10
#define SRC_PW_MASK       0x08
#define SRC_TW_MASK       0x04
#define SRC_PCHG_MASK     0x02
#define SRC_TCHG_MASK     0x01

/*
**  PT_DATA_CFG Sensor Data Configuration Register
*/
#define PT_DATA_CFG_REG      0x13
//
#define DREM_BIT             Bit._2
#define PDEFE_BIT            Bit._1
#define TDEFE_BIT            Bit._0
//
#define DREM_MASK            0x04
#define PDEFE_MASK           0x02
#define TDEFE_MASK           0x01

/*
**  BAR IN 
*/
#define BAR_IN_MSB      0x14
#define BAR_IN_LSB      0x15


/*
**  BAR IN 
*/
#define BAR_IN_MSB      0x14
#define BAR_IN_LSB      0x15

/*
**  Pressure/Altitude Alarm Registers
*/
#define P_TGT_MSB       0x16
#define P_TGT_LSB       0x17


/*
**  Pressure/Altitude Alarm Window Registers
*/
#define P_TGT_WND_MSB   0x19
#define P_TGT_WND_LSB   0x1A

/*
**  Temperature Alarm Register
*/
#define T_TGT           0x18

/*
**  Temperature Alarm Window Register
*/
#define T_TGT_WND       0x1B

/* Interval time for toggle LED which is used for flashing LED (0 - 262140ms) */
#ifndef mMPL3115A2Interval_c
#define mMPL3115A2Interval_c  700
#endif

/*
**  CTRL_REG1 Interrupt Control Register
*/
#define CTRL_REG1       0x26
//
#define ALT_BIT         Bit._7
#define RAW_BIT         Bit._6
#define OS2_BIT         Bit._5
#define OS1_BIT         Bit._4
#define OS0_BIT         Bit._3
#define RST_BIT         Bit._2
#define OST_BIT         Bit._1
#define SBYB_BIT        Bit._0
//
#define ALT_MASK        0x80
#define RAW_MASK        0x40
#define OS2_MASK        0x20
#define OS1_MASK        0x10
#define OS0_MASK        0x08
#define RST_MASK        0x04
#define OST_MASK        0x02
#define SBYB_MASK       0x01

#define STANDBY_SBYB_0        0xFE
#define ACTIVE_MASK           0x01
#define RESET_MASK            0x04
#define ACTIVE_SBYB_OST       OST_MASK+SBYB_MASK
#define FULL_SCALE_STANDBY    0x00
#define CLEAR_OSR             0xC3
#define OSR_2                 0x08
#define OSR_4                 0x10
#define OSR_8                 0x18
#define OSR_16                0x20
#define OSR_32                0x28
#define OSR_64                0x30
#define OSR_128               0x38

#define DR_MASK               0x38
#define MODE_MASK             0x80
#define CLEAR_MODE_MASK       0x7F

/*
**  CTRL_REG2 Interrupt Control Register
*/
#define CTRL_REG2       0x27
//
#define FIFO_GATE_BIT   Bit._4
#define ST3_BIT         Bit._3
#define ST2_BIT         Bit._2
#define ST1_BIT         Bit._1
#define ST0_BIT         Bit._0
//
#define FIFO_GATE_MASK  0x10
#define ST3_MASK        0x08
#define ST2_MASK        0x04
#define ST1_MASK        0x02
#define ST0_MASK        0x01
#define ST_MASK         0x0F
#define CLEAR_ST_MASK   0xF0
#define CLEAR_CTRLREG2  0x00

/*
**  CTRL_REG3 Interrupt Control Register
*/
#define CTRL_REG3       0x28
//

#define IPOL1_BIT       Bit._5
#define PP_OD1_BIT      Bit._4
#define IPOL2_BIT       Bit._1
#define PP_OD2_BIT      Bit._0
//

#define IPOL1_MASK      0x20
#define PP_OD1_MASK     0x10
#define IPOL2_MASK      0x02
#define PP_OD2_MASK     0x01

/*
**  CTRL_REG4 Interrupt Control Register
*/
#define CTRL_REG4       0x29
//
#define INT_EN_DRDY_BIT  Bit._7
#define INT_EN_FIFO_BIT  Bit._6
#define INT_EN_PW_BIT    Bit._5
#define INT_EN_TW_BIT    Bit._4
#define INT_EN_PTH_BIT   Bit._3
#define INT_EN_TTH_BIT   Bit._2
#define INT_EN_PCHG_BIT  Bit._1
#define INT_EN_TCHG_BIT  Bit._0
//
#define INT_EN_CLEAR         0x00
#define INT_EN_DRDY_MASK     0x80
#define INT_EN_FIFO_MASK     0x40
#define INT_EN_PW_MASK       0x20
#define INT_EN_TW_MASK       0x10
#define INT_EN_PTH_MASK      0x08
#define INT_EN_TTH_MASK      0x04
#define INT_EN_PCHG_MASK     0x02
#define INT_EN_TCHG_MASK     0x01

/*
**  CTRL_REG5 Interrupt Control Register
*/
#define CTRL_REG5             0x2A
//
#define INT_CFG_CLEAR         0x00
#define INT_CFG_DRDY_MASK     0x80
#define INT_CFG_FIFO_MASK     0x40
#define INT_CFG_PW_MASK       0x20
#define INT_CFG_TW_MASK       0x10
#define INT_CFG_PTH_MASK      0x08
#define INT_CFG_TTH_MASK      0x04
#define INT_CFG_PCHG_MASK     0x02
#define INT_CFG_TCHG_MASK     0x01





#endif  /* _MPL3115_A2 */
