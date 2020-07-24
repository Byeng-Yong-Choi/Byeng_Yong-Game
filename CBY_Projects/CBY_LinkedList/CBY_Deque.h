#pragma once
#include "CBY_LinkedList.h"

template <class T>
class CBY_Deque
{
private:
	CBY_LinkedList<T>* m_Link;

public:
	void PushFront(T data);
	T PopFront();
	void PushBack(T data);
	T PopBack();

public:
	int Size() { return m_Link.GetSize(); }

public:
	CBY_Deque();
	~CBY_Deque();
};


