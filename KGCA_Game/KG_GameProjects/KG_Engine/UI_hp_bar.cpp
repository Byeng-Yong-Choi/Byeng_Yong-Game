#include "UI_hp_bar.h"

KYS::UI_hp_bar::UI_hp_bar()
{
}

KYS::UI_hp_bar::~UI_hp_bar()
{
}

void KYS::UI_hp_bar::SetValue(UI_VALUE type, int paramCount, ...)
{
	va_list ap = nullptr;
	float value[4] = { 0 };

	va_start(ap, paramCount);

	for (int count = 0; count < paramCount; count++)
	{
		value[count] = static_cast<float>(va_arg(ap, double));
	}
	
	D3DXVECTOR3 scale;

	switch (type)
	{
	case  UI_VALUE::UI_VALUE_ADD_HP:
	{
		AddHp(value[0]);

		scale = _scale;
		scale.x += (value[0]/ _hpMax) *_scaleMax.x;
		SetScaleLimit(scale);
	}
	break;
	case  UI_VALUE::UI_VALUE_ADD_HPMAX:
	{
		_hpMax += value[0];
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_HP:
	{
		SetHp(value[0]);
		scale = _scale;
		scale.x = (value[0] / _hpMax) *_scaleMax.x;
		SetScaleLimit(scale);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_HPMAX:
	{
		_hpMax = value[0];
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_INIT_HP_HPMAX:
	{
		_hpMax = value[1];
		SetHp(value[0]);
		scale = _scale;
		scale.x = (value[0] / _hpMax) *_scaleMax.x;
		SetScaleLimit(scale);
	}
	break;

	}

	UpdateUv();
	
}

void KYS::UI_hp_bar::UpdateUv()
{
	m_VerTex[1].t.x = _hp / _hpMax;
	m_VerTex[3].t.x = _hp / _hpMax;

	m_obj.m_pContext->UpdateSubresource(
		m_obj.m_pVertexBuffer.Get(),
		0, nullptr,
		&m_VerTex.at(0), 0, 0);


}



void KYS::UI_hp_bar::AddHp(float hp)
{
	_hp += hp;
	
	if (_hp > _hpMax)
		_hp = _hpMax;
	else if (_hp <= 0)
		_hp = 0;
}

void KYS::UI_hp_bar::SetHp(float hp)
{
	_hp = hp;

	if (_hp > _hpMax)
		_hp = _hpMax;
	else if (_hp <= 0)
		_hp = 0;
}
