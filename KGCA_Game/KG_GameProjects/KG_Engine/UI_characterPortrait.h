#pragma once
#include "UI_portrait.h"
namespace KYS
{
	class UI_characterPortrait : public UI_portrait
	{
	public:
		UI_characterPortrait();
		virtual ~UI_characterPortrait();
	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) override;
		//virtual void SetType(UI_TYPE type, UI_VALUE value, int paramCount, ...) override;
	public:

		virtual bool Init()		override;
		virtual bool Frame()	override;
		virtual bool Render(KG_Camera* camera, ID3D11Device* _device, ID3D11DeviceContext* _context)	override;
		virtual bool Release()	override;

	public:

	};
}


