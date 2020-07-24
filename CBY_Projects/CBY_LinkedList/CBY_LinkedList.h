#pragma once
#include "CBY_Node.cpp"

template <class T>
class CBY_LinkedList
{
private:
	int m_iNodeSize;
	int m_iNodeIndex;
	CBY_Node<T>* m_pHead;		//�� ��
	CBY_Node<T>* m_pTail;		//�� ��

public:
	void Init();
	void Release();

public:
	void FrontNodeIn(T data);		//����
	void BackNodeIn(T data);
	void FrontInsert(CBY_Node<T>* node, T data);
	void BackInsert(CBY_Node<T>* node, T data);
	CBY_Node<T>* FrontNodePop();	//����
	CBY_Node<T>* BackNodePop();
	bool FrontNodeerase();
	bool BackNodeerase();
	void Erase(CBY_Node<T>* Node);
	CBY_Node<T>* FindNode(T data);			//Ž��
	CBY_Node<T>* SerchNode(CBY_Node<T>* node, T data);
	void NodeDataChanges(CBY_Node<T>* node, T data);	//����

public:
	CBY_Node<T>* GetHead() { return m_pHead; }
	CBY_Node<T>* GetTail() { return m_pTail; }
	int GetSize() { return m_iNodeSize; }

public:
	CBY_LinkedList<T>();
	~CBY_LinkedList<T>();
};

template <class T>
void CBY_LinkedList<T>::Init()
{
	m_pHead = new CBY_Node<T>;
	m_pTail = new CBY_Node<T>;
	m_pHead->SetBackNode(m_pTail);
	m_pTail->SetFrontNode(m_pHead);
}

template <class T>
void CBY_LinkedList<T>::Release()
{
	while (1)
	{
		if (!BackNodeerase())
		{
			break;
		}
	}

	if (m_pHead != nullptr)delete m_pHead;
	if (m_pTail != nullptr)delete m_pTail;
}

template <class T>
void CBY_LinkedList<T>::FrontNodeIn(T data)
{
	CBY_Node<T>* pNew = new CBY_Node<T>;
	pNew->MakeNode(data, m_iNodeIndex++);

	if (m_pHead->GetBackNode() == m_pTail)		//����Ʈ�� ���� ��
	{
		m_pHead->SetBackNode(pNew);
		pNew->SetFrontNode(m_pHead);
		pNew->SetBackNode(m_pTail);
		m_pTail->SetFrontNode(pNew);
	}
	else										//����Ʈ�� ���� ��
	{
		CBY_Node<T>* temp = m_pHead->GetBackNode();
		pNew->SetFrontNode(m_pHead);
		temp->SetFrontNode(pNew);
		pNew->SetBackNode(temp);
		m_pHead->SetBackNode(pNew);
	}
	m_iNodeSize++;
}

template <class T>
void CBY_LinkedList<T>::BackNodeIn(T data)
{
	CBY_Node<T>* pNew = new CBY_Node<T>;
	pNew->MakeNode(data, m_iNodeIndex++);

	if (m_pTail->GetFrontNode() == m_pHead)		//����Ʈ�� ���� ��
	{
		m_pTail->SetFrontNode(pNew);
		pNew->SetFrontNode(m_pHead);
		pNew->SetBackNode(m_pTail);
		m_pHead->SetBackNode(pNew);
	}
	else										//����Ʈ�� ���� ��
	{
		CBY_Node<T>* temp = m_pTail->GetFrontNode();
		temp->SetBackNode(pNew);
		pNew->SetBackNode(m_pTail);
		pNew->SetFrontNode(temp);
		m_pTail->SetFrontNode(pNew);
	}
	m_iNodeSize++;
}

template <class T>
void CBY_LinkedList<T>::FrontInsert(CBY_Node<T>* node, T data)
{
	CBY_Node<T>* pNew = new CBY_Node<T>;
	pNew->MakeNode(data, m_iNodeIndex++);

	CBY_Node<T>* pFront = node->GetFrontNode();

	pFront->SetBackNode(pNew);
	pNew->SetFrontNode(pFront);
	pNew->SetBackNode(node);
	node->SetFrontNode(pNew);
}

