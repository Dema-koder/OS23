#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    if (pipe(pipefd) == -1) {
        exit(-1);
    }
    pid = fork();
    if (pid == 0) {
        close(pipefd[1]); 
        char message[MAX_MESSAGE_SIZE];
        int bytes_read;
        while ((bytes_read = read(pipefd[0], message, sizeof(message))) > 0) {
            write(STDOUT_FILENO, message, bytes_read);
        }
        close(pipefd[0]); 
    } else {
        close(pipefd[0]);  
        char message[MAX_MESSAGE_SIZE];
        int bytes_read;
        while ((bytes_read = read(STDIN_FILENO, message, sizeof(message))) > 0) {
            write(pipefd[1], message, bytes_read);
        }
        close(pipefd[1]); 
    }
    return 0;
}

