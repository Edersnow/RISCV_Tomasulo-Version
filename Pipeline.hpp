#ifndef __PIPELINE
#define __PIPELINE
#include "Conponent.hpp"

void Issue(ROB &cur_ROB, Res_add &cur_Ra, Res_loa &cur_Rl){
    instruction_decoder cur_dins;
    Reservation_cell tmp;
    Reservation_name tmp_name;
    ROB_cell tmp_rob;
    uint cur_instruction;

    if(stop_issue)  return;
    memcpy(&cur_instruction, _memory + _pc, sizeof(uint));
    cur_dins=cur_instruction;
    tmp.Busy=true;
    tmp.isExing=false;
    tmp.isReady=false;
    tmp.Op=cur_dins._format;
    tmp_rob.Busy=true;
    tmp_rob.Op=cur_dins._format;

    if(cur_instruction == 0x0ff00513){
        stop_issue=true;
        return;
    }

    switch (cur_dins._format){
        //sent to ROB directly
        case LUI:  case AUIPC:  case JAL:  case JALR:  case SB:  case SH:  case SW:
            if(cur_ROB.isFull())  return;
            break;

        //sent to Adder
        case ADDI:  case SLTI:  case SLTIU:  case XORI:  case ORI:
        case ANDI:  case SLLI:  case SRLI:  case SRAI:  case ADD:
        case SUB:  case SLL:  case SLT:  case SLTU:  case XOR:
        case SRL:  case SRA:  case OR:  case AND:  case BEQ:
        case BNE:  case BLT:  case BGE:  case BLTU:  case BGEU:
            if(cur_ROB.isFull() || cur_Ra.isFull())  return;
            break;

        //sent to Load Buffers
        case LB:  case LH:  case LW:  case LBU:  case LHU:
            if(cur_ROB.isFull() || cur_Rl.isFull())  return;
            break;
    }

    switch (cur_dins._format){
        case LUI:
            tmp_rob.Regs = cur_dins._immediate;
            tmp_rob.target = cur_dins._rd;
            break;

        case AUIPC:
            tmp_rob.Regs = _pc + cur_dins._immediate;
            tmp_rob.target = cur_dins._rd;
            break;

        case JAL:
            tmp_rob.Regs = _pc + 4;
            tmp_rob.target = cur_dins._rd;
            break;

        case JALR:
            rename_rob(cur_ROB, tmp_rob, cur_dins._rs1, 0);
            tmp_rob.Vs += cur_dins._immediate;
            tmp_rob.Regs = _pc + 4;
            tmp_rob.target = cur_dins._rd;
            break;

        case SB:  case SH:  case SW:
            rename_rob(cur_ROB, tmp_rob, cur_dins._rs1, cur_dins._rs2);
            tmp_rob.Vs += cur_dins._immediate;
            break;
        
        case LB:  case LH:  case LW:  case LBU:  case LHU:
            rename_res(cur_ROB, tmp, cur_dins._rs1, 0);
            tmp.Vj += cur_dins._immediate;
            tmp_name = cur_Rl.push(tmp);
            tmp_rob.Qi = tmp_name;
            tmp_rob.target = cur_dins._rd;
            break;

        case ADDI:  case SLTI:  case SLTIU:  case XORI:  case ORI:
        case ANDI:  case SLLI:  case SRLI:  case SRAI:
            rename_res(cur_ROB, tmp, cur_dins._rs1, 0);
            tmp.Vk = cur_dins._immediate;
            tmp_name = cur_Ra.push(tmp);
            tmp_rob.Qi = tmp_name;
            tmp_rob.target = cur_dins._rd;
            break;

        case ADD:  case SUB:  case SLL:  case SLT:  case SLTU:
        case XOR:  case SRL:  case SRA:  case OR:  case AND:
        case BEQ:  case BNE:  case BLT:  case BGE:  case BLTU:
        case BGEU:
            rename_res(cur_ROB, tmp, cur_dins._rs1, cur_dins._rs2);
            tmp_name = cur_Ra.push(tmp);
            tmp_rob.Qi = tmp_name;
            tmp_rob.target = cur_dins._rd;
            break;
    }

    if(cur_dins._format>=28 && cur_dins._format<=33){
        tmp_rob.cur_pc = _pc;
        tmp_rob.cur_imm = cur_dins._immediate;
        if(judge_branch(((_pc>>2)&63), cur_dins))  _pc += cur_dins._immediate, tmp_rob.isTaken = true;
        else  _pc += 4, tmp_rob.isTaken = false;
    }
    else if(cur_dins._format == JAL){
        _pc += cur_dins._immediate;
    }
    else if(cur_dins._format == JALR){
        stop_issue = true;
    }
    else  _pc += 4;

    cur_ROB.set_ROB(tmp_rob);

    //for debug
    //if(cur_dins._rd == 1)  printf("%u %u\n", cur_dins._rs1, cur_dins._rs2);
}

