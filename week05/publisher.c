#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <sys/stat.h>

#define MAX_MESSAGE_SIZE 1024
#define SIZE_PATH_NAME 50

char path_name[SIZE_PATH_NAME];
int n = 0, fd;

void message() {
	char message[MAX_MESSAGE_SIZE];
	printf("Message for subscribers: ");
	fgets(message, MAX_MESSAGE_SIZE, stdin);
	for (int i = 0; i < n; i++) 
		if (fork() == 0) {
			sprintf(path_name, "/tmp/ex1/s%d", i+1);
			mkfifo(path_name, 0666);
			fd = open(path_name, O_RDWR | O_NONBLOCK);
			write(fd, message, strlen(message) + 1);
			close(fd);
			exit(0);
		}
	for (int i = 0; i < n; i++) 
		wait(NULL);
}

int main(int argc, char* argv[]) {
    n = atoi(argv[1]);
    mkdir("/tmp/ex1", 0777);
    while (1) 
    	message();
}

