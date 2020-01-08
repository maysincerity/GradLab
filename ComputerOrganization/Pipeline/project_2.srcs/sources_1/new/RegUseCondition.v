`include "instruction_def.v"
`include "ctrl_encode_def.v"

module RegUseCondition(
    OpCode,              //指令操作码字�?
    funct,               //指令功能字段    
    R1_Used,
    R2_Used
    );
    
    input [5: 0] OpCode;              //指令操作码字�?
    input [5: 0] funct;               //指令功能字段    
    
    output reg R1_Used;
    output reg R2_Used;
    
always@(OpCode or funct)
    begin
        case (OpCode)
            `INSTR_RTYPE_OP:
            begin
               
                case (funct)
                    `INSTR_ADD_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_ADDU_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_SUB_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_SUBU_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_AND_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_NOR_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_OR_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_XOR_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_SLT_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_SLTU_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_SLL_FUNCT:
                    begin
                        R1_Used=0;
                        R2_Used=1;
                     end
                    `INSTR_SRL_FUNCT:
                    begin
                        R1_Used=0;
                        R2_Used=1;
                     end
                    `INSTR_SRA_FUNCT:
                    begin
                        R1_Used=0;
                        R2_Used=1;
                     end
                    `INSTR_JR_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=0;
                     end
                    `INSTR_SYSCALL_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
                    `INSTR_SLLV_FUNCT:
                    begin
                        R1_Used=1;
                        R2_Used=1;
                     end
    
                endcase
            end
    
    
            `INSTR_J_OP:
             begin
                R1_Used=0;
                R2_Used=0;
             end
    
            `INSTR_JAL_OP:
             begin
                R1_Used=0;
                R2_Used=0;
             end
    
            `INSTR_BEQ_OP:
             begin
               R1_Used=1;
               R2_Used=1;
            end    
            `INSTR_BNE_OP:
             begin
               R1_Used=1;
               R2_Used=1;
            end    
            `INSTR_ADDI_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end    
            `INSTR_ANDI_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end    
    
            `INSTR_ADDIU_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end    
            `INSTR_SLTI_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end    
    
    
            `INSTR_ORI_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end    
            `INSTR_LW_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end    
    
            `INSTR_SW_OP:
             begin
               R1_Used=1;
               R2_Used=1;
            end    
            `INSTR_LB_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end    
            `INSTR_BLTZ_OP:
             begin
               R1_Used=1;
               R2_Used=0;
            end            
            default:
             begin
               R1_Used=0;
               R2_Used=0;
            end    
        endcase
    end    
    
    
    
    
    
    
    
    
endmodule
