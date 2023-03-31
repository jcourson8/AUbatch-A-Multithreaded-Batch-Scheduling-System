#include "performance_metrics.h"

// will take in a DLL, specifically the and print out the performance metrics
// for any "Fail" jobs it will skip in the calculations


// we will calculate avg. response time, throughput, avg. turnaround time, and avg. wait time, Maximum Response Time, Minimum Response Time

void get_performance_metrics(DLL *job_queue, bool display, double * wait_time) {
    DLL job_queue_finished = *job_queue;
    // loop through the DLL and calculate the performance metrics
    // for each job in the DLL
    // if the job is a "Fail" job, skip it

    int valid_jobs_seen = 0;
    int any_jobs_seen = 0;

    double total_response_time = 0;
    double avg_response_time = 0;
    double total_turnaround_time = 0;
    double avg_turnaround_time = 0;
    double total_cpu_time = 0;
    double ave_cpu_time = 0;
    double total_wait_time = 0;
    double avg_wait_time = 0;


    double max_response_time = 0;
    double min_response_time = 0;
    double throughput = 0;
    node * current;
    node * last_valid_node;
    node * first_valid_node;
    bool is_one_valid_yet = false;

    // if the DLL is empty, then there are no jobs to calculate
    if (DLL_isempty(&job_queue_finished)) {
        if (display) {
            printf("No jobs have been run yet.\n");
        }
        *wait_time = 0;
        return;
    }

    // current = DLL_gethead(&job_queue_finished);
    for (current = DLL_gethead(&job_queue_finished); current != NULL; ) {

        any_jobs_seen++;

        // if the job is a "Fail" job, determine if it is the last job in the DLL 
        // if it is the last job in the DLL, then we are done
        // if it is not the last job in the DLL, then we need to move on to the next job
        if (strcmp(current->thisjob->PROGRESS, "Fail") == 0) {
            if (current->next != NULL) {
                current = current->next;
                continue;
            } else {
                break;
            }
    
        }  
        
        // for throughput calculations
        if (is_one_valid_yet == false) {
            is_one_valid_yet = true;
            first_valid_node = current;
        }

        // for throughput calculations
        last_valid_node = current;

        valid_jobs_seen++; // for rolling avg. calculations

        // get the start time, arrival time, and finish time
        struct timeval start_time = current->thisjob->START_TIME;
        struct timeval arrival_time = current->thisjob->ARRIVAL_TIME;
        struct timeval finish_time = current->thisjob->FINISH_TIME;
        
        /* calculations for response time */
        double response_time = diff_time(start_time, arrival_time);
        total_response_time += response_time;
        avg_response_time = total_response_time / valid_jobs_seen;
        /* end calculations for response time */

        /* calculations for turnaround time */
        double turnaround_time = diff_time(finish_time, arrival_time);
        total_turnaround_time += turnaround_time;
        avg_turnaround_time = total_turnaround_time / valid_jobs_seen;
        /* end calculations for turnaround time */

        /* calculations for average cpu time */
        double cpu_time = current->thisjob->CPU_TIME;
        total_cpu_time += cpu_time;
        ave_cpu_time = total_cpu_time / valid_jobs_seen;
        /* end calculations for average cpu time */

        /* calculations for average wait time */
        double wait_time = turnaround_time - cpu_time;
        total_wait_time += wait_time;
        avg_wait_time = total_wait_time / valid_jobs_seen;
        /* end calculations for average wait time */


        /* calculations for max response time */
        if (response_time > max_response_time) {
            max_response_time = response_time;
        }
        /* end calculations for max response time */

        /* calculations for min response time */
        if (response_time < min_response_time) {
            min_response_time = response_time;
        }
        /* end calculations for min response time */
        
        if (current->next != NULL) {
            current = current->next;
            continue;
        } else {
            break;
        }
    }
    *wait_time = avg_wait_time;
    /* calculations for throughput */
    // if the DLL is only one job long, we will just use the CP
    
    // if the DLL is at least two jobs long, we will use the difference between the first and last jobs
    if (valid_jobs_seen == 0) {
        throughput = 0;
    } else if (valid_jobs_seen > 1) {
        struct timeval first_job_arrival_time = last_valid_node->thisjob->ARRIVAL_TIME;
        struct timeval last_job_finish_time = first_valid_node->thisjob->FINISH_TIME;
        double time_elapsed = diff_time(last_job_finish_time, first_job_arrival_time);
        throughput = valid_jobs_seen / time_elapsed;
    } else {
        // throughput = valid_jobs_seen / current->thisjob->CPU_TIME;
        throughput = avg_turnaround_time;
    }

    if (display == true) {
        printf("%sPerformance Metrics:%s\n", RED_COLOR, RESET_COLOR);
        printf("Total number of job submitted:      %s%d%s\n", RED_COLOR,any_jobs_seen, RESET_COLOR);
        printf("Total number of jobs completed:     %s%d%s\n", RED_COLOR,valid_jobs_seen, RESET_COLOR);
        printf("Average Turnaround Time:            %s%.04f%s seconds\n", RED_COLOR, avg_turnaround_time, RESET_COLOR);
        printf("Average CPU Time:                   %s%.04f%s seconds\n", RED_COLOR, ave_cpu_time, RESET_COLOR);
        printf("Average wait time:                  %s%.04f%s seconds\n", RED_COLOR, avg_wait_time, RESET_COLOR);
        printf("Average Response Time:              %s%.04f%s seconds\n", RED_COLOR, avg_response_time, RESET_COLOR);
        printf("Maximum Response Time:              %s%.04f%s seconds\n", RED_COLOR, max_response_time, RESET_COLOR);
        printf("Minimum Response Time:              %s%.04f%s seconds\n", RED_COLOR, min_response_time, RESET_COLOR);
        printf("Throughput:                         %s%.04f%s jobs/second\n", RED_COLOR, throughput, RESET_COLOR);

        printf("Goodbye!\n");
    }
}

// helper function to calculate the difference between two times
double diff_time(struct timeval end_time, struct timeval start_time) {
    struct timeval diff_time = {0};
    double diff_time_seconds = 0;

    // correctly calculate the difference between the two times using the .tv_sec and .tv_usec fields. make sure to account for carry over
    diff_time.tv_sec = end_time.tv_sec - start_time.tv_sec;
    diff_time.tv_usec = end_time.tv_usec - start_time.tv_usec;
    diff_time_seconds = diff_time.tv_sec + (diff_time.tv_usec / 1000000.0);

    return diff_time_seconds;
}
