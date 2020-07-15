#ifndef __Global
#define __Global
#include "Basic.hpp"

unsigned char _memory[3000000];
uint _pc;
Register_cell T_register[32];
bool stop_issue;
bool is_end;




//prediction part
//BTFN
uint app_time[64];
uint total_app;

//Global prediction
uint BHR;                  //16bits
uint PHT_for_BHR[65536];   //2bits

//Local prediction
uint BHT[64];              //8bits
uint PHT[64][256];         //2bits



//Statistics section
uint total_prediction;
uint correct_prediction;

//for debug
//const char *TYPES[]={"ADD","SUB","SLL","SLT","SLTU","XOR","SRL","SRA","OR","AND",
//"JALR","LB","LH","LW","LBU","LHU","ADDI","SLTI","SLTIU","XORI","ORI","ANDI","SLLI","SRLI",
//"SRAI","SB","SH","SW", "BEQ","BNE","BLT","BGE","BLTU","BGEU","LUI","AUIPC","JAL" };

#endif