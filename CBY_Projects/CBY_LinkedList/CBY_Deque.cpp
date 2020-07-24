#include "CBY_Deque.h"

template <class T>
void CBY_Deque<T>::PushFront(T data)
{
	m_Link->FrontNodeIn(data);
}

template <class T>
void CBY_Deque<T>::PushBack(T data)
{
	m_Link->BackNodeIn(data);
}

template <class T>
T CBY_Deque<T>::PopFront()
{
	CBY_Node<T>* pNode = m_Link->FrontNodePop();
	T Data = pNode->GetData();
	pNode->Delete();
	delete pNode;
	return Data;
}

template <class T>
T CBY_Deque<T>::PopBack()
{
	CBY_Node<T>* pNode = m_Link->BackNodePop();
	T Data = pNode->GetData();
	pNode->Delete();
	delete pNode;
	return Data;
}

template <class T>
CBY_Deque<T>::CBY_Deque()
{
	m_Link = new CBY_LinkedList<T>;
	m_Link->Init();
}

template <class T>
CBY_Deque<T>::~CBY_Deque()
{
	m_Link->Release();
	delete m_Link;
}