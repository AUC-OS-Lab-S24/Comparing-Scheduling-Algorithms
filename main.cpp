#include "proc.cpp"
#include <set>
#include <iostream>
using namespace std;

// Algorithims: Round Robin, First Come First Serve, Shortest Job First, Multilevel Feedback Queue
// Initialise a set of process with arrival time (which will be incremented one by one), execution_time (random value from 1 to 50), pids (start from 0 and increment)
set<proc> initSetProc(int n)
{
    set<proc> procs;
    for (int i = 0; i < n; i++)
    {
        proc p;
        p.pid = i;
        p.arrival_time = i;
        p.execution_time = rand() % 50 + 1;

        // Currently unkown
        p.waiting_time = 0;
        p.turnarround_time = 0; // will be known when waiting_time is known

        p.completion_time = 0; // TAT + AT
        p.response_time = 0;   // RT = TIME OF FIRST SCHEDULE - AT
        procs.insert(p);
    }
    return procs;
}

// Round Robin - https://www.geeksforgeeks.org/program-for-round-robin-scheduling-for-the-same-arrival-time/
void roundRobin(set<proc> procs, int quantum)
{
    int n = procs.size();
    int execution_times[n];
    for (auto p : procs)
    {
        execution_times[p.pid] = p.execution_time;
    }
    int waiting_time[n];
    int turnarround_time[n];
    int completion_time[n];
    int arrival_time[n];
    int response_time[n];
    int time = 0;
    int remaining = n;

    while (remaining > 0)
    {
        for (auto p : procs)
        {
            if (p.arrival_time > time) // if process is not ready yet skip it
            {
                continue;
            }

            // If I am here then I am a process that has arrived

            if (execution_times[p.pid] > 0) // If my execution time is not zero (I am not done yet)
            {
                if (p.response_time == 0) // Set the response time if it is not set
                {
                    p.response_time = time - p.arrival_time;
                }

                if (execution_times[p.pid] > quantum) // If my execution time is greater than quantum then I will be executed for quantum time
                {
                    time += quantum;
                    execution_times[p.pid] -= quantum;
                }
                else // else I will be executed for the remaining time and some other process will be executed for the remaining time
                {
                    time += execution_times[p.pid];
                    waiting_time[p.pid] = time - arrival_time[p.pid] - p.execution_time;
                    turnarround_time[p.pid] = time - arrival_time[p.pid];
                    completion_time[p.pid] = time;
                    execution_times[p.pid] = 0;
                    remaining--;
                }
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
