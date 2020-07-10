#include "UI_panel.h"

KYS::UI_panel::UI_panel()
{
}

KYS::UI_panel::~UI_panel()
{
}



KYS::UI_obj* KYS::UI_panel::AddUI(UI_obj* obj)
{
	if (obj == nullptr)
		return nullptr;

	UI_obj* reObj = nullptr;
	int size = 0;

	reObj = GetUIObj(obj);
	size = _objList.size();

	if (reObj != nullptr)
		return reObj;
	else
		_objList.push_back(obj);

	return reObj;

}

KYS::UI_obj* KYS::UI_panel::GetUIObj(UI_obj* destobj)
{
	UI_obj* reObj = nullptr;
	for (auto& obj : _objList)
	{
		if (obj == destobj)
			return reObj = obj;
	}
	return reObj;
	
}

KYS::UI_obj * KYS::UI_panel::GetUIObj(UI_TYPE type)
{

	UI_obj* reObj = nullptr;
	for (auto& obj : _objList)
	{
		if (obj->GetUiType() == type)
			return reObj = obj;
	}
	return reObj;

}

void KYS::UI_panel::ApplyToChildren(D3DXMATRIX mat)
{
	for (auto& obj : _objList)
	{
		obj->SetParentMatWorld(mat);
	}
}

bool KYS::UI_panel::Frame()
{
	UI_obj::Frame();
	ApplyToChildren(m_matWorld);

	return false;
}