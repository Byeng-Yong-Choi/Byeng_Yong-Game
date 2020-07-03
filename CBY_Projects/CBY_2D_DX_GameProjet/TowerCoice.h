#pragma once
#include "C_ObjSrc.h"

class TowerCoice:public C_ObjSrc
{
public:
	bool TowerClick;
	

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void TextArray(std::vector<RECT>& rt);
	void TextSrc();

public:
	bool Frame();
	
public:
	TowerCoice();
	~TowerCoice();
};

