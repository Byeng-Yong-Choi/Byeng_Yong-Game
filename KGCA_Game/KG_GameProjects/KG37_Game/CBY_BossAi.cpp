#include "CBY_BossAi.h"
#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "UI_objMgr.h"
#include "CBY_BossStateStd.h"
#include "CBY_Hero.h" 
#include "CBY_GameMgr.h"
#include"KG_SoundMgr.h"

namespace CBY
{
	bool CBY_BossAi::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{

		return true;
	}

	bool CBY_BossAi::Init()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Init();

		D3DXVECTOR3 scale = D3DXVECTOR3(5.0f, 5.0f, 0.0f);

		CBY_BossFSM::Get().Init();

		m_dwCharState = BOSS_STATE_IDLE;

		m_ActionList[BOSS_STATE_IDLE] = new CBY_BossIdle(this);
		m_ActionList[BOSS_STATE_FLYLAND] = new CBY_BossFlyLand(this);
		m_ActionList[BOSS_STATE_WALK] = new CBY_BossWalk(this);
		m_ActionList[BOSS_STATE_HIT] = new CBY_BossHit(this);
		m_ActionList[BOSS_STATE_ATTACK] = new CBY_BossAttack(this);
		m_ActionList[BOSS_STATE_FLAME] = new CBY_BossFlame(this);
		m_ActionList[BOSS_STATE_DIE] = new CBY_BossDie(this);

		m_Action = m_ActionList[BOSS_STATE_IDLE];

		SetSleep(true);
		SetState(BOSS_SLEEP);
		SetFly(false);

