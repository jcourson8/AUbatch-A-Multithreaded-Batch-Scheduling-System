
# Define compiler and compiler flags
CC = gcc
CFLAGS = -Wall --std=c11 -g -D_POSIX_C_SOURCE=200809L #-Werror

# Define source file and object file names
SRCS = aubatch.c commandline_parser.c scheduler.c DLL.c dispatcher.c performance_metrics.c colors.c
OBJS = $(SRCS:.c=.o)

# Define library names and paths
LIBS = -lpthread
LIB_PATHS = 

# Define header file paths
INC_PATHS = -I.

# Define the name of the executable
TARGET = aubatch

PROCESS_TARGET = process

# Define the build rule for the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_PATHS) $(LIBS) -o $(TARGET)

# Define the build rule for object files
%.o: %.c
	$(CC) $(CFLAGS) $(INC_PATHS) -c $< -o $@

# Define dependencies
commandline_parser.o: commandline_parser.h shared_vars.h scheduler.h performance_metrics.h colors.h
colors.o: colors.h
scheduler.o: scheduler.h DLL.h
DLL.o: DLL.h
dispatcher.o: DLL.h shared_vars.h
performance.o: performance_metrics.h DLL.h

process:
	$(CC) $(CFLAGS) $(INC_PATHS) process.c -o process

# Define clean rule
clean:
	rm -f $(OBJS) $(TARGET) $(PROCESS_TARGET)

all: $(TARGET) $(PROCESS_TARGET)
