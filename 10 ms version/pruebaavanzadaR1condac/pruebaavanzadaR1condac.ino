#include <Wire.h>
#include <DueTimer.h>
String cadtemp;


//MID

#define WAKEUP  0x3E
#define GOTOCONFIG  0x30
#define GOTOMEAS  0x10
#define RESET  0x40
#define REQDID  0x00
#define DEVID  0x01
#define INITMT  0x02
#define INITMTRES  0x03
#define REQPRODUCT  0x1C 
#define PRODUCT  0x1D
#define REQFIRM  0x12
#define FIRM  0x13
#define REQDATALEN  0x0a
#define DATALEN  0x0b
#define RUNSELFTEST 0x24
#define SELFTEST  0x25
#define ERROR  0x42 
#define REQGPSSTAT  0xA6
#define GPSSTAT  0xA7
#define REQBAUD  0x18
#define FACTORYRESET  0x0E
#define SYNCINSET  0xD6
#define SYNCOUTSET  0xD8
#define REQCONFIg  0x0C
#define CONFIG  0x0D
#define PERIOD  0x04
#define OUTSETTING  0xD2
#define REQDATA  0x34
#define DATA  0x32
#define DATA2  0x36
#define RESETORIENTATION  0xA4
#define REQUTC  0x60
#define UTC  0x61

//OPCODE
#define MTI_ADD  0x6B
#define CONTROL_PIPE 0X03
#define PIPE_STATUS 0X04
#define NOTIFICATION_PIPE  0x05
#define MEASUREMENT_PIPE 0X06



//DATAID
#define PACKCOUNTER  0x1020
#define SAMPTIME  0x1060
#define QUAT  0x2010
#define ROTMAT  0x2020
#define EULERD  0x2030
#define DV  0x4010
#define ACCEL  0x4020
#define FACCEL  0x4030
#define ACCELHR  0x4040
#define ROT  0x8020
#define DQ  0x8030
#define ROTHR  0x8040
#define MAG  0xC020
#define STAT  0xE020




float headingYaw=0;
float headingRoll=0;
float headingPitch=0;
float mmg1=0,mmg2=0,emg1=0,emg2=0; 
int cont=0;
int i=0;
int DRDY=0;
uint8_t data[4];
//uint8_t Setoutput[45]={0xFA, 0xFF, 0xC0 ,0x28, 0x10, 0x20, 0xFF, 0xFF, 0x10, 0x60, 0xFF, 0xFF, 0x20, 0x10, 0x00, 0x64, 0x40,0x20,0x01, 0x90, 0x80, 0x20, 0x01, 0x90, 0xC0, 0x20, 0x00, 0x64, 0xE0, 0x20, 0xFF, 0xFF, 0x50, 0x42, 0x00, 0x64, 0x50, 0x22, 0x00, 0x64, 0xD0, 0x12, 0x00, 0x64, 0x73};
uint8_t Setoutput[11]={0xC0,0x08, 0x40,0x20,0x01,0x90,0x20, 0x10, 0x00, 0x64,0xB4};
//uint8_t Setoutput[15]={0xC0,0x0C,0x20, 0x10, 0x00, 0x64, 0x40,0x20,0x00,0x64,0xE0,0x20,0xFF,0XFF,0xDF};
//uint8_t Setoutput[45]={0xC0 ,0x10, 0x10, 0x20, 0xFF ,0xFF ,0x10, 0x60, 0xFF, 0xFF, 0x20, 0x10, 0x00, 0x64, 0x40, 0x20, 0x01, 0x90 ,0x10};
uint8_t goconfig[3]={0x30, 0x00, 0xD1};
uint8_t resetorientation[5]={0xA4, 0x02, 0x00, 0x04, 0x57};
//byte goconfig[5]={ 0xFA, 0xFF, 0x30, 0x00, 0xD1};
//uint8_t filterprofile[7]={ 0xFA, 0xFF, 0x64, 0x02, 0x00, 0x02, 0x99};
uint8_t filterprofile[5]={0x64, 0x02, 0x00, 0x32, 0x69};
//uint8_t gomeas[5]={0xFA, 0xFF, 0x10, 0x00, 0xF1};
uint8_t gomeas[3]={0x10, 0x00, 0xF1};
uint8_t datanotif[4];
uint8_t datameas[256];
uint16_t notificationSize;
uint16_t measurementSize;
float quat[4]={0};
float accel[3]={0};
float mag[3]={0};
float rot[3]={0};
char aa;
int pruebat=0;


