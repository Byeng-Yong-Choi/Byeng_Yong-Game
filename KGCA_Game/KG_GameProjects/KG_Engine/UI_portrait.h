#pragma once
#include "UI_obj.h"


namespace KYS
{
	class UI_portrait : public UI_obj
	{

	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) = 0;
	public:

	};
}


