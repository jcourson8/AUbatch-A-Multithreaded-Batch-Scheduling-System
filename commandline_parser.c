/*
 * COMP7500/7506
 * Project 3: commandline_parser.c
 *
 * This sample source code demonstrates how to:
 * (1) separate policies from a mechanism
 * (2) parse a commandline using getline() and strtok_r()
 *
 * The sample code was derived from menu.c in the OS/161 project
 *
 * Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 *
 * Compilation Instruction:
 * gcc commandline_parser.c -o commandline_parser
 * ./commandline_parser
 *
 */
#include "commandline_parser.h"

#define MAXMENUARGS 8
#define MAXCMDLINE 64

int POLICY = FCFS;
DLL *job_queue_finished_local;
DLL *job_queue_local;

DLL **job_queue_finished_ptr_ptr;
DLL **job_queue_ptr_ptr;




/*
 * The run command - submit a job.
 */
int cmd_run(int nargs, char **args) {

  // check for correct number of arguments
	if (nargs != 4) {
		printf("Usage: run <job> <time> <priority>\n");
		return EINVAL;
	}
  
  // create space for the name of the job
  char * name = NULL;
  int str_len = strlen(args[1]);
  name = (char*) malloc(sizeof(char) * (str_len + 1));

  if (name == NULL) {
      perror("Memory allocation failed!\n");
      return 1;
  }
  // copy the name of the job
  strcpy(name, args[1]);

  // validate number input
  int num1;
  int num2;
  if (validate_int(&num1, args[2]) != 0) {
    return 0;
  }
  if (validate_int(&num2, args[3]) != 0) {
    return 0;
  }

  // print job name
  printf("Job %s was submitted.\n",name);
  // print expected waiting time
  double wait_time = 0;
  get_performance_metrics(job_queue_finished_local, false, &wait_time);
  printf("Expected waiting time: %.03f\n", wait_time);

  // print policy
  if (POLICY == FCFS){
    printf("Scheduling Policy: %s\n","FCFS");
  } else if (POLICY == SJF) {
    printf("Scheduling Policy: %s\n","SJF");
  } else if (POLICY == PRIORITY) {
    printf("Scheduling Policy: %s\n","PRIORITY");
  }

  // remove trailing spaces
  int i;
  for (i = strlen(name) - 1; i >= 0 && name[i] == ' '; i--) { 
      name[i] = '\0';
  }
  struct job * newjob = create_job(name, num1, num2);


  if (job_lock() != 0) perror("Error with lock!");
  // critical section
  add_job(newjob, POLICY, job_queue_local);
  // end critical section
  if (job_unlock() != 0) perror("Error with unlock!");

  // signal the dispatcher that there is a new job
  pthread_cond_signal(&job_queue_not_empty);

	/* Use execv to run the submitted job in AUbatch */
	// printf("use execv to run the job in AUbatch.\n");
	return 0; /* if succeed */
}

int validate_int(int * savetoint, char * input) {
  // Check if input is exactly 6 characters long (5 digits + newline character)
    if (strlen(input) >= 10) {
        printf("Invalid input: Input should be less than 6 digits\n");
        return 1;
    }

    // Check if each character is a digit
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] < '0' || input[i] > '9') {
            printf("Invalid input: Input should contain only digits\n");
            return 1;
        }
    }

    // At this point, the input is a valid 5-digit decimal number
    *savetoint = atoi(input);
    return 0;
}

/*
 * The quit command.
 */
int cmd_quit(int nargs, char **args) {
  double wait_time = 0;
	get_performance_metrics(job_queue_finished_local, true, &wait_time);
  return -1;
}

/*
 * The list command.
 */
int cmd_list(int nargs, char **args) {
  if (job_lock() != 0) perror("Error with lock!");
  // critical section
  list_jobs(job_queue_local);
  if (job_unlock() != 0) perror("Error with unlock!");

  return 0;
}

/*
 * The fcfs command.
 */
int cmd_fcfs(int nargs, char **args) {
	
	// TODO: Need to run function that will actually change the policy and confirm
  if (POLICY == FCFS) {
    printf("Scheduling policy is already FCFS. \n");
    return 0;
  }

  // printf("Scheduling policy is switched to FCFS. ");
  POLICY = FCFS;

  if (job_lock() != 0) perror("Error with lock!");

  if (new_policy(POLICY, job_queue_local) != 0) perror("failed!!!");
	int waiting_jobs = jobs_waiting(job_queue_local); // need to 

  if (job_unlock() != 0) perror("Error with unlock!");

	printf("All the %d waiting jobs have been rescheduled.\n",waiting_jobs);

  return 0;
}

/*
 * The sjf command.
 */
