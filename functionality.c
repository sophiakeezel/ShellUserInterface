
#include "functionality.h"

void background_process(pid_t pid, char *command) {
    bg_queue[count]->process_id = pid;
    bg_queue[count]->running = command;
    count++;
}

void background_finished(pid_t pid, char *command) {
    bg_queue[count]->process_id = -1;
    bg_queue[count]->running = command;
    count--;
}