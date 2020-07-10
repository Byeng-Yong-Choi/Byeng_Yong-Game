#pragma once
#include "UI_obj.h"

class KG_Camera;
namespace KYS
{

	class UI_status_bar : public UI_obj
	{
	public:
		UI_status_bar() ;
		virtual ~UI_status_bar() ;

	public:
		virtual void SetValue(UI_VALUE type, int paramCount, ...) = 0;
		virtual void UpdateUv();
	public:
		virtual bool Init()		override;
		virtual bool Frame()	override;
		virtual bool Release()	override;
	
	public:
		void SetScaleLimit(D3DXVECTOR3 scale, float limit = -1.0f);
	
	public:
		void AddLength(float length);
		void SetLength(float length) { _frameLength = length; }
		float GetLength() { return _frameLength; }

		void AddLengthMax(float length);
		void SetLengthMax(float length) { _frameLengthMax = length; }
		float GetLengthMax() { return _frameLengthMax; }

	protected:
		float _frameLength;
		float _frameLengthMax;
	};
}


