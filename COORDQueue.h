#ifndef _COORDQueue_H
#define _COORDQueue_H
#include <windows.h>

enum DirectionEnum
{
	up,
	down,
	left,
	right
};
typedef enum DirectionEnum DirectionEnum;

struct VectorData
{
	SHORT X;
	SHORT Y;
	DirectionEnum Direct;
};
typedef struct VectorData VectorData;

struct Node
{
	VectorData Data;
	struct Node* Next;
};
typedef struct Node Node;

struct Queue
{
	int Count;
	Node* Front;
	Node* Rear;
};
typedef struct Queue Queue;

extern void QueueInitialize(Queue* q);
int QueueIsempty(Queue* q);
VectorData* QueuePick(Queue* q);
VectorData Dequeue(Queue* q);
void Enqueue(Queue* q, VectorData data);

#endif
