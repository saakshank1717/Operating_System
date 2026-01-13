#include <stdio.h>

int main()
{
    int n;
    int bt[10], wt[10], tat[10], p[10];
    int i, j, temp;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Input Burst Time
    for(i = 0; i < n; i++)
    {
        printf("Enter Burst Time of P%d: ", i+1);
        scanf("%d", &bt[i]);
        p[i] = i + 1;   // process number
    }

    // Sort by Burst Time (SJF)
    for(i = 0; i < n-1; i++)
    {
        for(j = i+1; j < n; j++)
        {
            if(bt[i] > bt[j])
            {
                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    // Waiting Time
    wt[0] = 0;
    for(i = 1; i < n; i++)
        wt[i] = wt[i-1] + bt[i-1];

    // Turnaround Time
    for(i = 0; i < n; i++)
        tat[i] = wt[i] + bt[i];

    // Process Table
    printf("\n----------------------------------");
    printf("\nProcess\tBT\tWT\tTAT");
    printf("\n----------------------------------");

    for(i = 0; i < n; i++)
        printf("\nP%d\t%d\t%d\t%d", p[i], bt[i], wt[i], tat[i]);

    // SIMPLE GANTT CHART
    printf("\n\nGantt Chart:\n");

    for(i = 0; i < n; i++)
        printf("P%d ", p[i]);

    printf("\n0 ");
    int time = 0;
    for(i = 0; i < n; i++)
    {
        time += bt[i];
        printf("%d ", time);
    }

    printf("\n");

    return 0;
}