float time1=0;
float time2=0;
float angx=0;
float angy=0;
float angz=0;

float ax=0;
float ay=0;
float az=0;

String mensaje;
String aux="hola";

void LEERANGX(){
pruebat=map(analogRead(A11),2000,2680,0,1000);
if(pruebat<0)pruebat=0;
if(pruebat>1000)pruebat=1000;

angx=(360*pruebat)/1000;
}

void readata(){
 
 while(!DRDY){
  DRDY=digitalRead(3);
  }
  DRDY=0;
  
  readxsens();
 //Serial.println("sali");
}


 
void setup() {
  // put your setup code here, to run once:
Wire.begin();
Serial.begin(115200);
Serial2.begin(115200);
Timer3.attachInterrupt(readata);
//analogReadResolution(12);
//Serial.setTimeout(5);
//Serial1.begin(115200);
//pinMode(21,INPUT_PULLUP);
//pinMode(20,INPUT_PULLUP);
pinMode(43,INPUT);
pinMode(45,INPUT);
pinMode(47,INPUT);
pinMode(49,INPUT);
pinMode(51,INPUT);
pinMode(53,INPUT);
//attachInterrupt(43, LEERANGX, RISING);
//attachInterrupt(49, LEERANGY, RISING);
//attachInterrupt(50, LEERANGZ, RISING);
//attachInterrupt(51, LEERAX, RISING);
//attachInterrupt(52, LEERAY, RISING);
//attachInterrupt(53, LEERAZ, RISING);
pinMode(13,OUTPUT);
pinMode(12,OUTPUT);
pinMode(11,OUTPUT); 
pinMode(4,OUTPUT);

digitalWrite(13,HIGH);
digitalWrite(12,HIGH);
digitalWrite(11,HIGH);

digitalWrite(4,HIGH);

digitalWrite(4,LOW);

delay(2000);

  
  Wire.beginTransmission(0x6B);          //test for MPR121 on second TWI port
  
  word error = Wire.endTransmission();
  
  delay(10);

  if (error == 0)  {
   // Serial.println("0x68 1 found");
    //call MPR121 initialization function here
  }
  else  {
 //   Serial.println("0x68 1 not found");
  }


}

int flag=0;  
int por=0;

void loop() {

  if(flag==0){
  while(!DRDY){
  DRDY=digitalRead(3);
  }
  DRDY=0;
  
  readxsens();
  if(cont==0){
  Serial.println("Goto config");
  sendgotoconfig();
  cont=1;
  flag=0;
  }

  if(datanotif[0]==0x31){
  Serial.println("config state reached");
  sendsetoutput();
  flag=0;
  }
  if(datanotif[0]==0xC1){
  Serial.println("Set output reached");
  filteroutput();
  flag=0;
  //sendgotomeas();
  }

  if(datanotif[0]==0x65){
  Serial.println("Set filter reached");
  //filteroutput();
  sendgotomeas();
  flag=0;
  }
  
  if(datanotif[0]==0x11){
  Serial.println("Back to measure");
 if(flag==0){
  Serial.println("entro una vez");
  flag=1;
  Timer3.start(100000);
  }
    if(por==0){
    resetorient();
    por=1;
    }
  }

Serial.println("sigo aca");
delay(5);
cadtemp="";
  }else{
emg1=analogRead(A8);
mmg1=analogRead(A7);
mmg2=analogRead(A6);
emg2=analogRead(A0);



mensaje=Serial2.readStringUntil('n');
Serial2.flush();

//Serial.println(mensaje);
//Serial.println(mensaje.length());
//Serial.println(mensaje.substring(0,6));
//Serial.println(mensaje.substring(7,13));
//Serial.println(mensaje.substring(14,20));
//Serial.println(mensaje.substring(21,27));
//Serial.println(mensaje.substring(28,34));
//Serial.println(mensaje.substring(35,41));
//Serial.println(mensaje.substring(42,48));
  
//cadtemp+=' '+mensaje;
if(mensaje.length()!=48){
  
//Serial.println(aux); 
}else{

cadtemp+='Z';
cadtemp+=' '+convertfloattostring(mensaje.substring(28,34).toFloat());//ax
cadtemp+=' '+convertfloattostring(mensaje.substring(35,41).toFloat());//ay
cadtemp+=' '+convertfloattostring(mensaje.substring(42,48).toFloat());//az
cadtemp+=' '+convertfloattostring(quat[0]);//yaw 1
cadtemp+=' '+convertfloattostring(quat[1]);//cont
cadtemp+=' '+convertfloattostring(quat[2]);// roll 1
cadtemp+=' '+convertfloattostring(emg1);//emg1
cadtemp+=' '+convertfloattostring(mmg1);//mmg1
cadtemp+=' '+convertfloattostring(quat[3]);//pitch 1
cadtemp+=' '+convertfloattostring(mensaje.substring(0,6).toFloat());//yaw 2
cadtemp+=' '+convertfloattostring(mensaje.substring(7,13).toFloat());//roll 2
cadtemp+=' '+convertfloattostring(mensaje.substring(14,20).toFloat());//pitch 2
cadtemp+=' '+convertfloattostring(emg2);//emg2
cadtemp+=' '+convertfloattostring(mmg2);//mmg2
cadtemp+=' '+convertfloattostring(mensaje.substring(21,27).toFloat());//pitch 2

Serial.println(cadtemp);
aux=cadtemp;

}
 time2=micros();
 //Serial.println(time2-time1);
 time1=time2;
//Serial.println(".");

delay(1);
cadtemp="";
  }

}



