#pragma once
#include "UI_panel.h"
namespace KYS
{
	class UI_hpPanel : public UI_panel
	{
	public:
		UI_hpPanel();
		virtual ~UI_hpPanel();
	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) override;
		virtual void SetType(UI_TYPE type, UI_VALUE value ,int paramCount, ...) override;
	public:

		virtual bool Init()		override;
		virtual bool Frame()	override;
		virtual bool Release()	override;

	public:

	};
}

