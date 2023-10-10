#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

bool is_prime(int n) {
	if (n <= 1) return false;
	int i = 2;
	for (; i * i <= n; i++) {
		if (n % i == 0)
			return false;
	}
	return true;
}

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int c = 0;
int n = 0;
int m = 0;

int get_number_to_check() {
	int ret = k;
	if (k != n) 
		k++;
	return ret;
}

void increment_primes() {
	c++;
}

void *check_primes(void *arg) {
	pthread_mutex_lock(&global_lock);
	int r = (n / m);
	while (1) {
		int cur = get_number_to_check();
		if (is_prime(cur))
			increment_primes();
		r--;
		if (r == 0)
			break;
	}
	pthread_mutex_unlock(&global_lock);
	return NULL;
}

int main(int argc, char* argv[]) {
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	pthread_t threads[m];
	for (int i = 0; i < m; i++) {
		int result = 0;
		result = pthread_create(&threads[i], NULL, check_primes, NULL);
		result = pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&global_lock);
	printf("%d\n", c);
}
