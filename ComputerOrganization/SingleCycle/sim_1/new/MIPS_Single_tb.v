`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/19 19:52:51
// Design Name: 
// Module Name: MIPS_Single_tb
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


module MIPS_Single_tb(

    );
    
    reg clk;
    reg rst;
    reg go;
    wire [7:0] an;
    wire [7:0] seg;
    wire [15:0] led;
    //wire PCen;
    //wire JMP, JR, SignedExt, SLLV, LB, BLTZ, Beq, Bne, Mem2R, MemW, Alusrc, RegW, JAL, RegDst, syscall;
    //wire [5:0] OpCode;
   // wire [5:0] funct;
    //wire [31:0] regR1Data;
    
     MIPS_Single cpu1(
       clk, rst, go, an, seg, led//, PCen,JMP, JR, SignedExt, SLLV, LB, BLTZ, Beq, Bne, Mem2R, MemW, Alusrc, RegW, JAL, RegDst, syscall, OpCode, funct, regR1Data
       );
    
    
    
	always 
    begin
          clk<=0;
          #1;
          clk<=1;
          #1;
    end
    
    initial
    begin
        go<=0;
        rst<=1;
        #1;
        rst<=0;
    end
    
    
endmodule
