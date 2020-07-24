#include "CBY_Node.h"

template <class T>
void CBY_Node<T>::MakeNode(T data,int index)
{
	this->m_Data = data;
	this->m_iIndex = index;
}

template <class T>
void CBY_Node<T>::Delete()
{

}

template <class T>
CBY_Node<T>::CBY_Node<T>()
{
	m_pFrontNode = nullptr;
	m_pBackNode = nullptr;
}

template <class T>
CBY_Node<T>::~CBY_Node<T>()
{
}