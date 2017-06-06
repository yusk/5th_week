/************************************************************************************
* This header file is provided as part of the interface to the Freescale 802.15.4
* MAC and PHY layer.
*
* The file covers the interface to the Memory and Message Manager.
*
*
* (c) Copyright 2007, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/


#ifndef _MSG_SYSTEM_H_
#define _MSG_SYSTEM_H_

#include "NwkMacInterface.h"
#include "AppToMacPhyConfig.h"
#include "FunctionLib.h"

/************************************************************************************
*************************************************************************************
* Public macros - see description of functionality in the public prototypes section.
*************************************************************************************
************************************************************************************/

  // Free a message
#define MSG_Free(msg)  MM_Free(msg)

  // Put a message in a queue at the head.
#define MSG_QueueHead(anchor, element) List_AddHead((anchor), (element))

  // Get a message from a queue. Returns NULL if no messages in queue.
#define MSG_DeQueue(anchor) List_RemoveHead(anchor)
              
  // Check if a message is pending in a queue. Returns
  // TRUE if any pending messages, and FALSE otherwise.
#define MSG_Pending(anchor) ((anchor)->pHead != 0)

#define MSG_InitQueue(anchor) List_ClearAnchor(anchor)

  // Allocate a message of a certain type
#define MSG_AllocType(type) MM_Alloc(sizeof(type))

  // Allocate a message of a certain size
#define MSG_Alloc(size) MM_Alloc(size)

  // Put a message in a queue.
#define MSG_Queue(anchor, element) List_AddTail((anchor), (element))

  // Sending a message is equal to calling a Service Access Point function
  // If the sap argument is e.g. MLME, then a function called MLME_SapHandler
  // must exist that takes a message pointer as argument.
#define MSG_Send(sap, msg)  (sap##_SapHandler((void *)(msg)))

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

  // This struct is used privately but must be declared
  // here in order for the anchor_t struct to work.
  // The struct is used in memory blocks for linking them
  // in single chained lists. The struct is embedded in
  // list elements, and transparent to the user.
typedef struct listHeader_tag {
  struct listHeader_tag *pNext;
  struct pools_tag *pParentPool;
} listHeader_t;


  // List anchor with head and tail elements. Used 
  // for both memory pools and message queues.
typedef struct anchor_tag {
  listHeader_t *pHead;
  listHeader_t *pTail;
} anchor_t, msgQueue_t;

  // Buffer pool handle. An array of this type is filled
  // in by MM_Init(), and used by MM_AllocPool().
typedef struct pools_tag {
  anchor_t anchor; // MUST be first element in pools_t struct
  uint8_t nextBlockSize;
  uint8_t blockSize;
} pools_t;

  // Buffer pool description. Used by MM_Init()
  // for creating the buffer pools.
typedef struct poolInfo_tag {
  uint8_t poolSize;
  uint8_t blockSize;
  uint8_t nextBlockSize;
} poolInfo_t;


typedef struct MsgTracking_tag {
  uint16_t MsgAddr;        /*Addr of Msg, not that this pointer is 4 byte bigger than
                            the addr in the pool has the header of the msg is 4 bytes */
  uint16_t AllocAddr;      /*Return address of last Alloc made */
  uint16_t FreeAddr;       /*Return address of last Free made */
  uint8_t  AllocCounter;   /*No of time this msg has been allocated */
  uint8_t  FreeCounter;    /*No of time this msg has been freed */
  uint8_t  AllocStatus;    /*1 if currently allocated, 0 if currently free */
} MsgTracking_t;


/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Allocate a block from the specified memory pool. The function uses the size 
* argument to look up a pool with adequate block sizes.
*   
* Interface assumptions:
*   None
*   
* Return value:
*   Pointer to the allocated memory block or NULL if no blocks available.
* 
************************************************************************************/
void *MM_AllocPool
  (
  pools_t *pPools, // IN: Pool array to allocate from
  uint8_t numBytes // IN: Minimum number of bytes to allocate
  );