		obj = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_BOSS_FIRE);
		obj2 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_BOSS_FIREBALL);
		obj3 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_BOSS_GROUND);
		return true;
	}

	void CBY_BossAi::SetAction(DWORD Event)
	{
		m_dwCharState = CBY_BossFSM::Get().Output(m_dwCharState, Event);
		m_Action = m_ActionList[m_dwCharState];
	}

	void CBY_BossAi::Process()
	{
		Frame();
		if (m_bDamge)
		{
			SetAction(EVENT_DAMAGE);
			m_bDamge = false;
		}
		m_Action->Process();
	}

	bool CBY_BossAi::Frame()
	{
		CheckMonsterHit(true);
		CBY_CharacterSrc::Frame();
		return true;
	}

	bool CBY_BossAi::Render()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Render();
		return true;
	}

	bool CBY_BossAi::Release()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Release();
		for (int i = 0; i < BOSS_STATE_COUNT; i++)
		{
			delete m_ActionList[i];
		}
		m_dwSoundList.clear();
		return true;
	}

	void CBY_BossAi::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
	{
		CBY_EnemyAi::SetMatrix(world, view, proj);

		m_FlameRay.m_Orig = m_vCharPos;
		m_FlameRay.m_Dir = m_LookDir;
	}

	void CBY_BossAi::BossSleepState()
	{
		D3DXVECTOR3 pos = GetHeroPos() - CBY_GAMEMGR.GetCharacter()->GetHeroPos();
		float fDistance = D3DXVec3Length(&pos);

		if (fDistance <= GetRadian())
		{
			I_SOUND.list[m_dwSoundList[SOUND_BOSS_APPEAR]]->Play(I_SOUND.m_pSystem);
			SetState(BOSS_SCREAM);
		}

		if (GetStateAnimationEnd(BOSS_SCREAM))
		{
			SetSleep(false);
		}
	}

	bool CBY_BossAi::CheckMonsterHit(bool bDamage)
	{
		if (GetHP() > 0)
		{
			for (int i = 0; i < CBY_CHAR_BULLET.GetBulletNum(); i++)
			{
				if (CBY_CHAR_BULLET.GetEnemyBullet(i))
				{
					continue;
				}
				KG_Box AmmoBox = CBY_CHAR_BULLET.GetBulletBox(i);

				if (KG_COLLOSION::ChkOBBToOBB(GetCharBox(), AmmoBox))
				{
					for (int ibox = 0; ibox < GetColBoxSize(); ibox++)
					{
						if (KG_COLLOSION::ChkOBBToOBB(GetColBox(ibox), AmmoBox))
						{
							CBY_CHAR_BULLET.BulletPop(i);
							D3DXVECTOR3 zpos = D3DXVECTOR3(0, 0, 0);
							zpos -= GetCamera()->m_LookDir * GetEffectVal();
							obj2 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_CHARACTER_BLOOD_IMPACT);
							D3DXVECTOR3 pos = GetColPos(ibox);
							pos += zpos;
							D3DXMATRIX mat;
							mat = m_matWorld;
							mat._41 = pos.x;
							mat._42 = pos.y;
							mat._43 = pos.z;

							//m_pBoss->obj2 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_CHARACTER_BLOOD_IMPACT);
							obj2->SetScale(D3DXVECTOR3(5, 5, 0));
							//m_pBoss->obj2->SetParentMatWorld(mat);
							obj2->Execute(&mat);


							if (bDamage)
							{
								int hp = GetHP();
								hp -= 1;
								SetHP(hp);
								I_SOUND.list[m_dwSoundList[SOUND_BOSS_HIT]]->Play(I_SOUND.m_pSystem);
								UI_MGR->find(UI_TYPE::UI_TYPE_BOSS_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_BOSS_HP_BAR, UI_VALUE::UI_VALUE_ADD_HP, 1, -1.0f);
								if (hp <= 0)
								{
									SetAction(EVENT_DEATH);
								}
							}
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	
	bool CBY_BossAi::CheckFlameHit()
	{
		KG_Box box = CBY_GAMEMGR.GetCharacter()->GetCharBox();
		if (KG_COLLOSION::ChkOBBToRay(&box,m_FlameRay))
		{
			return true;
		}
		return false;
	}


	CBY_BossAi::CBY_BossAi()
	{
	}


	CBY_BossAi::~CBY_BossAi()
	{
	}

	D3DXVECTOR3 CBY_BossProcess::GetLookDir()
	{
		m_fDirTime += g_SecondTime;
		if (m_fDirTime > 3.0f)
		{
			m_fYawAfter = m_pBoss->GetYaw();
			srand((unsigned int)time(NULL));

			int iDir = (rand()) % 2;
			if (iDir == 0)
			{
				m_fYawAfter += rand() % 10;
			}
			else
			{
				m_fYawAfter -= rand() % 10;
			}
		}
		float fStep = KG_Math::Smoothstep(0, 1, g_SecondTime);
		m_fYaw = KG_Math::Lerp(m_fYaw, m_fYawAfter, fStep);

		D3DXMATRIX matLook;
		D3DXVECTOR3 Lookdir = D3DXVECTOR3(0, 0, -1);
		D3DXVECTOR3 Sidedir = D3DXVECTOR3(1, 0, 0);

		D3DXMatrixRotationYawPitchRoll(&matLook, m_fYaw, m_fPitch, m_fRoll);

		D3DXVec3TransformCoord(&Lookdir, &Lookdir, &matLook);		//룩벡터
		D3DXVec3TransformCoord(&Sidedir, &Sidedir, &matLook);

		matLook._41 = 0;
		matLook._42 = 0;
		matLook._42 = 0;

		m_pBoss->SetLookWorldMatrix(matLook);	//회전행렬
		m_pBoss->SetYaw(m_fYaw);

		m_LookDir = Lookdir;
		m_SideDir = Sidedir;
		m_fDirTime = 0.0f;

		m_pBoss->SetLookDir(m_LookDir);
		return m_LookDir;
	}

	bool CBY_BossProcess::SetColDirection()
	{
		float fM = 0;
		D3DXVECTOR3 vN;
		//m_vMove = m_pBoss->GetHeroPos();
		m_vLookPush = D3DXVECTOR3(0, 0, 0);
		m_vSidePush = D3DXVECTOR3(0, 0, 0);

		if (KG_COLLOSION::ChkOBBToOBBAndDirection(m_pBoss->GetCharBox(), CBY_GAMEMGR.GetCharacter()->GetCharBox(), vN, fM))
		{
			float fdot = D3DXVec3Dot(&m_pBoss->GetCamera()->m_LookDir, &vN);
			if (fdot < 0)
			{
				m_vLookPush = (2 * D3DXVec3Dot(&-m_LookDir, &vN))
					* vN + m_pBoss->GetCamera()->m_LookDir;									//벽밀림
				m_vSidePush = (2 * D3DXVec3Dot(&-m_SideDir, &vN))
					* vN + m_pBoss->GetCamera()->m_SideDir;

				//m_vLook =m_LookDir - vN * (D3DXVec3Dot(&m_LookDir, &vN));
				//m_vSide =m_SideDir - vN * (D3DXVec3Dot(&m_SideDir, &vN));
				m_LookDir = m_LookDir - vN * (D3DXVec3Dot(&m_LookDir, &vN));
				m_SideDir = m_SideDir - vN * (D3DXVec3Dot(&m_SideDir, &vN));

				m_vMove += m_vLookPush * (fM + 2)*g_SecondTime;
			}
			else
			{
				m_vLookPush = (2 * D3DXVec3Dot(&-m_LookDir, &vN)) * vN
					+ m_LookDir; //벽밀림
				m_vSidePush = (2 * D3DXVec3Dot(&-m_SideDir, &vN)) * vN
					+ m_SideDir;

				//m_vLook = m_LookDir - vN * (D3DXVec3Dot(&m_LookDir, &vN));
				//m_vSide = m_SideDir - vN * (D3DXVec3Dot(&m_SideDir, &vN));
				m_LookDir = m_LookDir - vN * (D3DXVec3Dot(&m_LookDir, &vN));
				m_SideDir = m_SideDir - vN * (D3DXVec3Dot(&m_SideDir, &vN));

				m_vMove -= m_vLookPush * (fM + 2) *g_SecondTime;
			}
			//m_pBoss->SetHeroPos(m_vMove);
			return true;
		}
		return false;
	}

	void CBY_BossProcess::UpdateLook()
	{
		D3DXMATRIX matlook;
		D3DXVECTOR3 vpos;
		vpos = CBY_GAMEMGR.GetCharacter()->GetHeroPos() - m_pBoss->GetHeroPos();
		D3DXVec3Normalize(&m_LookDir, &vpos);
		m_pBoss->SetLookDir(m_LookDir);

		D3DXMatrixLookAtLH(&matlook, &CBY_GAMEMGR.GetCharacter()->GetHeroPos(),
			&m_pBoss->GetHeroPos(), &D3DXVECTOR3(0, 1, 0));
		matlook._41 = 0;
		matlook._42 = 0;
		matlook._43 = 0;

		D3DXMatrixInverse(&matlook, NULL, &matlook);
		m_pBoss->SetLookWorldMatrix(matlook);
	}

	void CBY_BossProcess::BossWalk()
	{
		D3DXVECTOR3 vmove;
		//GetLookDir();
		//SetColDirection();

		vmove = m_pBoss->GetHeroPos();
		vmove += m_LookDir * m_pBoss->GetMoveSpeed() *g_SecondTime;
		m_pBoss->SetHeroPos(vmove);
	}
}

