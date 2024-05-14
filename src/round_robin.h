
#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "proc.cpp"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Round Robin - https://www.geeksforgeeks.org/program-for-round-robin-scheduling-for-the-same-arrival-time/
void roundRobin(vector<proc> &procs, int quantum)
{
    int n = procs.size();
    int execution_times[n]; // remaining execution time for each process
    for (auto p : procs)
    {
        execution_times[p.pid] = p.execution_time;
    }
    int time = 0;
    int remaining = n;
    int turn = 0;

    while (remaining > 0 && n > 0)
    {
        // represents a time quantum
        int remainingQuantum = quantum;
        while (remainingQuantum > 0)
        {
            // attempt to give turn to a process with that pid for time quantum
            if (procs[turn].arrival_time > time)
            {
                turn = (turn + 1) % n; // next turn if not arrived
                continue;
            }
            if (execution_times[procs[turn].pid] > 0) // proc has not completed
            {
                if (procs[turn].response_time == -1)
                {
                    procs[turn].response_time = time - procs[turn].arrival_time;
                }
                if (execution_times[procs[turn].pid] > remainingQuantum) // proc will not complete in this quantum
                {
                    time += remainingQuantum;
                    execution_times[procs[turn].pid] -= remainingQuantum;
                    remainingQuantum = 0;
                    turn = (turn + 1) % n; // next turn if process has not completed
                }
                else // proc will complete in this quantum
                {
                    time += execution_times[procs[turn].pid];
                    remainingQuantum -= execution_times[procs[turn].pid];
                    execution_times[procs[turn].pid] = 0;
                    procs[turn].completion_time = time;
                    procs[turn].turnarround_time = procs[turn].completion_time - procs[turn].arrival_time;
                    procs[turn].waiting_time = procs[turn].turnarround_time - procs[turn].execution_time;
                    remaining--;
                    turn = (turn + 1) % n; // next turn if process has completed
                }
            }
            else
            {
                turn = (turn + 1) % n; // next turn if process has completed
                continue;
            }
        }
    }
}

#endif