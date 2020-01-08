`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/19 09:08:10
// Design Name: 
// Module Name: divider
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

module divider
#(parameter N=1000)        // N=fclk/fclk_N
(clk,halt,clk_N, fre_sw);
input clk;                      // 系统时钟
input halt;
input fre_sw;
output reg clk_N;                   // 分频后的时钟


reg [31:0] counter;             /* 计数器变量，通过计数实现分频。
                                   当计数器从0计数到(N/2-1)时，
                                   输出时钟翻转，计数器清零 */
always @(posedge clk)  begin    // 时钟上升沿
     if(halt==0) counter = counter +1   ;                     // 功能实现

     case(fre_sw)
         1'b0:      
         if(counter>=N/2-1) begin
          counter = 0;
          clk_N = ~clk_N;
         end
         1'b1: 
         if(counter>=100/2-1) begin
          counter = 0;
          clk_N = ~clk_N;
         end
         default: clk_N = ~clk_N;
     endcase    
     
//     if(counter>=N/2-1) begin
//          counter = 0;
//          clk_N = ~clk_N;
//     end
end                           
endmodule
