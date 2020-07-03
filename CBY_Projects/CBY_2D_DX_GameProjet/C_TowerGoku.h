#pragma once
#include "TowerSrc.h"

class C_TowerGoku:public TowerSrc
{
public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void TextArray(int i);
	void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj) override;
	void SetWeapon();
public:
	C_TowerGoku();
	~C_TowerGoku();
};

