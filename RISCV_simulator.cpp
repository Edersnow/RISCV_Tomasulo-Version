#include "Pipeline.hpp"

Res_add add_reservation;
Res_loa loa_reservation;
Adder adders;
Mem_unit memory_access;
ROB rob;

int main()
{
    uint cnt=1;
    uint memory_start;
    char _input[20];

    for(int i=0;i<64;++i)
        for(int j=0;j<256;++j)
            PHT[i][j]=1;
    for(int i=0;i<65536;++i)
        PHT_for_BHR[i]=1;

    while(scanf("%s", _input)!=EOF){
        if(_input[0]=='@')  sscanf(_input+1, "%x", &memory_start);
        else{
            sscanf(_input, "%x", &_memory[memory_start++]);
            scanf("%x", &_memory[memory_start++]);
            scanf("%x", &_memory[memory_start++]);
            scanf("%x", &_memory[memory_start++]);
            //printf("Read data: %02x%02x%02x%02x\n", _memory[memory_start-1],
            //_memory[memory_start-2], _memory[memory_start-3], _memory[memory_start-4]);
        }
    }

    //simulate
    while (!is_end){
        Commit(rob, memory_access, add_reservation, loa_reservation, adders);
        Broadcast(adders, memory_access, add_reservation, loa_reservation, rob);
        Ex_add(add_reservation, adders);
        Ex_loa(loa_reservation, memory_access, rob);
        Issue(rob, add_reservation, loa_reservation);
    }

    //printf("Total prediction: %u\n", total_prediction);
    //printf("Correct prediction: %u\n", correct_prediction);
    //printf("Prediction accuracy: %.2f%\n", (double(correct_prediction)/total_prediction)*100);
    printf("%u\n", (T_register[10].Regs)&255);
    return 0;
}