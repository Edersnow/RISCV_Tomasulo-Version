#include "Pipeline.hpp"

int main()
{
    Res_add add_reservation;
    Res_loa loa_reservation;
    Adder adders;
    Mem_unit memory_access;
    ROB rob;
    uint cnt=1;
    uint memory_start;
    char _input[20];


    for(int i=0;i<64;++i)
        for(int j=0;j<16;++j)
            PHT[i][j]=1;
    for(int i=0;i<65536;++i)
        PHT_for_BHR[i]=1;


    //debug path
    /*
    if(!freopen("PPCA_2020/testcases/queens.data", "r", stdin)){
        printf("Error in reading!\n");
        return 0;
    }
    printf("Freopen successfully!\n");
    */

    /*
    if(!freopen("testcases/bulgarian.data", "r", stdin)){
        printf("Error in reading!\n");
        return 0;
    }
    //printf("Freopen successfully!\n");
    */


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
    //printf("Read data successfully!\n");

    //simulate
    while (!is_end){
        //printf("%u\n", cnt++);
        ++cnt;
        Commit(rob, memory_access, add_reservation, loa_reservation, adders);
        Broadcast(adders, memory_access, add_reservation, loa_reservation, rob);
        Ex_add(add_reservation, adders);
        Ex_loa(loa_reservation, memory_access, rob);
        Issue(rob, add_reservation, loa_reservation);
        //if(cnt==500)  break;
    }

    //printf("%u\n", cnt);
    //printf("Total prediction: %u\n", total_prediction);
    //printf("Correct prediction: %u\n", correct_prediction);
    //printf("Prediction accuracy: %.2f%\n", (double(correct_prediction)/total_prediction)*100);
    printf("%u\n", (T_register[10].Regs)&255);
    return 0;
}