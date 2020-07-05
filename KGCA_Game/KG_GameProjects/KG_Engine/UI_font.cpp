#include "UI_font.h"

KYS::UI_font::UI_font()
{
}

KYS::UI_font::~UI_font()
{
}
bool KYS::UI_font::Init()
{
	return false;
}

bool KYS::UI_font::Frame()
{
	UI_obj::Frame();
	return false;
}

bool KYS::UI_font::Render(KG_Camera * camera, ID3D11Device * _device, ID3D11DeviceContext * _context)
{

	UI_obj::Render(camera, _device, _context);

	return false;
}

bool KYS::UI_font::Release()
{
	return false;
}