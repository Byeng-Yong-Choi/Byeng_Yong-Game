#include "UI_weaponPortrait.h"

KYS::UI_weaponPortrait::UI_weaponPortrait()
{
}

KYS::UI_weaponPortrait::~UI_weaponPortrait()
{
}

void KYS::UI_weaponPortrait::SetValue(UI_VALUE type, int paramCount, ...)
{
}

bool KYS::UI_weaponPortrait::Init()
{
	return false;
}

bool KYS::UI_weaponPortrait::Frame()
{
	UI_obj::Frame();
	return false;
}

bool KYS::UI_weaponPortrait::Render(KG_Camera * camera, ID3D11Device * _device, ID3D11DeviceContext * _context)
{
	UI_obj::Render(camera, _device, _context);
	return false;
}

bool KYS::UI_weaponPortrait::Release()
{
	return false;
}
