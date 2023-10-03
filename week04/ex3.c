#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            sleep(5);
            //printf("Child process %d created\n", getpid());
        } else 
            if (pid < 0) {
                exit(1);
            }
    }
    for (int i = 0; i < n; i++)
    	wait(NULL);
    return 0;
}
