# AUbatch-A-Multithreaded-Batch-Scheduling-System
## Introduction
The aim of this project was to design and implement a batch scheduling system called AUbatch, using the C programming language and the Pthread library. The system is made up of two threads, the scheduling thread, and the dispatching thread, which work together to enforce scheduling policies and execute submitted jobs. Synchronization between the two threads was implemented using condition variables and mutexes to solve critical section problems. Three scheduling policies were implemented: First Come, First Served (FCFS), Shortest Job First (SJF), and Priority- based scheduling. The performance of these policies was compared under various workload conditions to determine their effectiveness. This report provides an overview of the project, including the design and implementation of AUbatch, the synchronization of the threads, and the comparison of the scheduling policies.

## Objectives
1. To design a batch scheduling system
2. To evaluate three scheduling policies/algorithms
3. To implement a scheduler where two threads are synchronized
4. To learn POSIX Threads Programming
5. To use the pthread ligrary and execv()functions
6. To study and apply the condition variables using the Pthread library 7. To address synchronization issues in the scheduler
8. To develop micro batch-job benchmarks
9. To design of performance metrics
10. To assess various workload conditions

## Design Implementation
The implementation of AUbatch relies on a custom doubly linked list class to store the jobs to be executed. This list is designed for easy insertion and removal of nodes, as well as traversal in both directions.
AUbatch uses two threads: the command line parser and the executor. The command line parser collects input from the terminal and sends it to a command dispatcher. The dispatcher validates the command and passes it to various command functions, which can add jobs to the list, display current jobs, update the scheduling policy, or perform
1
automated job creation for benchmark testing. These command functions access the doubly linked list through the scheduler to modify the job list.
The executor thread is responsible for searching the doubly linked list for jobs and executing them sequentially. Completed or failed jobs are removed from the list and added to a finished job list, which tracks both successful and failed jobs. Both the job list and the finished job list are passed as pointers of pointers to the threads, allowing for changes to the queue and communication between the executor and the command line parser. Mutexes manage any changes in these lists to ensure proper synchronization between the threads.
A performance metrics class is implemented to calculate various performance metrics for completed jobs. The finished job list is passed to this function, and several metrics are evaluated.
In summary, the design implementation uses the doubly linked list and the finished job list for efficient job manage- ment and performance metric tracking. Synchronization between threads is maintained through mutexes, ensuring proper communication and data access.

## Data Flow Diagram
![image](https://user-images.githubusercontent.com/80439017/229162396-ced2b2a5-dbe4-4ea1-839c-1767da64dcf6.png)
