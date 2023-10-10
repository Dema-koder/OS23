#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_MESSAGE_SIZE 1024
#define MAX_NAME_PIPE 50

int main(int argc, char* argv[]) {
    char pipe_name[MAX_NAME_PIPE];
    snprintf(pipe_name, sizeof(pipe_name), "/tmp/ex1/s%d", atoi(argv[1]));
    int fd = open(pipe_name, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open pipe");
        exit(1);
    }
    char message[MAX_MESSAGE_SIZE];
    while (1) {
    	while (1) {
		    ssize_t bytes_read = read(fd, message, sizeof(message));
		    if (bytes_read == -1) {
		        perror("Failed to read from pipe");
		        exit(1);
		    } else if (bytes_read == 0) 
		        break;
		    printf("Message from subscriber: %s\n", message);
        }
    }
    close(fd);
    return 0;
}

