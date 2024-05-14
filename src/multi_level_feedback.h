#ifndef __MULTILEVEL_FEEDBACK_H_
#define __MULTILEVEL_FEEDBACK_H_

#include <iostream>
#include <queue>
#include <vector>
#include "proc.cpp"

using namespace std;

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

#endif