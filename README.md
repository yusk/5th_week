# 5th_week

## contiributers

* iwasaki
* ueda
* uekumasu
* kawasaki


## kadai



### 全端末共通

PanID 0x7004
Channel 20

### 個別
Cordinator short adress 0x7004

MACAddress 0xFFFFFFFFFFF7004X
* Cordinator X=1
* Rooter１　X=2
* End-Device1　X=3 
* Rooter2　X=4
* End-Device2　X=5

short address (cordinator-rooter)
* rooter1 0x0001
* rooter2 0x0101


short address (rooter-end-device)
* rooter1-end-device1 0x0002
* rooter2-end-device2 0x0102


## unicast

Cordinator
ブロードキャストでいい。

End-Device

'''
mpPacket->msgData.dataReq.txOptions = gTxOptsAck_c | gTxOptsIndirect_c;
+ gTxOptsIndirect_c　ルーターは要る。
+ gTxOptsIndirect_c　end-deviceは要らないかも。

(broadcast mpPacket->msgData.dataReq.txOptions = 0)
'''


'''
pAssocRes->assocShortAddress[0] = 0x01;
(broadcast pAssocRes->assocShortAddress[0] = nwk_addr++;)
'''

## 情報解析

### kadai1

#### coordinator

##### state

1. stateInit
  * 初期化
1. stateScanEdStart
  * エネルギーサーチ
1. stateScanEdWaitConfirm
  * エネルギーサーチ結果待ち
1. stateStartCoordinator
  * コーディネート開始
1. stateStartCoordinatorWaitConfirm
  * コーディネート結果待ち
1. stateListen
  * 接続開始
    * App_HandleMlmeInput
      * App_SendAssociateResponse
        * ここでショートアドレスを発行している
    * App_TransmitUartData
    * App_HandleMcpsInput

```c
static uint8_t App_SendAssociateResponse(nwkMessage_t *pMsgIn)
{
  mlmeMessage_t *pMsg;
  mlmeAssociateRes_t *pAssocRes;
 
  UartUtil_Print("Sending the MLME-Associate Response message to the MAC...", gAllowToBlock_d);
 
  /* Allocate a message for the MLME */
  pMsg = MSG_AllocType(mlmeMessage_t);
  if(pMsg != NULL)
  {
    /* This is a MLME-ASSOCIATE.res command */
    pMsg->msgType = gMlmeAssociateRes_c;
    
    /* Create the Associate response message data. */
    pAssocRes = &pMsg->msgData.associateRes;
    
    /* Assign a short address to the device. In this example we simply
       choose 0x0001. Though, all devices and coordinators in a PAN must have
       different short addresses. However, if a device do not want to use 
       short addresses at all in the PAN, a short address of 0xFFFE must
       be assigned to it. */
    if(pMsgIn->msgData.associateInd.capabilityInfo & gCapInfoAllocAddr_c)
    {
      /* Assign a unique short address less than 0xfffe if the device requests so. */
      pAssocRes->assocShortAddress[0] = 0x01;
      pAssocRes->assocShortAddress[1] = 0x00;
    }
    else
    {
      /* A short address of 0xfffe means that the device is granted access to
         the PAN (Associate successful) but that long addressing is used.*/
      pAssocRes->assocShortAddress[0] = 0xFE;
      pAssocRes->assocShortAddress[1] = 0xFF;
    }
    /* Get the 64 bit address of the device requesting association. */
    FLib_MemCpy(pAssocRes->deviceAddress, pMsgIn->msgData.associateInd.deviceAddress, 8);
    /* Association granted. May also be gPanAtCapacity_c or gPanAccessDenied_c. */
    pAssocRes->status = gSuccess_c;
    /* Do not use security */
#ifndef gMAC2006_d
    pAssocRes->securityEnable = FALSE;
#else
	pAssocRes->securityLevel = 0;
#endif //gMAC2006_d	
    
    /* Save device info. */
    FLib_MemCpy(maDeviceShortAddress, pAssocRes->assocShortAddress, 2);
    FLib_MemCpy(maDeviceLongAddress,  pAssocRes->deviceAddress,     8);
    
    /* Send the Associate Response to the MLME. */
    if(MSG_Send(NWK_MLME, pMsg) == gSuccess_c)
    {
      UartUtil_Print("Done\n\r", gAllowToBlock_d);
      return errorNoError;
    }
    else
    {
      /* One or more parameters in the message were invalid. */
      UartUtil_Print("Invalid parameter!\n\r", gAllowToBlock_d);
      return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    UartUtil_Print("Message allocation failed!\n\r", gAllowToBlock_d);
    return errorAllocFailed;
  }
}
```


## coordinator
E→R
R→C

ルータ

