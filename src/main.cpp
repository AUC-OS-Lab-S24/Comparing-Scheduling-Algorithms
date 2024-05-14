#include "proc.cpp"
#include "first_come_sched.h"
#include "shortest_job_first.h"
#include "round_robin.h"
#include "multi_level_feedback.h"
#include "test_avg_times.h"
#include <vector>
#include <iostream>
#include <string.h>

using namespace std;

// Algorithims: Round Robin, First Come First Serve, Shortest Job First, Multilevel Feedback Queue


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
    // else if -r is passed run actual tests in test_avg_times.h
    else if (strcmp(argv[1], "-r") == 0)
    {
        cout << "\nRunning actual tests" << endl;
        test_avg_times(1000);
    }

    return 0;
}
