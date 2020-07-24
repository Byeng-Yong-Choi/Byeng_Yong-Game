#include "CBY_Queue.h"

template <class T>
void CBY_Queue<T>::Push(T data)
{
	m_Link->BackNodeIn(data);
}

template <class T>
T CBY_Queue<T>::Pop()
{
	CBY_Node<T>* pNode = m_Link->FrontNodePop();
	T Data = pNode->GetData();
	pNode->Delete();
	delete pNode;
	return Data;
}

template <class T>
CBY_Queue<T>::CBY_Queue()
{
	m_Link = new CBY_LinkedList<T>;
	m_Link->Init();
}

template <class T>
CBY_Queue<T>::~CBY_Queue()
{
	m_Link->Release();
	delete m_Link;
}