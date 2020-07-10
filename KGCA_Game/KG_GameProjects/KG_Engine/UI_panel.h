#pragma once
#include "UI_obj.h"

class KG_Camera;
namespace KYS
{
	class UI_panel : public UI_obj
	{
	public:
		UI_panel();
		virtual ~UI_panel();
	public:
		virtual UI_obj* AddUI(UI_obj* obj);
		UI_obj* GetUIObj(UI_obj* obj);
		UI_obj* GetUIObj(UI_TYPE type);
		void ApplyToChildren(D3DXMATRIX mat);

	public:
		virtual bool Frame() override;
	
	protected:
		std::vector<UI_obj*> _objList;
		//std::shared_ptr<UI_obj> _objList;
	};
}

