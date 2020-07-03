#include "C_Packet.h"

void C_Packet::PutData(const char* pData, int iLen)
{
	m_uPacket.ph.len += iLen;
	memcpy(m_pOffset, pData, iLen);
	m_pOffset += iLen;
}
void C_Packet::GetData(const char* pData, int iLen)
{
	memcpy(const_cast<char*>(pData), m_pOffset, iLen);
	m_pOffset += iLen;
}

C_Packet& C_Packet::operator << (int data)
{
	PutData((char*)&data, sizeof(int));
	return *this;
}
C_Packet& C_Packet::operator >> (int& data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
C_Packet& C_Packet::operator << (char* data)
{
	PutData(data, strlen(data) + 1);
	return *this;
}
C_Packet& C_Packet::operator >> (char* data)
{
	GetData(data, strlen(m_pOffset) + 1);
	return *this;
}
C_Packet& C_Packet::operator << (std::string data)
{
	PutData(data.c_str(), data.size() + 1);
	return *this;
}
C_Packet& C_Packet::operator >> (std::string& data)
{
	int iSize = strlen(m_pOffset) + 1;
	data.resize(iSize);
	GetData(const_cast<char*>(data.c_str()),
		iSize);
	return *this;
}

C_Packet& C_Packet::operator << (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
C_Packet& C_Packet::operator >> (short& data)
{
	GetData((char*)&data, sizeof(short));
	return *this;
}
C_Packet& C_Packet::operator << (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
C_Packet& C_Packet::operator >> (long& data)
{
	GetData((char*)&data, sizeof(long));
	return *this;
}
C_Packet& C_Packet::operator << (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
C_Packet& C_Packet::operator >> (float& data)
{
	GetData((char*)&data, sizeof(float));
	return *this;
}
C_Packet& C_Packet::operator << (DWORD data)
{
	PutData((char*)&data, sizeof(DWORD));
	return *this;
}
C_Packet& C_Packet::operator >> (DWORD& data)
{
	GetData((char*)&data, sizeof(DWORD));
	return *this;
}
C_Packet::C_Packet(int iType)
{
	ZeroMemory(&m_uPacket, sizeof(USERPAKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_uPacket.ph.type = iType;
	m_pOffset = m_uPacket.data;
}

C_Packet::C_Packet()
{
	ZeroMemory(&m_uPacket, sizeof(USERPAKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_pOffset = m_uPacket.data;
}

C_Packet::~C_Packet()
{
}
