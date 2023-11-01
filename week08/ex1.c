#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#define PASSWORD_LENGTH 14

void generate_password(char *password) {
    const char printable_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+[]{}|;':,.<>/?";
    size_t num_chars = sizeof(printable_chars) - 1;

    password[0] = 'p';
    password[1] = 'a';
    password[2] = 's';
    password[3] = 's';
    password[4] = ':';

    int urandom_fd = open("/dev/urandom", O_RDONLY);
    for (int i = 5; i < PASSWORD_LENGTH-2; i++) {
        unsigned char random_byte;
        ssize_t bytes_read = read(urandom_fd, &random_byte, sizeof(random_byte));
        password[i] = printable_chars[random_byte % num_chars];
    }
    close(urandom_fd);
}

int main() {
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);

    char password[PASSWORD_LENGTH];
    generate_password(password);
    printf("Generated password: %s\n", password);
    
    char *shared_memory = mmap(NULL, PASSWORD_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    memcpy(shared_memory, password, PASSWORD_LENGTH);

    while (1) {
        sleep(1);
    }
    
    munmap(shared_memory, PASSWORD_LENGTH);
    
    return 0;
}
