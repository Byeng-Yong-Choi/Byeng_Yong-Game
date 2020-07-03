#pragma once
#include "CBYNode.h"

template <class T>
class CBYLinkList
{
private:
	CBYNode<T>* LinkHead;
	CBYNode<T>* LinkTail;

public:
	CBYNode<T>* getHead() { return LinkHead; };		//헤드 반환
	CBYNode<T>* getTail() { return LinkTail; };		//꼬리 반환
	void firstNode();								//초기 메모리 생성
	void Nodefree();								//메모리 해제
	void FrontNodein(CBYNode<T>* newNode);
	void BackNodein(CBYNode<T>* newNode);
//	int numfind();									//삭제된 번호 찾아서 대입
	int Find(int a);								//노드 위치 찾기
//	T* NodeFind(int a);
	int NodeNumber();								//노드 개수 찾기

public:
	CBYLinkList();
	~CBYLinkList();
};

/*template <class T>
CBYNode<T>* CBYLinkList<T>::LinkHead = nullptr;
template <class T>
CBYNode<T>* CBYLinkList<T>::LinkTail = nullptr;*/

template <class T>
void CBYLinkList<T>::firstNode()
{
	if (LinkHead == nullptr || LinkTail == nullptr)
	{
		LinkHead = new CBYNode<T>;
		memset(LinkHead, 0, sizeof(CBYNode<T>));
		LinkTail = new CBYNode<T>;
		memset(LinkTail, 0, sizeof(CBYNode<T>));


		LinkHead->m_next = LinkTail;
		LinkTail->m_prev = LinkHead;
	}
}

template <class T>
void CBYLinkList<T>::Nodefree()
{
	CBYNode<T>* freetemp;
	if (LinkHead != nullptr || LinkTail != nullptr)
	{
		while (1)
		{
			freetemp = LinkHead->m_next;
			if (freetemp == LinkTail)
			{
				break;
			}
			LinkHead->m_next = freetemp->m_next;
			delete freetemp;

		}
		delete LinkHead;
		delete LinkTail;
		LinkHead = nullptr;
		LinkTail = nullptr;
	}

}


template <class T>
int CBYLinkList<T>::Find(int a)
{
	CBYNode<T>* Node = LinkHead->m_next;				//첫 노드 입력
	int count = 1;								//몇번째 노드인지 확인할 수
	for (int i = 1; i <= NodeNumber(); i++)
	{
		if (Node->IndexNumber == a)
		{
			return count;
		}
		count++;
		Node = Node->m_next;					//다음 노드 불러내기
	}
	return 0;
}

template <class T>
int CBYLinkList<T>::NodeNumber()							//노드 개수찾기
{
	CBYNode<T>* Node = LinkHead->m_next;
	int num = 0;

	while (1)
	{

		if (Node == LinkTail)
		{
			break;
		}
		num++;
		Node = Node->m_next;
	}

	return num;
}

template <class T>
void CBYLinkList<T>::FrontNodein(CBYNode<T>* newNode)
{
	//next 연결
	CBYNode<T>* temp = LinkHead->m_next;
	LinkHead->m_next = newNode;
	newNode->m_next = temp;

	//prev 연결
	newNode->m_prev = LinkHead;
	temp->m_prev = newNode;
}

template <class T>
void CBYLinkList<T>::BackNodein(CBYNode<T>* newNode)
{
	CBYNode<T>* temp = LinkTail->m_prev;
	LinkTail->m_prev = newNode;
	newNode->m_next = LinkTail;

	newNode->m_prev = temp;
	temp->m_next = newNode;
}

//int CBYLinkList::numfind()					//없는 숫자 집어넣기
//{
//	T* Node = LinkHead->m_next;
//	int a;
//	int size = NodeNumber();
//	for (int i = 1; i <= size; i++)
//	{
//		for (int j = 0; j < size; j++)
//		{
//			if (i == Node->studentNode.num)			//이미 있는 넘버일 경우
//			{
//				a = 0;
//				break;
//			}
//			Node = Node->m_next;
//			a = 1;
//		}
//		if (a == 1)
//		{
//			return i;					//값을 찾은 경우
//		}
//		Node = LinkHead->m_next;
//	}
//	return 0;							//값을 찾지 못한경우
//
//}
//
//T* CBYLinkList::NodeFind(int a)
//{
//	T* Node=LinkHead->m_next;
//	int num;
//	num = Find(a);
//	for (int i = 1; i < num; i++)
//	{
//		Node = Node->m_next;
//	}
//	return Node;
//}

template <class T>
CBYLinkList<T>::CBYLinkList()
{
	firstNode();
}

template <class T>
CBYLinkList<T>::~CBYLinkList()
{
	//Nodefree();
}
