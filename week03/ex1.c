#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int const_tri(int* const p, int n) {
	if (n == 0) 
		return p[0];
	if (n == 1) 
		return p[1];
	if (n == 2)
		return p[2];
	return const_tri(p, n - 3) + const_tri(p, n - 2) + const_tri(p, n - 1);
}

int main(void) {
	const int a = 1;
	const int* q = &a;
	int* const p = (int*)malloc(3 * sizeof(int));
    p[0] = 1;
    p[1] = 1;
    p[2] = 2;
    printf("%p\n%p\n%p\n", p, p+1, p+2);
    free(p);
}
