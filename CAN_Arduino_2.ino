#include <SPI.h>
#include <mcp2515.h>
struct can_frame canMsg;
int i =0;
struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;

MCP2515 mcp2515(10);


void setup() {
  canMsg1.can_id  = 2;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 1;
  canMsg1.data[1] = 69;
  canMsg1.data[2] = 69;
  canMsg1.data[3] = 69;
  canMsg1.data[4] = 69;
  canMsg1.data[5] = 69;
  canMsg1.data[6] = 69;
  canMsg1.data[7] = 69;

  canMsg2.can_id  = 0x036;
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 6;
  canMsg2.data[1] = 5;
  canMsg2.data[2] = 7;
  canMsg2.data[3] = 5;
  canMsg2.data[4] = 2;
  canMsg2.data[5] = 4;
  canMsg2.data[6] = 57;
  canMsg2.data[7] = 3;

   canMsg3.can_id  = 1;
  canMsg3.can_dlc = 8;
  canMsg3.data[0] = 12;
  canMsg3.data[1] = 0x00;
  canMsg3.data[2] = 0x00;
  canMsg3.data[3] = 12;
  canMsg3.data[4] = 12;
  canMsg3.data[5] = 0x00;
  canMsg3.data[6] = 0x00;
  canMsg3.data[7] = 11;
  
  
  while (!Serial);
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_40KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {
  mcp2515.readMessage(&canMsg); 
   if(canMsg.can_id == 2){
    mcp2515.sendMessage(&canMsg3);
    delay(100);
      Serial.println(i);
      i++;
   }  
}
