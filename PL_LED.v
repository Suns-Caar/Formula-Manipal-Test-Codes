/***************************************************
To Blink the PL LED on the Minized Board through FPGA using Fabric clock 

1. Perform all the steps done in the Lab 1 and Lab 2 of the the Avnet Minized course 
2. Make sure the DDR configuration is set MT41K256M16 RE -125 with DDR 3 LV
3. Clock configuration is enable with Fclk0 set to 50MHz 
4. Check the UART 1 box with MIO set to 48-49 (for booting up the PS using Hello World program)
5. Run block Automation 
6. make a Design source in verilog file (no need to define input outputs here)
7. Copy paste this Verilog code and dont forget to change the module name 
8. drag and drop the verilog file from the sources tab to the design 
9. Make led pin external and connect the FCLK to CLK 
10. run implementation 
11. ASSIGN THE LED PIN THE E13 ON THE PACKAGE WITH LVCMOS33 VOLTAGE 
12.Generate Bitstream, export and launch SDK 
13.Create an Application project program the FPGA and run the empty application or Hello world program 
(Incase PS7_CortexA9_  does not exist error comes, try making the project again from scratch )
***********************************************/

module Pl_led_final(
    output LED,// Led mapped to E13 pin having yellow colour 
    input clk ,// Fclk 
    input reset_n// no need 
    );
	 
integer counter;
reg state;// sets the PL to 0-1 

always @ (posedge clk) begin
   counter <= counter + 1;
  if(counter >= 50000000 )begin //50 million represents 1 sec delay 
		counter <=0;
		state <= !state;
	 end
    
end
	 
assign LED = state;	
  
  
  
// PL LED Controlled via PWM 
  // same procedure as above 
  
  module pwmLED(
		input clk, 
		input reset_n,
		output PL_LED_G
		);
reg [25:0] count;
reg [7:0] pwm;
always @(posedge clk or negedge reset_n)
	begin
	if(!reset_n) begin
			 count<=0;
			end else begin
   		  count<=count+1;
   		end
	end
wire [6:0] pwm_input = count[25] ? count[24:18] : ~count[24:18];
always @(posedge clk)
begin
   pwm <= pwm[6:0]+pwm_input;
end
assign PL_LED_G = pwm[7];
endmodule 
  
