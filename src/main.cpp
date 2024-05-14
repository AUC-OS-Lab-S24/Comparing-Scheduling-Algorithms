#include "proc.cpp"
#include "first_come_sched.h"
#include <vector>
#include <iostream>
#include <climits>
#include <string.h>

using namespace std;

// Algorithims: Round Robin, First Come First Serve, Shortest Job First, Multilevel Feedback Queue
// Initialise a set of process with arrival time (which will be incremented one by one), execution_time (random value from 1 to 50), pids (start from 0 and increment)
vector<proc> initProcVec(int n, bool random = true, bool highestExecutionTimeFirst = false)
{
    vector<proc> procs;

    for (int i = 0; i < n; i++)
    {
        proc p;
        p.pid = i;
        p.arrival_time = i;
        if (random)
            p.execution_time = rand() % 50 + 1;
        else if (highestExecutionTimeFirst)
            p.execution_time = 2 * (n - i);
        else
            p.execution_time = i + 1;

        // Currently unkown
        p.waiting_time = 0;
        p.turnarround_time = 0; // will be known when waiting_time is known
        p.timeRemaining = p.execution_time;
        p.status = proc::NEW;

        p.completion_time = 0; // TAT + AT
        p.response_time = -1;  // RT = TIME OF FIRST SCHEDULE - AT
        procs.push_back(p);
    }
    return procs;
}

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

bool areAllQueuesEmpty(const vector<queue<proc>> &queues)
{
    for (const auto &q : queues)
    {
        if (!q.empty())
        {
            return false;
        }
    }
    return true;
}

vector<proc> multilevelFeedbackQueue(vector<proc> &proces)
{
    vector<proc> finishedProcesses;
    vector<queue<proc>> processQueues(3);
    vector<int> timeSlices = {5, 10, 15};

    if (proces.empty())
    {
        return proces;
    }

    for (auto &process : proces)
    {
        processQueues[0].push(process);
    }

    int activeQueue = 0;
    int currentTime = 0;

    while (!areAllQueuesEmpty(processQueues))
    {
        if (processQueues[activeQueue].empty())
        {
            activeQueue = (activeQueue + 1) % 3;
            continue;
        }

        auto &currentProcess = processQueues[activeQueue].front();
        if (currentProcess.status == proc::NEW)
        {
            currentProcess.response_time = currentTime - currentProcess.arrival_time;
            currentProcess.status = proc::RUNNING;
        }

        int executionTime = min(timeSlices[activeQueue], currentProcess.timeRemaining);
        currentTime += executionTime;

        if (currentProcess.timeRemaining <= timeSlices[activeQueue])
        {
            currentProcess.timeRemaining = 0;
            currentProcess.completion_time = currentTime;
            currentProcess.turnarround_time = currentProcess.completion_time - currentProcess.arrival_time;
            currentProcess.waiting_time = currentProcess.turnarround_time - currentProcess.execution_time;
            currentProcess.status = proc::COMPLETE;
            finishedProcesses.push_back(currentProcess);
            processQueues[activeQueue].pop();
        }
        else
        {
            currentProcess.timeRemaining -= timeSlices[activeQueue];
            if (activeQueue < 2)
            {
                processQueues[activeQueue + 1].push(currentProcess);
            }
            else
            {
                processQueues[activeQueue].push(currentProcess);
            }
            processQueues[activeQueue].pop();
        }
    }

    return finishedProcesses;
}

int main(int argc, char const *argv[])
{
    // if -t argument is passed print out sample tests
    if (argc == 2 && strcmp(argv[1], "-t") == 0)
    {
        // tests for round robin
        cout << "Test for round robin" << endl;
        vector<proc> procs = initProcVec(3, false);
        roundRobin(procs, 3);
        for (auto p : procs)
        {
            cout << "pid: " << p.pid << " arrival_time: " << p.arrival_time << " execution_time: " << p.execution_time << " completion_time: " << p.completion_time << " response_time: " << p.response_time << " waiting_time: " << p.waiting_time << " turnarround_time: " << p.turnarround_time << endl;
        }
        cout << endl
             << endl
             << "Test for shortest job first" << endl;

        // tests for shortest job first
        procs = initProcVec(5, false);
        shortestJobFirst(procs);
        for (auto p : procs)
        {
            cout << "pid: " << p.pid << " arrival_time: " << p.arrival_time << " execution_time: " << p.execution_time << " completion_time: " << p.completion_time << " response_time: " << p.response_time << " waiting_time: " << p.waiting_time << " turnarround_time: " << p.turnarround_time << endl;
        }
        cout << endl
             << endl
             << "Test for shortest job first with decreasing execution time" << endl;
        procs = initProcVec(5, false, true);
        shortestJobFirst(procs);
        for (auto p : procs)
        {
            cout << "pid: " << p.pid << " arrival_time: " << p.arrival_time << " execution_time: " << p.execution_time << " completion_time: " << p.completion_time << " response_time: " << p.response_time << " waiting_time: " << p.waiting_time << " turnarround_time: " << p.turnarround_time << endl;
        }

        cout << "\nTest for FCFS" << endl;
        procs = initProcVec(5, false);
        first_come_first_serve(procs);
        for (auto p : procs)
        {
            cout << "pid: " << p.pid << " arrival_time: " << p.arrival_time << " execution_time: " << p.execution_time << " completion_time: " << p.completion_time << " response_time: " << p.response_time << " waiting_time: " << p.waiting_time << " turnarround_time: " << p.turnarround_time << endl;
        }

        cout << "\nTest for multilevel feedback queue" << endl;
        procs = initProcVec(5, false);
        vector<proc> finishedProcesses = multilevelFeedbackQueue(procs);
        for (auto p : finishedProcesses)
        {
            cout << "pid: " << p.pid << " arrival_time: " << p.arrival_time << " execution_time: " << p.execution_time << " completion_time: " << p.completion_time << " response_time: " << p.response_time << " waiting_time: " << p.waiting_time << " turnarround_time: " << p.turnarround_time << endl;
        }
    } 

    return 0;
}
