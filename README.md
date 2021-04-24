## Scheduling Algorithms

This project was done for EECS3221 class; it involves implementing four process scheduling algorithms:

* First-come
* Shortest-job-first (SJF)
* Priority scheduling
* Round-robin (RR) scheduling
* Priority with round-robin

<b> Note: <i>Priorities range from 1 to 10, where a higher numeric value indicates a higher relative priority. 
For round-robin scheduling, the length of a time `quantum` is `10` milliseconds. </i></b>

## How to run the schedulers?
To build the FCFS scheduler, we would enter:
        
    make fcfs  
and would execute the scheduler (using the schedule of tasks `schedule.txt`) as follows:  
    
    ./fcfs schedule.txt 

<b>Follow the same pattern for other schedulers.</b>

## Example Input and Output:
Consider the following input for the priority scheduling algorithm:

    P0, 9, 35
    P1, 10, 25
    P2, 7, 40
    P3, 6, 20
    P4, 10, 40
    P5, 4, 35
    P6, 10, 20
    P7, 3, 35
    P8, 3, 30
    P9, 4, 30
    P10, 4, 35
    P11, 8, 20
    P12, 4, 20
    P13, 5, 15
    P14, 7, 10
    P15, 1, 25
    P16, 8, 25
    P17, 6, 30
    P18, 2, 40
    P19, 3, 20
    P20, 9, 10
    P21, 7, 5
    P22, 6, 35
    P23, 3, 40
    P24, 9, 30



Output for the input above:

    Running task = [P1] [10] [25] for 25 units
    Running task = [P4] [10] [40] for 40 units
    Running task = [P6] [10] [20] for 20 units
    Running task = [P0] [9] [35] for 35 units
    Running task = [P20] [9] [10] for 10 units
    Running task = [P24] [9] [30] for 30 units
    Running task = [P11] [8] [20] for 20 units
    Running task = [P16] [8] [25] for 25 units
    Running task = [P2] [7] [40] for 40 units
    Running task = [P14] [7] [10] for 10 units
    Running task = [P21] [7] [5] for 5 units
    Running task = [P3] [6] [20] for 20 units
    Running task = [P17] [6] [30] for 30 units
    Running task = [P22] [6] [35] for 35 units
    Running task = [P13] [5] [15] for 15 units
    Running task = [P5] [4] [35] for 35 units
    Running task = [P9] [4] [30] for 30 units
    Running task = [P10] [4] [35] for 35 units
    Running task = [P12] [4] [20] for 20 units
    Running task = [P7] [3] [35] for 35 units
    Running task = [P8] [3] [30] for 30 units
    Running task = [P19] [3] [20] for 20 units
    Running task = [P23] [3] [40] for 40 units
    Running task = [P18] [2] [40] for 40 units
    Running task = [P15] [1] [25] for 25 units

    Average waiting time = 306.40
    Average turnaround time = 333.20
    Average response time = 306.40


<b>** <i>Please do not make any unauthorized use of the project</i> **</b>
