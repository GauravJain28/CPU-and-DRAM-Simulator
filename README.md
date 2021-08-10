# CPU and DRAM Simulator

## About:
A MIPS assembly simulator is implemented with support for multicore processors. There are N cores and to handle the DRAM requests, a Memory Request Manager is designed.


## How to Run the Code:
1. Compile the file ```code.cpp``` using the command-
    ```
        g++ -O3 code.cpp -o out
    ```
2. Run the command-
    ```
    ./out N M R C D
    ```    
    Where,
    * ```N``` is the number of cores. 
    * ```M``` is the maximum number of clock cycles available for execution. 
    * ```R``` and ```C``` are row access delay and column access delay respectively. 
    * ```D``` is a single bit to show the execution details or not. 


The code will ask the names of the text files containing the instructions for each core. This process will happen N times.

Number of instructions executed for each core and total instructions executed are shown. Instructions per cycle (IPC) can easily be calculated using these values.

Some sample testcases are also present to try out the simulator.

## Author:
* [Gaurav Jain](https://github.com/GAURAV-28)   -   2019CS10349
* [T Abishek](https://github.com/abishek2188)   -   2019CS10407

...