void Ex_add(Res_add &cur_Ra, Adder &cur_Ad){
    Reservation_cell tmp;
    Reservation_name for_wb;
    if(!cur_Ra.isReady() || !cur_Ad.is_empty)  return;

    tmp=cur_Ra.pop(for_wb);
    switch (tmp.Op){
        case ADD:  case ADDI:  cur_Ad._value = (tmp.Vj + tmp.Vk);  break;
        case SUB:  cur_Ad._value = (tmp.Vj - tmp.Vk);  break;
        case SLL:  case SLLI:  cur_Ad._value = (tmp.Vj << (tmp.Vk&31));  break;
        case SLT:  case SLTI:  cur_Ad._value = (int(tmp.Vj) < int(tmp.Vk));  break;
        case SLTU:  case SLTIU:  cur_Ad._value = (tmp.Vj < tmp.Vk);  break;
        case XOR:  case XORI:  cur_Ad._value = (tmp.Vj ^ tmp.Vk);  break;
        case SRL:  case SRLI:  cur_Ad._value = (tmp.Vj >> (tmp.Vk&31));  break;
        case SRA:  case SRAI:  cur_Ad._value = (int(tmp.Vj) >> (tmp.Vk&31));  break;
        case OR:  case ORI:  cur_Ad._value = (tmp.Vj | tmp.Vk);  break;
        case AND:  case ANDI:  cur_Ad._value = (tmp.Vj & tmp.Vk);  break;

        case BEQ:  cur_Ad._value = (tmp.Vj == tmp.Vk);  break;
        case BNE:  cur_Ad._value = (tmp.Vj != tmp.Vk);  break;
        case BLT:  cur_Ad._value = (int(tmp.Vj) < int(tmp.Vk));  break;
        case BGE:  cur_Ad._value = (int(tmp.Vj) >= int(tmp.Vk));  break;
        case BLTU:  cur_Ad._value = (tmp.Vj < tmp.Vk);  break;
        case BGEU:  cur_Ad._value = (tmp.Vj >= tmp.Vk);  break;
    }
    cur_Ad.target_name = for_wb;
    cur_Ad.is_empty = false;
}

void Ex_loa(Res_loa &cur_Rl, Mem_unit &cur_Ma, ROB &cur_ROB){
    Reservation_cell tmp;
    Reservation_name for_wb;
    if(!cur_Rl.isReady() || !cur_Ma.is_empty)  return;

    tmp=cur_Rl.find(for_wb);
    while(!cur_ROB.check_Ma(for_wb, tmp.Vj)){
        tmp=cur_Rl.find_next(for_wb);
        if(for_wb==NAME0)  return;
    }
    cur_Rl.pop(for_wb);

    switch (tmp.Op){
        case LB:
            char tmp1;
            memcpy(&tmp1, _memory + tmp.Vj, sizeof(char));
            cur_Ma._value=uint(tmp1);
            break;

        case LH:
            short tmp2;
            memcpy(&tmp2, _memory + tmp.Vj, sizeof(short));
            cur_Ma._value=uint(tmp2);
            break;

        case LW:
            memcpy(&(cur_Ma._value), _memory + tmp.Vj, sizeof(uint));
            break;

        case LBU:
            unsigned char tmp3;
            memcpy(&tmp3, _memory + tmp.Vj, sizeof(unsigned char));
            cur_Ma._value=uint(tmp3);
            break;

        case LHU:
            unsigned short tmp4;
            memcpy(&tmp4, _memory + tmp.Vj, sizeof(unsigned short));
            cur_Ma._value=uint(tmp4);
            break;
    }
    cur_Ma.target_name = for_wb;
    cur_Ma.is_empty = false;
    cur_Ma.cur_period = 1;
}

