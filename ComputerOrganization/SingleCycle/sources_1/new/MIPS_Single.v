`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/19 09:10:12
// Design Name: 
// Module Name: MIPS_Single
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


module MIPS_Single(
    clk, rst, go, an, seg, sw//, led//, PCen, JMP, JR, SignedExt, SLLV, LB, BLTZ, Beq, Bne, Mem2R, MemW, Alusrc, RegW, JAL, RegDst, syscall, OpCode, funct, regR1Data
    );
    input clk;
    input rst;
    input go;
    input [15:2] sw;
    //output JMP, JR, SignedExt, SLLV, LB, BLTZ, Beq, Bne, Mem2R, MemW, Alusrc, RegW, JAL, RegDst, syscall;
    //output [5:0] OpCode;
    //output [5:0] funct;
    output [7:0] an;
    output [7:0] seg;
    
    //output [31:0] regR1Data;
    
    //output PCen;
    
    //output [15:0] led;
    //PC
    
    wire [31:0] PCout;
    wire PCBranch;
    wire PCen;
    
    wire [31:0] PCnext1;
    wire [31:0] PCnext2;
    wire [31:0] PCnext3;
    
    wire[31:0] PCMux1Out;
    wire[31:0] PCMux2Out;
    wire[31:0] PCMux3Out;
    
    //IM
    wire [9:0] imAddr;
    wire [31:0] instrCode;
    //wire [31:0] ext_instr;
    
    //RegFile
    wire [4:0] regR1,regR2,regW;
    wire [31:0] Din;
    wire [31:0] regR1Data, regR2Data;
    
    wire [4:0] MUXregW1Out;
    
    wire [31:0] MUXDin;
    wire [31:0] MUXDin2;
    
    //Extender
    wire [15:0] extDataIn16;
    wire [7:0] extDataIn8;
    wire [31:0] signextDataOut;
    wire [31:0] unsignextDataOut;
    wire [31:0] extDataOut8;
    wire [31:0] MuxExt;
    
    //DM
    wire [9:0] dmAddr;
    wire [31:0] dmDataIn;
    wire [31:0] dmDataOut;
    wire [31:0] MuxDmOut;
    wire [7:0] MuxDmOut8;
    wire [31:0] dmAddr32;
    
    //Ctrl
    wire JMP, JR, SignedExt, SLLV, LB, BLTZ, Beq, Bne, Mem2R, MemW, Alusrc, RegW, JAL, RegDst, syscall;
    wire [5:0] OpCode;
    wire [5:0] funct;
    wire [4:0] Aluctrl;
    
    
    //Alu
    wire [31:0] aluA;
    wire [31:0] aluB;
    wire [31:0] MuxAluB1;
    wire [4:0] aluShamt;
    wire [31:0] aluResult;
    wire [31:0] aluResult2;
    wire aluEqual;
    
    
    //compare
    wire bltz_equal;
    wire print_interrupt;
    
    //LED
    wire [31:0] LedData;
    wire [31:0] total_cycle;
    wire [31:0] con_branch_cycle;
    wire [31:0] uncon_branch_cycle;
    wire [31:0] branch_cycle;
    wire [31:0] RAM_data;
    wire [31:0] show_data;
    
    
    //divider
    divider#(200000) clk_dec (.clk(clk),.halt(0),.clk_N(clk_N), .fre_sw(sw[9]));
    
    
    
    //mux pc1 instantiate
    assign PCnext1=PCout+4;
    assign extDataIn16=instrCode[15:0];
    sign_extend ext16(extDataIn16, signextDataOut);
    
    assign PCnext2= (signextDataOut<<2) + PCnext1 ;
    
    compare32 cmp_bltz(aluResult, 1, bltz_equal);
    
    assign PCBranch = (Beq&aluEqual) | (Bne&~aluEqual) | (bltz_equal&BLTZ);
    
    
    MUX2_32 muxpc1 (.Din1(PCnext1), .Din2(PCnext2), .sel(PCBranch), .Dout(PCMux1Out));
    
    //mux pc2 instantiate    
    
    MUX2_32 muxpc2 (.Din1(PCMux1Out), .Din2(regR1Data), .sel(JR), .Dout(PCMux2Out));    
    
    
    //mux pc3 instantiate    
    
    assign PCnext3[1:0]=2'b00;
    assign PCnext3[27:2]=instrCode[25:0];
    assign PCnext3[31:28]=PCout[31:28];
    
    
    MUX2_32 muxpc3 (.Din1(PCMux2Out), .Din2(PCnext3), .sel(JMP|JAL), .Dout(PCMux3Out));       
    
    
    //pc reg instantiate    
    
    compare32 cmp_print (regR1Data, 34, print_interrupt);
    
    assign PCen = (~syscall) | ((syscall&print_interrupt)|go);
    
    PCreg U_PC(
        .clk(clk_N),
        .rst(rst),
        .ena(PCen),
        .data_in(PCMux3Out),
        .data_out(PCout)
        );
        
    //IM instantiate         
    assign imAddr=PCout[11:2];
    parameter IM_DATA="D:\\IM_DATA.hex";
    
    ROM #(.ROM_DATA(IM_DATA)) U_IM  (PCout, instrCode );
    
    
    //Regfile instantiate
    
    MUX2_5 muxRegR1 (instrCode[25:21],2,syscall,regR1);
    MUX2_5 muxRegR2 (instrCode[20:16],4,syscall&print_interrupt,regR2);
    
    MUX2_5 muxRegW1 (instrCode[20:16],instrCode[15:11],RegDst,MUXregW1Out);    
    MUX2_5 muxRegW2 (MUXregW1Out,5'b11111,JAL,regW);         
    
    
    MUX2_32 DmOut (aluResult, dmDataOut, Mem2R, MuxDmOut);
    MUX2_32 muxDin (MuxDmOut, PCout+4, JAL, MUXDin);
    
    MUX4_8 muxDin8 (MuxDmOut[7:0],MuxDmOut[15:8],MuxDmOut[23:16],MuxDmOut[31:24],aluResult[1:0],MuxDmOut8);
    sign_extend #(8) ext8 (MuxDmOut8, MUXDin2);
    MUX2_32 muxDin2 (MUXDin, MUXDin2 , LB, Din);  
    
    REGFILE U_RegFile (.WE(RegW), .CLK(clk_N), .R1_ADDR(regR1), .R2_ADDR(regR2), .W_ADDR(regW), .Din(Din), .R1(regR1Data), .R2(regR2Data), .clr(0));  
    
    
    //Ctrl instantiate
    assign OpCode[5:0]=instrCode[31:26];
    assign funct[5:0]=instrCode[5:0];
    Ctrl U_Ctrl (JMP, JR, SignedExt, SLLV, LB, BLTZ, instrCode[31:26], instrCode[5:0], Beq, Bne, Mem2R, MemW, Aluctrl, Alusrc, RegW, JAL, RegDst, syscall);
    
    //Alu  instantiate  
    
    assign aluA=regR1Data;
    
        //sign_extend ext16(extDataIn16, signextDataOut);
    unsign_extend unsignExt16 (extDataIn16, unsignextDataOut);
    
    MUX2_32 muxext (unsignextDataOut, signextDataOut, SignedExt, MuxExt); 
    
    MUX2_32 muxAluB1 (regR2Data, MuxExt, Alusrc, MuxAluB1);
    
    MUX2_32 muxAluB2 (MuxAluB1, 32'b00000000000000000000000000000000, BLTZ, aluB);           
    
    MUX2_5 muxShamt (instrCode[10:6], regR1Data[4:0], SLLV, aluShamt);
    
    ALU U_ALU (.X(aluA), .Y(aluB), .S(Aluctrl), .shamt(aluShamt),
               .Result(aluResult), .Result2(aluResult2), .Equal(aluEqual)
        );   
        
    //DM  instantiate
   
    
    MUX2_32 mux_dm_in (aluResult, {{20{1'b0}},{6{1'b0}}, sw[13:10],{2{1'b0}} }, sw[15]&sw[14], dmAddr32);
    
    assign dmAddr= dmAddr32[11:2];
    
    RAM U_DM(
        .rst(rst),// 1 will reset the ram
        .clk(clk_N),
        .str(MemW), // store data enable
        .ld(1), // load data enable
        .sel(4'b1111), // 4bit selectors, 0 disables each component
        .addr(dmAddr), // 10bits addr
        .data_in(regR2Data), // 32bits data
        .data_out(dmDataOut) // 32bits data
        );
    
    
    //LED Reg instantiate
    PCreg LedReg (        
        .clk(clk_N),
        .rst(rst),
        .ena(syscall&print_interrupt),
        .data_in(regR2Data),
        .data_out(LedData)
    );
    
    counter con_branch (clk_N, con_branch_cycle, PCBranch, rst);
    counter uncon_branch (clk_N, uncon_branch_cycle, JMP|JR|JAL , rst);
    counter total_counter (clk_N, total_cycle, PCen, rst);
    
    assign branch_cycle[31:16] =uncon_branch_cycle[15:0];
    assign branch_cycle[15:0] = con_branch_cycle[15:0];
    
    //RAM_data
    
    
    
    MUX4_32 show (LedData, total_cycle, branch_cycle, dmDataOut, {sw[15], sw[14]}, show_data);
    
    reveal U_Led (.halt(0), .clk(clk), .num(show_data), .an(an), .seg(seg));    
    
    
endmodule
