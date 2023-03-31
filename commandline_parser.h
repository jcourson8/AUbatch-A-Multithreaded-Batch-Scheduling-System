#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include "shared_vars.h"
#include "scheduler.h"
#include "performance_metrics.h"
#include "colors.h"
#include <unistd.h>


void menu_execute(char *line, int isargs);
int cmd_run(int nargs, char **args);
int cmd_quit(int nargs, char **args);
int cmd_list(int nargs, char **args);
int cmd_fcfs(int nargs, char **args);
int cmd_sjf(int nargs, char **args);
int cmd_priority(int nargs, char **args);
int cmd_test(int nargs, char **args);
void showmenu(const char *name, const char *x[]);
int cmd_helpmenu(int n, char **a);
int cmd_enter(int n, char **a);
int cmd_dispatch(char *cmd);
int start_commandline_parser(DLL **job_queue_finished, DLL **job_queue);
void * commandline_parser_benchmark(void * benchmark);

int validate_int(int * savetoint, char * input);
