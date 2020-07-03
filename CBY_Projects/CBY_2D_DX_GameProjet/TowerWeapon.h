#pragma once
#include "CSprite.h"

class TowerWeapon :public CSprite
{
private:
	int m_iPower;

public:
	int WeaponSpeed;
	float fMove;
	int iMonsternum;
	bool Cold;
	int m_iTowerChar;
	bool bLV;

public:
	D3DXVECTOR3 m_Center;
	D3DXVECTOR3 m_Size;
	fRECT m_fRect;
	D3DXVECTOR3 m_vTargetpos;
	D3DXMATRIX m_World;

public:
	HRESULT CreateVertexData() override;

public:
	bool Frame();
	void SetMonsterPosition(D3DXVECTOR3 mon);
	void SetMonsterNum(int i);
	void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	void RectSet(D3DXVECTOR3 pos);
	void SetPower(int power);
	int GetPower();

public:
	TowerWeapon();
	~TowerWeapon();
};

