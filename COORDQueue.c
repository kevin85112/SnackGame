#include "COORDQueue.h"

void QueueInitialize(Queue* q)
{
	q->Count = 0;
	q->Front = NULL;
	q->Rear = NULL;
}

int QueueIsempty(Queue* q)
{
	return q->Count == 0;
}

VectorData* QueuePick(Queue* q)
{
	return(&q->Front->Data);
}

VectorData Dequeue(Queue* q)
{
	Node* tmp;
	VectorData n = q->Front->Data;
	tmp = q->Front;
	q->Front = q->Front->Next;
	if (q->Front == NULL)
		q->Rear = NULL;
	q->Count--;
	free(tmp);
	return(n);
}

void Enqueue(Queue* q, VectorData data)
{
	Node* tmp;
	tmp = malloc(sizeof(Node));
	tmp->Data = data;
	tmp->Next = NULL;
	if (!QueueIsempty(q))
	{
		q->Rear->Next = tmp;
		q->Rear = tmp;
	}
	else
	{
		q->Front = q->Rear = tmp;
	}
	q->Count++;
}