#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int count(char s[300], char c) {
	int k = 0;
	for (int i = 0; i < strlen(s); i++)
		if (s[i] == c)
			k++;
	return k;
}

void countAll(char s[300]) {
	for (int i = 0; i < strlen(s); i++)
		if (s[i] - 'A' < 26 && s[i] - 'A' >= 0)
			s[i] += 32;
	for (int i = 0; i < strlen(s); i++)
		if (i != strlen(s) - 1)
			printf("%c:%d, ", s[i], count(s, s[i]));
		else 
			printf("%c:%d", s[i], count(s, s[i]));
}

int main() {
	char s[300];
	scanf("%s", s);
	countAll(s);
}
