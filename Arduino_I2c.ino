#include <Wire.h>
int i;
int data[11];
char data1[11];
void setup() {
  Serial.begin(115200);
  Wire.setClock(100000);
  Wire.begin(4); 
  Wire.onReceive(receiveEvent);
}

void loop() {
//  for(i =0; i<8;i++){
//   Serial.println(data[i]);
//  }
  for(i =0; i<11;i++){
   Serial.println(data1[i]);
  }
}
void receiveEvent(char bytes) {
 for(i=0;i<10;i++){
  data1[i] = Wire.read();
  
  }
}
