#include "scheduler.h"

/*
 *   Creates a new job with the given parameters`
 */
job *create_job(char * namein, int cpu_time, int priority) {

    struct job *newjob = (struct job*)malloc(sizeof(struct job));
    struct timeval arrival_time; 
    gettimeofday(&arrival_time, NULL);

    newjob->NAME = namein;
    newjob->CPU_TIME = cpu_time;
    newjob->PRI = priority;
    newjob->ARRIVAL_TIME = arrival_time;
    newjob->PROGRESS = "     ";

    return newjob;
}

/*
 *   Adds a job to the job queue
 */
void add_job(job *newjob, int policy, DLL *job_queue) {
    if(policy == FCFS) {
        DLL_insertfirst(job_queue, newjob);
    } else { // SJF or PRIORITY
        DLL_insertfirst(job_queue, newjob);
        DLL_sortlist(job_queue, policy);
    }
}

/*
 *   Returns the number of jobs waiting in the job queue
 */
int jobs_waiting(DLL *job_queue) {
    return DLL_waitingjobs(job_queue);
}

/*
 *  Switches the scheduling policy and sorts the job queue
 */
int new_policy(int policy, DLL *job_queue) {
    char * poly;
    switch (policy) {
        case 0: {
            poly = "FCFS";
            break;
        }
        case 1: {
            poly = "SJF";
            break;
        }
        case 2: {
            poly = "PRIORITY";
            break;
        }
    }
    printf("Scheduling policy is switched to %s. ", poly);
    
    DLL_sortlist(job_queue, policy);
    return 0;
}

/*
 *   Lists the jobs in the job queue
 */
void list_jobs(DLL *job_queue) {
    DLL_display(job_queue);
}

// wrapper functions for the job queue lock
int job_lock() {
    return pthread_mutex_lock(&job_queue_lock);
}

// wrapper functions for the job queue unlock
int job_unlock() {
    return pthread_mutex_unlock(&job_queue_lock);
}

