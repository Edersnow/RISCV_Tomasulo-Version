#ifndef __Conponent
#define __Conponent
#include "Instruction.hpp"

struct Res_add;
struct Res_loa;
struct Adder;
struct Mem_unit;
struct ROB;





struct Res_add{
    Reservation_cell Arr[4];
    Reservation_name Name[4];
    uint TmpBusy;
    uint TmpReady;



    Res_add();
    bool isEmpty() {return TmpBusy==0;}
    bool isFull() {return TmpBusy==4;}
    bool isReady() {return TmpReady!=0;}

    Reservation_name push(Reservation_cell &other);
    Reservation_cell pop(Reservation_name &cur_name);
    void update(Reservation_name cur_name, uint val);
    void clear();
};





struct Res_loa{
    Reservation_cell Arr[5];
    Reservation_name Name[5];
    uint TmpBusy;
    uint TmpReady;



    Res_loa();
    bool isEmpty() {return TmpBusy==0;}
    bool isFull() {return TmpBusy==5;}
    bool isReady() {return TmpReady!=0;}

    Reservation_name push(Reservation_cell &other);
    Reservation_cell find(Reservation_name &cur_name);
    void pop(Reservation_name cur_name);
    void update(Reservation_name cur_name, uint val);
    void clear();
};





struct Adder{
    bool is_empty;
    Reservation_name target_name;
    uint _value;

    Adder(){is_empty=true;}
};





struct Mem_unit{
    bool is_empty;
    Reservation_name target_name;
    uint cur_period;
    uint _value;

    Mem_unit(){is_empty=true;}
};





struct ROB{
    //circular queues
    ROB_cell Arr[5];
    Reservation_name Name[5];
    uint head;
    uint tail;



    ROB();
    bool isFull() {return (tail+1)%5 == head;}
    bool isEmpty() {return tail == (head+1)%5;}
    void set_ROB(ROB_cell &other);
    ROB_cell top_ROB(Reservation_name &cur_name);
    void pop_ROB();
    void clear();

    void update(Reservation_name cur_name, uint val);
    bool check_Ma(Reservation_name cur_name, uint tmp_add);
};










Res_add::Res_add(){
    Name[0]=ADD0;
    Name[1]=ADD1;
    Name[2]=ADD2;
    Name[3]=ADD3;
    TmpBusy=0;
    TmpReady=0;
}

Res_loa::Res_loa(){
    Name[0]=LOA0;
    Name[1]=LOA1;
    Name[2]=LOA2;
    Name[3]=LOA3;
    Name[4]=LOA4;
    TmpBusy=0;
    TmpReady=0;
}

ROB::ROB(){
    Name[0]=ROB0;
    Name[1]=ROB1;
    Name[2]=ROB2;
    Name[3]=ROB3;
    Name[4]=ROB4;
    head=0;
    tail=1;
}

Reservation_name Res_add::push(Reservation_cell &other){
    int i;
    for(i=0; i<4; ++i){
        if(!Arr[i].Busy){
            Arr[i]=other;
            ++TmpBusy;
            if(!other.Qj && !Arr[i].Qk){
                Arr[i].isReady=true;
                ++TmpReady;
            }
            break;
        }
    }
    return Name[i];
}

Reservation_cell Res_add::pop(Reservation_name &cur_name){
    Reservation_cell tmp;
    for(int i=0; i<4; ++i){
        if(Arr[i].Busy && Arr[i].isReady && !Arr[i].Qj && !Arr[i].Qk){
            cur_name = Name[i];
            tmp=Arr[i];
            Arr[i].isExing = true;
            Arr[i].isReady = false;
            --TmpReady;
            break;
        }
    }
    return tmp;
}

void Res_add::update(Reservation_name cur_name, uint val){
    for(int i=0; i<4; ++i){
        if(Arr[i].Busy){
            if(Arr[i].Qj == cur_name){
                Arr[i].Vj=val;
                Arr[i].Qj=NAME0;
            }
            if(Arr[i].Qk == cur_name){
                Arr[i].Vk=val;
                Arr[i].Qk=NAME0;
            }
            if(!Arr[i].Qj && !Arr[i].Qk && !Arr[i].isReady && !Arr[i].isExing){
                Arr[i].isReady = true;
                ++TmpReady;
            }
        }
    }
    if(cur_name>=ADD0 && cur_name<=ADD3){
        Arr[cur_name-ADD0].Busy=false;
        --TmpBusy;
    }
}

