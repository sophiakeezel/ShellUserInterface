#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "path-search.c"
#include "functionality.h"

typedef struct {
    int size;
    char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);
tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);
void tilde_expansion(tokenlist *tokens, int i);
void env_variables(tokenlist *tokens, int i);
bool is_a_builtin(tokenlist *tokens, int count, process * bg_queue[]);
void echo_cmd(tokenlist* tokens);
void cd_cmd(char * dir);
void exit_cmd(int count, process * bg_queue[]);
void jobs_cmd(process * bg_queue[]);
void Piping();

int main() {

    // initialize background
    bool bg = false;
    bg_queue[10] = (process*)malloc(10 * sizeof(process));

    while (1) {
        
    // print prompt 
    printf("%s@%s:%s>", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

    /* input contains the whole command
    * tokens contains substrings from input split by spaces
    */
    char *input = get_input();
    //printf("whole input: %s\n", input);
    tokenlist *tokens = get_tokens(input);

    // loop through tokens
    for (int i = 0; i < tokens->size; i++) {
        // if token is a tilde
        if (*(tokens->items[i]) == '~') { tilde_expansion(tokens,i);}
        // if token is an env variable
        if (*(tokens->items[i]) == '$') { env_variables(tokens,i);}
        // if tokens contain a pipe |
        if (*(tokens->items[i]) == '|') { Piping();}
    }
    // check if token is a built-in command
    is_a_builtin(tokens, count, bg_queue);
    // if last token is an ampersand
    if (*(tokens->items[tokens->size-1]) == '&') {bg = true;}

    // conduct path search
    if (!(is_a_builtin(tokens, count, bg_queue))) {
        // conduct path search
        pathsearch(tokens->items[0], bg);
    }

    free(input);
    free_tokens(tokens);
}

return 0;
}

tokenlist *new_tokenlist(void) {
    tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
    tokens->size = 0;
    tokens->items = (char **) malloc(sizeof(char *));
    tokens->items[0] = NULL; /* make NULL terminated */
    return tokens;
}

void add_token(tokenlist *tokens, char *item) {
    int i = tokens->size;

    tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
    tokens->items[i] = (char *) malloc(strlen(item) + 1);
    tokens->items[i + 1] = NULL;

    strcpy(tokens->items[i], item);

    tokens->size += 1;
}

char *get_input(void) {
    char *buffer = NULL;
    int bufsize = 0;
    char line[5];

    while (fgets(line, 5, stdin) != NULL) {
        int addby = 0;
        char *newln = strchr(line, '\n');

    if (newln != NULL)
        addby = newln - line;
    else
        addby = 5 - 1;

    buffer = (char *) realloc(buffer, bufsize + addby);
    memcpy(&buffer[bufsize], line, addby);
    bufsize += addby;

    if (newln != NULL)
        break;
    }

    buffer = (char *) realloc(buffer, bufsize + 1);

    buffer[bufsize] = 0;

    return buffer;
}

tokenlist *get_tokens(char *input) {
    char *buf = (char *) malloc(strlen(input) + 1);
    strcpy(buf, input);

    tokenlist *tokens = new_tokenlist();

    char *tok = strtok(buf, " ");

    while (tok != NULL) {
        add_token(tokens, tok);
        tok = strtok(NULL, " ");
    }

    free(buf);
    return tokens;

}

void free_tokens(tokenlist *tokens) {
    for (int i = 0; i < tokens->size; i++)
    free(tokens->items[i]);
    free(tokens->items);
    free(tokens);

}

void tilde_expansion(tokenlist *tokens, int i) {
    char * path = &(tokens->items[i][1]);
    char * home = getenv("HOME");

    char * temp = tokens->items[i];

    tokens->items[i] = (char*)malloc(strlen(home) + strlen(path)+ 1);
    strcpy(tokens->items[i], home);
    strcat(tokens->items[i], path);

    free(temp);
}

void env_variables(tokenlist *tokens, int i) {
    char * env = getenv(&(tokens->items[i][1]));
    char * temp = tokens->items[i];
    tokens->items[i] = (char*)malloc(strlen(env)+1);
    strcpy(tokens->items[i], env);
    
    free(temp);
    // printf("%s\n",tokens->items[i]);
}

bool is_a_builtin(tokenlist *tokens, int count, process * bg_queue[])
{
    if(strcmp(tokens->items[0], "exit")==0) {
        exit_cmd(count, bg_queue); 
        return true;
    }
	else if(strcmp(tokens->items[0], "cd")==0){
		cd_cmd(tokens->items[1]);	
		count++;
    return true;
	}	
	else if(strcmp(tokens->items[0], "echo")==0){
		echo_cmd(tokens);
		count++;
    return true;
	}		
	else if(strcmp(tokens->items[0], "jobs")==0){
		jobs_cmd(bg_queue);
		count++;
    return true;
	}
}

void echo_cmd(tokenlist* tokens)
{
    for(int i = 1; i < tokens->size; i++)
        printf("%s ", tokens->items[i]);
}

void exit_cmd(int count, process * bg_queue[])
{
    waitpid(-1, NULL, 0);
    // if (count >= 3) {
    //     printf("The following was the last three valid commands executed:");
    //     for (int i = count; i > count - 3; i++){
    //         printf("%d,%s\n",i, bg_queue[i]->running);
    //     }
    // }
    // else if (count > 0) {
    //     printf("The last command executed was: ");
    //     printf("%s",bg_queue[count]->running);
    // }
    // else 
    //     printf(" No valid commands were executed in this shell.");

    // exit(0); 
}

void cd_cmd(char * dir)
{
    // cd with no arguments
    if(dir == NULL){ 
        chdir(getenv("HOME")); 
        setenv("PWD", getcwd(NULL, 0), 1); 
        free(getcwd(NULL, 0)); 
    }
    else if(chdir(dir) == -1)
        printf("error: invalid directory\n");
    else {
        setenv("PWD", getcwd(NULL, 0), 1); 
        free(getcwd(NULL, 0)); 
    }
}

void jobs_cmd(process * bg_queue[])
{
    int job = 1;
    for(int i = 1; i < 11; i++)
        if(bg_queue[i]->process_id != -1)
            printf("[%d]+[%d][%s]\n", job++, bg_queue[i]->process_id, bg_queue[i]->running);
}

void Piping() {

}