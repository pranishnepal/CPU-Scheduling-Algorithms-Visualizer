#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "task.h"
#include "schedulers.h"
#include "list.h"

#define ARRIVAL_TIME 0   /* All processes have the same arrival time (0) */

/**
 * Global Variables:
 * listHead -> head of the list of tasks being built
 * taskId -> current task's id #
 */
struct node *listHead = NULL;
int taskId = 1;

/* Function definitions */
void printStatistics(int numOfTasks, int totalWaitTime, int totalTurnAroundTime, int totalResponseTime);

Task *getLastNodeTask(struct node *currHead);

/**
 * This method, called from "driver.c", adds a new task to the list of tasks
 * @param name: name of the task
 * @param priority: priority of the task
 * @param burst: CPU burst time of the task
 */
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = taskId++;
    newTask->priority = priority;
    newTask->burst = burst;
    //add the task to the list. Calling insert() from "list.h"
    insert(&listHead, newTask);
}

/**
 * Iterates through the list of tasks and schedules each task struct.
 * At the end of the execution, it prints the scheduling statistics.
 */
void schedule() {
    int startTime = 0;
    int endTime = 0;
    int currResponseTime = 0;
    int currTurnAroundTime = 0;
    int currWaitTime = 0;
    int burstTime = 0;
    int numOfTasks = 0;
    /* Total time variables */
    int totalWaitTime = 0, totalResponseTime = 0, totalTurnAroundTime = 0;

    while (listHead != NULL) {
        Task *currTask = getLastNodeTask(listHead);
        //Set up variables that keep track of statistics:
        numOfTasks += 1;
        startTime = endTime;
        currResponseTime = startTime;
        burstTime = currTask->burst;

        //execute task:
        run(currTask, burstTime);

        //Update statistics:
        endTime = startTime + burstTime;
        currTurnAroundTime = endTime - ARRIVAL_TIME;
        currWaitTime = currTurnAroundTime - burstTime;

        //Update Total variables:
        totalWaitTime += currWaitTime;
        totalResponseTime += currResponseTime;
        totalTurnAroundTime += currTurnAroundTime;

        //now remove the task from the task list (ready queue):
        delete(&listHead, currTask);

        /* Check if we are at the end of the list, if so--print the statistics */
        if (listHead == NULL) {
            printStatistics(numOfTasks, totalWaitTime, totalTurnAroundTime, totalResponseTime);
        }
    }
}


/**
 *
 * @param currHead: head of the current list of tasks
 * @return: last node's task in the list of tasks
 */
Task *getLastNodeTask(struct node *currHead) {
    while (currHead->next != NULL)
        currHead = currHead->next;
    return currHead->task;
}

/**
 * Prints the average waiting, average turnaround, and average response time for the scheduler.
 * @param numOfTasks
 * @param totalWaitTime
 * @param totalTurnAroundTime
 * @param totalResponseTime
 */
void printStatistics(int numOfTasks, int totalWaitTime, int totalTurnAroundTime, int totalResponseTime) {
    double avgWaitTime = (double) totalWaitTime / numOfTasks;
    double avgTurnAroundTime = (double) totalTurnAroundTime / numOfTasks;
    double avgResponseTime = (double) totalResponseTime / numOfTasks;
    printf("\nAverage waiting time = %.2f\n", avgWaitTime);
    printf("Average turnaround time = %.2f\n", avgTurnAroundTime);
    printf("Average response time = %.2f\n", avgResponseTime);
}