void Broadcast(Adder &cur_Ad, Mem_unit &cur_Ma, Res_add &cur_Ra, Res_loa &cur_Rl, ROB &cur_ROB){
    if(!cur_Ad.is_empty){
        cur_Ra.update(cur_Ad.target_name, cur_Ad._value);
        cur_Rl.update(cur_Ad.target_name, cur_Ad._value);
        cur_ROB.update(cur_Ad.target_name, cur_Ad._value);
        cur_Ad.is_empty = true;
    }
    if(!cur_Ma.is_empty){
        if(cur_Ma.cur_period != 3){
            ++cur_Ma.cur_period;
            return;
        }
        else{ 
            cur_Ma.is_empty = true;
            if(cur_Ma.target_name != NAME0){
                cur_Ra.update(cur_Ma.target_name, cur_Ma._value);
                cur_Rl.update(cur_Ma.target_name, cur_Ma._value);
                cur_ROB.update(cur_Ma.target_name, cur_Ma._value);
            }
        }
    }
}

void Commit(ROB &cur_ROB, Mem_unit &cur_Ma, Res_add &cur_Ra, Res_loa &cur_Rl, Adder &cur_Ad){
    Reservation_name for_wb;
    ROB_cell tmp;
    if(cur_ROB.isEmpty()){
        if(stop_issue)  is_end=true;
        return;
    }

    tmp=cur_ROB.top_ROB(for_wb);
    switch (tmp.Op){
        case ADD:  case SUB:  case SLL:  case SLT:  case SLTU:
        case XOR:  case SRL:  case SRA:  case OR:  case AND:
        case ADDI:  case SLTI:  case SLTIU:  case XORI:  case ORI:
        case ANDI:  case SLLI:  case SRLI:  case SRAI:  case LUI:
        case AUIPC:  case JAL:  case LB:  case LH:  case LW:
        case LBU:  case LHU:
            if(tmp.Qi)  return;
            T_register[tmp.target].Regs=tmp.Regs;
            if(T_register[tmp.target].Qi == for_wb)  T_register[tmp.target].Qi=NAME0;
            cur_ROB.pop_ROB();
            break;

        case SB:  case SH:  case SW:
            if(tmp.Qi || tmp.Qs)  return;
            if(!cur_Ma.is_empty)  return;
            switch (tmp.Op){
                case SB:
                    char tmp1;
                    tmp1=tmp.Regs;
                    memcpy(_memory + tmp.Vs, &tmp1, sizeof(char));
                    break;

                case SH:
                    short tmp2;
                    tmp2=tmp.Regs;
                    memcpy(_memory + tmp.Vs, &tmp2, sizeof(short));
                    break;

                case SW:
                    memcpy(_memory + tmp.Vs, &(tmp.Regs), sizeof(uint));
                    break;
            }
            cur_Ma.target_name = NAME0;
            cur_Ma.is_empty = false;
            cur_Ma.cur_period = 0;
            cur_ROB.pop_ROB();
            break;

        case JALR:
            if(tmp.Qs)  return;
            _pc = tmp.Vs;
            stop_issue = false;
            T_register[tmp.target].Regs=tmp.Regs;
            if(T_register[tmp.target].Qi == for_wb)  T_register[tmp.target].Qi=NAME0;
            cur_ROB.pop_ROB();
            break;

        case BEQ:  case BNE:  case BLT:  case BGE:  case BLTU:
        case BGEU:
            if(tmp.Qi)  return;
            ++total_prediction;
            record_branch(tmp.Regs, ((tmp.cur_pc>>2)&63));
            if(bool(tmp.Regs)!=tmp.isTaken){
                cur_ROB.clear();
                cur_Ra.clear();
                cur_Rl.clear();
                if(cur_Ma.target_name)  cur_Ma.is_empty=true;
                cur_Ad.is_empty=true;
                stop_issue=false;
                clear_register();

                if(tmp.Regs)  _pc = tmp.cur_pc + tmp.cur_imm;
                else  _pc = tmp.cur_pc + 4;
            }
            else{
                ++correct_prediction;
                cur_ROB.pop_ROB();
            }
    }

    //for debug
    //printf("%s %u\n", TYPES[tmp.Op], tmp.Regs);
    //if(tmp.target==10)  printf("%s %u\n", TYPES[tmp.Op], (T_register[10].Regs)&255);
}

#endif