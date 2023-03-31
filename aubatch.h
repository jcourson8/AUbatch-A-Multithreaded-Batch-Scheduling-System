#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "commandline_parser.h"
#include "shared_vars.h"
#include "dispatcher.h"


void *commandline_start(void *ptr);
void *executor_start(void *ptr);
