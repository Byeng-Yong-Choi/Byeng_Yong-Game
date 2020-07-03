#pragma once
#include "CSprite.h"
#include "TowerWeapon.h"
#include "C_GameStd.h"
//#include "CharMgr.h"

enum MonsterState
{
	MONSTER_MOVE = 0,
	MONSTER_DAMAGE,
	MONSTER_DEATH,
	MONSTER_COUNT
};

enum MonsterSpeed
{
	MONSTER_SPEED = 10,
};

class C_MonsterSrc: public CSprite
{
private:
	int StateNUM;

public:
	std::vector<RECT> m_RtList;

public:
	D3DXMATRIX m_mat;
	D3DXMATRIX m_world;
	D3DXMATRIX m_view;
	D3DXMATRIX m_proj;
	D3DXVECTOR3 m_Pos;			//캐릭터의 위치 변화량
	D3DXVECTOR3 m_Center;		//캐릭터의 센터 위치
	D3DXVECTOR3 m_Size;			//센터에서 더해줄 크기이다
	fRECT m_fRect;
	int m_iHp;
	float m_fChangTime;			//스프라이트 변화 시간;

public:
	T_STR_VECTOR m_str;

public:
	std::vector<std::shared_ptr<C_MonsterSrc>> m_StateList;
	TowerWeapon m_Weapon;
	CSprite m_Sprite;

public:
	void SetAniTime(float Time);

public:
	virtual HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	HRESULT CreateVertexData() override;
	virtual void TextSrc();
	virtual void TextArray(int a);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual void SetState(int i);
	virtual int GetState();
	virtual bool GetSpliteEnd();
	void RectSet(D3DXVECTOR3 pos);
	void SetMonsterHP(int i);
public:
	C_MonsterSrc();
	virtual ~C_MonsterSrc();
};



