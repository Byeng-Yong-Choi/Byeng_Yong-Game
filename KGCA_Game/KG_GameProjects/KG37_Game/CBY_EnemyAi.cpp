#include "CBY_EnemyAi.h"
#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "CBY_EnemyStateStd.h"
#include "CBY_Hero.h" 

namespace CBY
{
	bool CBY_EnemyAi::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{

		return true;
	}

	void CBY_EnemyAi::SetAction(DWORD Event)
	{
		m_dwCharState = CBY_EnemyFSM::Get().Output(m_dwCharState, Event);
		m_Action = m_ActionList[m_dwCharState];
	}

	void CBY_EnemyAi::Process()
	{
		Frame();
		if (m_bDamge)
		{
			SetAction(EVENT_DAMAGE);
			m_bDamge = false;
		}
		m_Action->Process();
	}


	bool CBY_EnemyAi::Init()
	{
		if (m_Character == nullptr)
			return false;

		CBY_CharacterSrc::Init();

		SetMoveSpeed(7);
		D3DXVECTOR3 scale = D3DXVECTOR3(5.0f, 5.0f, 0.0f);

		CBY_EnemyFSM::Get().Init();

		m_dwCharState = CHAR_IDLE;

		m_ActionList[CHAR_IDLE] = new CBY_EnemyIdle(this);
		m_ActionList[CHAR_MOVE] = new CBY_EnemyMove(this);
		m_ActionList[CHAR_JUMP] = new CBY_EnemyJump(this);
		m_ActionList[CHAR_RUN] = new CBY_EnemyRun(this);
		m_ActionList[CHAR_FIRE] = new CBY_EnemyAttack(this);
		m_ActionList[CHAR_DAMAGE] = new CBY_EnemyDamage(this);
		m_ActionList[CHAR_DEATH] = new CBY_EnemyDeath(this);
		m_ActionList[CHAR_RELOAD] = new CBY_EnemyReload(this);

		m_Action = m_ActionList[CHAR_IDLE];
		m_bDie = false;
		return true;
	}

	bool CBY_EnemyAi::Frame()
	{
		CBY_CharacterSrc::Frame();

		/*if (m_Character == nullptr)
			return false;

		m_Character->Frame();*/
		return true;
	}

