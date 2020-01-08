`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/27 10:02:40
// Design Name: 
// Module Name: Forward
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Forward(
    R1EX,R1MEM,R1Forward1,R1Forward2
    );
    input R1EX;
    input R1MEM;
    output R1Forward1;
    output R1Forward2;
    
    assign R1Forward1 = R1EX;
    assign R1Forward2 = R1EX | R1MEM;
 
    
    
endmodule
