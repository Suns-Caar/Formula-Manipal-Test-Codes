#include <SPI.h>
#include <mcp2515.h>
struct can_frame canMsg;
struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;

MCP2515 mcp2515(10);


void setup() {
  canMsg1.can_id  = 2;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 1;
  canMsg1.data[1] = 2;
  canMsg1.data[2] = 3;
  canMsg1.data[3] = 4;
  canMsg1.data[4] = 5;
  canMsg1.data[5] = 6;
  canMsg1.data[6] = 7;
  canMsg1.data[7] = 8;

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

   canMsg3.can_id  = 3;
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
   Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
  

}

void loop() {
if(mcp2515.readMessage(&canMsg) ==  MCP2515::ERROR_OK){
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      Serial.print(canMsg.data[i],HEX);
      Serial.print(" ");
    }
     mcp2515.sendMessage(&canMsg1);
//  Serial.println(i);
//  delay(100);
  delay(2);
    mcp2515.sendMessage(&canMsg2);
// 
delay(2);

    Serial.println();      
}
//      }  
}
