#include <SoftwareSerial.h>
/*-----( Khai báo hằng số và Pin )-----*/
#define SSerialRX        4  //Serial Receive pin
#define SSerialTX        5  //Serial Transmit pin
 
#define SSerialTxControl 2   //Kiểm soát hướng cho rs485
 
#define RS485Transmit    HIGH
#define RS485Receive     LOW
 
#define Pin13LED         13
 
/*-----( Khai báo đối tượng )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
 
/*-----( Khai báo biến )-----*/
int byteReceived;
int byteSend;
int j=0;
unsigned char TxMsg [11]={0x01,0x04,0x06,0x00,0x02,0x00,0x09,0x00,0x0a,0x49,0x56};
void setup()   /****** SETUP ******/
{
  Serial.begin(9600);
  Serial.println("Arduino.vn");
  Serial.println("Su dung Serial Monitor, go vao cua so phia tren, ENTER");
  
  pinMode(Pin13LED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);  
  pinMode(3,OUTPUT);  
  
  //digitalWrite(SSerialTxControl, RS485Receive);  // bật chế độ thu phát 
  
  // Khởi động kết nối tới thiest bị khác
  RS485Serial.begin(9600);   // cài đặt tốc độ truyền dữ liệu 
     digitalWrite(SSerialTxControl, RS485Transmit);  // Cho phép rs485 nhận dữ liệu  
    digitalWrite(3,HIGH);
}//--(end setup )---
 ///
 
void loop()   /****** LOOP ******/
{     digitalWrite(SSerialTxControl, RS485Transmit);  // Cho phép rs485 nhận dữ liệu  
    digitalWrite(3,HIGH);
  digitalWrite(Pin13LED, HIGH);  
for(j=0;j<11;j++)
  {
    //byteReceived = Serial.read();
    

   // Serial.print(TxMsg[j],HEX);          // gửi byte cho arudino thu
    RS485Serial.write(TxMsg[j]);
    //digitalWrite(Pin13LED, LOW);   
    //delay(10);
    //digitalWrite(SSerialTxControl, RS485Receive);  // Vô hiệu hóa quá trình nhận dữ liệu     
  }
  //Serial.println("");
  j=0;
  digitalWrite(Pin13LED,LOW);
  delay(1000);
  /*
  if (RS485Serial.available())  //tìm dữ liệu từ thiết bị khác truyền về
   {
    digitalWrite(Pin13LED, HIGH);  
    byteReceived = RS485Serial.read();    // đọc byte nhận được
    Serial.write(byteReceived);        // hiển thị lên Serial Monitor
    delay(10);
    digitalWrite(Pin13LED, LOW);   
   }  
 */
}
