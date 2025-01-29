#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

// Structure to hold process information
struct Process {
    int id;              // Process ID
    int arrival_time;    // Arrival time of process
    int burst_time;      // CPU burst time required
    int completion_time; // Time when process completes
    int turnaround_time; // Total time from arrival to completion
    int wait_time;    // Time spent waiting in ready queue
};

void display_results(const std::vector<Process> &processes) {
    double avg_tat = 0, avg_wt = 0;
    //std::cout << "\nGantt Chart:\n";
    int current_time = 0;
    for (const auto &p : processes) {
            std::cout << "| P" << p.id << " ";
    }
    cout << "|\n";
    cout << 0;
    for (const auto &p : processes) {
        cout << std::setw(4) << p.completion_time << " ";
    }
    //std::cout << processes.back().completion_time << "\n";

    cout << "\nTable:\n";
    cout << "Process\tAT\tBT\tCT\tTT\tWT\n";
    for (const auto &p : processes) {
        avg_tat += p.turnaround_time;
        avg_wt += p.wait_time;
        std::cout << "P" << p.id << "\t" << p.arrival_time << "\t" << p.burst_time
                  << "\t" << p.completion_time << "\t" << p.turnaround_time
                  << "\t" << p.wait_time << "\n";
    }
    cout << "Average Turnaround Time: " << avg_tat / processes.size() << "\n";
    cout << "Average Waiting Time: " << avg_wt / processes.size() << "\n";
}

void fcfs(std::vector<Process> &processes) {
    sort(processes.begin(), processes.end(),
              [](const Process &a, const Process &b) 
              { return a.arrival_time < b.arrival_time; });

    int current_time = 0;
    for (auto &p : processes) {
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }
        current_time += p.burst_time;
        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.wait_time = p.turnaround_time - p.burst_time;
    }

    display_results(processes);
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
    fcfs(processes);
}