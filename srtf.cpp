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

void srtf(vector<Process> &processes) {
    vector<int> remaining_burst_time(processes.size()); // To track remaining burst time for each process
    for (size_t i = 0; i < processes.size(); ++i) {
        remaining_burst_time[i] = processes[i].burst_time;
    }

    int current_time = 0, completed = 0;
    vector<Process> result = processes; // Copy processes for result computation

    while (completed < processes.size()) {
        int min_burst_time = INT_MAX;
        int shortest = -1;

        // Find the process with the shortest remaining burst time
        for (size_t i = 0; i < processes.size(); ++i) {
            if (processes[i].arrival_time <= current_time && remaining_burst_time[i] > 0 &&
                remaining_burst_time[i] < min_burst_time) {
                min_burst_time = remaining_burst_time[i];
                shortest = i;
            }
        }

        if (shortest == -1) {
            // No process is available, move time forward
            ++current_time;
            continue;
        }

        // Process the shortest job
        --remaining_burst_time[shortest];
        ++current_time;

        if (remaining_burst_time[shortest] == 0) {
            // Process is completed
            ++completed;
            result[shortest].completion_time = current_time;
            result[shortest].turnaround_time =
                result[shortest].completion_time - result[shortest].arrival_time;
            result[shortest].wait_time =
                result[shortest].turnaround_time - result[shortest].burst_time;
        }
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
    srtf(processes);
}

