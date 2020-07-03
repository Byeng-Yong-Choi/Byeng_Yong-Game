#pragma once
#include "C_MonsterSrc.h"

class C_monPig
{
public:
	std::shared_ptr<C_MonsterSrc> m_Src;

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void TextArray(int i);
	void TextSrc();
	bool Release();

public:
	C_monPig();
	~C_monPig();
};
