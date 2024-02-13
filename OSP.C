#include <stdio.h>
#include <stdlib.h> // for rand() and srand()
#include <time.h>   // for time()

struct process
{
    char name;
    int ArrT, BurstT, WaitT, TurnArT, RemainT, ComplT; // Arrival time, burst time, waiting time, turnaround time ,completion time
} Queue1[10], Queue2[20], Queue3[30];

int n;

void generateInputs()
{
    srand((unsigned int)time(NULL));
    printf("Enter no of processes:");
    scanf("%d", &n);
    char c;
    for (int i = 0, c = 'A'; i < n; i++, c++)
    {
        Queue1[i].name = c;
        printf("\nEnter the arrival time %c: ", Queue1[i].name);
        scanf("%d", &Queue1[i].ArrT);

        Queue1[i].BurstT = rand() % 100 + 1; // Random burst time between 1 and 100
        Queue1[i].RemainT = Queue1[i].BurstT;
    }
}

void process_sort()
{
    struct process tmp;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (Queue1[i].ArrT > Queue1[j].ArrT)
            {
                tmp = Queue1[i];
                Queue1[i] = Queue1[j];
                Queue1[j] = tmp;
            }
        }
    }
}

int main()
{
    int i, k = 0, p = 0, time = 0, quanT1 = 8, quanT2 = 16, proc_level = 0;

    generateInputs();
    process_sort();
    int totalCPUTime = 0;

    // Calculate total CPU time
    for (i = 0; i < n; i++)
    {
        totalCPUTime += Queue1[i].BurstT;
    }

    // Distribute CPU time based on percentages
    int q1Time = (int)(0.5 * totalCPUTime);
    int q2Time = (int)(0.3 * totalCPUTime);
    int q3Time = totalCPUTime - q1Time - q2Time;

    time = Queue1[0].ArrT;
    printf("Process in first queue with RR having quantum time=8");
    printf("\nProcess\t\tRemainT\t\tWaitT\t\tTurnArT\t\t");
    for (i = 0; i < n; i++)
    {
        if (Queue1[i].RemainT <= quanT1)
        {
            time += Queue1[i].RemainT;
            Queue1[i].RemainT = 0;
            Queue1[i].WaitT = time - Queue1[i].ArrT - Queue1[i].BurstT;
            Queue1[i].TurnArT = time - Queue1[i].ArrT;
            printf("\n%c\t\t%d\t\t%d\t\t%d", Queue1[i].name, Queue1[i].BurstT, Queue1[i].WaitT, Queue1[i].TurnArT);
        }
        else
        {
            if (k < 20) // Check if queue 2 is not full
            {
                Queue2[k].WaitT = time;
                time += quanT1;
                Queue1[i].RemainT -= quanT1;
                Queue2[k].BurstT = Queue1[i].RemainT;
                Queue2[k].RemainT = Queue2[k].BurstT;
                Queue2[k].name = Queue1[i].name;
                k = k + 1;
                proc_level = 1;
            }
            else
            {
                // Process waits if queue 2 is full
                time = Queue2[0].WaitT; // Set time to the start time of the first process in queue 2
            }
        }
    }
    if (proc_level == 1)
    {
        printf("\nProcess in the second queue with RR having quantum time=16");
        printf("\nProcess\t\tRemainT\t\tWaitT\t\tTurnArT\t\t");
    }
    for (i = 0; i < k; i++)
    {
        if (Queue2[i].RemainT <= quanT2)
        {
            time += Queue2[i].RemainT;
            Queue2[i].RemainT = 0;
            Queue2[i].WaitT = time - quanT1 - Queue2[i].BurstT;
            Queue2[i].TurnArT = time - Queue2[i].ArrT;
            printf("\n%c\t\t%d\t\t%d\t\t%d", Queue2[i].name, Queue2[i].BurstT, Queue2[i].WaitT, Queue2[i].TurnArT);
        }
        else
        {
            if (p < 30) // Check if queue 3 is not full
            {
                Queue3[p].ArrT = time;
                time += quanT2;
                Queue2[i].RemainT -= quanT2;
                Queue3[p].BurstT = Queue2[i].RemainT;
                Queue3[p].RemainT = Queue3[p].BurstT;
                Queue3[p].name = Queue2[i].name;
                p = p + 1;
                proc_level = 2;
            }
            else
            {
                // Process waits if queue 3 is full
                time = Queue3[0].ArrT; // Set time to the start time of the first process in queue 3
            }
        }
    }

    if (proc_level == 2)
        printf("\nProcess in the third queue with FCFS ");

    for (i = 0; i < p; i++)
    {
        if (i == 0)
            Queue3[i].ComplT = Queue3[i].BurstT + time - quanT1 - quanT2;
        else
            Queue3[i].ComplT = Queue3[i - 1].ComplT + Queue3[i].BurstT;
    }

    for (i = 0; i < p; i++)
    {
        Queue3[i].TurnArT = Queue3[i].ComplT;
        Queue3[i].WaitT = Queue3[i].TurnArT - Queue3[i].BurstT;
        printf("\n%c\t\t%d\t\t%d\t\t%d\t\t", Queue3[i].name, Queue3[i].BurstT, Queue3[i].WaitT, Queue3[i].TurnArT);
    }

    return 0;
}
