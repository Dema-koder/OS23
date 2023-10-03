#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

void parse_input(char* input, char** command, char** arguments) {
    char* token = strtok(input, " ");
    *command = token;
    int i = 0;
    while (token != NULL) {
        arguments[i++] = token;
        token = strtok(NULL, " ");
    }
    arguments[i] = NULL;
}

int main() {
    char input[100];
    char* command;
    char* arguments[10];
    while (1) {
        printf("Shell >> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "exit") == 0)
            break;
        parse_input(input, &command, arguments);
        pid_t pid = fork();
        if (pid == 0) 
            execvp(command, arguments);
    	else {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}
