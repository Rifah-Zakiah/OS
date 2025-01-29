#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

struct Process {
    string name;
    int AT, BT, CT, TT, WT, priority, remainingBT;
};

void priorityScheduling(vector<Process> &processes, double &totalTT, double &totalWT) {
    int time = 0, completed = 0, n = processes.size();  
    
    for (auto &p : processes) {
        p.remainingBT = p.BT;
    }

    int idx = -1;  

    while (completed < n) {
        int highestPriority = INT_MAX;
        idx = -1;
        
        for (int i = 0; i < n; i++) {
            if (processes[i].remainingBT > 0 && processes[i].AT <= time && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx == -1) { 
            time++;
            continue; 
        }

        processes[idx].remainingBT--;
        time++;

        if (processes[idx].remainingBT == 0) {
            processes[idx].CT = time;
            processes[idx].TT = processes[idx].CT - processes[idx].AT;
            processes[idx].WT = processes[idx].TT - processes[idx].BT;
            totalTT += processes[idx].TT;
            totalWT += processes[idx].WT;
            completed++;
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter Process Name, Arrival Time, Burst Time, and Priority for process " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].AT >> processes[i].BT >> processes[i].priority;
    }

    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        return a.AT < b.AT;
    });

    double totalTT = 0, totalWT = 0;
    priorityScheduling(processes, totalTT, totalWT);

    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        return a.name < b.name;
    });
    
    cout << "\nProcess\tAT\tBT\tPriority\tCT\tTT\tWT\n";
    for (auto &p : processes) {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.priority << "\t"
             << p.CT << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    double avgTT = totalTT / n;
    double avgWT = totalWT / n;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;

    return 0;
}