String convertfloattostring(float a){
  
  String datastring="";
  if(a>0)datastring+='+';
  datastring+=String(a);
  datastring+="0000";
  datastring=datastring.substring(0,6);
  return datastring;
  }


 
  

void readpipestatus(){

  Wire.beginTransmission(MTI_ADD);
  Wire.write(PIPE_STATUS);
  Wire.endTransmission(false); 
  
  Wire.requestFrom(MTI_ADD,(uint8_t)4);
  
   for(i=0;i<4;i++){

   data[i]=Wire.read();

   }
   Wire.endTransmission();
//  for(i=0;i<4;i++){
//
//  Serial.println(data[i],HEX);
//
//   } 

  notificationSize=(uint16_t)data[0]|((uint16_t)data[1]<<8);
  measurementSize=(uint16_t)data[2]|((uint16_t)data[3]<<8);
  if(notificationSize>0xFF)notificationSize=0;
  if(measurementSize>0xFFF)measurementSize=0;
//  Serial.println("notification:");
//  Serial.println(notificationSize);
//  Serial.println("measurement:");
//  Serial.println(measurementSize); 

  }


void readPipeNotif(){

  if(notificationSize>1){
  Wire.beginTransmission(MTI_ADD);
  Wire.write(NOTIFICATION_PIPE);
  Wire.endTransmission(false);
  

  Wire.requestFrom(MTI_ADD,notificationSize);
  if(Wire.available()>0) {
    for(int i = 0; i < notificationSize; ++i){
      datanotif[i] = Wire.read();
    }
  }

   Wire.endTransmission();
//  for(i=0;i<notificationSize;i++){
//
//  Serial.println(datanotif[i],HEX);
//
//   } 

  }
}
  
void readPipeMeas(){
  
  if(measurementSize>1){
  Wire.beginTransmission(MTI_ADD);
  Wire.write(MEASUREMENT_PIPE);
  Wire.endTransmission(false);
  
  Wire.requestFrom(MTI_ADD,measurementSize);
 // delay(10);
  if(Wire.available()>0) {
    for(int i = 0; i < measurementSize; ++i){
      datameas[i] = Wire.read();
      //Wire.endTransmission(false);
      //delayMicroseconds(20);
   //   delay(1);
    }
  }

  // Wire.endTransmission();

//  for(i=0;i<measurementSize;i++){
//
//  Serial.println(datameas[i],HEX);
//
//   } 
  
}
}

void dataswapendian(uint8_t* data, uint8_t length){

  uint8_t cpy[length];
  memcpy(cpy,data,length);
  for(int i = 0; i < length/4; i++){
    for(int j = 0; j < 4; j++){
      data[j+i*4] = cpy[3-j+i*4];
    }
  }
  
  }

