#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int wait_time;
};

void display_results(const std::vector<Process> &processes) {
    double avg_tat = 0, avg_wt = 0;
    cout << "\nGantt Chart:\n";
    int current_time = 0;
    for (const auto &p : processes) {
            cout << "| P" << p.id << " ";
    }
    cout << "|\n";
    cout << 0 << " ";
    for (const auto &p : processes) {
        cout << std::setw(4) << p.completion_time << " ";
    }
    //std::cout << processes.back().completion_time << "\n";

    cout << "\nTable:\n";
    cout << "Process\tAT\tBT\tCT\tTT\tWT\n";
    for (const auto &p : processes) {
        avg_tat += p.turnaround_time;
        avg_wt += p.wait_time;
        cout << "P" << p.id << "\t" << p.arrival_time << "\t" << p.burst_time
                  << "\t" << p.completion_time << "\t" << p.turnaround_time
                  << "\t" << p.wait_time << "\n";
    }
    cout << "Average Turnaround Time: " << avg_tat / processes.size() << "\n";
    cout << "Average Waiting Time: " << avg_wt / processes.size() << "\n";
}

void sjf(vector<Process> &processes) {
    sort(processes.begin(), processes.end(),
              [](const Process &a, const Process &b) 
              { return a.arrival_time < b.arrival_time; });

    int current_time = 0;
    vector<Process> result;
    vector<Process> ready_queue;

    while (!processes.empty() || !ready_queue.empty()) {
        while (!processes.empty() && processes.front().arrival_time <= current_time) {
            ready_queue.push_back(processes.front());
            processes.erase(processes.begin());
        }

        if (ready_queue.empty()) {
            current_time = processes.front().arrival_time;
            continue;
        }

        auto it = min_element(ready_queue.begin(), ready_queue.end(),
                                   [](const Process &a, const Process &b) {
                                       return a.burst_time < b.burst_time;
                                   });

        Process current = *it;
        ready_queue.erase(it);

        current_time += current.burst_time;
        current.completion_time = current_time;
        current.turnaround_time = current.completion_time - current.arrival_time;
        current.wait_time = current.turnaround_time - current.burst_time;
        result.push_back(current);
    }

    display_results(result);
}

int main(){
    cout << "Enter the number of processes: ";
    int n;
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cout << "Enter arrival time and burst time for process " << i + 1 << ": ";
        cin >> processes[i].arrival_time >> processes[i].burst_time;
    }
    sjf(processes);
}