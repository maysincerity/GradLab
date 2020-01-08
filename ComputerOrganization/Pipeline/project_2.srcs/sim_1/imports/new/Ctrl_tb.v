`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/18 19:11:36
// Design Name: 
// Module Name: Ctrl_tb
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


module Ctrl_tb(

    );


    reg [5: 0] OpCode;
    reg [5: 0] funct;
    
    wire JMP, JR, SignedExt, SLLV, LB, BLTZ, Beq, Bne, Mem2R, MemW, Alusrc, RegW, JAL, RegDst, syscall;
    wire [4: 0] Aluctrl;
    
    Ctrl ctrl1 (JMP, JR, SignedExt, SLLV, LB, BLTZ, OpCode, funct, Beq, Bne, Mem2R, MemW, Aluctrl, Alusrc, RegW, JAL, RegDst, syscall);
    
    initial
    begin
        OpCode<=6'b000000;
        funct<=6'b000000;
        #10;
        
        OpCode<=6'b000000;
        funct<=6'b000011;
        #10;
        
        OpCode<=6'b000000;
        funct<=6'b000010;
        #10;
        
        OpCode<=6'b000000;
        funct<=6'b100000;
        #10;
        
        OpCode<=6'b000000;
        funct<=6'b100001;
        #10;

        OpCode<=6'b000000;
        funct<=6'b100010;
        #10;
        
        OpCode<=6'b000000;
        funct<=6'b100100;
        #10;        
       
        OpCode<=6'b000000;
        funct<=6'b100101;
        #10;   

        OpCode<=6'b000000;
        funct<=6'b100111;
        #10;   

        OpCode<=6'b000000;
        funct<=6'b101010;
        #10;  
        
        OpCode<=6'b000000;
        funct<=6'b101011;
        #10;          

        OpCode<=6'b000000;
        funct<=6'b001000;
        #10;  

        OpCode<=6'b000000;
        funct<=6'b001100;
        #10;  

        OpCode<=6'b000010;
        #10;
        
        OpCode<=6'b000011;
        #10;    
        
         OpCode<=6'b000100;
        #10;        
        
         OpCode<=6'b000101;
        #10;        
        
        OpCode<=6'b001000;
        #10; 


        OpCode<=6'b001100;
        #10;         

        OpCode<=6'b001001;
        #10;    

        OpCode<=6'b001010;
        #10;    
        
        OpCode<=6'b001010;
        #10;         

        OpCode<=6'b001101;
        #10; 

        OpCode<=6'b100011;
        #10; 

        OpCode<=6'b101011;
        #10; 
        
        OpCode<=6'b100000;
        #10;         

        OpCode<=6'b000001;
        #10; 
                    
        OpCode<=0;
        funct<=4;
        #10; 
 
         OpCode<=0;
         funct<=35;
        #10;        
            
    end
    
    
    
    
endmodule
