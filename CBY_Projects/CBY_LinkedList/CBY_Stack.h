#pragma once
#include "CBY_LinkedList.h"

template <class T>
class CBY_Stack
{
private:
	CBY_LinkedList<T>* m_Link;

public:
	void Push(T data);
	T Pop();

public:
	int Size() { return m_Link.GetSize(); }

public:
	CBY_Stack();
	~CBY_Stack();
};

