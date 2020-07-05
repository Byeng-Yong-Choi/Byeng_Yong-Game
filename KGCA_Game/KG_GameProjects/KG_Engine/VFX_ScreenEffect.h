#pragma once
#include "VFX_EffectObj.h"
namespace KYS
{
	class VFX_ScreenEffect : public VFX_EffectObj
	{
	public:
		VFX_ScreenEffect();
		virtual ~VFX_ScreenEffect();

	public:
		virtual bool Init()override;
		virtual bool Frame()override;
		virtual bool Render()override;
		virtual bool Release()override;
	public:
		virtual void SetMatrix(D3DXMATRIX* world, KG_Camera* camera)override;

	};
}
