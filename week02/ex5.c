#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

long long tribonacci(int n) {
	long long fir = 0, sec = 1, thi = 1, cur = 0;
	for (int i = 3; i <= n; i++) {
		cur = fir + sec + thi;
		fir = sec; sec = thi, thi = cur;
	}
	return cur;
}

int main() {
	printf("%lld %lld", tribonacci(4), tribonacci(36));
}
