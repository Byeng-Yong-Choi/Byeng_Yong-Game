#pragma once
#include "CSprite.h"
#include "TowerWeapon.h"
#include "C_EffectSrc.h"
#include "C_GameStd.h"
#include "C_SoundManager.h"

#define VEGETA_FIRESPEED 0.5f //베지터의 탄 발사 감속

enum TowerName
{
	GOKU = 0,
	VEGETA,
	COLD,
	TOWERCOUNT,
	HIDDEN = TOWERCOUNT
};

enum TowerState
{
	TOWER_STANDBY = 0,
	TOWER_ATTACK,
	TOWER_LEVELUP_STATE,
	TOWER_LEVELUP_ATTACK,
	TOWER_COUNT
};

enum TowerAttackSpeed
{
	GOKU_ATTACK = 2,
	GOKU_LV_ATTACK = 3,
	GOKU_SPEED = 700,
	VEGETA_ATTACK = 5,
	VEGETA_LV_ATTACK = 10,
	VEGETA_SPEED = 450,
	COLD_SLOW = 50,
	COLD_LV_SLOW = 80,
	HIDDEN_ATTACK=8,
	HIDDEN_SPEED=600

};

enum WEAPON
{
	GOKU_DEFAULT=0,
	GOKU_LV,
	VEGETA_DEFAULT,
	VEGETA_LV
};

class TowerSrc : public CSprite
{
private:
	int iLV;

protected:
	bool bCold;
	bool bGoku;
	bool bGoKuLVAttack;
	bool bVegeta;

public:
	std::vector<RECT> m_RtList;

public:
	D3DXMATRIX m_mat;
	D3DXMATRIX m_world;
	D3DXMATRIX m_view;
	D3DXMATRIX m_proj;
	D3DXVECTOR3 m_pos;			//캐릭터의 위치 변화량
	D3DXVECTOR3 m_Center;		//캐릭터의 센터 위치
	D3DXVECTOR3 m_Size;			//센터에서 더해줄 크기이다
	fRECT m_fRect;
	int m_iPrice[2];

public:
	T_STR_VECTOR m_str;
	//std::vector<RECT> m_rtlist;
	//RECT rtsrc;
	int StateNUM;
	//ID3D11Device* m_pd3dDevice;
	//ID3D11DeviceContext* m_pContext;

public:
	std::vector<std::shared_ptr<TowerSrc>> m_StateList;
	std::vector<TowerWeapon> m_WeaponList;
	std::vector<C_EffectSrc> m_EffectList;
	CSprite m_Sprite;
	int m_Index;
	int m_iSound;

public:
	HRESULT CreateVertexData() override;

public:
	void SetAniTime(float Time);

public:
	virtual HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	virtual void SetState(DWORD dwState);
	virtual void TextSrc();
	virtual void TextArray(int a);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	bool Update(float& ftime);
	void RectSet(D3DXVECTOR3 pos);
	bool Attack();
	void SetLV(int i);
	int GetLV();
	virtual TowerWeapon* GetWeapon();
	virtual C_EffectSrc* GetEffect();

public:
	TowerSrc();
	~TowerSrc();
};

