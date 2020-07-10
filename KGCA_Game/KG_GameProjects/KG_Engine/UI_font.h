#pragma once
#include "UI_obj.h"
#include "SpriteTexture.h"

namespace KYS
{
	class UI_font : public UI_obj
	{
	public:
		UI_font();
		virtual ~UI_font();

	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) = 0;
		virtual void UpdateUv() {};
	public:
		virtual bool Init()		override;
		virtual bool Frame()	override;
		virtual bool Render(KG_Camera* camera, ID3D11Device* _device, ID3D11DeviceContext* _context) override;
		virtual bool Release()	override;

	public:
		void SetTexSpriteInfo(Sprite_Info info) { _texSprite.setSpriteInfo(info); }
		Sprite_Info GetTexSpriteInfo() { return _texSprite.getSpriteInfo(); }

	protected:
		SpriteTexture _texSprite;
	};
}



