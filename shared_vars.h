#ifndef SHARED_VARS_H
#define SHARED_VARS_H

#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include "DLL.h"

#pragma once

extern pthread_mutex_t job_queue_lock;  /* Lock for critical sections */
extern pthread_cond_t job_queue_not_empty; /* Condition variable for waiting on job queue */
extern pthread_mutex_t change_job_queue_lock;

#endif /* SHARED_VARS_H */
