#include "DLL.h"
#include "shared_vars.h"
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>


job *create_job(char * namein, int cpu_time, int priority);
void add_job(job *newjob, int policy, DLL *job_queue);
int jobs_waiting(DLL *job_queue);
int new_policy(int policy, DLL *job_queue);
void list_jobs(DLL *job_queue);
int job_lock();
int job_unlock();
