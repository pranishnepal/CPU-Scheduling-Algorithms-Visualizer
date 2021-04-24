/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    /* Extra field added for preemptive algorithms: */
    int burstRemaining;
    int completionTime;
    int firstStartTime;
    int originalBurstTime;  //remembers the original burst time
} Task;

#endif
