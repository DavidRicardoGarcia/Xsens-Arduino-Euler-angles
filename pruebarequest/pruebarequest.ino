#include <XBee.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
//ZBRxResponse rx = ZBRxResponse();
ZBExplicitRxResponse rx = ZBExplicitRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
uint8_t payload[]={'C',' ','+','8','0','.','8','7',' ','+','0','.','8','7','0',' ','-','0','.','2','0','0',' ','-','0','.','2','0','0',' ','-','0','.','2','0','0',' ','-','0','.','2','0','0',' ','-','0','.','2','0','0'};
XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
  xbee.setSerial(Serial3);
}

void loop() {
  // put your main code here, to run repeatedly:
    xbee.readPacket();

    if (xbee.getResponse().isAvailable()) {
      // got something
      if (xbee.getResponse().getApiId() == ZB_EXPLICIT_RX_RESPONSE) {
        // got a zb rx packet
       // now fill our zb rx class
       // xbee.getResponse().getZBExplicitRxResponse(rx);
        Serial.print("llego");
        xbee.send(zbTx);
      } 
        // the local XBee sends this response on certain events, like association/dissociation
        
 
    }else if (xbee.getResponse().isError()) {
     Serial.print("error code:");
     Serial.println(xbee.getResponse().getErrorCode());
    }

delay(20);
  
}
