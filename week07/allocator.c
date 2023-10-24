#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 10000000

unsigned int arr[SIZE];

void allocate_first_fit(unsigned int adrs, int size) {
	for (int i = 0; i < SIZE; i++) {
		bool check = false;
		for (int j = i; j < i + size; j++)
			if (arr[j] != 0) {
				check = true;
				break;
			}
		if (!check) {
			for (int j = i; j < i + size; j++) 
				arr[j] = adrs;
			break;
		}	
	}
}

void allocate_best_fit(unsigned int adrs, int size) {
	int mn = 1e8, ind = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] == 0 && ind == -1) {
			ind = i;
		}
		if (arr[i] != 0) {
			if (ind != -1)
				if (i - ind >= size) {
					if (i - ind < mn)
						mn = i-ind, l = ind;
				}
			ind = -1;
		}
	}
	for (int i = l; i <= l + size; i++) 
		arr[i] = adrs;
}

void allocate_worst_fit(unsigned int adrs, int size) {
	int mx = 0, ind = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] == 0 && ind == -1) {
			ind = i;
		}
		if (arr[i] != 0) {
			if (ind != -1)
				if (i - ind >= size) {
					if (i - ind > mx)
						mx = i-ind, l = ind;
				}
			ind = -1;
		}
	}
	for (int i = l; i <= l + size; i++) 
		arr[i] = adrs;
}

void clear(unsigned int adrs) {
	for (int i = 0; i < SIZE; i++) 
		arr[i] = (arr[i] == adrs ? 0 : arr[i]);
} 

int main() {
	clock_t begin, end;
	double kol = 0;
	begin = clock();
	FILE* file = fopen("queries.txt", "r");
	while (!feof(file)) {
		char line[100];
		bool check = false;
		while (fgets(line, sizeof(line), file)) {
			char* token;
			token = strtok(line, " ");
			if (strcmp(&token[0], "end") == 0) {
				check = true;
				break;
			} else 
			if (strcmp(&token[0], "allocate") == 0) {
				token = strtok(NULL, " ");
				unsigned int adrs = atoi(&token[0]);
				token = strtok(NULL, "\n");
				int size = atoi(&token[0]);
				allocate_first_fit(adrs, size);
			} else
			if (strcmp(&token[0], "clear") == 0) {
				token = strtok(NULL, "\n");
				unsigned int adrs = atoi(&token[0]);
				clear(adrs);
			}
		}
		kol++;
		if (check)
			break;
	}
	fclose(file);
	end = clock();
	double time = ((double)(end - begin) / CLOCKS_PER_SEC);
	printf("%lf\n", kol / time);
	
	begin = clock();
	file = fopen("queries.txt", "r");
	while (!feof(file)) {
		char line[100];
		bool check = false;
		while (fgets(line, sizeof(line), file)) {
			char* token;
			token = strtok(line, " ");
			if (strcmp(&token[0], "end") == 0) {
				check = true;
				break;
			} else 
			if (strcmp(&token[0], "allocate") == 0) {
				token = strtok(NULL, " ");
				unsigned int adrs = atoi(&token[0]);
				token = strtok(NULL, "\n");
				int size = atoi(&token[0]);
				allocate_best_fit(adrs, size);
			} else
			if (strcmp(&token[0], "clear") == 0) {
				token = strtok(NULL, "\n");
				unsigned int adrs = atoi(&token[0]);
				clear(adrs);
			}
		}
		if (check)
			break;
	}
	fclose(file);
	end = clock();
	time = ((double)(end - begin) / CLOCKS_PER_SEC);
	printf("%lf\n", kol / time);
	
	begin = clock();
	file = fopen("queries.txt", "r");
	while (!feof(file)) {
		char line[100];
		bool check = false;
		while (fgets(line, sizeof(line), file)) {
			char* token;
			token = strtok(line, " ");
			if (strcmp(&token[0], "end") == 0) {
				check = true;
				break;
			} else 
			if (strcmp(&token[0], "allocate") == 0) {
				token = strtok(NULL, " ");
				unsigned int adrs = atoi(&token[0]);
				token = strtok(NULL, "\n");
				int size = atoi(&token[0]);
				allocate_worst_fit(adrs, size);
			} else
			if (strcmp(&token[0], "clear") == 0) {
				token = strtok(NULL, "\n");
				unsigned int adrs = atoi(&token[0]);
				clear(adrs);
			}
		}
		if (check)
			break;
	}
	fclose(file);
	end = clock();
	time = ((double)(end - begin) / CLOCKS_PER_SEC);
	printf("%lf\n", kol / time);
}
