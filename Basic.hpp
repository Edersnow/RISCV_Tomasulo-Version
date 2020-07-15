#ifndef __BASIC
#define __BASIC
typedef unsigned int uint;

enum typeT{
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,                                  //TYPE R
    JALR, LB, LH, LW, LBU, LHU, ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,  //TYPE I
    SB, SH, SW,                                                                        //TYPE S
    BEQ, BNE, BLT, BGE, BLTU, BGEU,                                                    //TYPE SB
    LUI, AUIPC,                                                                        //TYPE U
    JAL                                                                                //TYPE UJ
};

enum Reservation_name{
    NAME0,
    ADD0, ADD1, ADD2, ADD3,
    LOA0, LOA1, LOA2, LOA3, LOA4,
    ROB0, ROB1, ROB2, ROB3, ROB4
};

struct Register_cell{
    Reservation_name Qi;
    uint Regs;

    Register_cell() {Qi=NAME0; Regs=0;}
};

struct ROB_cell{
    bool Busy;
    Reservation_name Qi;
    typeT Op;
    uint target;
    uint Regs;
    //for speculation
    bool isTaken;
    uint cur_pc;
    uint cur_imm;
    //for store
    Reservation_name Qs;
    uint Vs;

    ROB_cell() {Busy=false, Qi=NAME0, Regs=0, Qs=NAME0, Vs=0;}
};

struct Reservation_cell{
    bool Busy;
    bool isExing;
    bool isReady;
    typeT Op;
    uint Vj;
    uint Vk;
    Reservation_name Qj;
    Reservation_name Qk;

    Reservation_cell() {Busy=false, Vj=0, Vk=0, Qj=NAME0, Qk=NAME0;}
};

#endif