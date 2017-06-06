/***********************************************************************************************\
* Freescale MAG3110
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

#ifndef _MAG3110_H_
#define _MAG3110_H_


/***********************************************************************************************\
* Public macros
\***********************************************************************************************/

#define MAG3110_SlaveAddressIIC   (0x0E<<1) 
 
#define MAG3110_sensitivity       (0.1)   // [uT/LSB]

#ifndef mMAG3110Interval_c
#define mMAG3110Interval_c  700
#endif

/***********************************************************************************************\
* Public prototypes
\***********************************************************************************************/
void  (*pfMAG3110_CallBack)(void);
void MAG3110_SetCallBack(void (*pfCallBack)(void));
void MAG3110_int(void);
void MAG3110_Periodical_data(uint8_t timerId);
int16_t MAG3110_CatchSensorData(uint8_t number);
void MAG3110_Start_Periodical_data(void);
void MAG3110_Init(void);
void MAG3110_CallBack(void);
void MAG3110_dump(event_t events);


/* Data struct received from ACC */
typedef struct dataFrom_MAG3110_tag {
  int16_t xOutReg;
  int16_t yOutReg;
  int16_t zOutReg;
  uint8_t samplingRateStatustReg;
  uint8_t modeReg;
}dataFrom_MAG3110_t;



/* **********************************************************************************************
**
**  MAG3110 Sensor Internal Registers
*/

/***********************************************************************************************\
* Public memory declarations
\***********************************************************************************************/
#define NUM_CTRL_REG                0x2
#define MAG3110_NUMBER_AXIS         0x3
#define MAG3110_NUMBER_BYTES_AXIS   0x2      // Number of bytes in each axis
#define MAG3110_DR                 (0x00)
#define MAG3110_OUT_X_MSB          (0x01)
#define MAG3110_OUT_X_LSB          (0x02)
#define MAG3110_OUT_Y_MSB          (0x03)
#define MAG3110_OUT_Y_LSB          (0x04)
#define MAG3110_OUT_Z_MSB          (0x05)
#define MAG3110_OUT_Z_LSB          (0x06)
#define MAG3110_WHO_AM_I           (0x07)
#define MAG3110_SYS_MOD            (0x08)
#define MAG3110_OFF_X_MSB          (0x09)
#define MAG3110_OFF_X_LSB          (0x0A)
#define MAG3110_OFF_Y_MSB          (0x0B)
#define MAG3110_OFF_Y_LSB          (0x0C)
#define MAG3110_OFF_Z_MSB          (0x0D)
#define MAG3110_OFF_Z_LSB          (0x0E)
#define MAG3110_DIE_TEMP           (0x0F)
#define MAG3110_CTRL_REG1          (0x10)
#define MAG3110_CTRL_REG2          (0x11)
#define MAG3110_DEVICE_ID          (0xC4)

#define MAG3110_CTRL_REG1_ACTIVE_MASK       (0x01)
#define MAG3110_CTRL_REG1_FAST_READ_MASK    (0x04)
#define MAG3110_CTRL_REG2_RAW_MASK          (0x20)
#define MAG3110_CTRL_REG2_AUTO_MRST_EN_MASK (0x80)

#define MAG3110_RATE_INDEX         (0x03)
#define MAG3100_READ_BUF_SIZE      (0x6)
#define MAG3110_ADC_RATE_START     (1280)
#define MAG3110_OVER_SAMPLE_FACTOR (16)
#define MAG3110_DEFAULT_RATE_80HZ  (0)

#define NUMBER_OF_BYTES_TIMESTAMP  (4)
#define MAG3110_CTRL_REG1_SIZE     (1)

#define MAG3110_WHOAMI_REG_SIZE    (1)
#define MAG3110_WHOAMI_VALUE       (0xC4)

#define REG_ADDR_INX               (0)
#define REG_VALUE_INX              (1)


// Device standby to active time, number of samples to wait.  MAG3110 datasheet states 
// that for 80 ODR (12.5ms sample period), the standby to active time is 25ms which 
// is 2 samples (2 * 12.5ms = 25ms).
#define MAG3110_STANDBY_TO_ACTIVE_SAMPLE_WAIT  (2)

/** 
 * Brief Enumeration of the MAG3110 available sample rates.
 */
enum ADC_rate{
        FSL_MAG3110_I2C_3D_MAG_ADCR_1280 = 0 ,
        FSL_MAG3110_I2C_3D_MAG_ADCR_640 = 1 ,
        FSL_MAG3110_I2C_3D_MAG_ADCR_320 = 2,
        FSL_MAG3110_I2C_3D_MAG_ADCR_160 = 3,
        FSL_MAG3110_I2C_3D_MAG_ADCR_80 = 4,
        FSL_MAG3110_I2C_3D_MAG_ADCR_40 = 5,
        FSL_MAG3110_I2C_3D_MAG_ADCR_20 = 6,
        FSL_MAG3110_I2C_3D_MAG_ADCR_10 = 7,
        FSL_MAG3110_I2C_3D_MAG_ADCR_MAX
        
};






#endif  /* _MAG3110_H_ */

