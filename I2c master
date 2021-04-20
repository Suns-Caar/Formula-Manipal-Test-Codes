#include <Wire.h>
int pushButton = A0;
int x = 0;
void setup()
{
  Wire.begin(); // bus 1 as a master 
  pinMode(pushButton, INPUT);
}

void loop()
{
   Wire.beginTransmission(1); // transmission starts 
   x = digitalRead(pushButton);
   Wire.write(x);
   Wire.endTransmission();
   delay(500);
}
