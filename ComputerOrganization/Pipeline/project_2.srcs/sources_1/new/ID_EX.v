`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/02/27 10:09:37
// Design Name: 
// Module Name: ID_EX
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

    
module ID_EX (clk, rst,ena,instr_in,PC_in,A_in,B_in,Imm_in,WReg_in,JAL_in,LB_in,RegW_in,RegDst_in, syscall_in, R1Equal22_in, JMP_in, JR_in, Beq_in, Bne_in, AluEqual_in, BLTZandAR1_in, MemW_in,ALUop_in,ALUsrc_in,BLTZ_in, SLLV_in, SignedExt_in, M2R_in, R2F2_in,instr_out,PC_out,A_out,B_out,Imm_out,WReg_out,JAL_out,LB_out,RegW_out,RegDst_out, syscall_out, R1Equal22_out, JMP_out, JR_out, Beq_out, Bne_out, AluEqual_out, BLTZandAR1_out, MemW_out,ALUop_out,ALUsrc_out,BLTZ_out, SLLV_out, SignedExt_out, M2R_out, R2F2_out);
               
   input         clk;
   input         rst;
   input         ena;
   input  [31:0] instr_in;
   input  [31:0] PC_in;
   input  [31:0] A_in;
   input  [31:0] B_in;
   input  [31:0] Imm_in;
   input  [4:0] WReg_in;
   //signal
   input  JAL_in;
   input LB_in;
   input RegW_in;
   input  RegDst_in;
   input syscall_in;
   input R1Equal22_in;
   input JMP_in;
   input JR_in;
   input Beq_in;
   input Bne_in;
   input AluEqual_in;
   input BLTZandAR1_in;
   input MemW_in;
   input  [3:0] ALUop_in;
   input  ALUsrc_in;
   input BLTZ_in;
   input SLLV_in;
   input SignedExt_in;
   input M2R_in;
   input R2F2_in;




   //output
   output  [31:0] instr_out;
   output  [31:0] PC_out;
   output  [31:0] A_out;
   output  [31:0] B_out;
   output  [31:0] Imm_out;
   output  [4:0] WReg_out;
   //signal
   output  JAL_out;
   output  LB_out;
   output  RegW_out;
   output  RegDst_out;
   output  syscall_out;
   output  R1Equal22_out;
   output  JMP_out;
   output  JR_out;
   output  Beq_out;
   output  Bne_out;
   output  AluEqual_out;
   output  BLTZandAR1_out;
   output  MemW_out;
   output  [3:0] ALUop_out;
   output  ALUsrc_out;
   output  BLTZ_out;
   output  SLLV_out;
   output  SignedExt_out;
   output  M2R_out;
   output  R2F2_out;
   

    
    reg_pipeline #(32) Instr_Code (   clk,
                           0,
                           ena,
                           rst ? 0 : instr_in ,
                           instr_out);
    
               
    reg_pipeline #(32) PC_IN (   clk,
                                      0,
                                      ena,
                                      rst ? 0 : PC_in ,
                                      PC_out);
                           
    reg_pipeline #(32) A_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  A_in,
                                     A_out);
                                                  
    reg_pipeline #(32) B_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 : B_in ,
                                     B_out);


    reg_pipeline #(32) Imm_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 : Imm_in ,
                                     Imm_out);

    reg_pipeline #(5) W_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  WReg_in,
                                     WReg_out);



    reg_pipeline #(1) JAL_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  JAL_in,
                                     JAL_out);

    reg_pipeline #(1) LB_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  LB_in,
                                     LB_out);

    reg_pipeline #(1) RegW_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  RegW_in,
                                     RegW_out);

    reg_pipeline #(1) RegDst_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  RegDst_in,
                                     RegDst_out);

    reg_pipeline #(1) syscall_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  syscall_in,
                                     syscall_out);

    reg_pipeline #(1) R1Equal22_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  R1Equal22_in,
                                     R1Equal22_out);

    reg_pipeline #(1) JMP_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  JMP_in,
                                     JMP_out);

    reg_pipeline #(1) JR_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  JR_in,
                                     JR_out);

    reg_pipeline #(1) Beq_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  Beq_in,
                                     Beq_out);

    reg_pipeline #(1) Bne_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  Bne_in,
                                     Bne_out);

    reg_pipeline #(1) AluEqual_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  AluEqual_in,
                                     AluEqual_out);

    reg_pipeline #(1) BLTZandAR1_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  BLTZandAR1_in,
                                     BLTZandAR1_out);

    reg_pipeline #(1) MemW_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  MemW_in,
                                     MemW_out);

    reg_pipeline #(4) ALUop_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  ALUop_in,
                                     ALUop_out);

    reg_pipeline #(1) ALUsrc_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  ALUsrc_in,
                                     ALUsrc_out);

    reg_pipeline #(1) BLTZ_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  BLTZ_in,
                                     BLTZ_out);

    reg_pipeline #(1) SLLV_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  SLLV_in,
                                     SLLV_out);

    reg_pipeline #(1) SignedExt_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  SignedExt_in,
                                     SignedExt_out);

    reg_pipeline #(1) M2R_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  M2R_in,
                                     M2R_out);

    reg_pipeline #(1) R2F2_IN (   clk,
                                     0,
                                     ena,
                                     rst ? 0 :  R2F2_in,
                                     R2F2_out);







   
endmodule

    
    
    
    
    
