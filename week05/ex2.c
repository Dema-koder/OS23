#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 1000

typedef struct {
	pthread_t id;
	int i;
	char message[256];
} Thread;

int thread_count = 0;

void* thread_func(void* arg) {
    Thread* thread = (Thread*)arg;
    printf("Thread %d is creadted\n", thread_count);
    printf("ID: %lu\n", thread->id);
    printf("%s\n", thread->message);
    thread_count++;
    return NULL;
}

int main() {
	int n;
	Thread threads[MAX_THREADS];
	printf("Enter the number of threads: ");
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		threads[i].i = i;
		sprintf(threads[i].message, "Hello from Thread %d", i);
		pthread_create(&threads[i].id, NULL, thread_func, &threads[i]);
		pthread_join(threads[i].id, NULL);
    		printf("Thread %d exit\n", i);
	}
	return 0;
}
