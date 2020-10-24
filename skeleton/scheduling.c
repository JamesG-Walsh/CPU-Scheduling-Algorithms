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
	//printf("Starting rr method...\n");
	rr_result *result = malloc(sizeof(rr_result));
	result->np = np;
	result->turnarounds = malloc(sizeof(int) * np);

	// code here to assign values to result->turnarounds, result->order, and result->order_n

	//printf("tq: %d\n", tq);

	int time_remaining[np]; //maybe just use queue instead? //or store here for checking against result?
	int elapsed_time = 0;

	int start_times[np]; //for calculating turnaround time.  start_times[i] is when pi started processing for the first time.
	for(int i = 0; i < np; i++)
	{
		start_times[i] = -1; //initialize every value of start time to -1 to show that it has not been set yet.
	}

	result->order_n = 0;

	node *firstNode = malloc(sizeof(node));
	node *currNode = malloc(sizeof(node));

	bool queue_finished_processing = false;
	//printf("Starting Loop...\n");
	while(!queue_finished_processing)
	{
		//printf("\n\nWHILE LOOP BEGINS.\n");
		for(int i = 0; i < np ; i++) //iterate through queue of burst times
		{
			//printf("\nFor loop begins\n");
			if(queue[i] == 0) //process already completed
			{
				//printf("Skipping p%d.\n", (i+1));				//skip
			}
			else if (0 < queue[i] && queue[i] <= tq) //process completes this round
			{
				//printf("p%d completes this round.\n", (i+1));
				if(start_times[i] == -1) //if start time hasn't been set already
				{
					start_times[i] = elapsed_time; //set start time to current elapsed_time
				}

				elapsed_time += queue[i];
				//printf("elapsed_time updated to %d\n", elapsed_time);
				queue[i] = 0;
				//printf("queue[%d] (time remaining for p%d) updated to %d\n", i, (i+1), queue[i]);

				if(result->order_n == 0)//first process
				{
					firstNode->value = (i+1);
					currNode = malloc(sizeof(node));
					firstNode->next = currNode;
				}
				else if(result->order_n > 0) //not the first process
				{
					currNode->value = (i+1);
					currNode->next = malloc(sizeof(node));
					currNode = currNode->next;
				}

				result->order_n += 1;
				//printf("result->order_n updated to %d\n", result->order_n);

				result->turnarounds[i] = (elapsed_time - start_times[i]);
				//printf("result->turnarounds[%d] set to %d\n", i, (elapsed_time - start_times[i]));
			}
			else if (queue[i] > tq) //process will not complete this round
			{
				//printf("Process p%d has %dms remaining and will not complete this round.\n", (i+1), queue[i]);
				if(start_times[i] == -1) //if start time hasn't been set already
				{
					//printf("setting start time to %d\n", elapsed_time);
					start_times[i] = elapsed_time; //set start time to current elapsed_time
					//printf("start_times[%d] = %d\n", i, start_times[i]);
				}

				elapsed_time += tq;
				//printf("elapsed_time updated to %d\n", elapsed_time);
				queue[i] -= tq;
				//printf("queue[%d] (time remaining for p%d) updated to %d\n", i, (i+1), queue[i]);

				//result->order[result->order_n] = (i+1);
				//printf("result->order\n");

				if(result->order_n == 0)//first process
				{
					firstNode->value = (i+1);
					firstNode->next = currNode;
				}
				else if(result->order_n > 0) //not the first process
				{
					currNode->value = (i+1);
					currNode->next = malloc(sizeof(node));
					currNode = currNode->next;
				}

				result->order_n += 1;
				//printf("result->order_n updated to %d\n", result->order_n);
			}
		}
		queue_finished_processing = true; //while loop will end unless a process with time remaining is found.
		for (int j = 0; j < np ; j++)// iterate through queue
		{
			if(queue[j] > 0)//if any process has time remaining
			{
				queue_finished_processing = false; //the while loop continues and round robin does another round.
				//printf("Time remains on p%d\n", (j+1));
			}
		}
	}
	//printf("\nQueue finished processing.\n");

	result->order = malloc(sizeof(int) * result->order_n);
	currNode = firstNode;
	for(int i = 0 ; i < result->order_n ; i++)
	{
		(result->order)[i] = (currNode->value - 1);
		//printf("result->order[%d] = %d\n", i, (currNode->value -1));
		currNode = currNode->next;
	}

	return result;
}
