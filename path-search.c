// $PATH search and External Command Execution
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include "functionality.h"

void pathsearch(char *cmd, bool background) {
    // get PATH string and split at colons
    char *path = getenv("PATH");
    char *tmp = NULL;
    tmp = strdup(path);
    char splitpath[200][100];
    char *pt;
    int i = 0;
    pt = strtok(tmp, ":");
    while (pt != NULL) {
        strcpy(splitpath[i], pt);
        i++;
        pt = strtok(NULL, ":");
    }
    // search for matching program in all paths
    for (int j = 0; j < 200; j++) {
        char *p = splitpath[j];
        strcat(p, "/");
        strcat(p, cmd);
        if ((access(p, F_OK) == 0) && background == false) {
            // run command with waiting
            pid_t pr;
            int status;
            pr = fork();
            if (pr == -1) {
                printf("fork error");
            }
            if (pr == 0) {
                char *args[] = {cmd, NULL};
                execv(p, args);
            } else {
                break;
            }
            waitpid(pr,&status,0); 
        } else if ((access(p, F_OK) == 0) && background == true) {
            int i = 1;
            pid_t pr;
            pr = fork();
            if (pr == -1) {
                printf("fork error");
            }
            if (pr == 0) {
                background_process(pr,cmd);
                setpgid(0,0);
                char *args[] = {cmd, NULL};
                printf("[%d] %d\n", i, pr);
                execv(p, args);
            } else {
                background_finished(pr,cmd);
                sleep(1);
                printf("[%d]+ [%s]\n",i,bg_queue[count]->running);
            }
            
        }
        
    }
}