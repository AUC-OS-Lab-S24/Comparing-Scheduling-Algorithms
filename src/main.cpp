#include "proc.cpp"
#include <vector>
#include <iostream>

using namespace std;

// Algorithims: Round Robin, First Come First Serve, Shortest Job First, Multilevel Feedback Queue
// Initialise a set of process with arrival time (which will be incremented one by one), execution_time (random value from 1 to 50), pids (start from 0 and increment)
vector<proc> initSetProc(int n)
{
    vector<proc> procs;
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
        procs.push_back(p);
    }
    return procs;
}

// Round Robin - https://www.geeksforgeeks.org/program-for-round-robin-scheduling-for-the-same-arrival-time/
void roundRobin(vector<proc> procs, int quantum)
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

    while (remaining > 0)
    {
        // represents a time quantum
        int remainingQuantum = quantum;
        while (remainingQuantum > 0)
        {
            // attempt to give turn to a process with that pid for time quantum
            proc p = procs[turn];
            if (p.arrival_time > time)
            {
                turn = (turn + 1) % n; // next turn if not arrived
                continue;
            }
            if (execution_times[p.pid] > 0) // proc has not completed
            {
                if (p.response_time == 0)
                {
                    p.response_time = time - p.arrival_time;
                }
                if (execution_times[p.pid] > remainingQuantum) // proc will not complete in this quantum
                {
                    time += remainingQuantum;
                    execution_times[p.pid] -= remainingQuantum;
                    remainingQuantum = 0;
                }
                else // proc will complete in this quantum
                {
                    time += execution_times[p.pid];
                    remainingQuantum -= execution_times[p.pid];
                    execution_times[p.pid] = 0;
                    p.completion_time = time;
                    p.turnarround_time = p.completion_time - p.arrival_time;
                    p.waiting_time = p.turnarround_time - p.execution_time;
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

int main(int argc, char const *argv[])
{
    // tests for round robin

    return 0;
}