void Res_add::clear(){
    for(int i=0; i<4; ++i)  Arr[i].Busy=false;
    TmpBusy=0;
    TmpReady=0;
}






Reservation_name Res_loa::push(Reservation_cell &other){
    int i;
    for(i=0; i<5; ++i){
        if(!Arr[i].Busy){
            Arr[i]=other;
            ++TmpBusy;
            if(!other.Qj){
                Arr[i].isReady=true;
                ++TmpReady;
            }
            break;
        }
    }
    return Name[i];
}

Reservation_cell Res_loa::find(Reservation_name &cur_name){
    Reservation_cell tmp;
    for(int i=0; i<4; ++i){
        if(Arr[i].Busy && Arr[i].isReady && !Arr[i].Qj){
            cur_name = Name[i];
            tmp=Arr[i];
            break;
        }
    }
    return tmp;
}

void Res_loa::pop(Reservation_name cur_name){
    Arr[cur_name-LOA0].isExing = true;
    Arr[cur_name-LOA0].isReady = false;
    --TmpReady;
}

/*
void Res_loa::pop(Reservation_name cur_name){
    Reservation_cell tmp;
    for(int i=0; i<4; ++i){
        if(Arr[i].Busy && Arr[i].isReady && !Arr[i].Qj){
            cur_name = Name[i];
            tmp=Arr[i];
            Arr[i].isExing = true;
            Arr[i].isReady = false;
            --TmpReady;
            break;
        }
    }
    return tmp;
}
*/
void Res_loa::update(Reservation_name cur_name, uint val){
    for(int i=0; i<5; ++i){
        if(Arr[i].Busy){
            if(Arr[i].Qj == cur_name){
                Arr[i].Vj += val;
                Arr[i].Qj = NAME0;
                if(!Arr[i].isExing && !Arr[i].isReady){
                    Arr[i].isReady = true;
                    ++TmpReady;
                }
            }
        }
    }
    if(cur_name>=LOA0 && cur_name<=LOA4){
        Arr[cur_name-LOA0].Busy=false;
        --TmpBusy;
    }
}

void Res_loa::clear(){
    for(int i=0; i<5; ++i)  Arr[i].Busy=false;
    TmpBusy=0;
    TmpReady=0;
}






void ROB::set_ROB(ROB_cell &other){
    Arr[tail]=other;
    if(other.target)  T_register[other.target].Qi=Name[tail];
    tail=(tail+1)%5;
}

ROB_cell ROB::top_ROB(Reservation_name &cur_name){
    cur_name = Name[(head+1)%5];
    return Arr[(head+1)%5];
}

void ROB::pop_ROB(){
    Arr[head=(head+1)%5].Busy=false;

}

void ROB::update(Reservation_name cur_name, uint val){
    for(int i=(head+1)%5; i!=tail; i=(i+1)%5){
        if(Arr[i].Busy){
            if(Arr[i].Qi == cur_name){
                Arr[i].Regs=val;
                Arr[i].Qi=NAME0;
            }
            if((Arr[i].Op>=25 && Arr[i].Op<=27) || Arr[i].Op == 10){
                if(Arr[i].Qs == cur_name){
                    Arr[i].Vs += val;
                    Arr[i].Qs = NAME0;
                }
            }
        }
    }
}

void ROB::clear(){
    for(int i=0; i<5; ++i)  Arr[i].Busy=false;
    head=0;
    tail=1;
}

bool ROB::check_Ma(Reservation_name cur_name, uint tmp_add){
    for(int i=(head+1)%5; Arr[i].Qi!=cur_name; i=(i+1)%5){
        if(Arr[i].Op>=25 && Arr[i].Op<=27){
            if(Arr[i].Qs!=NAME0 || Arr[i].Vs == tmp_add)
                return false;
        }
    }
    return true;
}





