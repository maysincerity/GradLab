
module  MUX2_32(Din1,Din2,sel,Dout);
parameter SIZE=32;

input [SIZE-1:0] Din1,Din2;
input sel;
output  reg [SIZE-1:0] Dout;
always@(*)begin
    case(sel)
        1'b0: Dout<=Din1;
        1'b1: Dout<=Din2;
        default: Dout<=Din1;
    endcase    
end

endmodule