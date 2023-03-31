#include "dispatcher.h"

/*
 *   executes a job by forking a child process
 */
void execute_job(job * job_to_exec, DLL *job_queue_finished) {


    // gets the job name and cpu time
    char * jobname = job_to_exec->NAME;
    int cpu_time = job_to_exec->CPU_TIME;
    char cpu_time_arg[30];
    sprintf(cpu_time_arg, "%d", cpu_time);

    // creates the argument array for execv
    char * arr[] = {jobname, cpu_time_arg, NULL};

    // modify the job's start time
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    job_to_exec->START_TIME = start_time;

    pthread_mutex_unlock(&job_queue_lock);
    
    pid_t pid;
    switch ((pid = fork()))
    {
      case -1: {
        /* Fork() has failed */
        perror("fork failed");
        break;
      }
      case 0: {
        /* This is processed by the child */
        if (access(arr[0], F_OK) == 0) {
          // file exists
          execv(arr[0], arr);
        } else {
          // file doesn't exist
          printf("File \"%s\" does not exist... (Press Enter to continue)", arr[0]);
          exit(-1);
        }
        puts("\n execv() failed: Check batch program name");
        exit(-1);
        break;
      }

      default: {
        /* This is processed by the parent */
        // parent process
        int status;
        wait(&status);
        
        pthread_mutex_lock(&job_queue_lock);

        if (WEXITSTATUS(status) != 0)  {
          // job failed
          struct timeval finish_time; 
          gettimeofday(&finish_time, NULL);
          job_to_exec->FINISH_TIME = finish_time;

          job_to_exec->PROGRESS = "Fail";
          
          // add the failed job to the finished queue
          DLL_insertfirst(job_queue_finished, job_to_exec);
          pthread_mutex_unlock(&job_queue_lock);
          break;
        }

        // job succeeded
        // modify the job's finish time
        struct timeval finish_time; 
        gettimeofday(&finish_time, NULL);
        job_to_exec->FINISH_TIME = finish_time; 
        job_to_exec->PROGRESS = "Done";

        // add the finished job to the finished queue
        DLL_insertfirst(job_queue_finished, job_to_exec);
        pthread_mutex_unlock(&job_queue_lock);
      }
    }
}