#pragma once
#include "C_PlayerHP.h"
#include "C_UIMoney.h"
#include "C_Score.h"

class C_Menu: public CShapeObject
{
private:
	C_PlayerHP m_HP;
	C_UIMoney m_UIMoney;
	C_Score m_Score;

public:
	HRESULT CreateVertexData() override;

public:
	bool Frame();
	bool UIRender();
	bool UIRelease();
	void UISetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	HRESULT UICreate(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);

public:
	void SetHP(int i);
	bool PlayerDeath();
	void GameSet();

public:
	C_Menu();
	virtual ~C_Menu();
};

