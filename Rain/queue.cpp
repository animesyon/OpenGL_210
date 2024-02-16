/*
   Class Name:

      CQueue

   Description:

      linked list FIFO  (first in, first out)
*/

#include "stdafx.h"
#include "queue.h"

// constructor
CQueue::CQueue()
{
	First = Last = Cursor = NULL;
	Count = 0;
}

// destructor
CQueue::~CQueue()
{
	RemoveAll();
}

// determine if list is empty
bool CQueue::IsEmpty()
{
	return (Count == 0);
}

// add node to the end of the list
//
//          First                  Last                 Node
//          +----+     +----+     +----+               +----+
//          |    |---->|    |---->|    |---->NULL      |    |----> NULL
//          +----+     +----+     +----+               +----+
//
//
void CQueue::Add(float x, float z, float c, float s, bool f)
{
	QUEUE *Node;

	Node = new QUEUE;

	Node->x    = x;
	Node->z    = z;
	Node->c    = c;
	Node->s    = s;
	Node->f    = f;
	Node->Next = NULL;

	if(IsEmpty())
	{
		First = Last = Cursor = Node;
	}
	else
	{
		Last->Next = Node;
		Last = Node;
	}

	Count++;
}

// remove the first node of the list
//
//          First                                  Last
//          +----+          +----+     +----+     +----+
//          |    |---->     |    |---->|    |---->|    |----> NULL
//          +----+          +----+     +----+     +----+
//           Node
//
void CQueue::Remove()
{
	QUEUE *Node;

	if(IsEmpty()) return;
	
	Node  = First;
	First = First->Next;

	if(IsEmpty()){
		Last = Cursor = First;
	}

	delete Node;
	Count--;
}

// remove all nodes
void CQueue::RemoveAll()
{
	while(!IsEmpty()) Remove();
}

// move to first node
void CQueue::MoveFirst()
{
	Cursor = First;
}

// move to next node
void CQueue::MoveNext()
{
	Cursor = Cursor->Next;
}

// determine if end of list is reached
bool CQueue::IsEndOfQueue()
{
	return (Cursor == NULL);
}

// read item at current node
void CQueue::Read(float *x, float *z, float *c, float *s, bool *f)
{
	*x = Cursor->x;
	*z = Cursor->z;
	*c = Cursor->c;
	*s = Cursor->s;
	*f = Cursor->f;
}

// update item at current node
void CQueue::Update(float c, float s)
{
	Cursor->c = c;
	Cursor->s = s;
}

// update item at current node
void CQueue::Update(bool f)
{
	Cursor->f = f;
}

// return total node of the list
unsigned int CQueue::GetCount()
{
	return Count;
}

//
