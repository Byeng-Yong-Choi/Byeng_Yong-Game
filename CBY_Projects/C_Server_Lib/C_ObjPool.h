#pragma once
#include "C_Object.h"

template<class T>
class C_ObjPool
{
public:
	enum
	{
		POOL_MAX_SIZE = 2048,
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1
	};
	static void Init();
	static void Release();
	static void* operator new(size_t size);
	static void operator delete(void* obj);

private:
	static void* volatile m_pPool[POOL_MAX_SIZE];
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;

public:
	C_ObjPool();
	~C_ObjPool();
};

template<class T>
void* volatile C_ObjPool<T>::m_pPool[POOL_MAX_SIZE] = {};
template<class T>
long long volatile C_ObjPool<T>::m_HeadPos = 0;
template<class T>
long long volatile C_ObjPool<T>::m_TailPos = 0;

template<class T>
void C_ObjPool<T>::Init()
{
	for (int i = 0; i < POOL_MAX_SIZE; i++)
	{
		m_pPool[i] = _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);
	}
	m_TailPos = POOL_MAX_SIZE;
}

template<class T>
void C_ObjPool<T>::Release()
{
	for (int i = 0; i < POOL_MAX_SIZE; i++)
	{
		void* pValue = InterlockedExchangePointer(&m_pPool[i], nullptr);
		if (pValue != nullptr)
		{
			_aligned_free(pValue);
		}
	}
}

template<class T>
void* C_ObjPool<T>::operator new(size_t size)
{
	long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
	void* pValue = InterlockedExchangePointer(&m_pPool[pos & POOL_SIZE_MASK],nullptr);
	if (pValue != nullptr)
	{
		return pValue;
	}
	return _aligned_malloc(sizeof(size), MEMORY_ALLOCATION_ALIGNMENT);
}

template<class T>
void C_ObjPool<T>::operator delete(void* obj)
{
	long long pos = InterlockedIncrement64(&m_TailPos) - 1;
	void* pValue = InterlockedExchangePointer(&m_pPool[pos & POOL_SIZE_MASK],obj);
	if (pValue != nullptr)
	{
		return _aligned_free(pValue);
	}
}


template<class T>
C_ObjPool<T>::C_ObjPool()
{
}

template<class T>
C_ObjPool<T>::~C_ObjPool()
{
}