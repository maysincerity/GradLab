`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/27 10:09:37
// Design Name: 
// Module Name: ID_EX
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

    
module IF_ID (clk, rst,ena,instr_in,PC_in,instr_out,PC_out);
               
   input         clk;
   input         rst;
   input         ena;
   input  [31:0] instr_in;
   input  [31:0] PC_in;


   //output
   output [31:0] instr_out;
   output [31:0] PC_out;

    
    reg_pipeline #(32) Instr_Code (   clk,
                           0,
                           ena,
                           rst ? 0 : instr_in ,
                           instr_out);
    
               
    reg_pipeline #(32) PC_IN (   clk,
                                      0,
                                      ena,
                                      rst ? 0 : PC_in ,
                                      PC_out);
                            
endmodule

    
    
    
    
    
