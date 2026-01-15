#include <stdio.h>

struct process {
    int pid;
    int bt;
    int priority;
    int wt;
    int tat;
};

int main() {
    int n;
    float avg_wt = 0, avg_tat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct process p[n];

    // Input
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Burst Time for P%d: ", p[i].pid);
        scanf("%d", &p[i].bt);
        printf("Enter Priority for P%d: ", p[i].pid);
        scanf("%d", &p[i].priority);
    }

    // Sort by priority (ascending)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].priority > p[j].priority) {
                struct process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    // Calculate WT and TAT
    p[0].wt = 0;
    for (int i = 1; i < n; i++) {
        p[i].wt = p[i - 1].wt + p[i - 1].bt;
    }

    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].wt + p[i].bt;
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }

    avg_wt /= n;
    avg_tat /= n;

    // Output
    printf("\nPID\tBT\tPriority\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid, p[i].bt, p[i].priority,
               p[i].wt, p[i].tat);
    }

    printf("\nAverage Waiting Time = %.2f", avg_wt);
    printf("\nAverage Turnaround Time = %.2f\n", avg_tat);

    return 0;
}
