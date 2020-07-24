#include "CBY_Stack.h"

template <class T>
void CBY_Stack<T>::Push(T data)
{
	m_Link->BackNodeIn(data);
}

template <class T>
T CBY_Stack<T>::Pop()
{
	CBY_Node<T>* pNode = m_Link->BackNodePop();
	T Data = pNode->GetData();
	pNode->Delete();
	delete pNode;
	return Data;
}

template <class T>
CBY_Stack<T>::CBY_Stack()
{
	m_Link = new CBY_LinkedList<T>;
	m_Link->Init();
}

template <class T>
CBY_Stack<T>::~CBY_Stack()
{
	m_Link->Release();
	delete m_Link;
}