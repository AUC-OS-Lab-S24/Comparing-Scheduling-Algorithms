
#ifndef TEST_AVG_TIMES_H
#define TEST_AVG_TIMES_H

#include <iostream>
#include <fstream>
#include "first_come_sched.h"
#include "shortest_job_first.h"
#include "round_robin.h"
#include "multi_level_feedback.h"
#include "proc.cpp"

typedef vector<vector<int>> avg_times_list;

using namespace std;

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

avg_times_list get_avg_times_iteration(vector<proc> &procs) {

    cout << "Iteration" << endl;

    vector<proc> shortest_job_first_procs = procs;
    vector<proc> round_robin_procs = procs;
    vector<proc> multi_level_feedback_procs = procs;
    vector<proc> first_come_sched_procs = procs;

    // now we run the schedulers on procs vector and print the average times in a csv file
    int shortest_job_first_avg_waiting_time = 0;
    int shortest_job_first_avg_turnaround_time = 0;
    int shortest_job_first_avg_response_time = 0;

    int round_robin_avg_waiting_time = 0;
    int round_robin_avg_turnaround_time = 0;
    int round_robin_avg_response_time = 0;

    int multi_level_feedback_avg_waiting_time = 0;
    int multi_level_feedback_avg_turnaround_time = 0;
    int multi_level_feedback_avg_response_time = 0;

    int first_come_sched_avg_waiting_time = 0;
    int first_come_sched_avg_turnaround_time = 0;
    int first_come_sched_avg_response_time = 0;



    for(int i = 0; i < (int)procs.size(); i++) {
        cout << "shortest job first" << endl;
        shortestJobFirst(shortest_job_first_procs);
        cout << "round robin" << endl;
        roundRobin(round_robin_procs, 3);
        cout << "multi level feedback queue" << endl;
        multi_level_feedback_procs = multilevelFeedbackQueue(multi_level_feedback_procs);
        cout << "first come first serve" << endl;
        first_come_first_serve(first_come_sched_procs);

        shortest_job_first_avg_waiting_time += shortest_job_first_procs[i].waiting_time;
        shortest_job_first_avg_turnaround_time += shortest_job_first_procs[i].turnarround_time;
        shortest_job_first_avg_response_time += shortest_job_first_procs[i].response_time;

        round_robin_avg_waiting_time += round_robin_procs[i].waiting_time;
        round_robin_avg_turnaround_time += round_robin_procs[i].turnarround_time;
        round_robin_avg_response_time += round_robin_procs[i].response_time;

        multi_level_feedback_avg_waiting_time += multi_level_feedback_procs[i].waiting_time;
        multi_level_feedback_avg_turnaround_time += multi_level_feedback_procs[i].turnarround_time;
        multi_level_feedback_avg_response_time += multi_level_feedback_procs[i].response_time;

        first_come_sched_avg_waiting_time += first_come_sched_procs[i].waiting_time;
        first_come_sched_avg_turnaround_time += first_come_sched_procs[i].turnarround_time;
        first_come_sched_avg_response_time += first_come_sched_procs[i].response_time;

        cout << "\ngetting averages: " << i << endl;
    }


    shortest_job_first_avg_waiting_time /= shortest_job_first_procs.size();
    shortest_job_first_avg_turnaround_time /= shortest_job_first_procs.size();
    shortest_job_first_avg_response_time /= shortest_job_first_procs.size();

    round_robin_avg_waiting_time /= round_robin_procs.size();
    round_robin_avg_turnaround_time /= round_robin_procs.size();
    round_robin_avg_response_time /= round_robin_procs.size();

    multi_level_feedback_avg_waiting_time /= multi_level_feedback_procs.size();
    multi_level_feedback_avg_turnaround_time /= multi_level_feedback_procs.size();
    multi_level_feedback_avg_response_time /= multi_level_feedback_procs.size();

    first_come_sched_avg_waiting_time /= first_come_sched_procs.size();
    first_come_sched_avg_turnaround_time /= first_come_sched_procs.size();
    first_come_sched_avg_response_time /= first_come_sched_procs.size();


    vector<int> avg_waiting_times = {shortest_job_first_avg_waiting_time, round_robin_avg_waiting_time, multi_level_feedback_avg_waiting_time, first_come_sched_avg_waiting_time};
    vector<int> avg_turnaround_times = {shortest_job_first_avg_turnaround_time, round_robin_avg_turnaround_time, multi_level_feedback_avg_turnaround_time, first_come_sched_avg_turnaround_time};
    vector<int> avg_response_times = {shortest_job_first_avg_response_time, round_robin_avg_response_time, multi_level_feedback_avg_response_time, first_come_sched_avg_response_time};

    avg_times_list list;
    list.push_back(avg_waiting_times);
    list.push_back(avg_turnaround_times);
    list.push_back(avg_response_times);

    return list;
}

void test_avg_times(int number_of_procs){
    cout << "Testing average times for " << number_of_procs << " processes" << endl;
    // generating increasing number of randomly generated processess 
    vector<proc> procs = {};

    ofstream waitingTimes, responseTimes, turnaroundTimes;
    waitingTimes.open ("avg_waiting_times.csv");
    responseTimes.open ("avg_response_times.csv");
    turnaroundTimes.open ("avg_turnaround_times.csv");

    //write file headers
    waitingTimes << "Number of Processes, Shortest Job First, Round Robin, Multi Level Feedback, First Come First Serve" << endl;
    responseTimes << "Number of Processes, Shortest Job First, Round Robin, Multi Level Feedback, First Come First Serve" << endl;
    turnaroundTimes << "Number of Processes, Shortest Job First, Round Robin, Multi Level Feedback, First Come First Serve" << endl;

    for (int i = 2; i <= number_of_procs; i += 100){
        procs = initProcVec(i, false, false);

    
        avg_times_list list = get_avg_times_iteration(procs);


        vector<int> avg_waiting_times = list[0];
        vector<int> avg_turnaround_times = list[1];
        vector<int> avg_response_times = list[2];


        waitingTimes << i << ",";
        responseTimes << i << ",";
        turnaroundTimes << i << ",";

        for (int j = 0; j < 4; j++){
            waitingTimes << avg_waiting_times[j] << ",";
            responseTimes << avg_response_times[j] << ",";
            turnaroundTimes << avg_turnaround_times[j] << ",";
        }


        waitingTimes << endl;
        responseTimes << endl;
        turnaroundTimes << endl;

        list = {};

        cout << "Done with " << i << " processes" << endl;
    }
        
    waitingTimes.close();
    responseTimes.close();
    turnaroundTimes.close();

}


#endif