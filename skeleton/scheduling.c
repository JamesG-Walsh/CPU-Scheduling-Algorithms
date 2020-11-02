/*	CS3305A	-			Fall 2020	-	Assignment 4
		James Walsh	-	jwalsh57	-	250481718
*/

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
	//printf("Starting rr function...\n");
	rr_result *result = malloc(sizeof(rr_result));
	result->np = np;
	result->turnarounds = malloc(sizeof(int) * np);

	// code here to assign values to result->turnarounds, result->order, and result->order_n

	//printf("tq: %d\n", tq);
	result->order_n = 0; //initialize order_n (it will be incremented later)
	int elapsed_time = 0;
	int start_times[np]; //for calculating turnaround time.  start_times[i] is when pi started processing for the first time.
	for(int i = 0; i < np; i++)
	{
		start_times[i] = -1; //initialize every value of start time to -1 to show that it has not been set yet.
	}

	node *firstNode;
	node *currNode; //initialize 2 pointers to node structures

	bool any_process_has_time_remaining = true;
	//printf("Starting Loop...\n");
	while(any_process_has_time_remaining)
	{
		//printf("\n\nWHILE LOOP BEGINS.\n");
		for(int i = 0; i < np ; i++) //iterate through queue of CPU burst times
		{
			//printf("\nFor loop begins\n");
			if (0 < queue[i])
			{
				if(start_times[i] == -1) //if start time hasn't been set already
				{
					start_times[i] = elapsed_time; //set start time to current elapsed_time
				}

				if (0 < queue[i] && queue[i] <= tq) //process finishes this round
				{
					elapsed_time += queue[i];				//printf("elapsed_time updated to %d\n", elapsed_time);
					queue[i] = 0;
					result->turnarounds[i] = (elapsed_time - start_times[i]); //compute turnaround time
				}
				else if(queue[i] > tq) //process is pre-empted and continues next round.
				{
					elapsed_time += tq;
					queue[i] -= tq;
				}
				//printf("queue[%d] (time remaining for p%d) updated to %d\n", i, (i+1), queue[i]);

				if(result->order_n == 0)//first process
				{
					firstNode = malloc(sizeof(node));
					currNode = malloc(sizeof(node));	//allocate memory for first 2 nodes
					firstNode->value = (i+1); // i+1 is used so that my own print statements made sense
					firstNode->next = currNode; //step to next node in linkedlist
				}
				else if(result->order_n > 0) //not the first process
				{
					currNode->value = (i+1); // i+1 is used so that my own print statements made sense
					currNode->next = malloc(sizeof(node)); //dynamically allocate memory as the linked list grows
					currNode = currNode->next;//step to next node in linked list
				}

				result->order_n += 1; //increase the size of order_n by 1
				//printf("result->order_n updated to %d\n", result->order_n);
				//printf("result->turnarounds[%d] set to %d\n", i, (elapsed_time - start_times[i]));
			}
		}
		any_process_has_time_remaining = false; //while loop will end unless a process with time remaining is found.
		for (int j = 0; j < np && !any_process_has_time_remaining ; j++)// iterate through queue
		{
			if(queue[j] > 0)//if any process has time remaining
			{
				any_process_has_time_remaining = true; //the while loop continues and round robin does another round.
				//printf("Time remains on p%d\n", (j+1));
			}
		}
	}
	//printf("\nQueue finished processing.\n");

	//all that remains now is to transfer the values from the linked list to the result->order array.
	result->order = malloc(sizeof(int) * result->order_n); //allocate memory for order array
	currNode = firstNode; //start with the firstNode
	node *prevNode;
	for(int i = 0 ; i < result->order_n ; i++) //iterate through order array
	{
		(result->order)[i] = (currNode->value - 1);  //value assigned to order array has to be reduced by one to match the array index
		//printf("result->order[%d] = %d\n", i, (currNode->value -1));
		prevNode = currNode;
		currNode = currNode->next; //step to next node in linked list
		free(prevNode); //The node structures aren't being returned and aren't needed any longer so their dynamically allocated memory can be deallocated
	}
	free(currNode);

	return result;
}
