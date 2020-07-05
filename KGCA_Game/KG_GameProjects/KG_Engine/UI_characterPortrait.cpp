#include "UI_characterPortrait.h"

KYS::UI_characterPortrait::UI_characterPortrait()
{
}

KYS::UI_characterPortrait::~UI_characterPortrait()
{
}

void KYS::UI_characterPortrait::SetValue(UI_VALUE type, int paramCount, ...)
{
}

bool KYS::UI_characterPortrait::Init()
{
	return false;
}

bool KYS::UI_characterPortrait::Frame()
{
	UI_obj::Frame();
	return false;
}

bool KYS::UI_characterPortrait::Render(KG_Camera * camera, ID3D11Device * _device, ID3D11DeviceContext * _context)
{
	UI_obj::Render(camera, _device, _context);
	return false;
}

bool KYS::UI_characterPortrait::Release()
{
	return false;
}
