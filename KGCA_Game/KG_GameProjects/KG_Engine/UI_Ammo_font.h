#pragma once
#include "UI_font.h"

namespace KYS
{
#define AMMOFONT_MIN 0
#define AMMOFONT_MAX 9
	class UI_Ammo_font : public UI_font
	{
	public:
		UI_Ammo_font();
		virtual ~UI_Ammo_font();

	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...);
		virtual void UpdateUv() override;
	public:
		virtual bool Init()		override;

	public:
		void SetAmmo(int ammo);
		int GetAmmo() { return _ammoCount; }

		void AddAmmo(int ammo);
			 
	private:
		int _ammoCount;
		MyUV _uv;
	};
}


