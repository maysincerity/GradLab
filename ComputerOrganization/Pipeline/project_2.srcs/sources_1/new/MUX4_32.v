`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/20 20:07:22
// Design Name: 
// Module Name: MUX4_32
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


module MUX4_32(Din0,Din1,Din2,Din3,sel,Dout);

input [1:0] sel;
input [31:0] Din0,Din1,Din2,Din3;
output reg [31:0] Dout;

always @(*)
begin  
    case(sel)
    2'b00: Dout<=Din0;
    2'b01: Dout<=Din1;
    2'b10: Dout<=Din2;
    2'b11: Dout<=Din3;
    default: Dout<=Din0;
    endcase
end
endmodule
