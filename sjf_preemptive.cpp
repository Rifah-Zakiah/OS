#include <iostream>
#include <vector>
#include <iomanip>
#include <climits>
#include <algorithm>

using namespace std;

// Structure to store process details
struct Process {
    int id, arrival_time, burst_time, remaining_time, completion_time, turnaround_time, waiting_time;
};

// Function to print the Gantt Chart
void printGanttChart(vector<int> &timeline) {
    cout << "\nGantt Chart:\n ";
   
    cout << "|";
    
    for (int i = 0; i < timeline.size(); i++) {
        if (timeline[i] == -1)
            cout << "##|";  // CPU idle
        else
            cout << "P" << timeline[i] << "|";
    }
    
    cout << "\n0";
    for (int i = 0; i < timeline.size(); i++) {
        cout << "  " << i + 1;
    }
    cout << "\n";
}

// Function to perform SRTF scheduling
void srtfScheduling(vector<Process> &processes) {
    int n = processes.size();
    vector<int> timeline; // Store process execution at each time unit
    
    int completed = 0, current_time = 0, min_index = -1;
    int min_remaining_time = INT_MAX;
    
    // Sort by arrival time initially
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrival_time < b.arrival_time;
    });

    while (completed < n) {
        min_index = -1;
        min_remaining_time = INT_MAX;

        // Find the process with the shortest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_remaining_time) {
                    min_remaining_time = processes[i].remaining_time;
                    min_index = i;
                }
            }
        }

        // If no process is found, CPU is idle
        if (min_index == -1) {
            timeline.push_back(-1); // Represent idle time
            current_time++;
            continue;
        }

        // Execute the selected process for 1 unit of time
        processes[min_index].remaining_time--;
        timeline.push_back(processes[min_index].id);

        // If a process is completed
        if (processes[min_index].remaining_time == 0) {
            processes[min_index].completion_time = current_time + 1;
            processes[min_index].turnaround_time = processes[min_index].completion_time - processes[min_index].arrival_time;
            processes[min_index].waiting_time = processes[min_index].turnaround_time - processes[min_index].burst_time;
            completed++;
        }

        current_time++;
    }

    // Print Gantt Chart
    printGanttChart(timeline);

    // Calculate average turnaround time and waiting time
    float total_turnaround_time = 0, total_waiting_time = 0;
    for (auto &p : processes) {
        total_turnaround_time += p.turnaround_time;
        total_waiting_time += p.waiting_time;
    }

    // Print Process Table
    cout << "\nProcess Table:\n";
    cout << "+----+--------------+------------+----------------+-----------------+------------+\n";
    cout << "| ID | Arrival Time | Burst Time | Completion Time | Turnaround Time | Wait Time |\n";
    cout << "+----+--------------+------------+----------------+-----------------+------------+\n";
    for (auto &p : processes) {
        cout << "| " << setw(2) << p.id
             << " | " << setw(12) << p.arrival_time
             << " | " << setw(10) << p.burst_time
             << " | " << setw(14) << p.completion_time
             << " | " << setw(15) << p.turnaround_time
             << " | " << setw(10) << p.waiting_time
             << " |\n";
    }
    cout << "+----+--------------+------------+----------------+-----------------+------------+\n";

    // Print averages
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (total_turnaround_time / n) << " units\n";
    cout << "Average Waiting Time: " << fixed << setprecision(2) << (total_waiting_time / n) << " units\n";
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    cout << "Enter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Process " << processes[i].id << ":\n";
        cout << "Arrival Time: ";
        cin >> processes[i].arrival_time;
        cout << "Burst Time: ";
        cin >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time; // Initialize remaining time
    }

    // Run SRTF Scheduling
    srtfScheduling(processes);

    return 0;
}
