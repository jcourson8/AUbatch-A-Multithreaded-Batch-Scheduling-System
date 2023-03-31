#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "DLL.h"
#include "shared_vars.h"

void execute_job(job * job_to_exec, DLL *job_queue_finished);
