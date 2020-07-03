#pragma once
#include "C_Object.h"

class C_Character : public C_Object
{
public:
	USERPAKET_CHARACTER_INFO m_Info;
	friend std::ostream& operator << (std::ostream& stream, C_Character& info);
	friend std::istream& operator >> (std::istream& stream, C_Character& info);

public:
	C_Character();
	~C_Character();
};

