#include "UI_Ammo_font.h"

KYS::UI_Ammo_font::UI_Ammo_font()
{
	_ammoCount = 0;
}

KYS::UI_Ammo_font::~UI_Ammo_font()
{
}

void KYS::UI_Ammo_font::SetValue(UI_VALUE type, int paramCount, ...)
{
	va_list ap = nullptr;
	int value = 0;

	va_start(ap, paramCount);

	for (int count = 0; count < paramCount; count++)
	{
		value = static_cast<float>(va_arg(ap, int));
	}

	switch (type)
	{
	case  UI_VALUE::UI_VALUE_ADD_AMMO:
	{
		AddAmmo(value);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_AMMO:
	{
		SetAmmo(value);
	}
	
	}

	UpdateUv();
	
}

void KYS::UI_Ammo_font::UpdateUv()
{
	_uv = _texSprite.getTextureUv(_ammoCount);

	m_VerTex[0].t = _uv._uv[0];
	m_VerTex[1].t = _uv._uv[1];
	m_VerTex[2].t = _uv._uv[2];
	m_VerTex[3].t = _uv._uv[3];

	m_obj.m_pContext->UpdateSubresource(
		m_obj.m_pVertexBuffer.Get(),
		0, nullptr,
		&m_VerTex.at(0), 0, 0);


}

bool KYS::UI_Ammo_font::Init()
{
	_texSprite.createTextureAnimInfo();
	UpdateUv();
	return false;
}

void KYS::UI_Ammo_font::SetAmmo(int ammo)
{
	_ammoCount = ammo;

	if (_ammoCount > AMMOFONT_MAX)
		_ammoCount = AMMOFONT_MAX;
	else if (_ammoCount < AMMOFONT_MIN)
		_ammoCount = AMMOFONT_MIN;
}

void KYS::UI_Ammo_font::AddAmmo(int ammo)
{
	_ammoCount += ammo;

	if (_ammoCount > AMMOFONT_MAX)
		_ammoCount = AMMOFONT_MAX;
	else if (_ammoCount < AMMOFONT_MIN)
		_ammoCount = AMMOFONT_MIN;
}

