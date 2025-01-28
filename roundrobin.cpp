#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>

using namespace std;

// Structure for a process
struct Process {
    int id;           // Process ID
    int arrivalTime;  // Arrival Time
    int burstTime;    // Burst Time
    int remainingTime; // Remaining Burst Time
    int completionTime; // Completion Time
    int waitTime;      // Wait Time
    int turnAroundTime; // Turnaround Time
};

// Function to implement Round Robin scheduling
void roundRobin(vector<Process> &processes, int quantum) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    queue<int> readyQueue;

    // Gantt chart representation
    vector<pair<int, int>> ganttChart;

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrivalTime < b.arrivalTime;
    });

    // Push first process into the queue
    readyQueue.push(0);
    vector<bool> inQueue(n, false);
    inQueue[0] = true;

    while (completed < n) {
        if (!readyQueue.empty()) {
            int currentProcess = readyQueue.front();
            readyQueue.pop();

            // Check for idle time
            if (currentTime < processes[currentProcess].arrivalTime) {
                ganttChart.push_back({-1, processes[currentProcess].arrivalTime - currentTime});
                currentTime = processes[currentProcess].arrivalTime;
            }

            // Add current process to Gantt chart
            ganttChart.push_back({processes[currentProcess].id, min(quantum, processes[currentProcess].remainingTime)});

            if (processes[currentProcess].remainingTime > quantum) {
                // Execute process for the quantum time
                currentTime += quantum;
                processes[currentProcess].remainingTime -= quantum;

                // Add newly arrived processes to the ready queue
                for (int i = 0; i < n; i++) {
                    if (!inQueue[i] && processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                        readyQueue.push(i);
                        inQueue[i] = true;
                    }
                }

                // Add the current process back to the queue
                readyQueue.push(currentProcess);
            } else {
                // Process completes
                currentTime += processes[currentProcess].remainingTime;
                processes[currentProcess].completionTime = currentTime;
                processes[currentProcess].remainingTime = 0;
                completed++;

                // Add newly arrived processes to the ready queue
                for (int i = 0; i < n; i++) {
                    if (!inQueue[i] && processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                        readyQueue.push(i);
                        inQueue[i] = true;
                    }
                }
            }
        } else {
            // CPU is idle
            ganttChart.push_back({-1, 1});
            currentTime++;
        }
    }

    // Calculate wait time and turnaround time
    double totalWaitTime = 0, totalTurnAroundTime = 0;
    for (auto &process : processes) {
        process.turnAroundTime = process.completionTime - process.arrivalTime;
        process.waitTime = process.turnAroundTime - process.burstTime;
        totalWaitTime += process.waitTime;
        totalTurnAroundTime += process.turnAroundTime;
    }

    // Output Gantt chart
    cout << "\nGantt Chart:\n";
    for (auto &[id, time] : ganttChart) {
        if (id == -1) {
            cout << "| Idle ";
        } else {
            cout << "| P" << id << " ";
        }
    }
    cout << "|\n";

    // Output Table
    cout << "\nProcess Table:\n";
    cout << setw(10) << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time"
         << setw(20) << "Completion Time" << setw(20) << "Turnaround Time" << setw(15) << "Wait Time" << "\n";

    for (const auto &process : processes) {
        cout << setw(10) << "P" << process.id << setw(15) << process.arrivalTime << setw(15)
             << process.burstTime << setw(20) << process.completionTime << setw(20)
             << process.turnAroundTime << setw(15) << process.waitTime << "\n";
    }

    // Output Average Times
    cout << "\nAverage Turnaround Time: " << totalTurnAroundTime / n << "\n";
    cout << "Average Wait Time: " << totalWaitTime / n << "\n";
}

int main() {
    int n, quantum;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    cout << "Enter the quantum time: ";
    cin >> quantum;

    cout << "Enter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Process P" << i + 1 << ":\n";
        cout << "Arrival Time: ";
        cin >> processes[i].arrivalTime;
        cout << "Burst Time: ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
    }

    roundRobin(processes, quantum);

    return 0;
}
