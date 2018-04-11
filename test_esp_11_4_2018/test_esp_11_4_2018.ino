#include <ESP8266WiFi.h>
#include <time.h>
//#include <SoftwareSerial.h>


#include <FirebaseArduino.h>
//SoftwareSerial mySerial(4,5);//rx,tx
//#define FIREBASE_HOST "nodemcu-6edef.firebaseio.com"
//#define FIREBASE_AUTH "SAAcgZRobeLw4eEBmltPc9AbfJyhsvFEwy5lKAyA"

#define FIREBASE_HOST "esp-android-45417.firebaseio.com"
#define FIREBASE_AUTH "q1D40Nuj8OymmG1f7flGqvL2LmcGIOrdSiRtYpau"


#define WIFI_SSID "ThanhHoai"
#define WIFI_PASSWORD "11119999"
#define MAX485_DE      4
#define MAX485_RE_NEG  5

unsigned char RxData[11]={
  0x01
};
unsigned char i,j,FLG,Rxflag;
unsigned short CRC;
unsigned long Time1,a;
int dem;
int timezone = 7;
int dst = 0;


void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, HIGH);
  digitalWrite(MAX485_DE, HIGH);
};

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, LOW);
  digitalWrite(MAX485_DE, LOW);
};

void CRC_RX(unsigned char Data[],unsigned char length)
      {
        unsigned char *Pointer;
        int i;
        unsigned int crc = 0xFFFF;
        unsigned char ByteCtr;
        Pointer=(unsigned char *)Data;
        ByteCtr =length-2;
        while (ByteCtr) {

          crc ^= *Pointer++;          // XOR byte into least sig. byte of crc

          for (i = 8; i != 0; i--) {    // Loop over each bit
            if ((crc & 0x0001) != 0) {      // If the LSB is set
              crc >>= 1;                    // Shift right and XOR 0xA001
              crc ^= 0xA001;
            }
            else                            // Else LSB is not set
              crc >>= 1;                    // Just shift right
          }
          ByteCtr--;
        }
        int crcl = (crc & 0xFF00)/256; //Swaping  the bytes
        int crch = (crc & 0x00FF);
        CRC = crch <<8| crcl;
        //if (CRC==Data[length-2]*256+Data[length-1]) FLG=1; else FLG=0;
        if (CRC==(Data[length-2]<<8|Data[length-1])) FLG=1; else FLG=0;
      }


void receivedata(){
  while (Serial.available()>0)
  {
    a=millis()-Time1;
    Time1=millis(); 
    if(a>=500) {j=0;}
    RxData[j]=Serial.read();
    j+=1;
    if (j==11) {FLG=0;CRC_RX(RxData,11);j=0;}
    
  }
};
      
void setup(){
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  pinMode(0,OUTPUT);
  pinMode(14,OUTPUT);
  digitalWrite(14,LOW);
  digitalWrite(0,LOW);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
    Serial.begin(9600);
  //mySerial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  };
  //Serial.println();
  //Serial.print("connected: ");
  //Serial.println(WiFi.localIP());
  configTime(timezone * 3600, dst * 0, "pool.ntp.org", "time.nist.gov");
  //Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    //Serial.print(".");
    delay(1000);
  }
  
      
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("STARTup","ready"); 
}

/*
void loop(){

while (WiFi.status() == WL_CONNECTED) {
  receivedata();
  //while((mySerial.available()>0)){
    //a=millis()-Time1;
    //Time1=millis(); 
  //if(a>=500) {i=1;}
  //if(i>=6) {i=0;FLG=1;}
  //RxData[i]=mySerial.read();
  //Serial.println(RxData[i],HEX);
  //Serial.print("*****time = ");
    //Serial.println(a);
  //i++;
 // }
  if(FLG==1){
  Firebase.setInt("Rx_0",(int)RxData[0]);
  Firebase.setInt("Rx_1",(int)RxData[1]); 
  Firebase.setInt("Rx_2",(int)RxData[2]); 
  Firebase.setInt("Rx_3",(int)RxData[3]);
  Firebase.setInt("Rx_4",(int)RxData[4]); 
  Firebase.setInt("Rx_5",(int)RxData[5]); 
  digitalWrite(0,HIGH);
  }
  if(FLG==0) digitalWrite(0,LOW);
  // Firebase.setInt("Test",12345);
  delay(500);
}}

*/

void loop(){          //xong,chạy ngon, nhớ nối đất chung.
  while (WiFi.status() == WL_CONNECTED) {
        while (Serial.available()>0)
  {
    RxData[j]=Serial.read();
    j+=1;
    if (j==11) {FLG=0;CRC_RX(RxData,11);j=0;}}
    if(FLG==1){
      digitalWrite(0,HIGH);
      time_t rawtime;
      struct tm * timeinfo;
      char buffer [80];
      time (&rawtime);
      timeinfo = localtime (&rawtime);
      Firebase.setString("STARTup","ok"); 
      //String chuoi =strcpy( "DATA/",String(dem));
  //Firebase.setInt("DATA/"+dem+"/HOUR",(int)RxData[3]<<8|RxData[4]); 
  //Firebase.setInt("DATA/"+dem+"/MINUTE",(int)RxData[5]<<8|RxData[6]); 
  //Firebase.setInt("DATA/"+dem+"/SECOND",(int)RxData[7]<<8|RxData[8]);
  Firebase.setInt("DATA/HOUR",(int)RxData[3]<<8|RxData[4]); 
  Firebase.setInt("DATA/MINUTE",(int)RxData[5]<<8|RxData[6]); 
  Firebase.setInt("DATA/SECOND",(int)RxData[7]<<8|RxData[8]);
  strftime (buffer,80,"%X",timeinfo);
  Firebase.setString("DATA/TIME",buffer);
  digitalWrite(0,LOW);
  }
  
  if(FLG==0) //digitalWrite(0,LOW);
  {Firebase.setString("STARTup","fail"); 
  Firebase.setInt("Rx_0",RxData[0]); 
  Firebase.setInt("Rx_1",RxData[1]); 
  Firebase.setInt("Rx_2",RxData[2]);
  Firebase.setInt("Rx_3",RxData[3]); 
  Firebase.setInt("Rx_4",RxData[4]); 
  Firebase.setInt("Rx_5",RxData[5]);
  Firebase.setInt("Rx_6",RxData[6]); 
  Firebase.setInt("Rx_7",RxData[7]);
  Firebase.setInt("Rx_8",RxData[8]); 
  Firebase.setInt("Rx_9",RxData[9]); 
  Firebase.setInt("Rx_10",RxData[10]);
FLG=0;
  digitalWrite(0,LOW);}
    delay(1000);
  FLG=0;
  dem++;
  }
  }