/************************************************************************************
* Allocate a block from the MAC memory pool. The function uses the size argument to 
* look up a pool with adequate block sizes.
*   
* Interface assumptions:
*   None
*   
* Return value:
*   Pointer to the allocated memory block or NULL if no blocks available.
* 
************************************************************************************/
void *MM_Alloc
  (
  uint8_t numBytes // IN: Minimum number of bytes to allocate
  );

/************************************************************************************
* Deallocate a memory block by putting it in the corresponding pool of free blocks.
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None.
* 
************************************************************************************/
void MM_Free
  (
  void *pBlock // IN: Block of memory to free
  );


/************************************************************************************
* This function initializes the message module private variables. Must be
* called at boot time, or if device is reset. Currently the module supports
* up to 4 memory pools, but pools may be added or removed. The following 
* constants control the memory pool layout:
* 
* gMmPoolSize0_c,  gMmBlockSize0_c
* gMmPoolSize1_c,  gMmBlockSize1_c
* gMmPoolSize2_c,  gMmBlockSize2_c
* gMmPoolSize3_c,  gMmBlockSize3_c
* 
* The gMmPoolSize*_c constants determine the number of blocks in a pool, and
* gMmBlockSize*_c is the number of bytes in each block for the corresponding pool.
* The number of bytes is rounded up to a value so that each block is aligned to
* a machine dependant boundary in order to avoid bus errors during pool access.
*
* The total amount of heap required is given by the constant mMmTotalPoolSize_c.
*
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
void MM_Init
  (
  uint8_t *pHeap, // IN: Memory heap. Caller must be sure to make this big enough
  const poolInfo_t *pPoolInfo,  // IN: Memory layout information
  pools_t *pPools // OUT: Will be initialized with requested memory pools.
  );


/************************************************************************************
* This function can be used to add a memory block to the specified pool during
* runtime. The following example shows how to add two buffers to the MAC buffer
* pool:
*
*   // Declare an array with two buffers
*   uint8_t myExtraMacBuffers[2][sizeof(listHeader_t) + gMaxPacketBufferSize_c];
*   // Get a pointer to the MAC pools. The size must be correct.
*   pools_t *macDataBufferPool = MM_GetMacPool(gMaxPacketBufferSize_c);
*   // Add the two extra buffers to the MAC buffer pool.
*   MM_AddToPool(macDataBufferPool, myExtraMacBuffers[0]);
*   MM_AddToPool(macDataBufferPool, myExtraMacBuffers[1]);
*
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
void MM_AddToPool
  (
  pools_t *pPool,
  void *pBlock
  );

/************************************************************************************
* This function returns a pointer to the MAC buffer pool which corresponds exactly
* to the size argument. This is used with the MM_AddToPool() function to add buffers
* to the MAC buffer pool.
*
* Interface assumptions:
*   None
*   
* Return value:
*   Pointer to the requested buffer pool
* 
************************************************************************************/
pools_t *MM_GetMacPool
  (
  uint8_t size // IN: Exact size which represents the requested buffer pool.
  );


/************************************************************************************
* Initialize a list anchor with a NULL list header. Used for preparing an anchor for
* first time use.
*
* Interface assumptions:
*   None
*   
* Return value:
*   None.
* 
************************************************************************************/
void List_ClearAnchor
  (
  anchor_t *pAnchor // IN: Anchor of list to reset
  );

/************************************************************************************
* Links a list element to the tail of the list given by the anchor argument.
* This function is amongst other useful for FIFO queues if combined with
* List_RemoveHead.
*
* Interface assumptions:
*   None
*   
* Return value:
*   None.
* 
************************************************************************************/
void List_AddTail
  (
  anchor_t *pAnchor, //IN: Anchor of list to add element to
  void *pBlock       //IN: Element to add to tail of list
  );

/************************************************************************************
* Links a list element to the head of the list given by the anchor argument.
* Useful for FILO buffers (push/pop stacks). This function should only be 
* included in the build if really required.
*
* Interface assumptions:
*   None
*   
* Return value:
*   None.
* 
************************************************************************************/
void List_AddHead
  (
  anchor_t *pAnchor, //IN: Anchor of list to add element to
  void *pBlock       //IN: Element to add to head of list
  );

