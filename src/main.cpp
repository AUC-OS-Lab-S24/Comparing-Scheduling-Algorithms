#include "proc.cpp"
#include "first_come_sched.h"
#include <vector>
#include <iostream>
#include <climits>

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

    while (remaining > 0)
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

enum SchedulerType
{
    ROUND_ROBIN,
    FIRST_COME_FIRST_SERVE,
    SHORTEST_JOB_FIRST
};

vector<proc> scheduleQueueRoundRobin(vector<proc> &procs, int quantum, int &totalRemaining)
{
    // we will run round robin but we will return the proccess that will be downgraded in a vecotor and we will remove them from procs and update remaining if a processes ends
    vector<proc> downgraded;
    int n = procs.size();
    int execution_times[n]; // remaining execution time for each process
    for (auto p : procs)
    {
        execution_times[p.pid] = p.execution_time;
    }
    int time = 0;
    int remaining = n;
    int turn = 0;

    // represents a time quantum
    while (remaining > 0)
    {
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
                    if (remainingQuantum == quantum)
                    {
                        downgraded.push_back(procs[turn]);
                        procs.erase(procs.begin() + turn);
                        n--;
                    }
                    remainingQuantum = 0;
                    turn = (turn + 1) % n; // next turn if process has not completed
                }
                else
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

vector<proc> scheduleQueueFirstComeFirstServe(vector<proc> &procs, int quantum, int &totalRemaining, int &time)
{
    // we will run first come first serve and return the proccess that will be downgraded in a vecotor and we will remove them from procs and update remaining if a processes ends
    int n = procs.size();

    int remaining = n;

    std::queue <int> fifo;

    std::vector<proc> downgraded;
    downgraded = {};
    std::vector<proc> procs_sorted;

    procs_sorted = procs;

    std::sort(procs_sorted.begin(), procs_sorted.end(), compareByArrivalTime);

    int current_executing_process = 0;

    while (remaining > 0) {
        // add all processes that have arrived by the current time to the queue
        while (current_executing_process < n && procs[current_executing_process].arrival_time <= time) {
            fifo.push(current_executing_process);
            current_executing_process++;
        }

        if (!fifo.empty()) {
                int process_to_execute = fifo.front();

                procs_sorted[process_to_execute].response_time = time - procs_sorted[process_to_execute].arrival_time;
                if (procs_sorted[process_to_execute].execution_time > quantum) {
                    procs_sorted[process_to_execute].execution_time -= quantum; 
                    time += quantum;

                    // downgrade the processes since it wont finish executing
                    downgraded.push_back(procs_sorted[process_to_execute]);
                }
                else {
                    fifo.pop();

                    // If the process is starting for the first time, set its response time
                    if (procs_sorted[process_to_execute].response_time == -1) {
                        procs_sorted[process_to_execute].response_time = time - procs_sorted[process_to_execute].arrival_time;
                    }

                    // Execute the process
                    time += procs_sorted[process_to_execute].execution_time;

                    // Calculate completion, turnaround, and waiting times
                    procs_sorted[process_to_execute].completion_time = time;
                    procs_sorted[process_to_execute].turnarround_time = procs[process_to_execute].completion_time - procs[process_to_execute].arrival_time;
                    procs_sorted[process_to_execute].waiting_time = procs[process_to_execute].turnarround_time - procs[process_to_execute].execution_time;
                }
                // Decrement the remaining process count
                remaining--;

        } else {
            // If no process is in the queue, move time forward to the next process arrival
            if (current_executing_process < n) {
                time = procs[current_executing_process].arrival_time;
            }
        }
    }

    //update procs according to procs_sorted on same pid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (procs_sorted[i].pid == procs[j].pid) {
                procs[j] = procs_sorted[i];
            }
        }
    }

    return downgraded;
}

vector<proc> scheduleQueueShortestJobFirst(vector<proc> &procs, int quantum, int &totalRemaining)
{
    // we will run shortest job first and return the proccess that will be downgraded in a vecotor and we will remove them from procs and update remaining if a processes ends
}

// feedback mechanism: if a process in a lower-priority queue uses up its time slice, it may be moved to a higher-priority queue to ensure it gets more CPU time.
void multilevelFeedbackQueue(vector<proc> &procs, int numberOfQueues = 3, vector<int> quantums = {2, 4, 8}, vector<SchedulerType> SchedulerTypes = {ROUND_ROBIN, ROUND_ROBIN, ROUND_ROBIN})
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
    // initialise queues
    vector<vector<proc>> queues;
    for (int i = 0; i < numberOfQueues; i++)
    {
        vector<proc> q;
        queues.push_back(q);
    }

    // copy all procs into the first queue
    for (auto aproc : procs)
    {
        proc p(aproc.pid, aproc.arrival_time, aproc.execution_time);
        queues[0].push_back(p);
    }

    while (remaining > 0)
    {
        for (int i = 0; i < numberOfQueues; i++)
        {
            // check if queue is empty or has no arrived yet
            if (queues[i].size() == 0)
            {
                continue;
            }
            bool hasArrived = false;
            for (auto p : queues[i])
            {
                if (p.arrival_time <= time)
                {
                    hasArrived = true;
                    break;
                }
            }
            if (!hasArrived)
            {
                continue;
            }
            // Call strategy on queue i
            vector<proc> downgrade;
            if (SchedulerTypes[i] == ROUND_ROBIN)
            {
                downgrade = scheduleQueueRoundRobin(queues[i], quantums[i], remaining);
            }
            else if (SchedulerTypes[i] == FIRST_COME_FIRST_SERVE)
            {
                downgrade = scheduleQueueFirstComeFirstServe(queues[i], quantums[i], remaining);
            }
            else if (SchedulerTypes[i] == SHORTEST_JOB_FIRST)
            {
                downgrade = scheduleQueueShortestJobFirst(queues[i], quantums[i], remaining);
            }
            // add downgraded processes to next queue
            for (auto p : downgrade)
            {
                if (i + 1 < numberOfQueues)
                    queues[i + 1].push_back(p);
                else
                    queues[0].push_back(p); // to implement feedback
            }
        }
    }
}

int main(int argc, char const *argv[])
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

    return 0;
}
