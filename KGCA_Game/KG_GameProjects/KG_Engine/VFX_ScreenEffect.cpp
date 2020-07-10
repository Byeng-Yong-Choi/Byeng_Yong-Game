
#include "VFX_ScreenEffect.h"
#include "KG_Camera.h"

KYS::VFX_ScreenEffect::VFX_ScreenEffect()
{
}

KYS::VFX_ScreenEffect::~VFX_ScreenEffect()
{
}

bool KYS::VFX_ScreenEffect::Init()
{
	VFX_EffectObj::Init();
	return false;
}

bool KYS::VFX_ScreenEffect::Frame()
{
	VFX_EffectObj::Frame();
	return false;
}

bool KYS::VFX_ScreenEffect::Render()
{
	CDX::ApplyDSS(m_obj.m_pContext, CDX::KG_DxState::g_pDSSDisable);
	VFX_EffectObj::Render();
	CDX::ApplyDSS(m_obj.m_pContext, CDX::KG_DxState::g_pDSS);

	
	return false;
}

bool KYS::VFX_ScreenEffect::Release()
{
	return false;
}

void KYS::VFX_ScreenEffect::SetMatrix(D3DXMATRIX * world, KG_Camera * camera)
{
	
	VFX_EffectObj::SetMatrix(world, &camera->m_orthoView, &camera->m_OrthoProj);
}
