#pragma once
#include <iostream>

template <class T>
class CBY_Node
{
private:
	CBY_Node<T>* m_pFrontNode;		//앞노드
	CBY_Node<T>* m_pBackNode;		//뒷노드
	T m_Data;
	int m_iIndex;					//노드번호

public:
	void MakeNode(T data,int index);	//노드 생성
	void Delete();				//노드 삭제

public:
	void SetFrontNode(CBY_Node<T>* pFront) { m_pFrontNode = pFront; }
	void SetBackNode(CBY_Node<T>* pBack) { m_pBackNode = pBack; }
	void SetData(T data) { m_Data = data; }

public:
	CBY_Node<T>* GetFrontNode() { return m_pFrontNode; }
	CBY_Node<T>* GetBackNode() { return m_pBackNode; }
	T GetData() { return m_Data; }

public:
	CBY_Node<T>();
	~CBY_Node<T>();
};

