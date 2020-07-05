#include "UI_hp_bar_frame.h"

void KYS::UI_hp_bar_frame::SetValue(UI_VALUE type, int paramCount, ...)
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
	case  UI_VALUE::UI_VALUE_ADD_LENGTH:
	{
		AddLength(value[0]);

		scale = _scale;
		scale.x += (value[0] / _frameLengthMax) *_scaleMax.x;
		SetScaleLimit(scale);
	}
	break;
	case  UI_VALUE::UI_VALUE_ADD_LENGTHMAX:
	{
		AddLengthMax(value[0]);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_LENGTH:
	{
		SetLength(value[0]);

		scale = _scale;
		scale.x = (value[0] / _frameLengthMax) *_scaleMax.x;
		SetScaleLimit(scale);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_LENGTHMAX:
	{
		SetLengthMax(value[0]);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_INIT_LENGTH_LENGTHMAX:
	{
		SetLength(value[0]);
		SetLengthMax(value[1]);
		scale = _scale;
		scale.x = (value[0] / _frameLengthMax) *_scaleMax.x;
		SetScaleLimit(scale);
	}
	break;

	}

	UpdateUv();

}

