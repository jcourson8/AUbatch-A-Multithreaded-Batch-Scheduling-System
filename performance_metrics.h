#include "DLL.h"
#include "shared_vars.h"
#include "colors.h"

// const char* RED_COLOR = "\033[0;31m";
// const char* RESET_COLOR = "\033[0m";

void get_performance_metrics(DLL *job_queue_finished, bool display, double * wait_time);
double diff_time(struct timeval end_time, struct timeval start_time);