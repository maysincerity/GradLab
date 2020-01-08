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
input clk;                      // ϵͳʱ��
input halt;
input fre_sw;
output reg clk_N;                   // ��Ƶ���ʱ��


reg [31:0] counter;             /* ������������ͨ������ʵ�ַ�Ƶ��
                                   ����������0������(N/2-1)ʱ��
                                   ���ʱ�ӷ�ת������������ */
always @(posedge clk)  begin    // ʱ��������
     if(halt==0) counter = counter +1   ;                     // ����ʵ��

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
