#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdbool.h>
#include <errno.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct{
	int idx; // process idx (index)
	int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
	int burst; // remaining burst (this should decrement when the process is being executed)
	bool stop;
	bool done;
	int num;
} ProcessData;

int quant, cur_quant;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

void read_file(FILE* file){

    // TODO: extract the data of processes from the {file} 
    // and store them in the array {data}
    // initialize ps array to zeros (the process is terminated or not created yet)
    
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    
    char id[10], a[10], b[10];
    fscanf(file, "%s%s%s", id, a, b);
    int idx, at, bt;
    while (fscanf(file, "%d%d%d", &idx, &at, &bt) != EOF) {
    	data[idx].idx = idx;
    	data[idx].at = at;
    	data[idx].bt = bt;
    	data[idx].rt = 0;
    	data[idx].wt = 0;
    	data[idx].tat = 0;
    	data[idx].ct = 0;
    	data[idx].burst = bt;
    	data[idx].stop = false;
    	data[idx].done = false;
    	data_size++;
    	data[idx].num = at;
    }
    
    fclose(file);
    int ps[PS_MAX] = {0};

}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    // TODO: send signal SIGCONT to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    // Check if the process is not created yet or terminated
    if (process == 0 || process == -1) {
        return;
    }
    int result = kill(process, SIGCONT);
    if (result == -1) {
        perror("Failed to send SIGCONT signal");
    }
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    // TODO: send signal SIGTSTP to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    if (process == 0 || process == -1) {
        return;
    }
    int result = kill(process, SIGTSTP);
    if (result == -1) {
        perror("Failed to send SIGTSTP signal");
    }
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    // TODO: send signal SIGTERM to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    int result = kill(process, SIGTERM);
    if (result == -1) {
        perror("Failed to send SIGTERM signal");
    }
}

// create a process using fork
void create_process(int new_process) {
    // TODO: fork a new process and add it to ps array
	pid_t new_proc = fork();
	ps[new_process] = new_proc;
	if (new_proc < 0) {
		perror("Failed to fork");
		exit(1);
	} else if (new_proc == 0) {
		ps[running_process] = new_process;
		// TODO: Now the idx of the running process is new_process
		running_process = new_process;
		// TODO: The scheduler process runs the program "./worker {new_process}" 
    	// using one of the exec functions like execvp
    	char str[10];
    	sprintf(str, "%d", new_process);
		char *args[] = {"./worker", str, NULL};
		if (execvp("./worker", args) == -1) {
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
}

// find next process for running
ProcessData find_next_process() {

  // location of next process in {data} array
	int location = -1;
	int mn = 1000000;
	for (int i = 0; i < data_size; i++) {
		if (data[i].burst != 0 && data[i].num < mn) {
			mn = data[i].num;
			location = i;
		}
	}

	// if next_process did not arrive so far, 
    // then we recursively call this function after incrementing total_time
	if(data[location].at > total_time){
        
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        
        // increment the time
        total_time++;
        cur_quant--;
        return find_next_process(); 
	}

  // return the data of next process
	return data[location];
}


// reports the metrics and simulation results
void report(){
	printf("Simulation results.....\n");
	int sum_wt = 0;
	int sum_tat = 0;
	for (int i=0; i< data_size; i++){
		printf("process %d: \n", i);
		printf("	at=%d\n", data[i].at);
		printf("	bt=%d\n", data[i].bt);
		printf("	ct=%d\n", data[i].ct);
		printf("	wt=%d\n", data[i].wt);
		printf("	tat=%d\n", data[i].tat);
		printf("	rt=%d\n", data[i].rt);
		sum_wt += data[i].wt;
		sum_tat += data[i].tat;
	}

	printf("data size = %d\n", data_size);
	float avg_wt = (float)sum_wt/data_size;
	float avg_tat = (float)sum_tat/data_size;
	printf("Average results for this run:\n");
	printf("	avg_wt=%f\n", avg_wt);
	printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){
	for(int i = 0; i < data_size; i++)
		if (data[i].burst > 0)
		    return;
    report();
	exit(EXIT_SUCCESS);
}

void schedule_handler(int signum) {
    total_time++;
    cur_quant--;
    //printf("%d\n", cur_quant);
    if (running_process != -1) {
        data[running_process].burst--; 
        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Scheduler: Process %d is running with %d seconds left\n", running_process, data[running_process].burst);
    	if (data[running_process].burst == 0) {
    		cur_quant = quant;
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            data[running_process].done = true;
            terminate(ps[running_process]);
            data[running_process].ct = total_time;
            data[running_process].tat = total_time - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            ps[running_process] = -1;
            running_process = -1; 
        }
        if (cur_quant == 0) {
			data[running_process].stop = true;
			printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
			suspend(ps[running_process]);
			//waitpid(ps[running_process], NULL, 0);
			running_process = -1;
			cur_quant = quant;
		}
    }
    check_burst();
    ProcessData next_process = find_next_process();
    //printf("Next process: %d\n", next_process.idx);
    if (next_process.idx != running_process) {
    	if (data[running_process].burst == 0) {
			running_process = next_process.idx;
			if (data[running_process].stop) {
				data[running_process].num = total_time;
				cur_quant = quant;
				data[running_process].stop = false;
				printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
				resume(ps[running_process]);
			} else {
				data[running_process].num = total_time;
				cur_quant = quant;
				printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst); 
				data[running_process].rt = total_time - data[running_process].at;
				create_process(data[running_process].idx);
			}
		}
    }
}

int main(int argc, char *argv[]) {

    // read the data file
    FILE *in_file  = fopen(argv[1], "r");
  	if (in_file == NULL) {   
		printf("File is not found or cannot open it!\n"); 
  		exit(EXIT_FAILURE);
    } else {
        read_file(in_file);  
    }
    
    scanf("%d", &quant);
    cur_quant = quant + 2;

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while(1){}// infinite loop
}