void parseData(uint8_t* data, uint8_t datalength){

  if(datalength < 2)
  return;
 
  uint8_t actualMID = data[0];
 // Serial.println("data en la pos 0:");
 //  Serial.println(data[0],HEX); 
  if(actualMID == 0x10 || actualMID == 0x20 || actualMID == 0x40 || actualMID == 0x80 || actualMID == 0xC0 || actualMID == 0xE0){
    uint8_t length = data[2];
 //  Serial.println("data en la pos 2:");
  // Serial.println(data[2],HEX); 
    switch(((uint16_t)data[1] |((uint16_t)data[0]<<8)) & (uint16_t)0xFFF0){
    case (uint16_t)QUAT:
   //   Serial.println("entro a quat");
      dataswapendian(data+3, sizeof(float)*4);
      memcpy(quat, data+3, sizeof(float)*4);
//      Serial.println("quaternion");
//       for(i=0;i<4;i++){
//       Serial.println(quat[i],8);
//       } 
      break;
    case (uint16_t)ACCEL:
      dataswapendian(data+3, 3*sizeof(float));
      memcpy(accel, data+3, sizeof(float)*3);
      break;
    case (uint16_t)MAG:
      dataswapendian(data+3, 3*sizeof(float));
      memcpy(mag, data+3, sizeof(float)*3);
      break;
    case (uint16_t)ROT:
      dataswapendian(data+3, 3*sizeof(float));
      memcpy(rot, data+3, sizeof(float)*3);
      break;
    }
    parseData(data+3+length, datalength - length - 3);
  }else{
    uint8_t length = data[1];
    if(actualMID == (uint8_t)DATA || actualMID == (uint8_t)DATA2)
    parseData(data+2, length);
  }
  
  
}

void quatToEulerAngles(){
  
  headingYaw = atan2(2*(quat[0]*quat[3]+quat[1]*quat[2]), 1-2*(quat[2]*quat[2]+quat[3]*quat[3]));
  headingRoll= atan2(2*(quat[0]*quat[1]+quat[3]*quat[2]), 1-2*(quat[1]*quat[1]+quat[2]*quat[2]));
  headingPitch=asin(2*(quat[0]*quat[2]-quat[1]*quat[3]));
  headingYaw*=180.0f/PI;
  headingRoll*=180.0f/PI;
  headingPitch*=180.0f/PI;
  }


  void readxsens(){
  readpipestatus();
  readPipeNotif();
  readPipeMeas();
  parseData(datameas, measurementSize);
  
  //quatToEulerAngles();
  
  }  




  uint8_t* buildMessage(int MID, uint8_t* data, uint8_t length){
  
  uint8_t* res = new uint8_t[length+4];
  res[0] = (uint8_t)MID;
  res[1] = length;
  if(length > 0 && data != NULL)
    memcpy(res+2, data, length);
  else
    res[2] = 0;
  uint8_t checksum = 0xFF;
  for(int i = 0; i< length+2; i++)
    checksum -= res[0];
  res[length+3] = checksum;
  
  return res;
  
  }


void resetorient(){
  Wire.beginTransmission(MTI_ADD);
  Wire.write(CONTROL_PIPE);
    for(int i = 0; i < sizeof(resetorientation); ++i){
      Wire.write(resetorientation[i]);
    }
   Wire.endTransmission();
}


void sendgotoconfig(){
  Wire.beginTransmission(MTI_ADD);
  Wire.write(CONTROL_PIPE);
    for(int i = 0; i < sizeof(goconfig); ++i){
      Wire.write(goconfig[i]);
    }
   Wire.endTransmission();
}

void sendgotomeas(){
   
  Wire.beginTransmission(MTI_ADD);
  Wire.write(CONTROL_PIPE);
    for(int i = 0; i < sizeof(gomeas); ++i){
      Wire.write(gomeas[i]);
   
    }
  Wire.endTransmission();
}


void sendsetoutput(){
  Wire.beginTransmission(MTI_ADD);
  Wire.write(CONTROL_PIPE);
    for(int i = 0; i < sizeof(Setoutput); ++i){
      Wire.write(Setoutput[i]);
       
    }
    Wire.endTransmission();
}

void filteroutput(){
  Wire.beginTransmission(MTI_ADD);
  Wire.write(CONTROL_PIPE);
    for(int i = 0; i < sizeof(filterprofile); ++i){
      Wire.write(filterprofile[i]);
       
    }
    Wire.endTransmission();
}
uint8_t checksum(uint8_t BID,uint8_t MID,uint8_t LEN,uint8_t DA){
  
  uint8_t res;

  res =0x100 - (BID+MID+LEN+DA) & 0xFF;

  return res;
  }
  
