#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <time.h>

int main() {
    pid_t child1, child2;
    child1 = fork();
    clock_t start1 = clock();
    if (child1 == 0) {
        printf("Child 1: PID = %d\n", getpid());
        clock_t end1 = clock();
        printf("Execution time: %f\n", (double)((end1-start1)));
        exit(0);
    }
    child2 = fork();
    clock_t start2 = clock();
    if (child2 == 0) {
        printf("Child 2: PID = %d\n", getpid());
        clock_t end2 = clock();
        printf("Execution time: %f\n", (double)((end2-start2)));
        exit(0);
    } 
    wait(NULL);
    wait(NULL);
    printf("Parent: PID = %d\n", getpid());
    return 0;
}

