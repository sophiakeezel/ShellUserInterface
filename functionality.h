#ifndef FUNCTIONALITY_H_
#define FUNCTIONALITY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    pid_t process_id; // hold the process id for each bg process
    char * running; // holds the command being run
} process;

process * bg_queue[10];
int count = 0;

void background_process(pid_t pid, char *command);
void background_finished(pid_t pid, char *command);

#endif
