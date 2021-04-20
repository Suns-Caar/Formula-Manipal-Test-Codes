char word[6] = "Hello"; 

void setup() {
 
  Serial.begin(9600);
}

void loop() {
  
  Serial.write(mystr,6); 
  delay(1000);
}
