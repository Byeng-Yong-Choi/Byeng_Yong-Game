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
	D3DXVECTOR3 m_Center;		//캐릭터의 센터 위치
	D3DXVECTOR3 m_Size;			//센터에서 더해줄 크기이다
	D3DXMATRIX m_Rot;
	fRECT m_fRect;
	fRECT m_frTowerScope;
	float m_fScope;
	float m_fChangTime;

public:
	float Ren;							//타워 아이콘이 벌어질 길이
	float LvRen;
	bool MakeTower;						//타워 생성 유무
	int MakeTowerNum;					//만들어질 타워 번호
	bool Click;							//포지션 클릭 여부
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
	void ScopeSize(int i);				//공격 범위 설정
	void TowerWeaponrt(int i);
	void Weaponspeed(int i);
	void EffectSrc();
	int MakeTowerNumber();
	void HiddenTowerSet();
	bool bHidden();
	virtual void GameSet();
	float GetColdSlowValue();				//cold타워의 몬스터 이동속도 감속 수치 받아오기

public:
	TowerPosition();
	~TowerPosition();
};
