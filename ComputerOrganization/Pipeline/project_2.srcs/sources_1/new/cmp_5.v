`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/19 08:46:04
// Design Name: 
// Module Name: compare32
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


module compare5(
    A, B, C
    );
    
    input [4:0] A;
    input [4:0] B;
    
    output reg C;
    

    always@(A or B) begin
        if(A == B) C<=1;
        else C<=0;
    end

endmodule
