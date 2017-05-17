/***********************************************************************************************\
* Freescale MAG3110 Driver
*
* Filename: MAG3110.h
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
\***********************************************************************************************/
#ifndef _FXAS21000_H_
#define _FXAS21000_H_


/***********************************************************************************************\
* Public macros
\***********************************************************************************************/

#define FXAS21000_SlaveAddressIIC   (0x20<<1) 

#define FXAS21000_sensitivity  0.2   // [dps/LSB]@1600[dps]mode


#ifndef mFXAS21000Interval_c
#define mFXAS21000Interval_c   100// min 
#endif

/***********************************************************************************************\
* Public prototypes
\***********************************************************************************************/

void FXAS21000_int(void);
void FXAS21000_SetCallBack(void (*pfCallBack)(void));
void FXAS21000_Start_Periodical_data(void);
void FXAS21000_Periodical_data(uint8_t timerId);
int16_t FXAS21000_CatchSensorData(uint8_t number);
void FXAS21000_Init(void);
void FXAS21000_CallBack(void);
void FXAS21000_dump(event_t events);
void  (*pfFXAS21000_CallBack)(void);

/* Data struct received from ACC */
typedef struct dataFrom_FXAS21000_tag {
  int16_t xOutReg;
  int16_t yOutReg;
  int16_t zOutReg;
  uint8_t samplingRateStatustReg;
  uint8_t modeReg;
}dataFrom_FXAS21000_t;


/***********************************************************************************************
**
**  MAG3110 Sensor Internal Registers
*/
/***********************************************************************************************\
* Public memory declarations
\***********************************************************************************************/


enum fxas21000_regmap_tag {
    FXAS21000_STATUS = 0,       /*!< Alias for ::FXAS21000_DR_STATUS or ::FXAS21000_F_STATUS */
    FXAS21000_OUT_X_MSB,        /*!< 14-bit X-axis measurement data bits 13:6 */
    FXAS21000_OUT_X_LSB,        /*!< 14-bit X-axis measurement data bits 5:0 */
    FXAS21000_OUT_Y_MSB,        /*!< 14-bit Y-axis measurement data bits 13:6 */
    FXAS21000_OUT_Y_LSB,        /*!< 14-bit Y-axis measurement data bits 5:0 */
    FXAS21000_OUT_Z_MSB,        /*!< 14-bit Z-axis measurement data bits 13:6 */
    FXAS21000_OUT_Z_LSB,        /*!< 14-bit Z-axis measurement data bits 5:0 */
    FXAS21000_DR_STATUS,        /*!< Data ready status information */
    FXAS21000_F_STATUS,         /*!< FIFO status */
    FXAS21000_F_SETUP,          /*!< FIFO setup */
    FXAS21000_F_EVENT,          /*!< FIFO event */
    FXAS21000_INT_SOURCE_FLAG,  /*!< Interrupt event source status flags */
    FXAS21000_WHO_AM_I,         /*!< Device ID */
    FXAS21000_CTRL_REG0,        /*!< Control register 0: full-scale range selection, highpass filter control, SPI mode selection */
    FXAS21000_RT_CFG,           /*!< Rate threshold function configuration */
    FXAS21000_RT_SRC,           /*!< Rate threshold event flags status register */
    FXAS21000_RT_THS,           /*!< Rate threshold function threshold value register */
    FXAS21000_COUNT,            /*!< Rate threshold function debounce counter */
    FXAS21000_TEMP,             /*!< Device temperature in degrees C */
    FXAS21000_CTRL_REG1,        /*!< Control register 1: operating mode, ODR selection, self-test, and reset control */
    FXAS21000_CTRL_REG2,        /*!< Control register 2: interrupt configuration settings */
};

// DR_STATUS
#define FXAS21000_DR_STATUS_ZYXDR_SHIFT  3
#define FXAS21000_DR_STATUS_ZYXDR_MASK   (1 << FXAS21000_DR_STATUS_ZYXDR_SHIFT)

