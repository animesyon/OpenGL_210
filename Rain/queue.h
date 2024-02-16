/*
   Class Name:

      CQueue

   Description:

      linked list FIFO  (first in, first out)
*/

#pragma once

typedef struct QUEUE QUEUE;

struct QUEUE{
	float x, z, c, s;
	bool f;
	QUEUE *Next;
};

class CQueue
{
private:
	QUEUE *First,*Last,*Cursor;
	unsigned int Count;

public:
	CQueue();
	~CQueue();

	bool IsEmpty();
	void Add(float x, float z, float c, float s, bool f);
	void Remove();
	void RemoveAll();

	void MoveFirst();
	void MoveNext();
	bool IsEndOfQueue();
	void Read(float *x, float *z, float *c, float *s, bool *f);
	void Update(float c, float s);
	void Update(bool f);

	unsigned int GetCount();
};
