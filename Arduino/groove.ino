#include <Wire.h>
#include <MsTimer2.h>
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(11,10);

int waitCount=10;
int t,m,s=0;
unsigned char c;

void startSensor(){
       waitCount--;
}

void times(){
    if(s==59){
        s=0; m++;
    }
    if(m==59){
        m=0; t++;
    }
}
void setup() {
  Serial.begin(9600);
  bluetooth.begin(115200);
  Serial.println("Start :: ");
   MsTimer2::set(1000,startSensor);
   MsTimer2::start();
  Wire.begin();
 
}
void loop() {
    if(waitCount<=0){
          s++;
          Wire.requestFrom(0xA0 >> 1, 1);    // request 1 bytes from slave device 
          while(Wire.available()) {          // slave may send less than requested
            c = Wire.read();   // receive heart rate value (a byte)
            
            Serial.print("Senser Value :");
            bluetooth.print("Senser Value :");
            
            Serial.print(c, DEC);   
            Serial.print("   ");   Serial.print(t); Serial.print(":"); Serial.print(m);  Serial.print(":");  Serial.println(s);    
            
            bluetooth.print(c, DEC);     
            bluetooth.print("   ");   bluetooth.print(t); bluetooth.print(":"); bluetooth.print(m);  bluetooth.print(":");  bluetooth.println(s);  
            times();
            }
         delay(1000);
  }
  else{
      Serial.print("wait..");
      Serial.println(waitCount);
      delay(1000);
  }
}