	bool CBY_EnemyAi::Render()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Render();
		return true;
	}

	bool CBY_EnemyAi::Release()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Release();
		for (int i = 0; i < CHAR_STATECOUNT; i++)
		{
			delete m_ActionList[i];
		}
		m_dwSoundList.clear();
		return true;
	}

	void CBY_EnemyAi::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
	{
		if (m_Character == nullptr)
			return;

		if (world == nullptr)
		{
			m_Character->SetMatrix(nullptr, view, proj);
		}
		else
		{
			m_matWorld = m_matLook * (*world);
			m_matWorld._41 = m_vCharPos.x;
			m_matWorld._42 = m_vCharPos.y;
			m_matWorld._43 = m_vCharPos.z;
			m_Character->SetMatrix(&m_matWorld, view, proj);
		}
	}

	CBY_EnemyAi::CBY_EnemyAi()
	{
		m_fEffetLookVal = 0.0f;
		m_fYaw = 0.0f;
		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixIdentity(&m_matLook);
	}


	CBY_EnemyAi::~CBY_EnemyAi()
	{
	}

	D3DXVECTOR3 CBY_EnemyProcess::GetLookDir()
	{
		m_fDirTime += g_SecondTime;
		if (m_fDirTime > 3.0f)
		{
			m_fYaw = m_pEnemy->GetYaw();
			D3DXMATRIX matLook;
			D3DXVECTOR3 Lookdir = D3DXVECTOR3(0, 0, -1);
			D3DXVECTOR3 Sidedir = D3DXVECTOR3(1, 0, 0);
			
			srand((unsigned int)time(NULL));
			
			int iDir = (rand()*m_pEnemy->GetIndex()) % 2;
			if (iDir == 0)
			{
				m_fYaw += (rand()*m_pEnemy->GetIndex()) % 10;
			}
			else
			{
				m_fYaw -= (rand()*m_pEnemy->GetIndex()) % 10;
			}

			D3DXMatrixRotationYawPitchRoll(&matLook, m_fYaw, m_fPitch, m_fRoll);


			D3DXVec3TransformCoord(&Lookdir, &Lookdir, &matLook);		//룩벡터
			D3DXVec3TransformCoord(&Sidedir, &Sidedir, &matLook);

			matLook._41 = 0;
			matLook._42 = 0;
			matLook._42 = 0;

			m_pEnemy->SetLookWorldMatrix(matLook);	//회전행렬
			m_pEnemy->SetYaw(m_fYaw);

			m_LookDir = Lookdir;
			m_SideDir = Sidedir;
			m_fDirTime = 0.0f;

		}

		m_pEnemy->SetLookDir(m_LookDir);
		return m_LookDir;
	}

	bool CBY_EnemyProcess::SetColDirection()
	{
		float fM = 0;
		D3DXVECTOR3 vN;
		//m_vMove = m_pEnemy->GetHeroPos();
		m_vLookPush = D3DXVECTOR3(0, 0, 0);
		m_vSidePush = D3DXVECTOR3(0, 0, 0);

		if (KG_COLLOSION::ChkOBBToOBBAndDirection(m_pEnemy->GetCharBox(), m_pEnemy->m_pHero->GetCharBox(), vN, fM))
		{
			float fdot = D3DXVec3Dot(&m_pEnemy->GetCamera()->m_LookDir, &vN);
			if (fdot < 0)
			{
				m_vLookPush = (2 * D3DXVec3Dot(&-m_LookDir, &vN))
					* vN + m_pEnemy->GetCamera()->m_LookDir;									//벽밀림
				m_vSidePush = (2 * D3DXVec3Dot(&-m_SideDir, &vN))
					* vN + m_pEnemy->GetCamera()->m_SideDir;

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
			//m_pEnemy->SetHeroPos(m_vMove);
			return true;
		}
		return false;
	}

	bool CBY_EnemyProcess::CheckMonsterHit(bool bDamage)
	{
		for (int i = 0; i < CBY_CHAR_BULLET.GetBulletNum(); i++)
		{
			if (CBY_CHAR_BULLET.GetEnemyBullet(i))
			{
				continue;
			}
			KG_Box AmmoBox = CBY_CHAR_BULLET.GetBulletBox(i);

			if (KG_COLLOSION::ChkOBBToOBB(m_pEnemy->GetCharBox(), AmmoBox))
			{
				for (int ibox = 0; ibox < m_pEnemy->GetColBoxSize(); ibox++)
				{
					if (KG_COLLOSION::ChkOBBToOBB(m_pEnemy->GetColBox(ibox), AmmoBox))
					{
						CBY_CHAR_BULLET.BulletPop(i);
						D3DXVECTOR3 zpos = D3DXVECTOR3(0, 0, 0);
						zpos -= m_pEnemy->GetCamera()->m_LookDir * m_pEnemy->GetEffectVal();
						m_pEnemy->obj2 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_CHARACTER_BLOOD_IMPACT);
						D3DXVECTOR3 pos = m_pEnemy->GetColPos(ibox);
						pos += zpos;
						D3DXMATRIX mat;
						mat = m_pEnemy->m_matWorld;
						mat._41 = pos.x;
						mat._42 = pos.y;
						mat._43 = pos.z;

						m_pEnemy->obj2->SetScale(D3DXVECTOR3(5, 5, 0));
						m_pEnemy->obj2->Execute(&mat);


						if (bDamage)
						{
							int hp = m_pEnemy->GetHP();
							hp -= 1;
							m_pEnemy->SetHP(hp);
							if (hp <= 0)
							{
								m_pEnemy->SetAction(EVENT_DEATH);
							}
						}
						return true;
					}
				}
			}
		}
		return false;
	}

}
