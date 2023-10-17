#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

pid_t agent_pid;

void sigint_handler(int signum) {
    kill(agent_pid, SIGTERM);
    exit(0);
}

int main() {
    FILE *pid_file;

    pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL) {
        printf("Error: No agent found.\n");
        return 1;
    }

    fscanf(pid_file, "%d", &agent_pid);
    fclose(pid_file);

    printf("Agent found.\n");

    signal(SIGINT, sigint_handler);

    char command[10];

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"}: ");
        scanf("%s", command);

        if (strcmp(command, "read") == 0) {
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(agent_pid, SIGUSR2);
            break;
        } else if (strcmp(command, "stop") == 0) {
            kill(agent_pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(agent_pid, SIGCONT);
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
