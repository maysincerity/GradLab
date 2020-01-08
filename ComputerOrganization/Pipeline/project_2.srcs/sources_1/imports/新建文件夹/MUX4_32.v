module MUX4_8(Din0,Din1,Din2,Din3,sel,Dout);

input [1:0] sel;
input [7:0] Din0,Din1,Din2,Din3;
output reg [7:0] Dout;

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