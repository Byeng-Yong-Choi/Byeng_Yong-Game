#pragma once
#include "CSprite.h"

class C_EffectSrc :public CSprite
{
public:
	int WeaponSpeed;

public:
	D3DXVECTOR3 m_Center;
	D3DXVECTOR3 m_Size;
	fRECT m_fRect;
	D3DXMATRIX m_World;
	int m_iSound;

public:
	HRESULT CreateVertexData() override;

public:
	void RectSet(D3DXVECTOR3 pos);
	void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);


public:
	C_EffectSrc();
	~C_EffectSrc();
};

