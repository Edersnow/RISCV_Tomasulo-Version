# RISC-V Simulator
An easy implementation of RISC-V simulator in C++.

The simulator adopt the Tomasulo algorithm (with ROB) to simulate Out-of-order execution. The detail structure is driven from CA:AQA, Chapter 3.

The prediction part is the same as the [5-Stages Pipeline version](https://github.com/Edersnow/RISCV_5-Stages-Pipeline-Version). But the results are quite different in some tests (probably because the delay of recording the result) :

|Test|Correction/Total|Accuracy|
|:--:|:--------------:|-------:|
|basicopt1|136586/155139|88.04%|
|bulgarian|49342/71493|69.02%|
|hanoi|16755/17457|95.98%|
|magic|48506/67869|71.47%|
|pi|25955749/39956380|64.96%|
|qsort|182386/200045|91.17%|
|queens|56426/77116|73.17%|
|superloop|393600/435027|90.48%|
|tak|50250/60639|82.87%|

Reference & more information: [acm_wiki](https://acm.sjtu.edu.cn/wiki/PPCA_2020)