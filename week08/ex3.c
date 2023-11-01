#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define MB 1024 * 1024

int main() {
    struct rusage usage;
    int i;

    for (i = 0; i < 10; i++) {
        char *buffer = (char *)malloc(10 * MB);

        memset(buffer, 0, 10 * MB);

        getrusage(RUSAGE_SELF, &usage);
        printf("Memory usage: %ld KB\n", usage.ru_maxrss);

        sleep(1);

        //free(buffer);
    }
    return 0;
}

