#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

int n, m, res = 0;

bool is_prime(int n) {
	if (n <= 1) return false;
	int i = 2;
	for (; i * i <= n; i++) {
		if (n % i == 0)
			return false;
	}
	return true;
}

int primes_count(int a, int b) {
	int ret = 0;
	for (int i = a; i < b; i++) 
		if (is_prime(i) != 0)
			ret++;
	return ret;
}

typedef struct prime_request {
	int a, b, c;
} prime_request;

void *prime_counter(void *arg) {
	prime_request* req = (prime_request*)arg;
	req->c = primes_count(req->a, req->b);
	return NULL;
}

int main(int argc, char* argv[]) {
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	pthread_t* threads = malloc(m * sizeof(pthread_t));
	prime_request* req = malloc(m * sizeof(prime_request));
	for (int i = 0; i < m; i++) {
		req[i].a = i * n/m;
		req[i].b = (i + 1) * n/m;
		req[i].c = 0;
		pthread_create(&threads[i], NULL, prime_counter, (void*)&req[i]);
	}
	for (int i = 0; i < m; i++) {
		pthread_join(threads[i], NULL);
		res += req[i].c;
	}
	printf("%d\n", res);
}
