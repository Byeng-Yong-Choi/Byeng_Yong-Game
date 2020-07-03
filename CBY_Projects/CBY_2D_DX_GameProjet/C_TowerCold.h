#pragma once
#include "TowerSrc.h"

class C_TowerCold :public TowerSrc
{
public:
	float fSpeedSlow[2];

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void TextArray(int i);
	void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj) override;
	void SetWeapon();
	TowerWeapon* GetWeapon() override;
	C_EffectSrc* GetEffect() override;

public:
	C_TowerCold();
	~C_TowerCold();
};

