#pragma once
using namespace std;

#include <iostream>
#include <time.h>		
#include <stdio.h>			
#include <stdlib.h>
#include <conio.h>
#include <string.h>

template <class T>
class CBYNode
{
public:
	CBYNode<T>();
	~CBYNode<T>();

public:
	//void swap(CBYNode<T>& Node, CBYNode<T>& Daum);					//노드 데이터 교환
	//void swappa(CBYNode<T>& Node, CBYNode<T>& Daum);				//노드의 주소값을 바꿔서 데이터 교환
	CBYNode<T>* Delete(CBYNode<T>* Node);									//노드 삭제
	CBYNode<T>* make();						//노드 새로 만들기
	/*T getData(int i);*/													//자료의 데이터 받아오기
	T ShowData();

public:
	static int nodenum;								//노드의 개수
	static int index;								//고유 노드 번호
	T NodeData;
	int IndexNumber;
	CBYNode<T>* m_next;
	CBYNode<T>* m_prev;
};


template <class T>
int CBYNode<T>::index;			//생성시 부여되는 고유 노드 넘버
template <class T>
int CBYNode<T>::nodenum;			//노드의 개수

template <class T>
CBYNode<T>::CBYNode()
{
}

//template <class T>
//T CBYNode<T>::getData(int i)
//{
//	T a(i);
//	studentNode = a;
//	return studentNode;
//}

template <class T>
CBYNode<T>* CBYNode<T>::make()
{
	index++;
	nodenum++;
	CBYNode<T>* Newp;
	Newp = new CBYNode<T>;
	T aa;
	aa = new T;
	Newp->IndexNumber = index;
	return Newp;
}

template <class T>
CBYNode<T>* CBYNode<T>::Delete(CBYNode<T>* Node)
{
	CBYNode<T>* temp = Node->m_prev;
	CBYNode<T>* temp_2 = Node->m_next;
	temp->m_next = temp_2;
	temp_2->m_prev = temp;
	delete Node;		//삭제
	nodenum--;
	return temp_2;
}

template <class T>
T CBYNode<T>::ShowData()
{
	return NodeData;
}

//template <class T>
//int CBYNode<T>::NodeNumber()					//노드 개수찾기
//{
//	CBYNode<T>* Node = Link_Head->m_next;
//	int num = 0;
//
//	while (1)
//	{
//
//		if (Node == Link_Tail)
//		{
//			break;
//		}
//		num++;
//		Node = Node->m_next;
//	}
//
//	return num;
//}

//template <class T>
/*void CBYNode<T>::swap(CBYNode<T>& Node, CBYNode<T>& Daum)
{
	CBYNode<T>* temp = new CBYNode<T>;

	strcpy_s(temp->name, 5, Node.name);
	strcpy_s(Node.name, 5, Daum.name);
	strcpy_s(Daum.name, 5, temp->name);

	temp->kuk = Node.kuk;
	Node.kuk = Daum.kuk;
	Daum.kuk = temp->kuk;

	temp->eng = Node.eng;
	Node.eng = Daum.eng;
	Daum.eng = temp->eng;

	temp->su = Node.su;
	Node.su = Daum.su;
	Daum.su = temp->su;

	temp->sum = Node.sum;
	Node.sum = Daum.sum;
	Daum.sum = temp->sum;

	temp->avg = Node.avg;
	Node.avg = Daum.avg;
	Daum.avg = temp->avg;

	delete temp;
}

//template <class T>
void student::swappa(student& Node, student& Daum)
{
	student* temp;
	student* swap;

	//바꾸는 노드들의 앞 주소 교환
	temp = Node.m_prev;
	swap = Daum.m_prev;
	temp->m_next = &Daum;
	swap->m_next = &Node;
	Daum.m_prev = temp;
	Node.m_prev = swap;

	//바꾸는 노드들의 뒷 주소 교환
	temp = Node.m_next;
	swap = Daum.m_next;
	temp->m_prev = &Daum;
	swap->m_prev = &Node;
	Node.m_next = swap;
	Daum.m_next = temp;
}*/

template <class T>
CBYNode<T>::~CBYNode()
{
}

