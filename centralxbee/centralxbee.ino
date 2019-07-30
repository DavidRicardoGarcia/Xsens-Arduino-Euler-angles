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
uint8_t payload[]={'0'};
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200 , 0x40978B6E);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

XBeeAddress64 addr64b = XBeeAddress64(0x0013A200 , 0x40978B74);
ZBTxRequest zbTxb = ZBTxRequest(addr64b, payload, sizeof(payload));
//0013A200 40978B6E
float ang1yaw , ang1roll, ang1pitch , ang2yaw, ang2roll, ang2pitch;
float ang1yawk1 , ang1rollk1, ang1pitchk1 , ang2yawk1, ang2rollk1, ang2pitchk1;
float ax,ay,az;
float emg1,mmg1,emg2,mmg2;
//double matrizdez[4][4]={0};
//double matrizdex[4][4]={0};
//double matrizdey[4][4]={0};
int k=0,p=0,R1=0,R2=0,c1=0,c2=0,m=0,toogle=0;
int cont=0;
char inChar='0';

void setup() {

  
  // start serial
  Serial.begin(115200);
  Serial3.begin(115200);
  xbee.setSerial(Serial3);
  pinMode(8,OUTPUT);
  digitalWrite(8,toogle);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

   inChar=Serial.read(); 
  
  if(inChar=='j'){
  Serial.println("Inicializacion MPU:     CORRECTA  ");  
  Serial.println("acabe inicializacion J");
  }
    
   if (inChar!='f'){ 
      
//      Serial.println("llego la f");
//
//      //se captura la senal cada 10ms si se recibe un caracater diferente de n y se envia.
//  
   while(inChar!='n'){ 

  
    if(m==0){
    xbee.send(zbTx);  
    xbee.send(zbTxb);
    // Serial.println("pedir datos");
    //delay(5);
    }
    m=1;
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      
    //  Serial.println("got something");
      if (xbee.getResponse().getApiId() == ZB_EXPLICIT_RX_RESPONSE) {
        // got a zb rx packet
       // now fill our zb rx class
        xbee.getResponse().getZBExplicitRxResponse(rx);
      //  Serial.println("got a zb rx packet");
        if( rx.getRemoteAddress64().getLsb()==0x40978B6E and c2==0){

        for (int i = 0; i < rx.getDataLength(); i++) {

          cadena+=char(rx.getData()[i]);
          
        }
          //Serial.println("router 2");
          //Serial.println(cadena);
          ang2yaw=cadena.substring(2,9).toFloat();
          ang2roll=cadena.substring(9,16).toFloat();
          ang2pitch=cadena.substring(16,23).toFloat();
          ax=cadena.substring(23,30).toFloat();
          ay=cadena.substring(30,37).toFloat();
          az=cadena.substring(37,44).toFloat();
          //p=p+1;
          cadena="";
          R2=1;
          c2=1;
          //m=0;
          //delay(10);
          }

        if( rx.getRemoteAddress64().getLsb()==0x40978B74 and c1==0){
        
        for (int i = 0; i < rx.getDataLength(); i++) {

          cadena+=char(rx.getData()[i]);
          
        }
         // Serial.println("router 1");
          //Serial.println(cadena);
          ang1yaw=cadena.substring(2,9).toFloat();
          ang1roll=cadena.substring(9,16).toFloat();
          ang1pitch=cadena.substring(16,23).toFloat();
          emg1=cadena.substring(23,30).toFloat();
          mmg1=cadena.substring(30,37).toFloat();
          emg2=cadena.substring(37,44).toFloat();
          mmg2=cadena.substring(44,51).toFloat();
          k=k+1;
          cadena="";
          R1=1;
          c1=1;
          //m=0;
          }

        if(R1 and R2){
//
//            cadena='Z';
//            cadena+=" ";
//            cadena+=ax+" ";
//            cadena+=ay+" ";
//            cadena+=az+" ";
//            cadena+=ang1yaw+" ";
//            cadena+=String(cont)+" ";
//            cadena+=ang1roll+" ";
//            cadena+=emg1+" ";
//            cadena+=mmg1+" ";
//            cadena+=ang1pitch+" ";
//            cadena+=ang2yaw+" ";
//            cadena+=ang2roll+" ";
//            cadena+=ang2pitch+" ";
//            cadena+=emg2+" ";
//            cadena+=mmg2;
//            Serial.println(cadena);
          Serial.print("Z"); Serial.print(" "); //0  // marca el inicio de la cadena, para que python identifique inicio de los datos
          Serial.print(ax); Serial.print(" ");//1
          Serial.print(ay); Serial.print(" ");//2
          Serial.print(az); Serial.print(" ");  //3
          Serial.print(ang1yaw,1); Serial.print(" "); //4   // Espacio Libre para futuro uso  // numero de prueba
          Serial.print(cont); Serial.print(" "); //5   // Espacio Libre para futuro uso  // cuenta el numero del mensaje para detectar si faltan mensajes
          //zB: 1, 2, 4, 7 -> se perdio mensaje 3 5 y 6.
          Serial.print(ang1roll,1); Serial.print(" ");   //6     //MMG2       // Espacio libre
          Serial.print(emg1); Serial.print(" ");   //7     //EMG 1
          Serial.print(mmg1); Serial.print(" ");    //8    //MMG 1
          // MOdificacion quitar esclavo
          // res=" 0 1 2 3";        //9 10  11  12           // Podes usarlo
          //Serial.print(" 0 1 2 3"); 
          //Serial.print(ang1yaw,1); Serial.print(" ");//no
          //Serial.print(ang1roll,1); Serial.print(" ");//no
          Serial.print(ang1pitch,1); Serial.print(" ");   
          Serial.print(ang2yaw,1); Serial.print(" "); 
          Serial.print(ang2roll,1); Serial.print(" "); 
          Serial.print(ang2pitch,1); Serial.print(" "); 
          Serial.print(emg2); Serial.print(" ");  //13  // EMG 2
          Serial.println(mmg2);                     //14  // MMG 2
          cont=cont+1;                     
          toogle=!toogle;
          digitalWrite(8,toogle);
          R1=0;
          R2=0;
          c1=0;
          c2=0;
          m=0;
         // delay(20);
          Serial.flush();
          Serial3.flush();
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
    inChar=Serial.read();
       }
   if (cont==4000){
      cont=0;
       }
     
    // Serial.println("llego la n");
  }
   
}
     
       

//
//void diferenciarangulosrout1(float yaw,float roll,float pitch, int cont){
//  
//  if(cont==0){
//    ang1yaw=0;
//    ang1roll=0;
//    ang1pitch=0;
//    ang1yawk1=yaw;
//    ang1rollk1=roll;
//    ang1pitchk1=pitch;
//    }else {
//      
//    ang1yaw=yaw-ang1yawk1;
//    ang1roll=roll-ang1rollk1;
//    ang1pitch=pitch-ang1pitchk1;
//    ang1yawk1=yaw;
//    ang1rollk1=roll;
//    ang1pitchk1=pitch;
//    
//    }
//   
//  
//  }
//
//void diferenciarangulosrout2(float yaw,float roll,float pitch, int cont){
//  
//  if(cont==0){
//    ang2yaw=0;
//    ang2roll=0;
//    ang2pitch=0;
//    ang2yawk1=yaw;
//    ang2rollk1=roll;
//    ang2pitchk1=pitch;
//    }else {
//      
//    ang2yaw=yaw-ang2yawk1;
//    ang2roll=roll-ang2rollk1;
//    ang2pitch=pitch-ang2pitchk1;
//    ang2yawk1=yaw;
//    ang2rollk1=roll;
//    ang2pitchk1=pitch;
//    }
//   
//  
//  }
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



  


