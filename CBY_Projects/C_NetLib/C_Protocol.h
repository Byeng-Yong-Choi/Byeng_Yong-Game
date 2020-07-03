#pragma once
#include <sstream>
//#include <Windows.h>

#define PACKET_HEADER_SIZE 6
#define PACKET_MAX_DATA_SIZE 2044
#define PACKET_MAX_SIZE ((PACKET_MAX_DATA_SIZE+PACKET_HEADER_SIZE)*10)

//************계정 아이디와 비밀번호 자릿수***********
#define ID_SIZE 8
#define PASS_SIZE 8
#define NAME_SIZE 10

#pragma pack(push, 1)
typedef struct
{
	WORD ioType;
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER ph;
	char data[PACKET_MAX_DATA_SIZE];
}USERPAKET;

typedef struct
{
	PACKET_HEADER ph;
	char ID[ID_SIZE];
	char PS[PASS_SIZE];
}USER_LOGIN_PACKET;

typedef struct
{
	PACKET_HEADER ph;
	char ID[ID_SIZE];
	char PS[PASS_SIZE];
	char Name[NAME_SIZE];
}USER_CREATEID_PACKET;
#pragma pack(pop)


//패킷 용도
#define PACKET_CHAT_MSG 2000
#define PACKET_LOGIN 3000
#define PACKET_CREATID 4000
#define LOGIN_SUCCESS 100
#define LOGIN_IDFAIL 105
#define LOGIN_PSFAIL 110
#define CREATID_SUCCESS 200
#define CREATID_FAIL 205

//패킷 처리 방식
#define BROADCAST 8000
#define INDIVIDUAL 8001

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
	PACKET_HEADER hd;
	hd.type = iType;
	hd.len = PACKET_HEADER_SIZE + data->str().length();
	sendData << hd;
	sendData << data->str();
	return std::move(sendData);
}