// CTRL_REG0
#define FXAS21000_CTRL_REG0_FS_SHIFT     0
#define FXAS21000_CTRL_REG0_HPF_EN_SHIFT 2
#define FXAS21000_CTRL_REG0_SEL_SHIFT    3
#define FXAS21000_CTRL_REG0_SPIW_SHIFT   5

#define FXAS21000_CTRL_REG0_FS_MASK      (3 << FXAS21000_CTRL_REG0_FS_SHIFT)
#define FXAS21000_CTRL_REG0_HPF_EN_MASK  (1 << FXAS21000_CTRL_REG0_HPF_EN_SHIFT)
#define FXAS21000_CTRL_REG0_SEL_MASK     (3 << FXAS21000_CTRL_REG0_SEL_SHIFT)
#define FXAS21000_CTRL_REG0_SPIW_MASK    (1 << FXAS21000_CTRL_REG0_SPIW_SHIFT)

#define FXAS21000_CTRL_REG0_FS(x)        (((x) << FXAS21000_CTRL_REG0_FS_SHIFT) & FXAS21000_CTRL_REG0_FS_MASK)
#define FXAS21000_CTRL_REG0_SEL(x)       (((x) << FXAS21000_CTRL_REG0_SEL_SHIFT) & FXAS21000_CTRL_REG0_SEL_MASK)

// CTRL_REG1
#define FXAS21000_CTRL_REG1_READY_SHIFT  0
#define FXAS21000_CTRL_REG1_ACTIVE_SHIFT 1
#define FXAS21000_CTRL_REG1_DS_SHIFT     2
#define FXAS21000_CTRL_REG1_RST_SHIFT    6

#define FXAS21000_CTRL_REG1_READY_MASK   (1 << FXAS21000_CTRL_REG1_READY_SHIFT)
#define FXAS21000_CTRL_REG1_ACTIVE_MASK  (1 << FXAS21000_CTRL_REG1_ACTIVE_SHIFT)
#define FXAS21000_CTRL_REG1_DS_MASK      (7 << FXAS21000_CTRL_REG1_DS_SHIFT)
#define FXAS21000_CTRL_REG1_RST_MASK     (1 << FXAS21000_CTRL_REG1_RST_SHIFT)

#define FXAS21000_CTRL_REG1_DS(x)        (((x) << FXAS21000_CTRL_REG1_DS_SHIFT) & FXAS21000_CTRL_REG1_DS_MASK)

// WHO_AM_I
#define FXAS21000_WHO_AM_I_VALUE         0xD1

/**
 * @brief FXAS21000 internal error codes
 */
enum fxas21000_error_tag {
    FXAS21000_ERR_WHOAMI,          /*!< Error occurred while reading ::FXAS21000_WHO_AM_I or the value was incorrect */
    FXAS21000_ERR_WRITECTRLREG,    /*!< Error occurred while writing the control registers to the device */
    FXAS21000_ERR_WAITFORACTIVE,   /*!< Error occurred while attempting to put device into active mode */
};

/**
 * @brief FXAS21000 driver internal states
 */
typedef enum fxas21000_InternalState_tag {
    FXAS21000_STATE_UNINITIALIZED, /*!< Unknown */
    FXAS21000_STATE_INITIALIZED,   /*!< Sensor has been reset, ::FXAS21000_WHO_AM_I has been verified, some digital blocks are enabled (standby mode), I2C/SPI communication is possible */
    FXAS21000_STATE_CONFIGURED,    /*!< Drive circuits are running, but no measurements are being made (ready mode). May transition to active mode within 1/ODR ms */
    FXAS21000_STATE_STARTED,       /*!< All blocks are enabled, the sensor is actively measuring the angular rate (active mode) */
} fxas21000_InternalState_t;








#endif  /* _MAG3110_H_ */
