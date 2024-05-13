
#ifndef FIRST_COME_SCHED_H
#define FIRST_COME_SCHED_H

#include <iostream>
#include <vector>
#include "proc.cpp"
#include <queue>
#include <algorithm>

bool compareByArrivalTime(const proc &process, const proc &other_process) 
{
    return process.arrival_time < other_process.arrival_time; // sort by arrival time
}

// first come first serve 
// a FIFO queue is made and processes are added to it in the order of arrival time
// the process with the earliest arrival time is executed first
// the process is removed from the queue and the next process is executed
// non preemptive
void first_come_first_serve(std::vector<proc> &procs) { 

    int n = procs.size();

    int time = 0;
    int remaining = n;

    std::queue <int> fifo;

    std::sort(procs.begin(), procs.end(), compareByArrivalTime);

    int current_executing_process = 0;

    while (remaining > 0) {
        // Add all processes that have arrived by the current time to the queue
        while (current_executing_process < n && procs[current_executing_process].arrival_time <= time) {
            fifo.push(current_executing_process);
            current_executing_process++;
        }

        if (!fifo.empty()) {
            int process_to_execute = fifo.front();
            fifo.pop();

            // If the process is starting for the first time, set its response time
            if (procs[process_to_execute].response_time == -1) {
                procs[process_to_execute].response_time = time - procs[process_to_execute].arrival_time;
            }

            // Execute the process
            time += procs[process_to_execute].execution_time;

            // Calculate completion, turnaround, and waiting times
            procs[process_to_execute].completion_time = time;
            procs[process_to_execute].turnarround_time = procs[process_to_execute].completion_time - procs[process_to_execute].arrival_time;
            procs[process_to_execute].waiting_time = procs[process_to_execute].turnarround_time - procs[process_to_execute].execution_time;

            // Decrement the remaining process count
            remaining--;
        } else {
            // If no process is in the queue, move time forward to the next process arrival
            if (current_executing_process < n) {
                time = procs[current_executing_process].arrival_time;
            }
        }
    }

}


#endif