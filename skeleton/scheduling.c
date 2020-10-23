#include <stdlib.h>
#include <stdbool.h> //added this myself //TODO ask if this is ok?
#include <stdio.h> //same here

#include "scheduling.h"

/*
##  Round-Robin Scheduling Algorithm  ##

	Parameters:-

	queue: 1D int array containing the CPU burst time for every process in a ready queue
	np: number of processes (length of `queue` array)
	tq: time quantum for the RR algorithm


	Returns:-

	rr_result struct pointer, where
	rr_result -> order: 1D int array, containing the execution order of processes (i.e. `(rr_result -> order)[0]` is the process number of the first process)
		** hint: p1 is always the first item in order of selection in round-robin and FCFS
    rr_result -> order_n: int, representing the size of the `order` array, described above ^
	rr_result -> turnarounds: 1D int array, containing the turnaround time for each process (i.e. `(rr_result -> turnarounds)[0]` is the turnaround time for p1)
*/
rr_result *rr(int *queue, int np, int tq)
{
	printf("Starting rr method...\n");
	rr_result *result = malloc(sizeof(rr_result));
	result->np = np;
	result->turnarounds = malloc(sizeof(int) * np);

	// code here to assign values to result->turnarounds, result->order, and result->order_n

	int time_remaining[np]; //maybe just use queue instead? //or store here for checking against result?
	int elapsed_time = 0;

	int start_times[np]; //for calculating turnaround time.  start_times[i] is when pi started processing for the first time.
	for(int i = 0; i < np; i++)
	{
		start_times[i] = -1; //initialize every value of start time to -1 to show that it has not been set yet.
	}

	result->order_n = 0;
	//result->turnarounds = int[np];

	bool queue_finished_processing = false;
	printf("Starting Loop...\n");
	while(!queue_finished_processing)
	{
		for(int i = 0; i < np ; i++) //iterate through queue of burst times
		{
			if(queue[i] == 0) //process already completed
			{
				printf("Skipping p%d.\n", (i+1));				//skip
			}
			else if (0 < queue[i] && queue[i] <= tq) //process completes this round
			{
				if(start_times[i] == -1) //if start time hasn't been set already
				{
					start_times[i] = elapsed_time; //set start time to current elapsed_time
				}

				elapsed_time += queue[i];
				queue[i] = 0;

				result->order[result->order_n] = i;
				result->order_n += 1;

				result->turnarounds[i] = (elapsed_time - start_times[i]);
			}
			else if (queue[i] > tq) //process will not complete this round
			{
				if(start_times[i] == -1) //if start time hasn't been set already
				{
					start_times[i] = elapsed_time; //set start time to current elapsed_time
				}

				elapsed_time += tq;
				queue[i] -= tq;

				result->order_n += 1;
				result->order[result->order_n] = i;
			}
		}
		queue_finished_processing = true; //while loop will end unless a process with time remaining is found.
		for (int j = 0; j < np ; j++)// iterate through queue
		{
			if(queue[j] > 0)//if any process has time remaining
			{
				queue_finished_processing = false; //the while loop continues and round robin does another round.
			}
		}
	}
	return result;
}
