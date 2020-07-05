#include "UI_weaponPanel.h"

KYS::UI_weaponPanel::UI_weaponPanel()
{
	_curAmmoCount = 0;
	_ammoMaxCount = 0;
	_magazineMaxCount = 0;
	ZeroMemory(&_ammo, sizeof(_ammo));
	ZeroMemory(&_ammoMax, sizeof(_ammoMax));
}

KYS::UI_weaponPanel::~UI_weaponPanel()
{
}

void KYS::UI_weaponPanel::UpdateAmmoData()
{
	int figures[3] = { 0, };

	figures[0] = _curAmmoCount % 10;
	figures[1] = _curAmmoCount / 10;

	_ammo[0]->SetValue(UI_VALUE::UI_VALUE_SET_AMMO, 1, figures[0]);
	_ammo[1]->SetValue(UI_VALUE::UI_VALUE_SET_AMMO, 1, figures[1]);

	figures[0] = _ammoMaxCount % 10;
	figures[1] = (_ammoMaxCount % 100) / 10;
	figures[2] = _ammoMaxCount / 100;

	_ammoMax[0]->SetValue(UI_VALUE::UI_VALUE_SET_AMMO, 1, figures[0]);
	_ammoMax[1]->SetValue(UI_VALUE::UI_VALUE_SET_AMMO, 1, figures[1]);
	_ammoMax[2]->SetValue(UI_VALUE::UI_VALUE_SET_AMMO, 1, figures[2]);

}

void KYS::UI_weaponPanel::SetType(UI_TYPE type, UI_VALUE valueType, int paramCount, ...)
{
	va_list ap = nullptr;
	int value[4] = { 0 };

	va_start(ap, paramCount);

	for (int count = 0; count < paramCount; count++)
	{
		value[count] = va_arg(ap, int);
	}

	switch (type)
	{
	case UI_TYPE::UI_TYPE_AMMO_FONT:
	{
		SetValue(valueType, paramCount, value[0], value[1], value[2], value[3]);
	}
	break;

	}
}

void KYS::UI_weaponPanel::SetValue(UI_VALUE type, int paramCount, ...)
{
	va_list ap = nullptr;
	int value[4] = { 0 };

	va_start(ap, paramCount);

	for (int count = 0; count < paramCount; count++)
	{
		value[count] = static_cast<UINT>(va_arg(ap, int));
	}

	switch (type)
	{
	case  UI_VALUE::UI_VALUE_ADD_AMMO:
	{
		AddAmmoCount(value[0]);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_AMMO:
	{
		SetAmmoCount(value[0]);
	}
	break;
	case  UI_VALUE::UI_VALUE_ADD_AMMOMAX:
	{
		AddAmmoMAXCount(value[0]);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_AMMOMAX:
	{
		SetAmmoMaxCount(value[0]);
	}
	break;

	case  UI_VALUE::UI_VALUE_SET_MAGAZINEMAX:
	{
		SetMagazineMaxCount(value[0]);
	}
	break;

	case  UI_VALUE::UI_VALUE_SET_INIT_AMMO:
	{
		SetAmmoMaxCount(value[0]);
		SetMagazineMaxCount(value[1]);
		SetAmmoCount(value[2]);
	}
	break;
	}

	UpdateAmmoData();
}

KYS::UI_obj* KYS::UI_weaponPanel::AddUI(UI_obj * obj)
{
	UI_obj* reObj = UI_panel::AddUI(obj);


	return reObj;
}

void KYS::UI_weaponPanel::SetAmmoCount(int ammo)
{
	_curAmmoCount = ammo;

	if (_curAmmoCount > _magazineMaxCount)
		_curAmmoCount = _magazineMaxCount;
	else if (_curAmmoCount < 0)
		_curAmmoCount = 0;
}

void KYS::UI_weaponPanel::AddAmmoCount(int ammo)
{
	_curAmmoCount += ammo;

	if (_curAmmoCount > _magazineMaxCount)
		_curAmmoCount = _magazineMaxCount;
	else if (_curAmmoCount <= 0)
		_curAmmoCount = 0;
}

void KYS::UI_weaponPanel::AddAmmoMAXCount(int ammo)
{
	_ammoMaxCount += ammo;

	if (_ammoMaxCount <= 0)
		_ammoMaxCount = 0;
	else if (_ammoMaxCount >= 999)
		_ammoMaxCount = 999;
}

void KYS::UI_weaponPanel::SetAmmoObj(UI_obj * obj)
{
	for (int index = 0; index < AMMO_COUNT; index++)
	{
		if (_ammo[index] != nullptr)
			continue;
		if (_ammo[index] != obj)
			_ammo[index] = obj;

		break;
	}
}

void KYS::UI_weaponPanel::SetAmmoMaxObj(UI_obj * obj)
{
	for (int index = 0; index < AMMOMAX_COUNT; index++)
	{
		if (_ammoMax[index] != nullptr)
			continue;
		if (_ammoMax[index] != obj)
			_ammoMax[index] = obj;
		break;
	}
}

bool KYS::UI_weaponPanel::Init()
{
	return false;
}

bool KYS::UI_weaponPanel::Frame()
{
	UI_obj::Frame();
	ApplyToChildren(m_matWorld);
	return false;
}


bool KYS::UI_weaponPanel::Release()
{
	return false;
}
