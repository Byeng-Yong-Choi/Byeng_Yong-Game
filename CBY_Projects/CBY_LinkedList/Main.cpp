#pragma once
#include "CBY_Stack.cpp"
#include "CBY_Queue.cpp"
#include "CBY_Deque.cpp"

void main()
{
	CBY_Stack<int> Stack;
	CBY_Queue<int> Queue;
	CBY_Deque<int> Deque;

	std::cout << "Stack �Է�(���� ����)" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << std::endl;
		Stack.Push(i);
	}
	std::cout << "Stack ���" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << Stack.Pop() << std::endl;
	}

	std::cout << "Queue �Է�(���� ����)" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << std::endl;
		Queue.Push(i);
	}
	std::cout << "Queue ���" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << Queue.Pop() << std::endl;
	}

	std::cout << "Deque �����Է�,�������" << std::endl;
	std::cout << "Deque �Է�" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << std::endl;
		Deque.PushFront(i);
	}
	std::cout << "Deque ���" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << Deque.PopFront() << std::endl;
	}

	std::cout << "Deque �Ĺ��Է�,�������" << std::endl;
	std::cout << "Deque �Է�" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << std::endl;
		Deque.PushBack(i);
	}
	std::cout << "Deque ���" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << Deque.PopFront() << std::endl;
	}

	std::cout << "Deque �����Է�,�Ĺ����" << std::endl;
	std::cout << "Deque �Է�" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << std::endl;
		Deque.PushFront(i);
	}
	std::cout << "Deque ���" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << Deque.PopBack() << std::endl;
	}

	std::cout << "Deque �Ĺ��Է�,�Ĺ����" << std::endl;
	std::cout << "Deque �Է�" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << std::endl;
		Deque.PushBack(i);
	}
	std::cout << "Deque ���" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << Deque.PopBack() << std::endl;
	}
	/*CBY_LinkedList<int> m_Link;
	m_Link.Init();
	m_Link.FrontNodeIn(5);
	m_Link.FrontNodeIn(7);
	CBY_Node<int>* node = m_Link.FindNode(7);
	m_Link.NodeDataChanges(node, 3);

	for (int i = 0; i < 2; i++)
	{
		std::cout << m_Link.BackNodePop()->GetData() << std::endl;
	}
	m_Link.Release();*/

	return;
}