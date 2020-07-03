#pragma once
#include "C_NetStd.h"

class C_Packet
{
public:
	char*		m_pOffset;
	USERPAKET	m_uPacket;
	void PutData(const char* pData, int iLen);
	void GetData(const char* pData, int iLen);
public:
	C_Packet & operator << (short data);
	C_Packet & operator << (long data);
	C_Packet & operator << (float data);
	C_Packet & operator << (DWORD data);
	
	C_Packet & operator >> (short& data);
	C_Packet & operator >> (long& data);
	C_Packet & operator >> (float& data);
	C_Packet & operator >> (DWORD& data);
	
	C_Packet& operator << (int data);
	C_Packet& operator >> (int& data);
	C_Packet& operator << (char* data);
	C_Packet& operator >> (char* data);
	C_Packet& operator << (std::string data);
	C_Packet& operator >> (std::string& data);
;
public:
	C_Packet();
	C_Packet(int iType);
	virtual ~C_Packet();
};

