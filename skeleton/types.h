#ifndef TYPES_H
#define TYPES_H

typedef struct input_queues
{
	int nq;
	int **queues;
	int *np;
	int *tq;
} input_queues;

typedef struct rr_result
{
	int np; //number of processes
	int order_n; // size of the order array
	int *order; // pointer to array containing the execution order of processes
	int *turnarounds; //pointer to array containing the turnaround time for each process.  turnarounds[0] = turnaround time of p1
} rr_result;

typedef struct node
{
	int value;
	struct node *next;
} node;

#endif
