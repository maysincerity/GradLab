// NPC control signal
`define NPC_PLUS4   2'b00
`define NPC_BRANCH  2'b01
`define NPC_JUMP    2'b10

// EXT control signal
`define EXT_ZERO    2'b00
`define EXT_SIGNED  2'b01
`define EXT_HIGHPOS 2'b10

// ALU control signal
`define ALUOp_NOP   4'b0000
`define ALUOp_ADDU  4'b0101
`define ALUOp_ADD   4'b0101
`define ALUOp_SUBU  4'b0110
`define ALUOp_SUB   4'b0110
`define ALUOp_AND   4'b0111
`define ALUOp_OR    4'b1000
`define ALUOp_NOR   4'b1010
`define ALUOp_XOR   4'b1000
`define ALUOp_SLT   4'b1011
`define ALUOp_SLTU  4'b1100
`define ALUOp_EQL   4'b1011
`define ALUOp_BNE   4'b1100
`define ALUOp_GT0   4'b1101
`define ALUOp_GE0   4'b1110
`define ALUOp_LT0   4'b1111
`define ALUOp_LE0   4'b0000
`define ALUOp_SLL   4'b0000
`define ALUOp_SRL   4'b0010
`define ALUOp_SRA   4'b0001
`define ALUOp_LUI   4'b0100

// GPR control signal
`define GPRSel_RD   2'b00
`define GPRSel_RT   2'b01
`define GPRSel_31   2'b10

`define WDSel_FromALU 2'b00
`define WDSel_FromMEM 2'b01
`define WDSel_FromPC  2'b10

// Memory control signal
`define BE_SB  2'b00
`define BE_SH  2'b01
`define BE_SW  2'b10

`define ME_LB  3'b000
`define ME_LBU 3'b001
`define ME_LH  3'b010
`define ME_LHU 3'b011
`define ME_LW  3'b100