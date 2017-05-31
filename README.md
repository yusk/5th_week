# 5th_week

## contiributers

* ueda

## kadai



### 全端末共通

+PanID 0x7004
+Channel 20

### 個別
Cordinator short adress 0x7004

MACAddress 0xFFFFFFFFFFF7004X
*Cordinator X=1
*Rooter１　X=2
*End-Device1　X=3 
*Rooter2　X=4
*End-Device2　X=5

short address (cordinator-rooter)
*rooter1 0x0001
*rooter2 0x0101


short address (rooter-end-device)
*rooter1-end-device1 0x0002
*rooter2-end-device2 0x0102


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