'''

case stateRooterListen:
    /* Stay in this state forever. 
       Transmit the data received on UART */
    if (events & gAppEvtMessageFromMLME_c)
    {
      /* Get the message from MLME */
      if (pMsgIn)
      {      
        /* Process it */
        rc = Router_HandleMlmeInput(pMsgIn);
        /* Messages from the MLME must always be freed. */
      }
    }
     if (events & gAppEvtRxFromUart_c)
    {      
      /* get byte from UART */
      Router_TransmitUartData();
    
    }  
    '''

'''
    case stateListen:
    /* Transmit to coordinator data received from UART. */
    if (events & gAppEvtMessageFromMLME_c)
    {  
      if (pMsgIn)
      {  
        /* Process it */
        rc = App_HandleMlmeInput(pMsgIn);
      }
    } 
    

#if gMPL3115A2_enable    
    MPL3115A2_dump(events);
#endif
  
#if gMMA8652_enable    
  // Start to receive periodical data 
  if(MMA8652_start_flag){
    MMA8652_start_flag = 0;
    MMA8652_Start_Periodical_data();
  }
  
  if (events & gAppEvt_FromMMA8652_c){
    /* get byte from UART */
    App_TransmitSensorData();
  }

'''

'''
  if (events & gAppEvtMessageFromMCPS_c)
  {      
    /* Get the message from MCPS */
    pMsgIn = MSG_DeQueue(&mMcpsNwkInputQueue);
    if (pMsgIn)
    {              
      /* Process it */
      App_HandleMcpsInput(pMsgIn);
      /* Messages from the MCPS must always be freed. */
      MSG_Free(pMsgIn);
    }
  }
'''

'''
static void Router_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn)
{
  switch(pMsgIn->msgType)
  {
    /* The MCPS-Data confirm is sent by the MAC to the network 
       or application layer when data has been sent. */
  case gMcpsDataCnf_c:
    if(mcPendingPackets)
      mcPendingPackets--;
    break;

  case gMcpsDataInd_c:
    /* Copy the received data to the UART. */
    UartUtil_Tx(pMsgIn->msgData.dataInd.pMsdu, pMsgIn->msgData.dataInd.msduLength);
    UartUtil_print(pMsgIn->msgData.dataInd.pMsdu, pMsgIn->msgData.dataInd.msduLength);//added by ueda

    

    break;
  }
}


'''




case stateListen:
    /* Transmit to coordinator data received from UART. */
    if (events & gAppEvtMessageFromMLME_c)
    {  
      if (pMsgIn)
      {  
        /* Process it */
        rc = App_HandleMlmeInput(pMsgIn);
      }
    } 
    

#if gMPL3115A2_enable    
    MPL3115A2_dump(events);
#endif
  
#if gMMA8652_enable    
  // Start to receive periodical data 
  if(MMA8652_start_flag){
    MMA8652_start_flag = 0;
    MMA8652_Start_Periodical_data();
  }
  
  if (events & gAppEvt_FromMMA8652_c){
    /* get byte from UART */
    App_TransmitSensorData();
  }
    //MMA8652_dump(events);
#endif
    
#if gMAG3110_enable    
    MAG3110_dump(events);
#endif
    
#if gFXAS21000_enable    
    FXAS21000_dump(events);
#endif
    
#if gMMA9553_enable    
    MMA9553_dump(events);
#endif
    
    break;


'''


'''
case stateInitRooter:
     /* Print a welcome message to the UART */
    UartUtil_Print(" MyWirelessApp Demo Beacon Coordinator application is initialized and ready.\n\r\n\r", gAllowToBlock_d);            
    /* Goto Energy Detection state. */
    gState = stateScanEdStart;
    TS_SendEvent(gAppTaskID_c, gAppEvtDummyEvent_c);    
    break;
    
  case stateScanEdStart:
      /* Start the Energy Detection scan, and goto wait for confirm state. */
      UartUtil_Print("Initiating the Energy Detection Scan\n\r", gAllowToBlock_d);
      /*Print the message on the LCD also*/
      LCD_ClearDisplay();
      LCD_WriteString(1,"Starting Energy");
      LCD_WriteString(2,"Detection Scan");      
      rc = App_StartScan(gScanModeED_c);
      if(rc == errorNoError)
      {
        gState = stateScanEdWaitConfirm;
      }
      break;

'''


'''
case stateRooterListen:
    /* Stay in this state forever. 
       Transmit the data received on UART */
    if (events & gAppEvtMessageFromMLME_c)
    {
      /* Get the message from MLME */
      if (pMsgIn)
      {      
        /* Process it */
        rc = Router_HandleMlmeInput(pMsgIn);
        /* Messages from the MLME must always be freed. */
      }
    }
     if (events & gAppEvtRxFromUart_c)
    {      
      /* get byte from UART */
      Router_TransmitUartData();
    
    }  
    break;     
  }
  
'''