int cmd_sjf(int nargs, char **args) { 
	// TODO: Need to run function that will actually change the policy and confirm
  if (POLICY == SJF) {
    printf("Scheduling policy is already SJF. \n");
    return 0;
  }

  POLICY = SJF;

  if (job_lock() != 0) perror("Error with lock!");

  if (new_policy(POLICY, job_queue_local) != 0) perror("failed!!!");
  int waiting_jobs = jobs_waiting(job_queue_local); // need to 

  if (job_unlock() != 0) perror("Error with unlock!");

	printf("All the %d waiting jobs have been rescheduled.\n",waiting_jobs);


  return 0;
}

/*
 * The priority command.
 */
int cmd_priority(int nargs, char **args) { 
	// TODO: Need to run function that will actually change the policy and confirm
  if (POLICY == PRIORITY) {
    printf("Scheduling policy is already PRIORITY. \n");
    return 0;
  }

  POLICY = PRIORITY;

  if (job_lock() != 0) perror("Error with lock!");

  if (new_policy(POLICY, job_queue_local) != 0) perror("failed!!!");
  int waiting_jobs = jobs_waiting(job_queue_local); // need to 

  if (job_unlock() != 0) perror("Error with unlock!");

	printf("All the %d waiting jobs have been rescheduled.\n",waiting_jobs);

  return 0;
}

/*
 * The test command.
 */
