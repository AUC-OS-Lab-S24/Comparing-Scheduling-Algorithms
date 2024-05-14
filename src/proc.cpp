#ifndef _PROC_H_
#define _PROC_H_

#include "proc.h"
#include <random>   

class proc
{
public:
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
    int timeRemaining; //remaining execution time


    enum Status { NEW, RUNNING, COMPLETE } status;

    bool operator<(const proc &other_process) const
    {
        return pid < other_process.pid; // sort by id
    }

    void randProc(int n) {
        this->pid = n;
        this->arrival_time = rand() % 1000;
        this->execution_time = rand() % 1000;
        this->turnarround_time = 0;
        this->waiting_time = 0;
        this->completion_time = 0;
        this->response_time = -1;
        this->timeRemaining = execution_time;
        this->status = NEW; 
    }

    proc(int pid, int arrival_time, int execution_time) {
        this->pid = pid;
        this->arrival_time = arrival_time;
        this->execution_time = execution_time;
        this->turnarround_time = 0;
        this->waiting_time = 0;
        this->completion_time = 0;
        this->response_time = 0;
        this->timeRemaining = execution_time;
        this->status = NEW;
    }

    proc() {
        this->pid = 0;
        this->arrival_time = 0;
        this->execution_time = 0;
        this->turnarround_time = 0;
        this->waiting_time = 0;
        this->completion_time = 0;
        this->response_time = 0;
        this->timeRemaining = 0;
        this->status = NEW;
    }

    ~proc() {}

    void findTurnarroundTime()
    {
        turnarround_time = waiting_time + execution_time;
    }
};

#endif