/************************************************************************************
* Unlinks a list element from the head of the list given by the anchor argument.
*
* Interface assumptions:
*   None
*   
* Return value:
*   Pointer to the unlinked list element or NULL if no elements available.
* 
************************************************************************************/
void *List_RemoveHead
  (
  anchor_t *pAnchor  //IN: Anchor of list to remove head from
  );


/************************************************************************************
*************************************************************************************
* MAC Private definitions - DO NOT CHANGE TYPES/MACROS BELOW THIS POINT.
*************************************************************************************
************************************************************************************/

  // Used for calculating size of blocks in pool #0
typedef union {
  nwkMessage_t    nwkMessage;
  panDescriptor_t panDescriptor;
  msgApsdeDataReq_t apsDataReq;
} block1sizeOrigin_t;


  // Max length of MPDU/PSDU without CRC
#ifdef gMAC2006_d  
  #define gMaxRxTxDataLength_c (156)
#else
  #define gMaxRxTxDataLength_c (141)
#endif //gMAC2006
#define gMaxMsduDataLength_c (118)

/************************************************************************************
* The following two structures must only be used internally in the MAC. They need
* to be defined here in order to calculate the size of the MAC data buffers. The
* size is required for defining the maMacHeap[] byte array.
* NOTE: DO NOT USE OR CHANGE THE rxPacket_tag OR THE txPacket_tag STRUCTURES.
************************************************************************************/
struct rxPacket_tag {
  uint8_t frameLength;
  uint8_t linkQuality;
  uint8_t headerLength;
  uint8_t rxData[gMaxRxTxDataLength_c];
  zbClock24_t timeStamp;
#ifdef gMAC2006_d
  uint8_t securityLevel;
  uint8_t keyIdMode;
  uint8_t keySource[8];
  uint8_t keyIndex;
#endif //gMAC2006_d  
};                

struct txPacket_tag {
  uint8_t frameLength;
  uint8_t txInfo;
  struct {
    uint8_t csmaCaNb;
    uint8_t csmaCaBe;
    uint8_t txCount;
  } csmaAndTx;
  uint16_t expireTime;
  uint8_t msduHandle;
  uint8_t txData[gMaxRxTxDataLength_c];
#ifdef gMAC2006_d
  uint8_t securityLevel;
  uint8_t keyIdMode;
  uint8_t keySource[8];
  uint8_t keyIndex;
#endif //gMAC2006_d  
};

  // Maximum buffer sizes to be used:
#define gMaxRxPacketBufferSize_c   (sizeof(struct rxPacket_tag))
#define gMaxTxPacketBufferSize_c   (sizeof(struct txPacket_tag))
#define gMaxMcpsDataReqSize_c      (sizeof(nwkToMcpsMessage_t) + gMaxMsduDataLength_c)
#define gMaxMcpsDataIndSize_c      (sizeof(mcpsToNwkMessage_t) + gMaxMsduDataLength_c)
#define gMaxMcpsDataBufferSize_c   (FLib_GetMax(gMaxMcpsDataReqSize_c, gMaxMcpsDataIndSize_c))
#define gMaxRxTxPacketBufferSize_c (FLib_GetMax(gMaxRxPacketBufferSize_c, gMaxTxPacketBufferSize_c))
#define gMaxPacketBufferSize_c     (FLib_GetMax(gMaxRxTxPacketBufferSize_c, gMaxMcpsDataBufferSize_c))


  // Number of pools
#if gBigMsgsMacOnly_d > 0
#define gMmNumPools_c 3 // 1 Small message pool, 1 Large data pool, 1 Large data pool (MAC private)
#else
#define gMmNumPools_c 2 // 1 Small message pool, 1 Large data pool
#endif // gBigMsgsMacOnly_d

  // The block sizes will be rounded up to multipla of largest
  // data integer type (a pointer or uint16_t for HCS08).
  // Extra space for list headers will be added automagically.
  // The block sizes must be in ascending order. If fewer than
  // 4 pools are used, then gMmPoolSize*_c, and gMmBlockSize*_c
  // for the unused pools must be defined to the value 0.

  // TBD: Currently many "small" messages uses union in deciding message size even if
  //      the message is smaller. Not a big problem, but could be improve and that
  //      may affect necessary sizes of messages!
