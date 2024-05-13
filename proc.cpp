#include "proc.h"
struct proc
{
    int pid;
    // enum procstate state;

    // TAT = WT + ET
    // RT = TIME OF FIRST SCHEDULE - AT
    // WT = TAT - ET
    // TAT = CT - AT

    int waiting_time;

    int execution_time; // aka burst time
    int turnarround_time;

    int arrival_time;    // time process enters ready state for the first time
    int completion_time; // time process completes execution aka exit time

    int response_time; // time between arrival time and getting the CPU for the first time
};