/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <XBee.h>

/*
This example is for Series 2 XBee
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
//ZBRxResponse rx = ZBRxResponse();
ZBExplicitRxResponse rx = ZBExplicitRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
String cadena="";

float ang1yaw , ang1roll, ang1pitch , ang2yaw, ang2roll, ang2pitch;
float ang1yawk1 , ang1rollk1, ang1pitchk1 , ang2yawk1, ang2rollk1, ang2pitchk1;
//double matrizdez[4][4]={0};
//double matrizdex[4][4]={0};
//double matrizdey[4][4]={0};
int k=0,p=0,R1=0,R2=0,c1=0,c2=0;

void setup() {

  
  // start serial
  Serial.begin(115200);
  Serial3.begin(115200);
  xbee.setSerial(Serial3);
  

}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
    //  Serial.println( "got something");
    //  Serial.println(xbee.getResponse().getApiId(),HEX);
     // Serial.println(ZB_RX_RESPONSE,HEX);
      if (xbee.getResponse().getApiId() == ZB_EXPLICIT_RX_RESPONSE) {
        // got a zb rx packet
//        Serial.println("got a zb rx packet");
       // now fill our zb rx class
        xbee.getResponse().getZBExplicitRxResponse(rx);
        
//        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
//          // the sender got an ACK
//          Serial.println("packet acknowledged");
//        } else {
//          Serial.println("packet not acknowledged");
//        }


       //Serial.print("packet length is ");
       //Serial.println(rx.getPacketLength(), DEC);
      

        if( rx.getRemoteAddress64().getLsb()==0x40978B6E and c2==0){

        for (int i = 0; i < rx.getDataLength(); i++) {

          cadena+=char(rx.getData()[i]);
          
        }
//          Serial.println("router 2");
//          Serial.println(cadena);
//          Serial.println(cadena);
//          Serial.println(cadena.substring(2,9).toFloat());
//          Serial.println(cadena.substring(9,16).toFloat());
//          Serial.println(cadena.substring(16,23).toFloat());
            ang2yaw=cadena.substring(2,9).toFloat();
            ang2roll=cadena.substring(9,16).toFloat();
            ang2pitch=cadena.substring(16,23).toFloat();
         // diferenciarangulosrout2(cadena.substring(2,9).toFloat(),cadena.substring(9,16).toFloat(),cadena.substring(16,23).toFloat(),p);
          p=p+1;
//          Serial.println(ang2yaw);
//          Serial.println(ang2roll);
//          Serial.println(ang2pitch);
          cadena="";
          R2=1;
          c2=1;
          }

        if( rx.getRemoteAddress64().getLsb()==0x40978B74 and c1==0){
        
        for (int i = 0; i < rx.getDataLength(); i++) {

          cadena+=char(rx.getData()[i]);
          
        }
//          Serial.println("router 1");
//          Serial.println(cadena);
//          Serial.println(cadena.substring(2,9).toFloat());
//          Serial.println(cadena.substring(9,16).toFloat());
//          Serial.println(cadena.substring(16,23).toFloat());
          //diferenciarangulosrout1(cadena.substring(2,9).toFloat(),cadena.substring(9,16).toFloat(),cadena.substring(16,23).toFloat(),k);
            ang1yaw=cadena.substring(2,9).toFloat();
            ang1roll=cadena.substring(9,16).toFloat();
            ang1pitch=cadena.substring(16,23).toFloat();
          k=k+1;
//          Serial.println(ang1yaw);
//          Serial.println(ang1roll);
//          Serial.println(ang1pitch);
          cadena="";
          R1=1;
          c1=1;
          }

        if(R1 and R2){
          Serial.print(ang1yaw,1);Serial.print(",");
          Serial.print(ang1roll,1);Serial.print(",");
          Serial.print(ang1pitch,1);Serial.print(",");
          Serial.print(ang2yaw,1);Serial.print(",");
          Serial.print(ang2roll,1);Serial.print(",");
          Serial.print(ang2pitch,1);Serial.println(",");

          R1=0;
          R2=0;
          c1=0;
          c2=0;
          }
       // Serial.println(cadena);
        cadena="";    
       // Serial.println( rx.getRemoteAddress64().getLsb(),HEX);
        
      } 
        // the local XBee sends this response on certain events, like association/dissociation
        
 
    }else if (xbee.getResponse().isError()) {
     Serial.print("error code:");
     Serial.println(xbee.getResponse().getErrorCode());
    }   
}


void diferenciarangulosrout1(float yaw,float roll,float pitch, int cont){
  
  if(cont==0){
    ang1yaw=0;
    ang1roll=0;
    ang1pitch=0;
    ang1yawk1=yaw;
    ang1rollk1=roll;
    ang1pitchk1=pitch;
    }else {
      
    ang1yaw=yaw-ang1yawk1;
    ang1roll=roll-ang1rollk1;
    ang1pitch=pitch-ang1pitchk1;
    ang1yawk1=yaw;
    ang1rollk1=roll;
    ang1pitchk1=pitch;
    
    }
   
  
  }

void diferenciarangulosrout2(float yaw,float roll,float pitch, int cont){
  
  if(cont==0){
    ang2yaw=0;
    ang2roll=0;
    ang2pitch=0;
    ang2yawk1=yaw;
    ang2rollk1=roll;
    ang2pitchk1=pitch;
    }else {
      
    ang2yaw=yaw-ang2yawk1;
    ang2roll=roll-ang2rollk1;
    ang2pitch=pitch-ang2pitchk1;
    ang2yawk1=yaw;
    ang2rollk1=roll;
    ang2pitchk1=pitch;
    }
   
  
  }
//
//void matrizderotz(float tp){
//  tp=tp*PI/180.0;
//  matrizdez[0][0]=cos(tp);
//  matrizdez[0][0]=cos(tp);
//  matrizdez[0][0]=cos(tp);
//  matrizdez[0][0]=cos(tp);
//     
////  matrizdez[4][4]={{cos(tp),-sin(tp),0,0},{sin(tp),cos(tp),0,0},{0,0,1,0},{0,0,0,1}};
//
//}



  


