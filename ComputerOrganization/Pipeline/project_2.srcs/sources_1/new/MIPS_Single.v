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

    wire [31:0] IF_PC;
    wire [31:0] ID_PC;
    wire [31:0] EX_PC;
    wire [31:0] MEM_PC;
    wire [31:0] WB_PC;


    
    //IM
    wire [9:0] imAddr;
    wire [31:0] IF_instrCode;
    wire [31:0] ID_instrCode;
    wire [31:0] EX_instrCode;
    wire [31:0] MEM_instrCode;
    wire [31:0] WB_instrCode;    
    //wire [31:0] ext_instr;
    
    //RegFile
    wire [4:0] regR1,regR2,regW;
    wire [31:0] Din;
    wire [31:0] regR1Data, regR2Data;
    wire [31:0] ID_regR1Data,ID_regR2Data;
    wire [31:0] EX_regR1Data,EX_regR2Data;
    wire [31:0] MEM_regR2Data;
    wire [4:0] ID_WReg, EX_WReg, MEM_WReg, WB_WReg;
    
    wire [4:0] MUXregW1Out;
    
    wire [31:0] MUXDin;
    wire [31:0] MUXDin2;

    wire LoadUse;
    
    //Extender
    wire [15:0] extDataIn16;
    wire [7:0] extDataIn8;
    wire [31:0] signextDataOut;
    wire [31:0] unsignextDataOut;
    wire [31:0] extDataOut8;
    wire [31:0] MuxExt;

    wire [31:0] ID_signextDataOut;
    
    //DM
    wire [9:0] dmAddr;
    wire [31:0] dmDataIn;
    wire [31:0] dmDataOut;
    wire [31:0] MuxDmOut;
    wire [7:0] MuxDmOut8;
    wire [31:0] dmAddr32;

    wire [31:0] WB_DATA;

    wire [31:0] MEM_dmDataOut;
    wire [31:0] WB_dmDataOut;
    
    //Ctrl
    wire JMP, JR, SignedExt, SLLV, LB, BLTZ, Beq, Bne, Mem2R, MemW, Alusrc, RegW, JAL, RegDst, syscall;
    wire EX_Beq, EX_Bne, EX_BLTZ, EX_JR, EX_JMP, EX_JAL, EX_syscall, EX_R1Equal22, EX_LB, EX_SignedExt, EX_SLLV, EX_Mem2R, EX_MemW, EX_Alusrc, EX_RegW, EX_RegDst;
    wire MEM_Mem2R, MEM_JAL, MEM_LB, MEM_RegW, MEM_syscall, MEM_R1Equal22, MEM_MemW;
    wire WB_Mem2R, WB_JAL, WB_LB, WB_RegW, WB_syscall, WB_R1Equal22;

    wire ID_JMP, ID_JR, ID_SignedExt, ID_SLLV, ID_LB, ID_BLTZ, ID_Beq, ID_Bne, ID_Mem2R, ID_MemW, ID_Alusrc, ID_RegW, ID_JAL, ID_RegDst, ID_syscall;


    wire [5:0] OpCode;
    wire [5:0] funct;
    wire [3:0] ID_Aluctrl;
    wire [3:0] EX_Aluctrl;
    
    
    //Alu
    wire [31:0] aluA;
    wire [31:0] aluB;
    wire [31:0] MuxAluB1;
    wire [4:0] aluShamt;
    wire [31:0] aluResult;
    wire [31:0] aluResult2;
    wire EX_aluEqual;
    wire [31:0] EX_aluResult;
    wire [31:0] WB_aluResult;

    wire [31:0] EX_MuxA;
    wire [31:0] EX_MuxB;

    
    //compare
    wire EX_bltz_equal;
    wire print_interrupt;
    
    //LED
    wire [31:0] LedData;
    wire [31:0] total_cycle;
    wire [31:0] con_branch_cycle;
    wire [31:0] uncon_branch_cycle;
    wire [31:0] branch_cycle;
    wire [31:0] RAM_data;
    wire [31:0] show_data;

    wire [31:0] loaduse_cycle;

    wire [31:0] total_loaduse_cycle;

    //colide
    wire R1EX, R1MEM, R2EX, R2MEM;

    wire ID_R1Forward1, ID_R1Forward2;
    wire EX_R1Forward1, EX_R1Forward2;

    wire ID_R2Forward1, ID_R2Forward2;
    wire EX_R2Forward1, EX_R2Forward2;


    //
    wire [31:0] ID_Imm;
    wire [31:0] MEM_ALUResult;
    wire [31:0] EX_Imm;
    
    //divider
    divider#(1250000) clk_dec (.clk(clk),.halt(0),.clk_N(clk_N), .fre_sw(sw[9]));
    
    
    
    //IF stage
    //mux pc1 instantiate
    assign PCnext1=IF_PC+4;
    assign extDataIn16=EX_instrCode[15:0];
    sign_extend ext16(extDataIn16, signextDataOut);
    
    assign PCnext2= (signextDataOut<<2) + EX_PC + 4 ;
    
    compare32 cmp_bltz(EX_aluResult, 1, EX_bltz_equal);
    
    assign PCBranch = (EX_Beq&EX_aluEqual) | (EX_Bne&(~EX_aluEqual)) | (EX_bltz_equal&EX_BLTZ);
    
    
    MUX2_32 muxpc1 (.Din1(PCnext1), .Din2(PCnext2), .sel(PCBranch), .Dout(PCMux1Out));
    
    //mux pc2 instantiate    
    
    MUX2_32 muxpc2 (.Din1(PCMux1Out), .Din2(EX_MuxA), .sel(EX_JR), .Dout(PCMux2Out));    
    
    
    //mux pc3 instantiate    
    
    assign PCnext3[1:0]=2'b00;
    assign PCnext3[27:2]=EX_instrCode[25:0];
    assign PCnext3[31:28]=IF_PC[31:28];
    
    
    MUX2_32 muxpc3 (.Din1(PCMux2Out), .Din2(PCnext3), .sel(EX_JMP|EX_JAL), .Dout(PCMux3Out));       
    
    
    //pc reg instantiate    
    
    compare32 cmp_print (EX_MuxA, 34, EX_R1Equal22);
    
    assign PCen = (~EX_syscall) | ((EX_syscall&EX_R1Equal22)|go);
    
    PCreg U_PC(
        .clk(clk_N),
        .rst(rst),
        .ena(PCen&(~LoadUse)),
        .data_in(PCMux3Out),
        .data_out(IF_PC)
        );

    //IM instantiate         
    assign imAddr=IF_PC[11:2];
    parameter IM_DATA="D:\\IM_DATA.hex";
    
    ROM #(.ROM_DATA(IM_DATA)) U_IM  (IF_PC, IF_instrCode );
    
    
    IF_ID IF2ID (clk_N, EX_JMP|EX_JR|EX_JAL|PCBranch , PCen&(~LoadUse) ,IF_instrCode , IF_PC , ID_instrCode , ID_PC); 




    //ID stage
    //Regfile instantiate
    
    MUX2_5 muxRegR1 (ID_instrCode[25:21],2,ID_syscall,regR1);
    MUX2_5 muxRegR2 (ID_instrCode[20:16],4,ID_syscall,regR2);
    
    MUX2_5 muxRegW1 (ID_instrCode[20:16],ID_instrCode[15:11],ID_RegDst,MUXregW1Out);    
    MUX2_5 muxRegW2 (MUXregW1Out,5'b11111,ID_JAL,ID_WReg);         
    
    
    // MUX2_32 DmOut (aluResult, dmDataOut, Mem2R, MuxDmOut);
    // MUX2_32 muxDin (MuxDmOut, PCout+4, JAL, MUXDin);
    
    // MUX4_8 muxDin8 (MuxDmOut[7:0],MuxDmOut[15:8],MuxDmOut[23:16],MuxDmOut[31:24],aluResult[1:0],MuxDmOut8);
    // sign_extend #(8) ext8 (MuxDmOut8, MUXDin2);
    // MUX2_32 muxDin2 (MUXDin, MUXDin2 , LB, Din);  
    
    REGFILE U_RegFile (.WE(WB_RegW), .CLK(clk_N), .R1_ADDR(regR1), .R2_ADDR(regR2), .W_ADDR(WB_WReg), .Din(WB_DATA), .R1(ID_regR1Data), .R2(ID_regR2Data), .clr(0));  
    
    
    //Ctrl instantiate
    assign OpCode[5:0]=ID_instrCode[31:26];
    assign funct[5:0]=ID_instrCode[5:0];
    Ctrl U_Ctrl (ID_JMP, ID_JR, ID_SignedExt, ID_SLLV, ID_LB, ID_BLTZ, ID_instrCode[31:26], ID_instrCode[5:0], ID_Beq, ID_Bne, ID_Mem2R, ID_MemW, ID_Aluctrl, ID_Alusrc, ID_RegW, ID_JAL, ID_RegDst, ID_syscall);
    
    Redirect_data_relate R_data_relate(
    .OpCode(ID_instrCode[31:26]),              //指令操作码字?
    .funct(ID_instrCode[5:0]),              //指令功能字段    
    .EX_WReg(EX_WReg),
    .MEM_WReg(MEM_WReg),
    .ID_R1(regR1),
    .ID_R2(regR2),
    .EX_RegW(EX_RegW),
    .MEM_RegW(MEM_RegW),
    .R1EX_Data_relate(R1EX),
    .R1MEM_Data_relate(R1MEM),
    .R2EX_Data_relate(R2EX),
    .R2MEM_Data_relate(R2MEM)
    );

    Forward R1_Forward(
    R1EX,R1MEM,ID_R1Forward1,ID_R1Forward2
    );

    Forward R2_Forward(
    R2EX,R2MEM,ID_R2Forward1,ID_R2Forward2
    );

    assign LoadUse = ( (regR1==EX_WReg)|(regR2==EX_WReg) ) & ( (EX_instrCode[31:26]==35) | (EX_instrCode[31:26]==32) );


    unsign_extend unsignExt16 (ID_instrCode[15:0], unsignextDataOut);
    sign_extend ext16_ID(ID_instrCode[15:0], ID_signextDataOut);
    MUX2_32 muxext (unsignextDataOut, ID_signextDataOut, ID_SignedExt, ID_Imm); 


    ID_EX ID2EX(.clk(clk_N), 
        .rst(EX_JMP|EX_JR|EX_JAL|PCBranch|LoadUse),
        .ena(PCen),
        .instr_in(ID_instrCode),
        .PC_in(ID_PC),
        .A_in(ID_regR1Data),
        .B_in(ID_regR2Data),
        .Imm_in(ID_Imm),
        .WReg_in(ID_WReg),
        .JAL_in(ID_JAL),
        .LB_in(ID_LB),
        .RegW_in(ID_RegW),
        .RegDst_in(ID_RegDst), 
        .syscall_in(ID_syscall), 
        .R1Equal22_in( ), .JMP_in(ID_JMP), .JR_in(ID_JR), .Beq_in(ID_Beq), .Bne_in(ID_Bne), 
        .AluEqual_in(ID_R1Forward1), .BLTZandAR1_in(ID_R1Forward2), .MemW_in(ID_MemW),.ALUop_in(ID_Aluctrl),.ALUsrc_in(ID_Alusrc),.BLTZ_in(ID_BLTZ), 
        .SLLV_in(ID_SLLV), .SignedExt_in(ID_R2Forward1), .M2R_in(ID_Mem2R), .R2F2_in(ID_R2Forward2),
        .instr_out(EX_instrCode),.PC_out(EX_PC),.A_out(EX_regR1Data),.B_out(EX_regR2Data),
        .Imm_out(EX_Imm),.WReg_out(EX_WReg),.JAL_out(EX_JAL),.LB_out(EX_LB),.RegW_out(EX_RegW),.RegDst_out(EX_RegDst), .syscall_out(EX_syscall),
        .R1Equal22_out( ), .JMP_out(EX_JMP), .JR_out(EX_JR), .Beq_out(EX_Beq), .Bne_out(EX_Bne), .AluEqual_out(EX_R1Forward1), 
        .BLTZandAR1_out(EX_R1Forward2), .MemW_out(EX_MemW),.ALUop_out(EX_Aluctrl),.ALUsrc_out(EX_Alusrc),.BLTZ_out(EX_BLTZ),.SLLV_out(EX_SLLV), 
        .SignedExt_out(EX_R2Forward1), .M2R_out(EX_Mem2R), .R2F2_out(EX_R2Forward2));




    //EX stage

    //Alu  instantiate  
    

    MUX4_32 muxAluA_redirect (EX_regR1Data,WB_DATA,0,MEM_ALUResult,{EX_R1Forward1,EX_R1Forward2},EX_MuxA);

    MUX4_32 muxAluB_redirect (EX_regR2Data,WB_DATA,0,MEM_ALUResult,{EX_R2Forward1,EX_R2Forward2},EX_MuxB);    
    
    MUX2_32 muxAluB1 (EX_MuxB, EX_Imm,EX_Alusrc, MuxAluB1);
    
    MUX2_32 muxAluB2 (MuxAluB1, 32'b00000000000000000000000000000000, EX_BLTZ, aluB);           
    
    MUX2_5 muxShamt (EX_instrCode[10:6], EX_regR1Data[4:0], EX_SLLV, aluShamt);
    
    ALU U_ALU (.X(EX_MuxA), .Y(aluB), .S(EX_Aluctrl), .shamt(aluShamt),
               .Result(EX_aluResult), .Result2(aluResult2), .Equal(EX_aluEqual)
        );   


    EX_MEM EX2MEM (.clk(clk_N), .rst(rst),.ena(1),.instr_in(EX_instrCode),.PC_in(EX_PC),.A_in(EX_MuxA),.B_in(EX_MuxB),.AluResult_in(EX_aluResult),
                    .WReg_in(EX_WReg),.JAL_in(EX_JAL),.LB_in(EX_LB),.RegW_in(EX_RegW),.RegDst_in(EX_RegDst), .syscall_in(EX_syscall), .R1Equal22_in(EX_R1Equal22), .JMP_in(EX_JMP), .JR_in(EX_JR), 
                    .Beq_in(EX_Beq), .Bne_in(EX_Bne), .AluEqual_in(), .BLTZandAR1_in(), .MemW_in(EX_MemW), .M2R_in(EX_Mem2R), 
                    .instr_out(MEM_instrCode),.PC_out(MEM_PC),.A_out(),.B_out(MEM_regR2Data),
                    .AluResult_out(MEM_ALUResult),.WReg_out(MEM_WReg),.JAL_out(MEM_JAL),.LB_out(MEM_LB),.RegW_out(MEM_RegW),.RegDst_out(), 
                    .syscall_out(MEM_syscall), .R1Equal22_out(MEM_R1Equal22), .JMP_out(), 
                    .JR_out(), .Beq_out(), .Bne_out(), .AluEqual_out(), .BLTZandAR1_out(), .MemW_out(MEM_MemW), .M2R_out(MEM_Mem2R));
      



    //MEM stage
    //DM  instantiate
   
    
    MUX2_32 mux_dm_in (MEM_ALUResult, {{20{1'b0}},{6{1'b0}}, sw[13:10],{2{1'b0}} }, sw[15]&sw[14], dmAddr32);
    
    assign dmAddr= dmAddr32[11:2];
    
    RAM U_DM(
        .rst(rst),// 1 will reset the ram
        .clk(clk_N),
        .str(MEM_MemW), // store data enable
        .ld(1), // load data enable
        .sel(4'b1111), // 4bit selectors, 0 disables each component
        .addr(dmAddr), // 10bits addr
        .data_in(MEM_regR2Data), // 32bits data
        .data_out(MEM_dmDataOut) // 32bits data
        );
    
    MEM_WB MEM2WB (.clk(clk_N), .rst(rst),.ena(1),.instr_in(),.PC_in(MEM_PC),.A_in(),.B_in(),.AluResult_in(MEM_ALUResult),.MD_in(MEM_dmDataOut),
        .WReg_in(MEM_WReg),.JAL_in(MEM_JAL),.LB_in(MEM_LB),.RegW_in(MEM_RegW),.RegDst_in(), .syscall_in(MEM_syscall), .R1Equal22_in(MEM_R1Equal22), .JMP_in(), .JR_in(), 
        .Beq_in(), .Bne_in(), .AluEqual_in(), .BLTZandAR1_in(), .M2R_in(MEM_Mem2R), .instr_out(),.PC_out(WB_PC),.A_out(),.B_out(),.AluResult_out(WB_aluResult),.MD_out(WB_dmDataOut),
        .WReg_out(WB_WReg),.JAL_out(WB_JAL),.LB_out(WB_LB),.RegW_out(WB_RegW),.RegDst_out(),.syscall_out(WB_syscall), .R1Equal22_out(WB_R1Equal22), .JMP_out(), .JR_out(), .Beq_out(), 
        .Bne_out(), .AluEqual_out(), .BLTZandAR1_out(),  .M2R_out(WB_Mem2R));

    

    //WB stage
    //LED Reg instantiate
    PCreg LedReg (        
        .clk(clk_N),
        .rst(rst),
        .ena(EX_syscall&EX_R1Equal22),
        .data_in(EX_MuxB),
        .data_out(LedData)
    );
    
    MUX2_32 DmOut (WB_aluResult, WB_dmDataOut, WB_Mem2R, MuxDmOut);
    MUX2_32 muxDin (MuxDmOut, WB_PC+4, WB_JAL, MUXDin);
    
    MUX4_8 muxDin8 (MuxDmOut[7:0],MuxDmOut[15:8],MuxDmOut[23:16],MuxDmOut[31:24],WB_aluResult[1:0],MuxDmOut8);
    sign_extend #(8) ext8 (MuxDmOut8, MUXDin2);
    MUX2_32 muxDin2 (MUXDin, MUXDin2 , WB_LB, WB_DATA); 


    counter con_branch (clk_N, con_branch_cycle, PCBranch, rst);
    counter uncon_branch (clk_N, uncon_branch_cycle, EX_JMP|EX_JR|EX_JAL , rst);
    counter total_counter (clk_N, total_cycle, (~WB_syscall) | ((WB_syscall&WB_R1Equal22)|go), rst);

    counter loaduse_count (clk_N, loaduse_cycle, LoadUse, rst);

    
    assign branch_cycle[31:16] =uncon_branch_cycle[15:0];
    assign branch_cycle[15:0] = con_branch_cycle[15:0];
    
    assign total_loaduse_cycle[31:16] = loaduse_cycle[15:0];
    assign total_loaduse_cycle[15:0] = total_cycle[15:0];

    //RAM_data
    
    
    
    MUX4_32 show (LedData, total_loaduse_cycle, branch_cycle, MEM_dmDataOut, {sw[15], sw[14]}, show_data);
    
    reveal U_Led (.halt(0), .clk(clk), .num(show_data), .an(an), .seg(seg));    
    
    
endmodule
