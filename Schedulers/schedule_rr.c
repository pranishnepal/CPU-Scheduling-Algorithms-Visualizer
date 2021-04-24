#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "task.h"
#include "schedulers.h"
#include "list.h"

#define ARRIVAL_TIME 0   /* All processes have the same arrival time (0) */
#define DEFAULT_RT -1   /* Default response time */

/**
 * Global Variables:
 * listHead -> head of the list of tasks being built
 * taskId -> current task's id #
 * currElementPtr -> current element in the queue being processed: points to the head if at the end [Behaves like a circular array]
 */
struct node *listHead = NULL;
int taskId = 1;
struct node *currElementPtr = NULL;

/* Function definitions */
void printStatistics(int numOfTasks, int totalWaitTime, int totalTurnAroundTime, int totalResponseTime);

Task *getNextTask();

struct node *reverseLinkedList();

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
    //Additional Variables:
    newTask->burstRemaining = burst;
    newTask->originalBurstTime = burst;
    newTask->completionTime = 0;
    newTask->firstStartTime = DEFAULT_RT;
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
    int burstTime = 0;
    int numOfTasks = 0;
    /* Total time variables */
    int totalWaitTime = 0, totalResponseTime = 0, totalTurnAroundTime = 0;
    listHead = reverseLinkedList();
    currElementPtr = listHead;

    while (listHead != NULL) {
        Task *currTask = getNextTask();
        burstTime = currTask->burstRemaining;
        /* Update Variables */
        startTime = endTime;
        //Update firstStart variable if uninitialized before:
        if (currTask->firstStartTime == DEFAULT_RT) {
            currTask->firstStartTime = startTime;
        }
        //update endTime according to time quantum:
        endTime = (burstTime > QUANTUM) ? startTime + QUANTUM : startTime + burstTime;

        if (burstTime > QUANTUM) {
            run(currTask, QUANTUM);
            currTask->burstRemaining = currTask->burstRemaining - QUANTUM;
            currTask->burst = currTask->burstRemaining;
            //Update 'current' completion time:
            currTask->completionTime = endTime;
        } else {
            run(currTask, burstTime);
            currTask->burstRemaining = currTask->burstRemaining - burstTime;
            currTask->burst = currTask->burstRemaining;
            //Update variables:
            currTask->completionTime = endTime;
            numOfTasks++;
            totalResponseTime += (currTask->firstStartTime - ARRIVAL_TIME);
            int currTurnAroundTime = currTask->completionTime - ARRIVAL_TIME;
            int currWaitTime = currTurnAroundTime - currTask->originalBurstTime;
            totalTurnAroundTime += currTurnAroundTime;
            totalWaitTime += currWaitTime;

            delete(&listHead, currTask);
        }

        if (listHead == NULL) {
            printStatistics(numOfTasks, totalWaitTime, totalTurnAroundTime, totalResponseTime);
        }
    }
}

/**
 * @return: the next task to give CPU time to. Works like a Circular Array.
 *          If currElementPtr is at the end of the list, then it is set to the head.
 */
Task *getNextTask() {
    if (currElementPtr == NULL)
        return listHead->task;
    Task *nextTask = currElementPtr->task;
    //update currElementPtr to point to next element for next iteration
    currElementPtr = (currElementPtr->next != NULL) ? currElementPtr->next : listHead;
    return nextTask;
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

/**
 * @return the reversed list pointed to by listHead
 */
struct node *reverseLinkedList() {
    struct node *p = listHead;
    struct node *q = NULL;
    struct node *r = NULL;
    while (p != NULL) {
        r = q;
        q = p;
        p = p->next;
        q->next = r;
    }
    return q;
}


