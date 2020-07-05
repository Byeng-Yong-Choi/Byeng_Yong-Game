#pragma once
#include "UI_panel.h"
namespace KYS
{
#define AMMO_COUNT 2
#define AMMOMAX_COUNT 3
	class UI_weaponPanel : public UI_panel
	{
	public:
		UI_weaponPanel();
		virtual ~UI_weaponPanel();
	public:
		void UpdateAmmoData();
		virtual void SetType(UI_TYPE type, UI_VALUE value, int paramCount, ...) override;
		virtual void SetValue(UI_VALUE type, int paramCount, ...) override;
		virtual UI_obj* AddUI(UI_obj* obj) override;
	public:

		virtual bool Init()		override;
		virtual bool Frame()	override;
		virtual bool Release()	override;



	public:
		void SetAmmoCount(int ammo);
		void AddAmmoCount(int ammo);
		int GetAmmoCount() { return _curAmmoCount; }

		void SetAmmoMaxCount(int ammo) { _ammoMaxCount = ammo; }
		void AddAmmoMAXCount(int ammo);
		int GetAmmoMaxCount() { return _ammoMaxCount; }

		void SetMagazineMaxCount(int count) { _magazineMaxCount = count; }
		int GetMagazineMaxCount() { return _magazineMaxCount; }

		void SetAmmoObj(UI_obj* obj);
		void SetAmmoMaxObj(UI_obj* obj);
	private:
		int _curAmmoCount;
		int _ammoMaxCount;
		int _magazineMaxCount;
		UI_obj* _ammo[AMMO_COUNT];
		UI_obj* _ammoMax[AMMOMAX_COUNT];
	};
}


