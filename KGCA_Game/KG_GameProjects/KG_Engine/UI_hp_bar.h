#pragma once
#include "UI_status_bar.h"
class KG_Camera;
namespace KYS
{
	class UI_hp_bar : public UI_status_bar
	{
	public:
		UI_hp_bar();
		virtual ~UI_hp_bar();

	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) override;
		virtual void UpdateUv() override;
	public:

	public:
		void AddHp(float hp);
		void SetHp(float hp);
		float GetHp() { return _hp; }

	private:
		float _hp;
		float _hpMax;
	};
}
