#pragma once
#pragma once
#include "UI_status_bar.h"
class KG_Camera;
namespace KYS
{
	class UI_hp_bar_Inner : public UI_status_bar
	{
	public:
		UI_hp_bar_Inner();
		virtual ~UI_hp_bar_Inner();

	public:
		void computeHp();
	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) override;
		virtual void UpdateUv() override;
		virtual bool Frame() override;
	public:
		void AddHp(float hp);
		void SetHp(float hp);
		float GetHp() { return _hp; }

	private:
		float _hp;
		float _hpMax;
		float _currentValue;
		float _goalValue;
		float _time;
		float _timeLimit;
		bool	_isChange;
	};
}
