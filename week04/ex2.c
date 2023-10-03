#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#define N 120

int main() {
    int n;
    scanf("%d", &n);
    int u[N];
    int v[N];
    for (int i = 0; i < N; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }
    int cur = N / n;
    FILE *file = fopen("temp.txt", "w+");
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            int result = 0;
            for (int j = i * cur; j < start + cur; j++) 
                result += u[j] * v[j];
            fprintf(file, "%d\n", result);
            fclose(file);
            exit(0);
        }
    }
    for (int i = 0; i < n; i++) 
        wait(NULL);
    fseek(file, 0, SEEK_SET);
    int total_result = 0;
    int result;
    while (fscanf(file, "%d", &result) == 1) 
        total_result += result;
    printf("Dot product: %d\n", total_result);
    fclose(file);
    return 0;
}
