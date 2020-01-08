`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/27 10:27:40
// Design Name: 
// Module Name: reg_pipeline
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


module reg_pipeline# (parameter in_width = 1)(
    input clk,
    input rst,
    input ena,
    input [in_width-1:0]data_in,
    output [in_width-1:0]data_out
    );
    
    reg [in_width-1:0]data = 0;
    
    always @(posedge clk or posedge rst) begin
        if(rst) data<=0;        //reset key
        else begin
            if(ena) data<=data_in;        //enable ,input
        end
    end
    
    assign data_out = data;

endmodule
