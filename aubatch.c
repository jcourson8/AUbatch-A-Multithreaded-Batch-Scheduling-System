/*
 * COMP7500/7506
 * Project 3: AUbatch - A Batch Scheduling System
 *
 * James Courson
 * Auburn University
 * Mar. 17, 2023. Version 1.0
 *
 * This source code demonstrates the development of
 * a batch-job scheduler using pthread.
 *
 */


#include "aubatch.h"
#include "DLL.h"
#include <pthread.h>

// initialize the mutex and condition variable
pthread_mutex_t job_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t change_job_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_queue_not_empty = PTHREAD_COND_INITIALIZER;

typedef unsigned int u_int;

// this tells the executor thread to quit
bool should_quit = false;

// the queues of jobs both waiting and finished
DLL job_queue;
DLL job_queue_finished;

// pointers to the queues so the threads can modify them
DLL *job_queue_ptr = &job_queue;
DLL *job_queue_finished_ptr = &job_queue_finished;


int main() {
  pthread_t command_thread, executor_thread; /* Two concurrent threads */

  if ( pthread_mutex_init( &job_queue_lock, NULL) != 0 )
    perror( "mutex init failed\n" );
  if ( pthread_cond_init( &job_queue_not_empty, NULL) != 0 )
    perror( "pthread cond failed\n" );

  // initialize the Doubly Linked Lists
  DLL_ctor(&job_queue);
  DLL_ctor(&job_queue_finished);

  // start the threads
  int ret1 = pthread_create(&command_thread, NULL, &commandline_start, NULL);
  int ret2 = pthread_create(&executor_thread, NULL, &executor_start, NULL);
  if (ret1 != 0) {
        perror("Error creating command thread");
        exit(1);
  }
  if (ret2 != 0) {
        perror("Error creating executor thread");
        exit(1);
  }

  // wait for the threads to finish
  pthread_join(command_thread, NULL);

  // destroy the mutex and condition variable
  pthread_mutex_destroy(&job_queue_lock);
  pthread_cond_destroy(&job_queue_not_empty);

  exit(0);
}

/*
 * This function is the thread that parses the command line
 * and adds jobs to the job queue
 */
void *commandline_start(void *ptr) {
  start_commandline_parser(&job_queue_finished_ptr, &job_queue_ptr);
  should_quit = true;
  pthread_exit(NULL);
}

/*
 * This function is the thread that executes the jobs
 */
void *executor_start(void *ptr) {
  while (!should_quit) {

    pthread_mutex_lock(&job_queue_lock);
    pthread_mutex_lock(&change_job_queue_lock);

    // if the job queue is empty, unlock the mutex and continue
    if (DLL_length(job_queue_ptr) == 0) {
      pthread_mutex_unlock(&job_queue_lock);
      pthread_mutex_unlock(&change_job_queue_lock);
      pthread_cond_wait(&job_queue_not_empty, &job_queue_lock);
      pthread_mutex_lock(&change_job_queue_lock);
    } 

    
    // Here the job queue is not empty, get the tail of the queue
    // if the tail is NULL, unlock the mutex and continue
    node *node_to_exec = DLL_gettail(job_queue_ptr);
    if (node_to_exec == NULL) {
      pthread_mutex_unlock(&job_queue_lock);
      pthread_mutex_unlock(&change_job_queue_lock);
      continue;
    }

    // Here the tail is not NULL, get the job from the tail
    // if the job progress is "Run", unlock the mutex and continue
    // this is to prevent the same job from being executed twice
    job *job_to_exec = node_to_exec->thisjob;
    if (strcmp(job_to_exec->PROGRESS, "Run") == 0) {
      pthread_mutex_unlock(&job_queue_lock);
      pthread_mutex_unlock(&change_job_queue_lock);
      continue;
    } 

    // Here the job progress is not "Run", check if the job is done or failed 
    // if the job is done or failed, remove it from the queue and unlock the mutex
    if (strcmp(job_to_exec->PROGRESS, "Done") == 0 || strcmp(job_to_exec->PROGRESS, "Fail") == 0) {
      DLL_removedone(job_queue_ptr);
      pthread_mutex_unlock(&job_queue_lock);
      pthread_mutex_unlock(&change_job_queue_lock);
      continue;
    }

    job_to_exec->PROGRESS = "Run";
    execute_job(job_to_exec, job_queue_finished_ptr); /* in dispatcher.c */
    pthread_mutex_unlock(&change_job_queue_lock);
  }

  pthread_exit(NULL);
}
