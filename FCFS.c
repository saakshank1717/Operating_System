#include <stdio.h>

int main()
{
    int n;
    int at[10], bt[10], ct[10], wt[10], tat[10];
    int i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Input Arrival Time and Burst Time
    for(i = 0; i < n; i++)
    {
        printf("\nEnter Arrival Time of P%d: ", i+1);
        scanf("%d", &at[i]);
        printf("Enter Burst Time of P%d: ", i+1);
        scanf("%d", &bt[i]);
    }

    // FCFS Calculation
    ct[0] = at[0] + bt[0];

    for(i = 1; i < n; i++)
    {
        if(ct[i-1] < at[i])
            ct[i] = at[i] + bt[i];
        else
            ct[i] = ct[i-1] + bt[i];
    }

    for(i = 0; i < n; i++)
    {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    // Output Table
    printf("\n--------------------------------------------------");
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT");
    printf("\n--------------------------------------------------");

    for(i = 0; i < n; i++)
    {
        printf("\nP%d\t%d\t%d\t%d\t%d\t%d",
               i+1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    // Gantt Chart
    printf("\n\nGantt Chart:\n");

    printf("|");
    for(i = 0; i < n; i++)
        printf("  P%d  |", i+1);

    printf("\n");

    printf("%d", at[0]);
    for(i = 0; i < n; i++)
        printf("     %d", ct[i]);

    printf("\n");

    return 0;
}
