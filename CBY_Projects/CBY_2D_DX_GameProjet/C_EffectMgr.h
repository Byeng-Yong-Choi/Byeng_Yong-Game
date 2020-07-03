#pragma once
#include "C_EffectQueue.h"

typedef std::vector<C_EffectSrc*>::iterator Effectiter;

class C_EffectMgr :public Singleton<C_EffectMgr>
{
public:
	std::vector<C_EffectSrc*> m_pEffectList;
	C_EffectQueue m_EffectQueue;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void EffectMake(C_EffectSrc* effect, D3DXMATRIX* world);
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	void EffectPop(int ieffect);

public:
	C_EffectMgr();
	~C_EffectMgr();
};

#define TOWER_EFFECT C_EffectMgr::GetInstance()


