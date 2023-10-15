#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigusr1_handler(int signum) {
    FILE *file;
    char c;

    file = fopen("text.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    printf("Contents of text.txt:\n");
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }
    fclose(file);
}

void sigusr2_handler(int signum) {
    printf("Process terminating...\n");
    exit(0);
}

int main() {
    FILE *pid_file;
    pid_t pid;

    pid_file = fopen("/var/run/agent.pid", "w");
    if (pid_file == NULL) {
        printf("Error creating PID file\n");
        return 1;
    }

    pid = getpid();
    fprintf(pid_file, "%d", pid);
    fclose(pid_file);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    FILE *file;
    char c;

    file = fopen("text.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    printf("Contents of text.txt:\n");
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }
    fclose(file);

    while (1) {
        sleep(1);
    }

    return 0;
}

