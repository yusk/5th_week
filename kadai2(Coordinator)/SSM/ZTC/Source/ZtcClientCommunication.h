/******************************************************************************
* Declarations for ZTC routines that handle the ZTC <--> external client protocol.
*
* Copyright (c) 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from  Freescale Semiconductor.
*
* Declarations relating to the communication path between Ztc and a external
* (UART based) client, such as the Test Tool.
*
******************************************************************************/

#ifndef _ZtcClientCommunication_h
#define _ZtcClientCommunication_h

#include "UART_Interface.h"

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
******************************************************************************/

/* Expected value of an mZtcStx_t. */
#define mZtcStxValue_c     ((unsigned char) 0x02)

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/

/* A complete UART packet is a UART/ZTC envelope + a variable length */
/* clientPacket_t. */
/* A UART/SPI envelope is a leading mZTCStx_t and a trailing mUARTFcs_t. */
typedef uint8_t mZtcStx_t;             /* A sync octet. */
typedef uint8_t clientPacketChecksum_t;

/******************************************************************************
*******************************************************************************
* Private prototypes
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public macros
*******************************************************************************
******************************************************************************/

/* used to extend the maximum number of PAN Descriptors returned by Scan Confirm*/
/* gZtcExtendedScanConfirmSupport_c = TRUE -->  maximum number = aMaxScanResults */
/* gZtcExtendedScanConfirmSupport_c = FALSE --> maximum number = aScanResultsPerBlock */
#define gZtcExtendedScanConfirmSupport_c TRUE

#if (gZtcExtendedScanConfirmSupport_c == TRUE)
/* maximum size of a packet send to Client is 255 */
/* maximum payload included in that packet is 245 */
#define gZtcPacketPayloadLen_c  245
#else
/* size of a big buffer */
#define gZtcPacketPayloadLen_c  gMaxRxTxDataLength_c
#endif /* (gZtcExtendedScanConfirmSupport_c == TRUE) */


/******************************************************************************
*******************************************************************************
* Public type definitions
*******************************************************************************
******************************************************************************/

/* Format of packets exchanged between the external client and Ztc. */
/* This excludes the UART wrapper (a leading STX byte and a trailing FCS). */
typedef uint8_t clientPacketStatus_t;

typedef struct clientPacketHdr_tag {
  ztcOpcodeGroup_t opcodeGroup;
  ztcMsgType_t msgType;
  index_t len;                          /* Actual length of payload[] */
} clientPacketHdr_t;

/* The terminal checksum is actually stored at payload[len]. The checksum */
/* field insures that there is always space for it, even if the payload */
/* is full. */
typedef struct clientPacketStructured_tag {
  clientPacketHdr_t header;
  uint8_t payload[gZtcPacketPayloadLen_c];
  uint8_t checksum;
} clientPacketStructured_t;

typedef union clientPacket_tag {
  /* The entire packet as unformatted data. */
  uint8_t raw[sizeof(clientPacketStructured_t)];

  /* The packet as header + payload. */
  clientPacketStructured_t structured;

  /* A minimal packet with only a status value as a payload. */
  struct {                              /* The packet as header + payload. */
    clientPacketHdr_t header;
    uint8_t status;
  } headerAndStatus;
} clientPacket_t;

/******************************************************************************
*******************************************************************************
* Public prototypes
*******************************************************************************
******************************************************************************/

/* Read data from the external client. Returns TRUE when gZtcPacketFromClient */
/* contains a complete, well formed packet. */
extern bool_t ZtcReadPacketFromClient(void);

/* Send gZtcPacketToClient to the external client. If gZtcUartTxBlocking */
/* is TRUE, blocks until the entire packet has been sent. */
/* Regardless of the state of the gZtcUartTxBlocking flag, sets the global */
/* mZtcPacketToClientIsBusy to TRUE until the entire packet has been sent. */
extern void ZtcWritePacketToClient(index_t const len);

/* Send an arbitrary buffer to the external client. If gZtcUartTxBlocking */
/* is TRUE, blocks until the entire buffer has been sent. */
extern void ZtcTransmitBuffer(uint8_t const *pSrc, index_t const len, void (*pfCallBack)(unsigned char const *pBuf));

/* Send an arbitrary buffer to the external client. Block until the entire */
/* buffer has been sent, regardless of the state of the gZtcUartTxBlocking */
/* flag. */
extern void ZtcTransmitBufferBlock(uint8_t const *pSrc, index_t const len);

/******************************************************************************
*******************************************************************************
* Public memory declarations
*******************************************************************************
******************************************************************************/

/* UART packet received from the external client, minus the UART envelope. */
extern clientPacket_t gZtcPacketFromClient;

/* UART packet to be sent to external client, minus the UART envelope. */
extern clientPacket_t gZtcPacketToClient;

/* TRUE if gZtcPacketToClient is being written to the external client. */
/* FALSE if it is available to be rewritten. */
extern volatile bool_t mZtcPacketToClientIsBusy;

/* If true, ZtcWritePacketToClient() will block after enabling the UART/SPI Tx */
/* interrupt, until the UART/SPI driver's output buffer is empty again. */
extern bool_t gZtcTxBlocking;

/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
******************************************************************************/

#endif                                  /* #ifndef _ZtcClientCommunication_h */
