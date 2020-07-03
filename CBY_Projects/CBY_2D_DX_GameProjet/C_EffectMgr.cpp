#include "C_EffectMgr.h"
#include "C_SoundManager.h"

bool C_EffectMgr::Init()
{
	for (int i = 0; i < WEAPON_EFFECT_NUMBER; i++)
	{
		C_EffectSrc* wea = new C_EffectSrc;
		m_EffectQueue.Push(wea);
	}
	return true;
}

bool C_EffectMgr::Frame()
{
	for (int i = 0; i < m_pEffectList.size(); i++)
	{
		m_pEffectList[i]->Frame();
		
		if (m_pEffectList[i]->End)
		{
			EffectPop(i);
			i--;
		}
	}
	return true;
}

bool C_EffectMgr::Render()
{
	for (int i = 0; i < m_pEffectList.size(); i++)
	{
		m_pEffectList[i]->Render();
	}
	return true;
}

bool C_EffectMgr::Release()
{
	for (int i = 0; i < m_pEffectList.size(); i++)
	{
		m_pEffectList[i]->Release();
	}
	m_pEffectList.clear();
	return true;
}

void C_EffectMgr::EffectMake(C_EffectSrc* effect, D3DXMATRIX* world)
{
	C_EffectSrc* peffect = m_EffectQueue.Pop();
	*peffect = *effect;
	peffect->m_World = *world;
	D3DXVECTOR3 pos = D3DXVECTOR3(peffect->m_World._41, peffect->m_World._42, peffect->m_World._44);
	peffect->RectSet(pos);
	m_pEffectList.push_back(peffect);
	I_SOUND.list[peffect->m_iSound]->EffectPlay(I_SOUND.m_pSystem);
}

void C_EffectMgr::EffectPop(int ieffect)
{
	Effectiter iter = m_pEffectList.begin() + ieffect;
	m_EffectQueue.Push(*iter);
	m_pEffectList.erase(iter);
}

void C_EffectMgr::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	for (int i = 0; i < m_pEffectList.size(); i++)
	{
		m_pEffectList[i]->SetWorldandMatrix(world, view, proj);
	}
}

C_EffectMgr::C_EffectMgr()
{
}


C_EffectMgr::~C_EffectMgr()
{
}
