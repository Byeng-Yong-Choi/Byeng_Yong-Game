#pragma once
#include <Windows.h>
#include <sstream>

#define PACKET_HEADER_SIZE 4
#define PACKET_MAX_DATA_SIZE 2044
#define PACKET_MAX_SIZE (PACKET_MAX_DATA_SIZE+PACKET_HEADER_SIZE)

#pragma pack(push, 1)
typedef struct
{
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER ph;
	char data[PACKET_MAX_DATA_SIZE];
}USERPAKET;
#pragma pack(pop)

#define PACKET_CHAT_MSG 2000

static std::ostream& operator << (std::ostream&out, PACKET_HEADER& h)
{
	out.write(reinterpret_cast<char*>(&h), sizeof(PACKET_HEADER));
	return out;
}
static std::istream& operator >> (std::istream& in, PACKET_HEADER& h)
{
	in.read(reinterpret_cast<char*>(&h), sizeof(PACKET_HEADER));
	return in;
}
static std::ostream& operator << (std::ostream&out, USERPAKET& h)
{
	out.write(reinterpret_cast<char*>(&h.ph), sizeof(PACKET_HEADER));
	out.write(reinterpret_cast<char*>(&h.data), h.ph.len - PACKET_HEADER_SIZE);
	return out;
}
static std::istream& operator >> (std::istream& in, USERPAKET& h)
{
	in.read(reinterpret_cast<char*>(&h.ph), sizeof(PACKET_HEADER));
	in.read(reinterpret_cast<char*>(&h.data), h.ph.len - PACKET_HEADER_SIZE);
	return in;
};

static std::stringstream MakePacket(int iType, std::stringstream* data)
{
	std::stringstream sendData;
	USERPAKET uPacket;
	uPacket.ph.type = iType;
	uPacket.ph.len = PACKET_HEADER_SIZE + data->str().length();
	sendData << uPacket;
	return std::move(sendData);
}