char mystr[10]; 
void setup() {
 
  Serial.begin(9600);
}

void loop() {
  Serial.readBytes(mystr,6); //Read the serial data and store in var
  Serial.println(mystr); //Print data on Serial Monitor
  delay(1000);
}