template <class T>
void CBY_LinkedList<T>::BackInsert(CBY_Node<T>* node, T data)
{
	CBY_Node<T>* pNew = new CBY_Node<T>;
	pNew->MakeNode(data, m_iNodeIndex++);

	CBY_Node<T>* pBack = node->GetFrontNode();

	pBack->SetBackNode(pNew);
	pNew->SetBackNode(pBack);
	pNew->SetFrontNode(node);
	node->SetBackNode(pNew);
}

template <class T>
CBY_Node<T>* CBY_LinkedList<T>::FrontNodePop()
{
	CBY_Node<T>* temp = nullptr;
	if (m_pHead->GetBackNode() == m_pTail)		//����Ʈ�� ���� ��
	{
	}
	else										//����Ʈ�� ���� ��
	{
		temp = m_pHead->GetBackNode();
		temp->GetBackNode()->SetFrontNode(m_pHead);		//�� ����� �� ��� ����
		m_pHead->SetBackNode(temp->GetBackNode());	//�� ����� �� ��� ����
	}
	m_iNodeSize--;
	return temp;
}

template <class T>
CBY_Node<T>* CBY_LinkedList<T>::BackNodePop()
{
	CBY_Node<T>* temp = nullptr;
	if (m_pTail->GetFrontNode() == m_pHead)		//����Ʈ�� ���� ��
	{
	}
	else										//����Ʈ�� ���� ��
	{
		temp = m_pTail->GetFrontNode();
		temp->GetFrontNode()->SetBackNode(m_pTail);		//�� ����� �� ��� ����
		m_pTail->SetFrontNode(temp->GetFrontNode());	//�� ����� �� ��� ����
	}
	m_iNodeSize--;
	return temp;
}



template <class T>
bool CBY_LinkedList<T>::FrontNodeerase()
{
	if (m_pHead->GetBackNode() == m_pTail)		//����Ʈ�� ���� ��
	{
		return false;
	}
	else										//����Ʈ�� ���� ��
	{
		CBY_Node<T>* temp = m_pHead->GetBackNode();
		temp->GetBackNode()->SetFrontNode(m_pHead);		//�� ����� �� ��� ����
		m_pHead->SetBackNode(temp->GetBackNode());	//�� ����� �� ��� ����
		temp->Delete();
		delete temp;
	}
	m_iNodeSize--;
	return;
}

template <class T>
bool CBY_LinkedList<T>::BackNodeerase()
{
	if (m_pTail->GetFrontNode() == m_pHead)		//����Ʈ�� ���� ��
	{
		return false;
	}
	else										//����Ʈ�� ���� ��
	{
		CBY_Node<T>* temp = m_pTail->GetFrontNode();
		temp->GetFrontNode()->SetBackNode(m_pTail);		//�� ����� �� ��� ����
		m_pTail->SetFrontNode(temp->GetFrontNode());	//�� ����� �� ��� ����
		temp->Delete();
		delete temp;
	}
	m_iNodeSize--;
	return true;
}

template <class T>
void CBY_LinkedList<T>::Erase(CBY_Node<T>* Node)
{
	CBY_Node<T>* pFront = Node->GetFrontNode();
	CBY_Node<T>* pBack = Node->GetBackNode();

	pFront->SetBackNode(pBack);
	pBack->SetFrontNode(pFront);

	Node->Delete();
	delete Node;
}

template <class T>
CBY_Node<T>* CBY_LinkedList<T>::SerchNode(CBY_Node<T>* node,T data)
{
	CBY_Node<T>* pNode = node->GetBackNode();
	if (pNode == m_pTail)
	{
		return nullptr;
	}

	if (pNode->GetData() == data)
	{
		return pNode;
	}
	else
	{
		return SerchNode(pNode,data);
	}
}

template <class T>
CBY_Node<T>* CBY_LinkedList<T>::FindNode(T data)
{
	return SerchNode(m_pHead,data);
}

template <class T>
void CBY_LinkedList<T>::NodeDataChanges(CBY_Node<T>* node, T data)
{
	node->SetData(data);
}


template <class T>
CBY_LinkedList<T>::CBY_LinkedList<T>()
{
	m_iNodeIndex = 0;
	m_iNodeSize = 0;
}

template <class T>
CBY_LinkedList<T>::~CBY_LinkedList<T>()
{
}

