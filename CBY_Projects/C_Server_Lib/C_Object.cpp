#include "C_Object.h"



C_Object::C_Object()
{
	InitializeCriticalSection(&m_CS);
}


C_Object::~C_Object()
{
	DeleteCriticalSection(&m_CS);
}
