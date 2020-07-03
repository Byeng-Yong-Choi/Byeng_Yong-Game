#include "C_Character.h"

std::ostream& operator << (std::ostream& stream, C_Character& info)
{
	stream << info.m_Info;
	return stream;
}
std::istream& operator >> (std::istream& stream, C_Character& info)
{
	stream >> info.m_Info;
	return stream;
}

C_Character::C_Character()
{
	ZeroMemory(&m_Info, sizeof(USERPAKET_CHARACTER_INFO));
}


C_Character::~C_Character()
{
}
