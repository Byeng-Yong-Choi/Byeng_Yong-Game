#pragma once
#include <iostream>

template <class T>
class CBY_Node
{
private:
	CBY_Node<T>* m_pFrontNode;		//�ճ��
	CBY_Node<T>* m_pBackNode;		//�޳��
	T m_Data;
	int m_iIndex;					//����ȣ

public:
	void MakeNode(T data,int index);	//��� ����
	void Delete();				//��� ����

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

