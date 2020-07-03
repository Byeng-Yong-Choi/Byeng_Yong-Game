#pragma once
#include "GameMgr.h"
#include "C_TowerSrcMgr.h"
#include "C_HiddenTower.h"
#include "TowerCoice.h"
#include "CShapeObject.h"

typedef std::vector<RECT> RTLIST;

enum TowerScopeSize
{
	GOKUSCOPE = 95,
	VEGETASCOPE = 95,
	COLDSCOPE = 115,
	HIDDENSCOPE=110
};
	

class TowerPosition:public CShapeObject
{
public:
	std::vector<TowerSrc*> m_Tower;
	std::shared_ptr<C_TowerGoku> m_TowerGoku;
	std::shared_ptr<C_TowerVegeta> m_TowerVegeta;
	std::shared_ptr<C_TowerCold> m_TowerCold;
	std::shared_ptr<C_HiddenTower> m_TowerHidden;

public:
	std::vector<std::shared_ptr<TowerCoice>> m_TowerChoice;

public:
	D3DXMATRIX m_matWorld;
	//std::vector<PNCT_VERTEX> m_vertexdata;
	D3DXVECTOR3 m_Center;		//ĳ������ ���� ��ġ
	D3DXVECTOR3 m_Size;			//���Ϳ��� ������ ũ���̴�
	D3DXMATRIX m_Rot;
	fRECT m_fRect;
	fRECT m_frTowerScope;
	float m_fScope;
	float m_fChangTime;

public:
	float Ren;							//Ÿ�� �������� ������ ����
	float LvRen;
	bool MakeTower;						//Ÿ�� ���� ����
	int MakeTowerNum;					//������� Ÿ�� ��ȣ
	bool Click;							//������ Ŭ�� ����
	float ClickTime;
	bool MakeLvTower;
	bool bHiddenb;


public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	HRESULT CreateInit(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	HRESULT CreateVertexData() override;
	void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	void TextArray(int a);
	void RectSet(D3DXVECTOR3 pos);

public:
	void TowerRtsrc();
	void LVTowerRtsrc();
	void TowerChoicert(int i);
	void TowerLVChoicert(int i);
	void ScopeSize(int i);				//���� ���� ����
	void TowerWeaponrt(int i);
	void Weaponspeed(int i);
	void EffectSrc();
	int MakeTowerNumber();
	void HiddenTowerSet();
	bool bHidden();
	virtual void GameSet();
	float GetColdSlowValue();				//coldŸ���� ���� �̵��ӵ� ���� ��ġ �޾ƿ���

public:
	TowerPosition();
	~TowerPosition();
};
