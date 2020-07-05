#include "UI_status_bar.h"

KYS::UI_status_bar::UI_status_bar()
{
}

KYS::UI_status_bar::~UI_status_bar()
{
}

void KYS::UI_status_bar::UpdateUv()
{
	m_VerTex[1].t.x = _frameLength / _frameLengthMax;
	m_VerTex[3].t.x = _frameLength / _frameLengthMax;

	m_obj.m_pContext->UpdateSubresource(
		m_obj.m_pVertexBuffer.Get(),
		0, nullptr,
		&m_VerTex.at(0), 0, 0);
}

bool KYS::UI_status_bar::Init()
{
	return false;
}

bool KYS::UI_status_bar::Frame()
{
	UI_obj::Frame();
	return false;
}


bool KYS::UI_status_bar::Release()
{
	return false;
}

void KYS::UI_status_bar::SetScaleLimit(D3DXVECTOR3 scale, float limit)
{

	_scale = scale; 
	if (_scale.x <= 0)
		_scale.x = 0;

	if (limit <= 0)
	{
		if (_scale.x >= _scaleMax.x)
			_scale.x = _scaleMax.x;
	}
	else if (_scale.x >= limit)
			_scale.x = limit;
	
}

void KYS::UI_status_bar::AddLength(float length)
{
	_frameLength += length;

	if (_frameLength > _frameLengthMax)
		_frameLength = _frameLengthMax;
	else if (_frameLength <= 0)
		_frameLength = 0;
}

void KYS::UI_status_bar::AddLengthMax(float length)
{
	_frameLengthMax += length;

	if (_frameLengthMax > 999)
		_frameLengthMax = 999;
	else if (_frameLengthMax <= 0)
		_frameLengthMax = 0;
}
