#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int count;
};

int hits = 0;
int total = 0;

struct PTE* page_table;
void printPTE(struct PTE* page_table, int pages_num) {
    printf("Page table \n");
    for (int i = 0; i < pages_num; i++)
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
}
int main(int argc, char* argv[]) {
    int pages_num = atoi(argv[1]);
    int pager_pid = atoi(argv[argc - 1]);
    int fd = open("pagetable2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    size_t page_table_size = pages_num * sizeof(struct PTE);

    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }
	page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("Initialized page table \n");
	printPTE(page_table, pages_num);
	
	for (int i = 2; i < argc - 1; i++) {
        char* access = argv[i];
        char mode = access[0];
        int page = atoi(access + 1);
        switch (mode) {
            case 'R':
                if (page_table[page].valid == 0) {
              		printf("Read request for page %d\nIt is not a valid page --> page fault \nAsk pager to load it from disk (SIGUSR1 signal) and wait \nMMU resumed by SIGCONT signal from pager\n", page);
              		printPTE(page_table, pages_num);
              		printf("---------------------\n");
              		page_table[page].referenced = getpid();
              		total++;
              		page_table[page].count++;
              		kill(pager_pid,SIGUSR1);
              		kill(getpid(), SIGSTOP);
              		break;
                }
                if (page_table[page].valid == 1) {
                	printf("Read request for page %d\nIt is a valid page\n", page);
                	printPTE(page_table, pages_num);
              		printf("---------------------\n");
              		page_table[page].count++;
                	total++;
                	hits++;
              		break;
                }
            case 'W':
                printf("Write request for page %d\n", page);
                if (page_table[page].valid == 0) {
              		printf("It is not a valid page --> page fault \nAsk pager to load it from disk (SIGUSR1 signal) and wait \nMMU resumed by SIGCONT signal from pager\n");
              		page_table[page].referenced = getpid();
              		page_table[page].count++;
              		kill(pager_pid,SIGUSR1);
              		kill(getpid(), SIGSTOP);
              		printPTE(page_table, pages_num);
              		printf("---------------------\n");
                } else {
		            printf("It is a valid page\nIt is a write request then set the dirty field\n");
		       		page_table[page].dirty = 1;
		       		printPTE(page_table, pages_num);
              		page_table[page].count++;
		          	printf("---------------------\n");
		          	hits++;
                }
                total++;
                break;
            default:
                printf("Unknown access: %s\n", access);
                break;
        }
    }
    float hit_ratio = (float)hits / total;
	float memory_loss = 1.0 - hit_ratio;
	printf("Hit ratio: %2.f\n", hit_ratio);
	printf("Memory loss: %2.f\n", memory_loss);
    close(fd);
	munmap(page_table,page_table_size);
	kill(pager_pid,SIGUSR1);
    return 0;
}
