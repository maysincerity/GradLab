`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/19 08:26:31
// Design Name: 
// Module Name: Display
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


module reveal(halt,clk,num,an,seg);
input halt;   //��ͣ�ź�
input clk;
input  [31:0]num;
output [7:0] seg; //7�����������
output [7:0] an;  //7�������Ƭѡ�ź�

wire clk_N;
wire [2:0]out;
wire [3:0]seginput;

wire [31:0]num;


  

divider #(10000)div(clk,0,clk_N);//8.���д���Ϊ�Ͽ�����Ĵ���
 //assign clk_N = clk;//8.���д���Ϊ�������Ĵ���
 
segcounter cou(clk_N,out);
_7Seg_Driver_Choice cho(out,an);
evalu eva(out,num,seginput);
_7Seg_Driver_Decode dec (seginput,seg);


endmodule

module _7Seg_Driver_Decode(sw, seg);//7����ʾ������
   input [3:0] sw;      //����16λ����
   output reg [7:0] seg; //7λ���������

   
   always @(sw) begin
      case(sw[3:0])
      
      4'b0000 :seg[7:0]=8'b11000000;
      4'b0001 :seg[7:0]=8'b11111001;
      4'b0010 :seg[7:0]=8'b10100100;
      4'b0011 :seg[7:0]=8'b10110000;
      4'b0100 :seg[7:0]=8'b10011001;
      4'b0101 :seg[7:0]=8'b10010010;
      4'b0110 :seg[7:0]=8'b10000010;
      4'b0111 :seg[7:0]=8'b11111000;
      4'b1000 :seg[7:0]=8'b10000000;
      4'b1001 :seg[7:0]=8'b10011000;
      4'b1010 :seg[7:0]=8'b10001000;
      4'b1011 :seg[7:0]=8'b10000011;
      4'b1100 :seg[7:0]=8'b11000110;
      4'b1101 :seg[7:0]=8'b10100001;
      4'b1110 :seg[7:0]=8'b10000110;
      4'b1111 :seg[7:0]=8'b10001110;
      default : seg = 8'b11111111;
       
      endcase
    end 
  endmodule
  
 module segcounter(clk, out);
  input clk;                    // ����ʱ��
  output reg[2:0] out;             // ����ֵ
  initial begin
  out = 0;
  end
  
  always @(posedge clk)  begin  // ��ʱ�������ؼ�������1
      out = out + 1;            // ����ʵ��
      if(out>= 8) begin     
          out = 0;
      end                        
  end                           
  endmodule
  
 module _7Seg_Driver_Choice(sw, an); 
    input [2:0] sw;      
    output reg [7:0]an;  //7λ�����Ƭѡ���ź�
    always @(sw) begin
    case(sw[2:0])
    3'b000:an = 8'b11111110;
    3'b001:an = 8'b11111101;
    3'b010:an = 8'b11111011;
    3'b011:an = 8'b11110111;
    3'b100:an = 8'b11101111;
    3'b101:an = 8'b11011111;
    3'b110:an = 8'b10111111;
    3'b111:an = 8'b01111111;
    default:an = 8'b11111110;
      endcase
    end 
endmodule


module evalu(out,num,seginput);
   input [2:0]out;
   input [31:0]num;
   output reg [3:0]seginput;
   
   always @* begin 
   case(out)
   3'b000:seginput = num[3:0];
   3'b001:seginput = num[7:4];
   3'b010:seginput = num[11:8];
   3'b011:seginput = num[15:12];   
   3'b100:seginput = num[19:16];
   3'b101:seginput = num[23:20];
   3'b110:seginput = num[27:24];
   3'b111:seginput = num[31:28];
   default: seginput = 4'h0;
      endcase
   end

endmodule