//get name and value
void rename_res(ROB &cur_rob, Reservation_cell &cur_res, uint rs1, uint rs2){
    if(rs1){
        if(T_register[rs1].Qi != NAME0){
            if(cur_rob.Arr[T_register[rs1].Qi-ROB0].Qi != NAME0){
                cur_res.Qj = cur_rob.Arr[T_register[rs1].Qi-ROB0].Qi;
            }
            else{
                cur_res.Vj = cur_rob.Arr[T_register[rs1].Qi-ROB0].Regs;
            }
        }
        else{
            cur_res.Vj = T_register[rs1].Regs;
        }
    }

    if(rs2){
        if(T_register[rs2].Qi != NAME0){
            if(cur_rob.Arr[T_register[rs2].Qi-ROB0].Qi != NAME0){
                cur_res.Qk = cur_rob.Arr[T_register[rs2].Qi-ROB0].Qi;
            }
            else{
                cur_res.Vk = cur_rob.Arr[T_register[rs2].Qi-ROB0].Regs;
            }
        }
        else{
            cur_res.Vk = T_register[rs2].Regs;
        }
    }
}

//get name and value
void rename_rob(ROB &cur_rob, ROB_cell &cur_res, uint rs1, uint rs2){
    if(rs1){
        if(T_register[rs1].Qi != NAME0){
            if(cur_rob.Arr[T_register[rs1].Qi-ROB0].Qi != NAME0){
                cur_res.Qs = cur_rob.Arr[T_register[rs1].Qi-ROB0].Qi;
            }
            else{
                cur_res.Vs = cur_rob.Arr[T_register[rs1].Qi-ROB0].Regs;
            }
        }
        else{
            cur_res.Vs = T_register[rs1].Regs;
        }
    }

    if(rs2){
        if(T_register[rs2].Qi != NAME0){
            if(cur_rob.Arr[T_register[rs2].Qi-ROB0].Qi != NAME0){
                cur_res.Qi = cur_rob.Arr[T_register[rs2].Qi-ROB0].Qi;
            }
            else{
                cur_res.Regs = cur_rob.Arr[T_register[rs2].Qi-ROB0].Regs;
            }
        }
        else{
            cur_res.Regs = T_register[rs2].Regs;
        }
    }
}





//predict part
bool judge_branch(uint hash_value, instruction_decoder &cur_dins){
    //banche predict(2-bit)
    if(app_time[hash_value]<8){
        //BTFN
        if(cur_dins._immediate<0)
            return true;
    }
    else{
        //local
        if(PHT[hash_value][BHT[hash_value]]==3)
            return true;
        //global
        else if(total_app==16 && PHT_for_BHR[BHR]==3)
            return true;
    }
    return false;
}

void record_branch(uint res, uint hash_value){
    if(res){
        if(app_time[hash_value]<8)  ++app_time[hash_value];
        else  PHT[hash_value][BHT[hash_value]]=std::min(PHT[hash_value][BHT[hash_value]]+1, 3u);
        if(total_app<16)  ++total_app;
        else  PHT_for_BHR[BHR]=std::min(PHT_for_BHR[BHR]+1, 3u);

        BHT[hash_value]=(BHT[hash_value]<<1)&255;
        BHT[hash_value]|=1;
        BHR=(BHR<<1)&65535;
        BHR|=1;
    }
    else{
        if(app_time[hash_value]<8)  ++app_time[hash_value];
        else  PHT[hash_value][BHT[hash_value]]=std::max(PHT[hash_value][BHT[hash_value]]-1, 0u);
        if(total_app<16)  ++total_app;
        else  PHT_for_BHR[BHR]=std::max(PHT_for_BHR[BHR]-1, 0u);

        BHT[hash_value]=(BHT[hash_value]<<1)&255;
        BHR=(BHR<<1)&65535;
    }
}





//register
/*
void update_register(Reservation_name cur_name, uint val){
    for(int i=1; i<32; ++i){
        if(T_register[i].Qi == cur_name){
            T_register[i].Regs = val;
            T_register[i].Qi = NAME0;
            return;
        }
    }
}
*/

void clear_register(){
    for(int i=1; i<32; ++i)  T_register[i].Qi = NAME0;
}

#endif