int cmd_test(int n, char **a) { 
  // if its not one check if there is a tack to start it, and then check for a valid item to return help for
  // run, list, fcfs, sjf, priority, test, quit

  if (n != 8) {
    printf("test <benchmark> <policy> <num_of_jobs> <arrival_rate> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
    return 1;
  }

  // check if the policy is valid
  if (strcmp(a[2], "fcfs") != 0 && strcmp(a[2], "sjf") != 0 && strcmp(a[2], "priority") != 0) {
    printf("Invalid policy. Valid policies are fcfs, sjf, and priority.\n");
    return 1;
  } else {
    //valid policy - save it
    if (strcmp(a[2], "fcfs") == 0) {
      POLICY = FCFS;
    } else if (strcmp(a[2], "sjf") == 0) {
      POLICY = SJF;
    } else {
      POLICY = PRIORITY;
    }
  }
  // validate the number of jobs- must be less than 20
  int num_of_jobs;
  if (validate_int(&num_of_jobs, a[3]) != 0) {
    printf("Invalid number for <num_of_jobs>. Try again.\n");
    return 1;
  } else if (num_of_jobs > 20) {
    printf("Invalid number of jobs. Number of jobs must be less than 30. Try again.\n");
    return 1;
  }

  // validate the arrival rate - must be less than 3 more at least 0
  int arrival_rate;
  if (validate_int(&arrival_rate, a[4]) != 0) {
    printf("Invalid number for <arrival_rate>. Try again.\n");
    return 1;
  } else if (arrival_rate > 3 || arrival_rate < 0) {
    printf("Invalid arrival rate. Arrival rate must be between 0 and 3. Try again.\n");
    return 1;
  }

  // validate the number of priority levels - must be less than 100
  int priority_levels;
  if (validate_int(&priority_levels, a[5]) != 0) {
    printf("Invalid number for <priority_levels>. Try again.\n");
    return 1;
  } else if (priority_levels > 100) {
    printf("Invalid number of priority levels. Number of priority levels must be less than 100. Try again.\n");
    return 1;
  }

  // validate the min CPU time - must more than 1, but only a digit
  int min_CPU_time;
  if (validate_int(&min_CPU_time, a[6]) != 0) {
    printf("Invalid number for <min_CPU_time>. Try again.\n");
    return 1;
  } else if (min_CPU_time < 1) {
    printf("Invalid number for <min_CPU_time>. Must be greater than 1. Try again.\n");
    return 1;
  }

  // validate the max CPU time - must be less than 15
  int max_CPU_time;
  if (validate_int(&max_CPU_time, a[7]) != 0) {
    printf("Invalid number for <max_CPU_time>. Try again.\n");
    return 1;
  } else if (max_CPU_time > 15) {
    printf("Invalid number for <max_CPU_time>. Must be less than 15. Try again.\n");
    return 1;
  }

  // for the number of jobs, create a job and add it to the queue
  // for the job name use "process"
  // for the priority, use a random number between 1 and priority_levels
  // for the CPU time, use a random number between min_CPU_time and max_CPU_time

  // keep a copy pointer to the local job queue
  // point the local job queue to a new queue
  DLL * job_queue_localcp = job_queue_local;
  DLL benchmark_queue;
  DLL_ctor(&benchmark_queue);
  DLL *benchmark_queue_ptr = &benchmark_queue;
  
  // keep a copy pointer to the local finished job queue
  // point the local finished job queue to a new queue
  DLL * job_queue_finished_localcp = job_queue_finished_local;
  DLL benchmark_queue_finished;
  DLL_ctor(&benchmark_queue_finished);
  DLL * benchmark_queue_finished_ptr = &benchmark_queue_finished;

  // change the pointers in the main function to point to the new queue
  printf("Waiting of current running job to finish... \n");
  printf("%s---START OF \"%s\" BENCHMARK---%s\n", RED_COLOR, a[1], RESET_COLOR);

  // change the pointers in the main function to point to the new queue
  pthread_mutex_lock(&change_job_queue_lock);
  *job_queue_ptr_ptr = benchmark_queue_ptr;
  *job_queue_finished_ptr_ptr = benchmark_queue_finished_ptr;
  pthread_mutex_unlock(&change_job_queue_lock);

  // change the local pointers to point to the new queue
  job_queue_local = &benchmark_queue;
  job_queue_finished_local = &benchmark_queue_finished;

  // create a thread to run the benchmark
  int i;
  pthread_t commandline_parser_benchmark_thread;
  int result = pthread_create(&commandline_parser_benchmark_thread, NULL, &commandline_parser_benchmark, (void*)a[1]);

  if (result != 0) {
        perror("Error creating inner thread");
        exit(1);
  }

  // create the jobs and add them to the queue
  for (i = 0; i < num_of_jobs; i++) {
    char * job_name = "process";
    int priority = rand() % priority_levels + 1;
    int CPU_time = rand() % (max_CPU_time - min_CPU_time + 1) + min_CPU_time;
    job * newjob = create_job(job_name, CPU_time, priority);

    if (job_lock() != 0) perror("Error with lock!");
    add_job(newjob, POLICY, job_queue_local);
    if (job_unlock() != 0) perror("Error with unlock!");
    pthread_cond_signal(&job_queue_not_empty);
    sleep(arrival_rate);
  }

  // wait for the benchmark thread to finish
  pthread_join(commandline_parser_benchmark_thread, NULL);
  
  // change the local pointers to point to the old queue before the benchmark
  job_queue_local = job_queue_localcp;
  job_queue_finished_local = job_queue_finished_localcp;
  
  // change the pointers in the main function to point to the old queue before the benchmark
  pthread_mutex_lock(&change_job_queue_lock);
  *job_queue_ptr_ptr = job_queue_localcp;
  *job_queue_finished_ptr_ptr = job_queue_finished_localcp;
  pthread_mutex_unlock(&change_job_queue_lock);

  printf("%s----END OF \"%s\" BENCHMARK----%s\n\n", RED_COLOR,a[1], RESET_COLOR);


  return 0;
}

/*
 * Display menu information
 */
void showmenu(const char *name, const char *x[]) {
	int line;
	printf("%s\n", name);
	for (line = 0; x[line] != NULL; line++) {
		printf("%s",x[line]);
	}
}

static const char entrymessage[] =
    "Welcome to James' batch job scheduler Version 1.0\n"
    "Type ‘help’ to find more about AUbatch commands.\n";


static const char *helpmenu[] = {
    "\033[0;31m[run]\033[0m <job> <time> <priority>: submit a job named <job>,\n",
    "                               execution time is <time>,\n",
    "                               priority is <pri>.\n",
    "\033[0;31m[list]\033[0m display the job status.\n",
    "\033[0;31m[fcfs]\033[0m change the scheduling policy to FCFS.\n",
    "\033[0;31m[sjf]\033[0m change the scheduling policy to SJF.\n",
    "\033[0;31m[priority]\033[0m change the scheduling policy to priority.\n",
    "\033[0;31m[test]\033[0m <benchmark> <policy> <num_of_jobs> <priority_levels>\n",
    "       <min_CPU_time> <max_CPU_time>\n",
    "\033[0;31m[quit]\033[0m exit AUbatch\n\n",
	NULL
};

int cmd_helpmenu(int n, char **a) {
	(void)n;
	(void)a;

  if (n == 1) {
    showmenu("AUbatch help menu", helpmenu);
	  return 0;
  }
  // if its not one check if there is a tack to start it, and then check for a valid item to return help for
  // run, list, fcfs, sjf, priority, test, quit
  if (n != 2) {
    printf("Not a valid help item.\nUsage: help [ -run | -list | -fcfs | -sjf | -priority | -test | -quit ]\n");
    return 1;
  }
  // n == 1
  char helplist[7][10] = {"-run", "-list", "-fcfs", "-sjf", "-priority", "-test", "-quit"};
  char helplistmessage[7][200] = {"\033[0;31m[run]\033[0m <job> <time> <priority>: submit a job named <job>,\n                               execution time is <time>,\n                               priority is <pri>.\n",
                               "\033[0;31m[list]\033[0m display the job status.\n", 
                               "\033[0;31m[fcfs]\033[0m change the scheduling policy to FCFS.\n", 
                               "\033[0;31m[sjf]\033[0m change the scheduling policy to SJF.\n", 
                               "\033[0;31m[priority]\033[0m change the scheduling policy to priority.\n", 
                               "\033[0;31m[test]\033[0m <benchmark> <policy> <num_of_jobs> <priority_levels>\n       <min_CPU_time> <max_CPU_time>\n", 
                               "\033[0;31m[quit]\033[0m exit AUbatch\n"
                               };
  int i;
  if (a[0][0] != '-') { // must start with '-'
    for(i = 0; i < 7; i++) {
      if (strcmp(a[1],helplist[i]) == 0) {
        printf("%s", helplistmessage[i]);
        return 0;
      }
    }
    printf("Not a valid help item.\nUsage: help [ -run | -list | -fcfs | -sjf | -priority | -test | -quit ]\n");
    return 1;
  }
  return 0;
}

int cmd_enter(int n, char **a) {
	return 0;
}

/*
 *  Command table.
 */
static struct {
	const char *name;
	int (*func)(int nargs, char **args);
} cmdtable[] = {
    /* commands: single command must end with \n */
    {"?", cmd_helpmenu},
    {"h", cmd_helpmenu},
    {"help", cmd_helpmenu},
    {"r", cmd_run},
    // {"run", cmd_run},
    {"run", cmd_run},
    {"q", cmd_quit},
    {"quit", cmd_quit},
    {"fcfs", cmd_fcfs},
	  // {"list", cmd_list},
    {"list", cmd_list},
    {"sjf", cmd_sjf},
    {"priority", cmd_priority},
    {"test", cmd_test},
    {"", cmd_enter},
    /* Please add more operations below. */
    {NULL, NULL}
};

/*
 * Process a single command.
 */
int cmd_dispatch(char *cmd) {
	// time_t beforesecs, aftersecs, secs;
	// u_int32_t beforensecs, afternsecs, nsecs;
	char *args[MAXMENUARGS];
	int nargs = 0;
	char *word;
	char *context;
	int i, result;

  for (word = strtok_r(cmd, " ", &context); word != NULL;
       word = strtok_r(NULL, " ", &context)) {

    if (nargs > MAXMENUARGS) {
      printf("Command line has too many words\n");
      return E2BIG;
    }
    args[nargs++] = word;
  }

  if (nargs == 0) {
    return 0;
  }

  for (i = 0; cmdtable[i].name; i++) {
    if (*cmdtable[i].name && strcmp(args[0], cmdtable[i].name) == 0) {
      assert(cmdtable[i].func != NULL);
      result = cmdtable[i].func(nargs, args);
      return result;
    }
  }

  printf("%s: Command not found\n", args[0]);
  return EINVAL;
}

/*
 * Command line main loop.
 */
int start_commandline_parser(DLL **job_queue_finished, DLL **job_queue) {
  job_queue_ptr_ptr = job_queue;
  job_queue_finished_ptr_ptr = job_queue_finished;

  job_queue_finished_local = *job_queue_finished;
  job_queue_local = *job_queue;


	char *buffer;
	size_t bufsize = 64;
	buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL) {
		perror("Unable to malloc buffer");
		exit(1);
	}

	printf("%s", entrymessage);

	while (1) {
		printf("%s>%s ",RED_COLOR, RESET_COLOR);
    fgets(buffer, bufsize, stdin);

    // removes trailing spaces
    int i;
    for (i = strlen(buffer) - 1; i >= 0 && buffer[i] == ' '; i--) {
        buffer[i] = '\n';
    }

    // Trim trailing newline
    if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }

    // dispatch trimmed command
		if (cmd_dispatch(buffer) == -1) {
      free(buffer);
      return -1;
    }
	}
	return 0;
}

/*
 * Command line benchmark main loop. To be used in a separate thread.
 */
void * commandline_parser_benchmark(void * benchmark) {
  char *benchmark_name = (char *)benchmark;
  char *buffer;
	size_t bufsize = 64;
  buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL) {
		perror("Unable to malloc buffer");
		exit(1);
	}
  printf("You are now in benchmark mode. Please enter a command.\n");
  while (1) {
		printf("%s[%s]>%s ",RED_COLOR, benchmark_name, RESET_COLOR);
    fgets(buffer, bufsize, stdin);

    // removes trailing spaces
    int i;
    for (i = strlen(buffer) - 1; i >= 0 && buffer[i] == ' '; i--) {
        buffer[i] = '\n';
    }

    // Trim trailing newline
    if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }

    // dispatch trimmed command
		if (cmd_dispatch(buffer) == -1) {
      free(buffer);
      return NULL;
    }
      // return -1;
	}
}