#include "CBY_HeroDamage.h"
#include "KG_SoundMgr.h"
#include "CBY_EnemyAi.h"
#include "UI_objMgr.h"
#include "VFX_ObjMgr.h"

namespace CBY
{
	void CBY_HeroDamage::Process()
	{
		m_pHero->SetState(CHAR_DAMAGE);

		if (m_pHero->GetStateAnimationStart(CHAR_DAMAGE))
		{
			//»ç¿îµå
		}

		//if (m_pHero->GetStateAnimationEnd(CHAR_DAMAGE))
		//{
			int iHp = m_pHero->GetHP();
			iHp -= 1;
			UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_ADD_HP, 1, -1.0f);
			m_pHero->SetHP(iHp);

//			obj1->Execute(pos);
			obj1 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_SCREEN_BLOOD_IMPACT);
			D3DXVECTOR3 pos = D3DXVECTOR3(8.0f, 0.0f, 10.0f);
			obj1->Execute(nullptr, pos);

			if (iHp <= 0)
			{
				m_pHero->SetAction(EVENT_DEATH);
			}
			else
			{
				m_pHero->SetAction(EVENT_IDLE);
			}
		//}
	}

	CBY_HeroDamage::CBY_HeroDamage(CBY_Hero* host) : CBY_HeroProcess(host)
	{
	}


	CBY_HeroDamage::~CBY_HeroDamage()
	{
	}
}
