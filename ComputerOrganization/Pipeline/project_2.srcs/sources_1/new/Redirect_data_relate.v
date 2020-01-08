`include "instruction_def.v"
`include "ctrl_encode_def.v"


module Redirect_data_relate(
    OpCode,              //指令操作码字?
    funct,              //指令功能字段    
    EX_WReg,
    MEM_WReg,
    ID_R1,
    ID_R2,
    EX_RegW,
    MEM_RegW,
    R1EX_Data_relate,
    R1MEM_Data_relate,
    R2EX_Data_relate,
    R2MEM_Data_relate
    );
    
    input [5: 0] OpCode;              //指令操作码字?
    input [5: 0] funct;               //指令功能字段    
    input [4:0] EX_WReg;
    input [4:0] MEM_WReg;
    input [4:0] ID_R1;
    input [4:0] ID_R2;
    input EX_RegW;
    input MEM_RegW;
    output R1EX_Data_relate;
    output R1MEM_Data_relate;
    output R2EX_Data_relate;
    output R2MEM_Data_relate; 
    wire R1_Used, R2_Used;
            
    RegUseCondition redirect_reg_use (  OpCode,              //指令操作码字?
                                        funct,               //指令功能字段    
                                        R1_Used,
                                        R2_Used
    );
    
    assign R1EX_Data_relate = R1_Used & ( ~(ID_R1 == 0)) & ( (ID_R1==EX_WReg)&EX_RegW );
    assign R1MEM_Data_relate = R1_Used & ( ~(ID_R1 == 0)) & ( (ID_R1==MEM_WReg)&MEM_RegW );
    assign R2EX_Data_relate = R2_Used & ( ~(ID_R2 == 0)) & ( (ID_R2==EX_WReg)&EX_RegW );
    assign R2MEM_Data_relate = R2_Used & ( ~(ID_R2 == 0)) & ( (ID_R2==MEM_WReg)&MEM_RegW );
    

    
    
endmodule
