#include "CBY_CharacterSrc.h"
#include "KG_Math.h"
#include "UI_objMgr.h"

namespace CBY
{
	bool CBY_CharacterSrc::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		return true;
	}

	bool CBY_CharacterSrc::Init()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Init();
		m_RayCrash.resize(RAY_SIZE);
		m_dwColBox.resize(RAY_SIZE);
		return true;
	}

	bool CBY_CharacterSrc::Frame()
	{
		if (m_Character == nullptr)
			return false;

		float fMove = g_SecondTime * GetMoveSpeed();
		fMove = KG_Math::Smoothstep(0, 1, fMove);
		m_vCharPos.x = KG_Math::Lerp(m_vCharPos.x, m_vAfterCharPos.x, fMove);
		m_vCharPos.y = KG_Math::Lerp(m_vCharPos.y, m_vAfterCharPos.y, fMove);
		m_vCharPos.z = KG_Math::Lerp(m_vCharPos.z, m_vAfterCharPos.z, fMove);

		m_Character->Frame();
		return true;
	}

	bool CBY_CharacterSrc::Render()
	{
		if (m_Character == nullptr)
			return false;

		//m_Character->Render();
		m_Character->NonCharacterRender();
		return true;
	}

	bool CBY_CharacterSrc::Release()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Release();
		m_Bullet.Release();
		return true;
	}

	void CBY_CharacterSrc::InitSet()
	{
		SetHP(m_dwInitHP);
	}


	/////////////Set////////////
	void CBY_CharacterSrc::SetState(int i)
	{
		if (m_Character == nullptr)
			return;

		m_Character->SetState(i);
	}

	void CBY_CharacterSrc::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
	{
		if (m_Character == nullptr)
			return;

		if (world != nullptr)
		{
			m_vCharPos.y += m_vPosHeight;
			world->_41 = m_vCharPos.x;
			world->_42 = m_vCharPos.y;
			world->_43 = m_vCharPos.z;
			m_Character->SetMatrix(world, view, proj);
		}
		else
		{
			m_Character->SetMatrix(nullptr, view, proj);
		}
	}
	void CBY_CharacterSrc::SetFireTime(float ftime)
	{
		m_fFireSetTime = ftime;
	}
	void CBY_CharacterSrc::SetHeroPos(D3DXVECTOR3 vpos)
	{
		if (vpos.x > 245)
		{
			vpos.x = 245;
		}

		if (vpos.x < -245)
		{
			vpos.x = -245;
		}

		if (vpos.z > 245)
		{
			vpos.z = 245;
		}

		if (vpos.z < -245)
		{
			vpos.z = -245;
		}

		m_vAfterCharPos = vpos;
		//m_Character->SetRayPos(m_vAfterCharPos);
		/*if (m_dwFrame >= m_dwMoveFrame)
		{
			m_vChangPos = m_vAfterCharPos;
			m_dwFrame = 0;
		}
		else
		{
			m_dwFrame++;
		}*/

		m_vCharPos = vpos;
	}

	//////////// Get//////////////
	KG_Box CBY_CharacterSrc::GetCharBox()
	{
		if (m_Character == nullptr)
			return KG_Box();

		return m_Character->GetCharBox();
	}

	KG_Box CBY_CharacterSrc::GetColBox(int ibox)
	{
		return m_Character->GetColBox(ibox);
	}
	
	D3DXVECTOR3 CBY_CharacterSrc::GetColPos(int ibox)
	{
		return m_Character->GetColPos(ibox);
	}

	int CBY_CharacterSrc::GetColBoxSize()
	{
		return m_Character->GetColBoxSize();
	}

	D3DXVECTOR3 CBY_CharacterSrc::GetFirePos(int i)
	{
		if (m_Character == nullptr)
			return D3DXVECTOR3(0, 0, 0);
		
		
		return m_Character->GetFirePos(i);
	}

	bool CBY_CharacterSrc::GetStateAnimationStart(int i)
	{
		if (m_Character == nullptr)
			return false;

		return m_Character->GetStateAnimationStart(i);
	}

	bool CBY_CharacterSrc::GetStateAnimationEnd(int i)
	{
		if (m_Character == nullptr)
			return false;

		return m_Character->GetStateAnimationEnd(i);
	}


	bool CBY_CharacterSrc::GetFire(int i)
	{
		if (m_Character == nullptr)
			return false;

		if (GetStateAnimationStart(i))
		{
			m_bFireTime = true;
			m_fFireTime = 0.0;
		}

		if (m_bFireTime)
		{
			m_fFireTime += g_SecondTime;
		}

		if (m_fFireTime > m_fFireSetTime)
		{
			m_fFireTime = 0.0f;
			m_bFireTime = false;
			return true;
		}

		return false;
	}

	D3DXVECTOR3 CBY_CharacterSrc::GetHeroPos()
	{
		return m_vAfterCharPos;
		//return m_vCharPos;
	}

	CBY_Bullet* CBY_CharacterSrc::GetBullet()
	{
		if (m_Character == nullptr)
			return nullptr;

		return &m_Bullet;
	}

	CBY_CharacterSrc::CBY_CharacterSrc()
	{
		m_dwMoveFrame = 2;
		m_dwFrame = 5;
		m_fFireSetTime = 0;
		m_fFireTime = 0;
		m_bFireTime = false;
		m_fSpeed = 0;
		m_vCharPos = D3DXVECTOR3(0, 0, 0);
		m_bDamge = false;
	}


	CBY_CharacterSrc::~CBY_CharacterSrc()
	{
	}
}
