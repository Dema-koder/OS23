#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
	void* ans = malloc(size);
	memcpy(ans, initial_value, size);
	for (int i = 0; i < n; i++) 
		ans = opr(ans, base), 
		base += size;
	return ans;
}

void* add_int(const void *a, const void *b) {
	int ab = (*((int*)a) + *((int*)b));
	return memcpy(malloc(sizeof(int)), &ab, sizeof(int));
}

void* add_double(const void *a, const void *b) {
	double ab = (*((double*)a) + *((double*)b));
	return memcpy(malloc(sizeof(double)), &ab, sizeof(double));
}

void* mul_double(const void *a, const void *b) {
	double ab = *((double*)a) * *((double*)b); 
	return memcpy(malloc(sizeof(double)), &ab, sizeof(double));
}

void* mul_int(const void *a, const void *b) {
	int ab = *((int*)a) * *((int*)b);
	return memcpy(malloc(sizeof(int)), &ab, sizeof(int));
}

void* max_int(const void *a, const void *b) {
	int ab = (*((int*)a) > *((int*)b) ? *((int*)a) : *((int*)b));
	return memcpy(malloc(sizeof(int)), &ab, sizeof(int));
}

void* max_double(const void *a, const void *b) {
	double ab = (*((double*)a) > *((double*)b) ? *((double*)a) : *((double*)b));
	return memcpy(malloc(sizeof(double)), &ab, sizeof(double));
}

int main(void) {
	printf("Array of Integer:\n");
	int Int[] = {10, 15, 20, 25, 30};
	int init = 0;
	void* res = aggregate(Int, sizeof(int), 5, &init, add_int);
	printf("Addition result: %d\n", *(int*)res);
	init = 1;
	res = aggregate(Int, sizeof(int), 5, &init, mul_int);
	printf("Multiplication result: %d\n", *(int*)res);
	init = INT_MIN;
	res = aggregate(Int, sizeof(int), 5, &init, max_int);
	printf("Max result: %d\n", *(int*)res);
	
	printf("\nArray of double:\n");
	double Dbl[] = {1.0, 1.5, 2.0, 2.5, 3.0};
	double init2 = 0;
	res = aggregate(Dbl, sizeof(double), 5, &init2, add_double);
	printf("Addition result: %lf\n", *(double*)res);
	init2 = 1;
	res = aggregate(Dbl, sizeof(double), 5, &init2, mul_double);
	printf("Multiplication result: %lf\n", *(double*)res);
	init2 = DBL_MIN;
	res = aggregate(Dbl, sizeof(double), 5, &init2, max_double);
	printf("Max result: %lf\n", *(double*)res);
}