#define gMmPoolSize0_c  (gTotalSmallMsgs_d)
#define gMmBlockSize0_c (sizeof(block1sizeOrigin_t)) // ~22 bytes

#define gMmPoolSize1_c  (gTotalBigMsgs_d - gBigMsgsMacOnly_d)
#define gMmBlockSize1_c gMaxPacketBufferSize_c

  // Pool[2] is private to the MAC.
#define gMmPoolSize2_c  gBigMsgsMacOnly_d
// Make sure that NWK-MLME data struct is SMALLER than this. Consider adding
// getMax(gMaxPacketBufferSize_c,sizeof(nwkToMcpsMessage_t)+aMaxMacFrameSize-1).
#if gMmPoolSize2_c != 0
 #define gMmBlockSize2_c gMaxPacketBufferSize_c
#else
 #define gMmBlockSize2_c 0
#endif // gBigMsgsMacOnly_d != 0

#define gMmPoolSize3_c  0
#define gMmBlockSize3_c 0


  // Make sure that all blocks are aligned correctly (  Round up: (((a + (s-1)) / s) * s), s=4 -> (((a+3) >> 2) << 2)  )
#define mMmBlockSize0_c ((((gMmBlockSize0_c) + (sizeof(uint8_t *) - 1)) / sizeof(uint8_t *)) * sizeof(uint8_t *))
#define mMmBlockSize1_c ((((gMmBlockSize1_c) + (sizeof(uint8_t *) - 1)) / sizeof(uint8_t *)) * sizeof(uint8_t *))
#define mMmBlockSize2_c ((((gMmBlockSize2_c) + (sizeof(uint8_t *) - 1)) / sizeof(uint8_t *)) * sizeof(uint8_t *))
#define mMmBlockSize3_c ((((gMmBlockSize3_c) + (sizeof(uint8_t *) - 1)) / sizeof(uint8_t *)) * sizeof(uint8_t *))

  // The total number of bytes in each pool including list headers
#define mMmPoolByteSize0_c ((gMmPoolSize0_c) * (mMmBlockSize0_c + sizeof(listHeader_t)))
#define mMmPoolByteSize1_c ((gMmPoolSize1_c) * (mMmBlockSize1_c + sizeof(listHeader_t)))
#define mMmPoolByteSize2_c ((gMmPoolSize2_c) * (mMmBlockSize2_c + sizeof(listHeader_t)))
#define mMmPoolByteSize3_c ((gMmPoolSize3_c) * (mMmBlockSize3_c + sizeof(listHeader_t)))

  // Total number of bytes in all pools together
#define mMmTotalPoolSize_c (mMmPoolByteSize0_c + mMmPoolByteSize1_c + mMmPoolByteSize2_c + mMmPoolByteSize3_c)


#if gMmNumPools_c == 1
#define mMmBlockSizeLargest_c gMmBlockSize0_c // Size of largest block
#define mMmLastPoolIdx_c 0  // Index of last pool
#endif // gMmNumPools_c == 1

#if gMmNumPools_c == 2
#define mMmBlockSizeLargest_c gMmBlockSize1_c // Size of largest block
#define mMmLastPoolIdx_c 1 // Index of last pool
#endif // gMmNumPools_c == 2

#if gMmNumPools_c == 3
#define mMmBlockSizeLargest_c gMmBlockSize2_c // Size of largest block
#define mMmLastPoolIdx_c 2 // Index of last pool
#endif // gMmNumPools_c == 3

#if gMmNumPools_c == 4
#define mMmBlockSizeLargest_c gMmBlockSize3_c // Size of largest block
#define mMmLastPoolIdx_c 3 // Index of last pool
#endif // gMmNumPools_c == 4

#endif /* _MSG_SYSTEM_H_ */
