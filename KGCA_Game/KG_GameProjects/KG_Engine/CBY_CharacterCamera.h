#pragma once
#include "KG_Camera.h"

namespace CBY
{
	class CBY_CharacterCamera : public KG_Camera
	{
	public:
		KG_Box m_CameraBox;
		D3DXMATRIX m_mModelLastRot;
		D3DXMATRIX m_mModelRot;
		bool m_bClip;
		POINT m_Center;
		D3DXVECTOR3 m_MousePos;
		float m_fBeginYaw;
		float m_fBeginPitch;
		float m_fAfterYaw;
		float m_fAfterPitch;

	public:
		virtual void CameraMove();
		virtual bool Frame();

	public:
		virtual void Update(D3DXVECTOR4 rotvalue);
		virtual void SetClientCursor(fPOINT pot);
		virtual bool SetViewProj(float fFar = 1000.0f, float fNear = 1.0f);

	public:
		virtual void SetCameraSpeed(float fspeed);
		inline virtual void SetCameraBox(KG_Box box) { m_CameraBox = box; }
		inline virtual void SetBeginYawRot() { m_fYaw = m_fBeginYaw; }
		//virtual float GetCameraSpeed(float fspeed);

	public:
		inline virtual KG_Box GetCameraBox() {return m_CameraBox;}

	public:
		CBY_CharacterCamera();
		virtual ~CBY_CharacterCamera();
	};
}

