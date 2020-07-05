#include "CBY_HeroFire.h"
#include "CBY_BulletMgr.h"
#include "VFX_ObjMgr.h"
#include "MyEffectParser.h"
#include "KG_SoundMgr.h"
#include "UI_objMgr.h"

namespace CBY
{
	void CBY_HeroFire::Process()
	{
		m_pHero->SetState(CHAR_FIRE);
		bool bFire = false;
		bool bMove = false;


		//SetColDirection();
		if (!m_pHero->GetAmmo())		//총알이 없을시
		{
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_DIE]]->Play(I_SOUND.m_pSystem);
			m_pHero->SetAction(EVENT_IDLE);
			return;
		}

		if (m_pHero->GetStateAnimationStart(CHAR_FIRE))
		{
			if (!m_bBeforFire)
			{
				I_SOUND.list[m_pHero->m_dwSoundList[SOUND_ATTACK]]->EffectPlay(I_SOUND.m_pSystem);

				D3DXVECTOR3 shootpos = m_pHero->GetFirePos(0);
				shootpos += (m_pHero->GetFireSocketMove().x * m_pHero->GetCamera()->m_SideDir);	//캐릭터가 보는 SideDirection으로 원하는 소켓 이동
				shootpos += ((m_pHero->GetFireSocketMove().z) * m_pHero->GetCamera()->m_LookDir);//캐릭터가 보는 LookDirection으로 원하는 소켓 이동
				shootpos.y += m_pHero->GetFireSocketMove().y;									//소켓 높이 이동
				CBY::CBY_CHAR_BULLET.BulletMake(m_pHero->GetBullet(), shootpos,
					m_pHero->GetCamera()->m_LookDir, &m_pHero->GetCamera()->m_World);
				m_fFireTime = 0.0f;
				D3DXVECTOR3 scale(10, 10, 0);
				//m_pHero->obj2 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_GUN1_SHOT);

				D3DXMATRIX mat = m_pHero->GetCamera()->m_World;
				mat._41 = shootpos.x;
				mat._42 = shootpos.y;
				mat._43 = shootpos.z;

				//m_pHero->obj2->SetScale(scale);
				//m_pHero->obj2->Execute(&mat);

				m_pHero->SetAmmo(1); //총알 감소
				UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_ADD_AMMO, 1, -1);
				bFire = true;
				m_bBeforFire = true;
			}
			else
			{
				m_bBeforFire = false;
			}
		}

		if (m_pHero->GetStateAnimationEnd(CHAR_FIRE))
		{
			m_fFireTime = 0;
		}

		if (I_Input.GetKeyCheck(VK_SPACE))
		{
			m_pHero->SetAction(EVENT_JUMP);
			return;
		}

		if (I_Input.GetKeyCheck(VK_SHIFT))
		{
			m_pHero->SetAction(EVENT_RUN);
			return;
		}
		m_vMove = m_pHero->GetHeroPos();
		m_vLook = m_pHero->GetCamera()->m_LookDir;
		m_vSide = m_pHero->GetCamera()->m_SideDir;

		CharcterMove(false);

		if (bFire)
		{
			m_pHero->SetHeroPos(m_vMove);
			return;

		}
		else if( bMove && !bFire)
		{
			m_pHero->SetHeroPos(m_vMove);
			m_fFireTime = m_pHero->GetFireTime();
			m_pHero->SetAction(EVENT_MOVE);
			return;
		}
		else
		{
			m_fFireTime = m_pHero->GetFireTime();
			m_pHero->SetAction(EVENT_IDLE);
		}
	}

	CBY_HeroFire::CBY_HeroFire(CBY_Hero* host) : CBY_HeroProcess(host)
	{
		m_fFireTime = m_pHero->GetFireTime();
		m_bBeforFire = false;
	}


	CBY_HeroFire::~CBY_HeroFire()
	{
	}
}
