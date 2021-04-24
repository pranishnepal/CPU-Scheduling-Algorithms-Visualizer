#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "task.h"
#include "schedulers.h"
#include "list.h"

#define ARRIVAL_TIME 0   /* All processes have the same arrival time (0) */
#define DEFAULT_RT -1   /* Default response time */
#define ARRAY_SIZE 10

/**
 * Global Variables:
 * arrayOfTasks -> array of tasks indexed by priority value; each cell has a pointer to the head of the task LinkedList. In Java, we would implement it as: Map<Integer, Task>
 * taskId -> current task's id #
 * currElementPtr -> current element in the queue being processed: points to the head if at the end of the list [Behaves like a circular array]
 * numOfTasks -> count the total number of processes
 * totalWaitTime, totalResponseTime, totalTurnAroundTime -> keep track of the respective times; declared as globals to encapsulate update in a method.
 */

struct node *arrayOfTasks[ARRAY_SIZE + 1];
int taskId = 1;
struct node *currElementPtr = NULL;
int numOfTasks = 0;
int totalWaitTime = 0, totalResponseTime = 0, totalTurnAroundTime = 0;

/* Function definitions */
void printStatistics(int numOfTasks, int totalWaitTime, int totalTurnAroundTime, int totalResponseTime);

Task *getNextTask(struct node *currHead);

struct node *reverseLinkedList(struct node *currHead);

void updateVariables(Task *currTask, int endTime);

/**
 * This method, called from "driver.c", adds a new task to arrayOfTask:
 * it finds the appropriate cell based on the priority value and creates a LinkedList in that cell
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
    insert(&arrayOfTasks[priority], newTask);
}

/**
 * Iterates through the list of tasks and schedules each task struct.
 * At the end of the execution, it prints the scheduling statistics.
 */
void schedule() {
    int startTime = 0;
    int endTime = 0;

    bool applyPriorityAlg = false;
    bool applyRRAlg = false;

    for (int currIdx = ARRAY_SIZE; currIdx != -1; currIdx--) {
        currElementPtr = arrayOfTasks[currIdx];
        struct node *currList = arrayOfTasks[currIdx];

        while (currList != NULL) {
            startTime = endTime;
            //check if there's only one task in this list; if so, apply priority scheduler:
            if (currList->next == NULL) {
                applyPriorityAlg = true;
            } else {
                applyRRAlg = true;
            }

            //Based on the boolean value, apply respective algorithm:

            if (applyPriorityAlg) { /* Priority value is not shared--give full execution time */

                Task *currTask = getNextTask(currList);
                //Update times:
                if (currTask->firstStartTime == DEFAULT_RT) {
                    currTask->firstStartTime = startTime;
                    totalResponseTime += (currTask->firstStartTime - ARRIVAL_TIME);
                }
                endTime = startTime + currTask->burst;

                run(currTask, currTask->burst);
                updateVariables(currTask, endTime);
                delete(&currList, currTask);

            } else if (applyRRAlg) {  /* Priority value is shared--apply RR algorithm to the shared processes */

                currList = reverseLinkedList(currList);  //Get FIFO Order of the tasks
                currElementPtr = currList;
                bool onlyOneTaskLeft = false;

                while (currList != NULL) {
                    //Update startTime:
                    startTime = endTime;

                    onlyOneTaskLeft = (currList->next == NULL) ? true : false;

                    if (!onlyOneTaskLeft) {
                        Task *currTask = getNextTask(currList);
                        //Update Response Time:
                        if (currTask->firstStartTime == DEFAULT_RT) {
                            currTask->firstStartTime = startTime;
                            totalResponseTime += (currTask->firstStartTime - ARRIVAL_TIME);
                        }

                        int executeFor = (currTask->burstRemaining > QUANTUM) ? QUANTUM : currTask->burstRemaining;
                        endTime = startTime + executeFor;

                        run(currTask, executeFor);

                        //Alternate the processing of the task: reverse
                        //currList = reverseLinkedList(currList);
                        //Update burst time remaining:
                        currTask->burstRemaining = currTask->burstRemaining - executeFor;
                        currTask->burst = currTask->burstRemaining;

                        //Increment numOfTasks before each deletion of a task
                        if (currTask->burstRemaining == 0) {
                            updateVariables(currTask, endTime);
                            delete(&currList, currTask);
                        }
                    } else {
                        //Only one task remaining, give the remainder of the burst time to that task.
                        Task *currTask = getNextTask(currList);
                        //Update Time:
                        if (currTask->firstStartTime == DEFAULT_RT) {
                            currTask->firstStartTime = startTime;
                            totalResponseTime += (currTask->firstStartTime - ARRIVAL_TIME);
                        }
                        endTime = startTime + currTask->burst;

                        run(currTask, currTask->burst);
                        updateVariables(currTask, endTime);
                        delete(&currList, currTask);
                    }

                }

            }

            applyPriorityAlg = false;
            applyRRAlg = false;

        }

    }

    printStatistics(numOfTasks, totalWaitTime, totalTurnAroundTime, totalResponseTime);

}

/**
 *
 * @param currHead: points to the head of the list passed
 * @return: the next task to give CPU time to from the list passed. Works like a Circular Array.
 *          If currElementPtr is at the end of the list, then it is set to the head.
 */
Task *getNextTask(struct node *currHead) {
    if (currElementPtr == NULL)
        return currHead->task;
    Task *nextTask = currElementPtr->task;
    //update currElementPtr to point to next element for next iteration
    currElementPtr = (currElementPtr->next != NULL) ? currElementPtr->next : currHead;
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
 * @return the reversed list pointed by currHead
 */
struct node *reverseLinkedList(struct node *currHead) {
    struct node *p = currHead;
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


/**
 * This method updates completion time for a task, totalTurnAroundTime so far, totalWaitTime so far, & numOfTasks so far
 *
 * @param currTask: Pointer to current task which needs to be updated.
 * @param endTime:  time at which currTask was done executing.
 */
void updateVariables(Task *currTask, int endTime) {
    /* Update CT, TAT, WT, #of tasks */
    currTask->completionTime = endTime;
    int currTurnAroundTime = currTask->completionTime - ARRIVAL_TIME;
    int currWaitTime = currTurnAroundTime - currTask->originalBurstTime;
    totalTurnAroundTime += currTurnAroundTime;
    totalWaitTime += currWaitTime;
    numOfTasks++;
}




