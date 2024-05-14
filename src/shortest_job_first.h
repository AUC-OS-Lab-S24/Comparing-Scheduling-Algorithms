#ifndef SHORTEST_JOB_FIRST_H
#define SHORTEST_JOB_FIRST_H

#include "proc.cpp"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

void shortestJobFirst(vector<proc> &procs) // https://www.geeksforgeeks.org/program-for-shortest-job-first-or-sjf-cpu-scheduling-set-1-non-preemptive/
{
    int n = procs.size();
    int execution_times[n]; // remaining execution time for each process
    for (auto p : procs)
    {
        execution_times[p.pid] = p.execution_time;
    }
    int time = 0;
    int remaining = n;

    while (remaining > 0)
    {
        int min = INT_MAX;
        int turn = -1;
        for (int i = 0; i < n; i++)
        {
            if (procs[i].arrival_time <= time && execution_times[i] < min && execution_times[i] > 0)
            {
                min = execution_times[i];
                turn = i;
            }
        }
        if (turn == -1)
        {
            time++;
            continue;
        }
        if (procs[turn].response_time == -1)
        {
            procs[turn].response_time = time - procs[turn].arrival_time;
        }
        time += 1;
        execution_times[turn] -= 1;
        if (execution_times[turn] == 0)
        {
            remaining--;
            procs[turn].completion_time = time;
            procs[turn].turnarround_time = procs[turn].completion_time - procs[turn].arrival_time;
            procs[turn].waiting_time = procs[turn].turnarround_time - procs[turn].execution_time;
        }
    }
}





#endif