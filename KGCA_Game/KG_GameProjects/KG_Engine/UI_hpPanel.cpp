#include "UI_hpPanel.h"
#include "KG_Camera.h"

KYS::UI_hpPanel::UI_hpPanel()
{
}

KYS::UI_hpPanel::~UI_hpPanel()
{
}

void KYS::UI_hpPanel::SetValue(UI_VALUE type, int paramCount, ...)
{
}

void KYS::UI_hpPanel::SetType(UI_TYPE type, UI_VALUE valueType, int paramCount, ...)
{
	KYS::UI_obj* findObj = nullptr;
	va_list ap = nullptr;
	float value[4] = { 0 };

	va_start(ap, paramCount);

	for (int count = 0; count < paramCount; count++)
	{
		value[count] = static_cast<float>(va_arg(ap, double));
	}

	switch (type)
	{
	case UI_TYPE::UI_TYPE_HP_BAR:
	{
		GetUIObj(UI_TYPE::UI_TYPE_HP_BAR)->SetValue(valueType, paramCount,value[0], value[1], value[2], value[3]);
		GetUIObj(UI_TYPE::UI_TYPE_HP_BAR_INNER)->SetValue(valueType, paramCount, value[0], value[1], value[2], value[3]);
	}
	break;
	case UI_TYPE::UI_TYPE_HP_BAR_FRAME:
	{
		GetUIObj(UI_TYPE::UI_TYPE_HP_BAR_FRAME)->SetValue(valueType, paramCount, value[0], value[1], value[2], value[3]);
	}
	break;
	case UI_TYPE::UI_TYPE_BOSS_HP_BAR:
	{
		findObj = GetUIObj(UI_TYPE::UI_TYPE_BOSS_HP_BAR);
		if (findObj)
		{
			findObj->SetValue(valueType, paramCount, value[0], value[1], value[2], value[3]);
		}

		findObj = GetUIObj(UI_TYPE::UI_TYPE_BOSS_HP_BAR_INNER);
		if (findObj)
		{
			findObj->SetValue(valueType, paramCount, value[0], value[1], value[2], value[3]);
		}
	}
	break;
	case UI_TYPE::UI_TYPE_BOSS_HP_BAR_FRAME:
	{
		findObj = GetUIObj(UI_TYPE::UI_TYPE_BOSS_HP_BAR_FRAME);
		if (findObj)
		{
			findObj->SetValue(valueType, paramCount, value[0], value[1], value[2], value[3]);
		}
	}
	break;
	}

}

bool KYS::UI_hpPanel::Init()
{
	return false;
}

bool KYS::UI_hpPanel::Frame()
{
	UI_obj::Frame();
	ApplyToChildren(m_matWorld);

	return false;
}





bool KYS::UI_hpPanel::Release()
{
	return false;
}
