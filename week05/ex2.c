#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 10

typedef struct {
	pthread_t id;
	int i;
	char message[256];
} Thread;

pthread_mutex_t mutex;
int thread_count = 0;

void* thread_func(void* arg) {
    Thread* thread = (Thread*)arg;
    int current_thread;
    pthread_mutex_lock(&mutex);
    current_thread = thread_count;
    thread_count++;
    printf("Thread %d is creadted\n", current_thread);
    snprintf(thread->message, sizeof(thread->message), "Hello from thread %d", current_thread);
    printf("Thread %d: %s\n", current_thread, thread->message);
    pthread_mutex_unlock(&mutex);
    printf("Thread %d exits\n", thread->i);
    return NULL;
}

int main() {
	int n;
	Thread threads[MAX_THREADS];
	printf("Enter the number of threads: ");
	scanf("%d", &n);
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < n; i++) {
		threads[i].i = i;
		pthread_create(&threads[i].id, NULL, thread_func, &threads[i]);
		
	}
	for (int i = 0; i < n; i++) {
		pthread_join(threads[i].id, NULL);
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}

