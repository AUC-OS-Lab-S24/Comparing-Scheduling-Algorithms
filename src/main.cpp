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

// Round Robin

int main(int argc, char const *argv[])
{
    /* code */
    cout << "hello world";
    return 0;
}
