#include "CBY_Hero.h"
#include "CBY_HeroStateStd.h"
#include "CBY_EnemyAi.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "JH_MapMgr.h"
#include "KG_Math.h"
#include "UI_objMgr.h"

D3DXVECTOR3 g_CharPos = D3DXVECTOR3(0, 0, 0);

namespace CBY
{
	void CBY_Hero::InitSet()
	{
		SetHP(m_dwInitHP);
		SetAmmoSize(30);
		SetMaxAmmo(300);
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_AMMOMAX, 1, 300);
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_MAGAZINEMAX, 1, 30);
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_AMMO, 1, 30);

		//hp
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HPMAX, 1, 10.0f);
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HP, 1, 10.0f);
		//hp frame
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR_FRAME, UI_VALUE::UI_VALUE_SET_LENGTHMAX, 1, 10.0f);
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR_FRAME, UI_VALUE::UI_VALUE_SET_LENGTH, 1, 10.0f);
	}

	bool CBY_Hero::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		return true;
	}

	void CBY_Hero::SetAction(DWORD Event)
	{
		m_bStateChange = false;
		m_dwCharState = CBY_HeroFSM::Get().Output(m_dwCharState, Event);
		m_Action = m_ActionList[m_dwCharState];
	}

	void CBY_Hero::Process()
	{
		Frame();
		m_fDamageTime += g_SecondTime;
		if (m_bDamge && (m_fDamageTime > 2.0f))
		{
			SetAction(EVENT_DAMAGE);
			m_bDamge = false;
			m_fDamageTime = 0.0f;
		}
		m_Action->Process();
	}

	bool CBY_Hero::Init()
	{
		if (m_Character == nullptr)
			return false;

		CBY_CharacterSrc::Init();
		//SetMoveSpeed(10);

		CBY_HeroFSM::Get().Init();

		m_dwCharState = CHAR_IDLE;

		m_ActionList[CHAR_IDLE] = new CBY_HeroIdle(this);
		m_ActionList[CHAR_MOVE] = new CBY_HeroMove(this);
		m_ActionList[CHAR_RELOAD] = new CBY_HeroReload(this);
		m_ActionList[CHAR_RUN] = new CBY_HeroRun(this);
		m_ActionList[CHAR_FIRE] = new CBY_HeroFire(this);
		m_ActionList[CHAR_DAMAGE] = new CBY_HeroDamage(this);
		m_ActionList[CHAR_DEATH] = new CBY_HeroDeath(this);
		m_ActionList[CHAR_JUMP] = new CBY_HeroJump(this);

		m_Action = m_ActionList[CHAR_IDLE];

		obj2 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_GUN1_SHOT);
		return true;
	}

	bool CBY_Hero::Frame()
	{
		CBY_CharacterSrc::Frame();
		SetRayCrahClear();
		return true;
	}

	bool CBY_Hero::Render()
	{
		if (m_Character == nullptr)
			return false;

		g_CharPos = m_vCharPos;
		m_Character->Render();
		//m_Character->NonCharacterRender();
		return true;
	}

	bool CBY_Hero::Release()
	{
		if (m_Character == nullptr)
			return false;

		m_Character->Release();
		m_Bullet.Release();
		for (int i = 0; i < CHAR_STATECOUNT; i++)
		{
			delete m_ActionList[i];
		}
		return true;
	}

	//Set
	void CBY_Hero::SetState(int i)
	{
		if (m_Character == nullptr)
			return;

		m_Character->SetState(i);
		m_bStateChange = true;
	}

	void CBY_Hero::SetReloading()
	{
		if (m_dwAmmoMaxSize > 0)
		{
			m_dwAmmoNum = m_dwAmmoSize;
			m_dwAmmoMaxSize -= m_dwAmmoSize;
		}
	}

	void CBY_Hero::SetAmmoSize(DWORD dwsize)
	{
		m_dwAmmoSize = dwsize;
		m_dwAmmoNum = m_dwAmmoSize;
	}

	void CBY_Hero::SetRayCrahClear()
	{
		for (DWORD dw = 0; dw < m_RayCrash.size(); dw++)
		{
			m_RayCrash[dw] = false;
		}
	}

	//Get

	D3DXVECTOR3 CBY_Hero::GetCamerPos()
	{
		if (m_Character == nullptr)
			return D3DXVECTOR3(0, 0, 0);

		D3DXMATRIX world;
		world = m_Character->m_matWorld;
		world._41 = 0;
		world._42 = 0;
		world._43 = 0;

		D3DXVECTOR3 vcamerapos;
		D3DXVec3TransformCoord(&vcamerapos, &m_vCameraPos, &world);

		world._41 = m_vCharPos.x + vcamerapos.x;
		world._42 = m_vCharPos.y + vcamerapos.y;// +m_vPosHeight;
		world._43 = m_vCharPos.z + vcamerapos.z;		//이동 변화량이 SetMatrix전에 이루워지므로 이동값을 넣어준다.

		D3DXMATRIX charmat = m_Character->m_pMatrixList[m_dwCameraSocket]* world;
		D3DXVECTOR3 bonepos = D3DXVECTOR3(charmat._41, charmat._42, charmat._43);
		
		return bonepos;
	}

	bool CBY_Hero::GetAmmo()
	{
		if (m_dwAmmoNum <= 0)
		{
			return false;
		}
		return true;
	}

	CBY_Hero::CBY_Hero()
	{
		m_vCameraPos = D3DXVECTOR3(0, 0, 0);
		m_vMovePos = D3DXVECTOR3(0, 0, 0);
		m_dwCameraSocket = 0;
		m_dwAmmoSize = 0;
		m_dwAmmoNum = 0;
		m_fDamageTime = 0.0f;
	}


	CBY_Hero::~CBY_Hero()
	{
	}


	void CBY_HeroProcess::SetColDirection()
	{
		float fM = 0;
		D3DXVECTOR3 vN;
		m_vMove = m_pHero->GetHeroPos();
		m_vLookPush = D3DXVECTOR3(0, 0, 0);
		m_vSidePush = D3DXVECTOR3(0, 0, 0);
		m_vLook = m_pHero->GetCamera()->m_LookDir;
		m_vSide = m_pHero->GetCamera()->m_SideDir;

		DWORD size = JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList.size();
		size = JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjNodeList.size();
		DWORD dwColSize = 0;
		for (int i=0;  i< JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList.size(); i++)
		{
			if (JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList[i]->GetFlag() == JH::WALL)
			{
				KG_Box box = JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList[i]->GetObj()->GetCharBox();
				for (int iray = 0; iray < CBY::RAY_SIZE; iray++)
				{
					if (m_pHero->GetRayCrash(iray))
					{
						continue;
					}
					for (int irayj = 0; irayj < CBY::RAY_DIRSIZE; irayj++)
					{
						KGCA37_Ray ray = m_pHero->GetRay(iray, irayj);
						if (KG_COLLOSION::ChkOBBToSegment(box, ray, fM))
						{
							m_pHero->SetRayCrash(iray, i, true);
							KG_COLLOSION::GetBoxToSegmentNormal(box, ray, vN);		//슬라이딩 벡터
							m_vLook = m_vLook - vN * (D3DXVec3Dot(&m_vLook, &vN));
							m_vSide = m_vSide - vN * (D3DXVec3Dot(&m_vSide, &vN));

							/*switch (iray)
							{
							case CBY::RAY_FRONT:
							{
								m_vLookPush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
									* vN + ray.m_Dir;
								m_vMove += m_vLookPush * fM;
							}
							break;

							case CBY::RAY_BACK:
							{
								m_vLookPush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
									* vN + ray.m_Dir;
								m_vMove += m_vLookPush * fM;
							}
							break;

							case CBY::RAY_LEFT:
							{
								m_vSidePush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
									* vN + ray.m_Dir;
								m_vMove += m_vSidePush * fM;
							}
							break;

							case CBY::RAY_RIGHT:
							{
								m_vSidePush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
									* vN + ray.m_Dir;
								m_vMove += m_vSidePush * fM;
							}
							break;
							}*/

							dwColSize++;
							break;
						}
					}
				}
			}
			if(dwColSize==3)	//모든 레이 충돌시
			{ 
				return;
			}
		}
	}


	int CBY_HeroProcess::CheckColSize(D3DXVECTOR3& vReflection)
	{
		int isize = 0; //충돌 갯수
		DWORD dwSize=0;
		float fM = 0;
		D3DXVECTOR3 vN;
		//for (int iray = 0; iray < CBY::RAY_SIZE; iray++)
		//{
		//	if (m_pHero->GetRayCrash(iray))
		//	{
		//		DWORD dwBox = m_pHero->GetRayCrashBox(iray);
		//		KGCA37_Ray ray;
		//		KG_Box box = JH::I_MapMgr.GetCurrentMap().m_pQuadTree->m_DrawObjectList[dwBox]->GetObj()->GetCharBox();
		//		ray = m_pHero->GetRay(iray, 0);		//캐릭터의 중간값을 가진 Ray가 0번이다
		//		KG_COLLOSION::ChkOBBToSegment(box, ray, fM);
		//		KG_COLLOSION::GetBoxToSegmentNormal(box, ray, vN);
		//		switch (iray)
		//		{
		//		case CBY::RAY_FRONT:
		//		{
		//			m_vLookPush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
		//				* vN + ray.m_Dir;
		//			vReflection += m_vLookPush * fM;
		//		}
		//		break;

		//		case CBY::RAY_BACK:
		//		{
		//			m_vLookPush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
		//				* vN + ray.m_Dir;
		//			vReflection += m_vLookPush * fM;
		//		}
		//		break;

		//		case CBY::RAY_LEFT:
		//		{
		//			m_vSidePush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
		//				* vN + ray.m_Dir;
		//			vReflection += m_vSidePush * fM;
		//		}
		//		break;

		//		case CBY::RAY_RIGHT:
		//		{
		//			m_vSidePush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
		//				* vN + ray.m_Dir;
		//			vReflection += m_vSidePush * fM;
		//		}
		//		break;
		//		}
		//	}
		//}


		for (int i = 0; i < JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList.size(); i++)
		{
			if (JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList[i]->GetFlag() == JH::WALL)
			{
				KG_Box box = JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList[i]->GetObj()->GetCharBox();
				for (int iray = 0;  iray < CBY::RAY_SIZE; iray++)
				{
					KGCA37_Ray ray = m_pHero->GetRay(iray, 0);
					if (KG_COLLOSION::ChkOBBToSegment(box, ray, fM))
					{
						//ray = m_pHero->GetRay(iray, 0);		//캐릭터의 중간값을 가진 Ray가 0번이다
						KG_COLLOSION::GetBoxToSegmentNormal(box, ray, vN);

						switch (iray)
						{
						case CBY::RAY_FRONT:
						{
							m_vLookPush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
								* vN + ray.m_Dir;
							vReflection += m_vLookPush * (fM+0.01f);
						}
						break;

						case CBY::RAY_BACK:
						{
							m_vLookPush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
								* vN + ray.m_Dir;
							vReflection += m_vLookPush * (fM + 0.01f);
						}
						break;

						case CBY::RAY_LEFT:
						{
							m_vSidePush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
								* vN + ray.m_Dir;
							vReflection += m_vSidePush * (fM + 0.01f);
						}
						break;

						case CBY::RAY_RIGHT:
						{
							m_vSidePush = (2 * D3DXVec3Dot(&-ray.m_Dir, &vN))
								* vN + ray.m_Dir;
							vReflection += m_vSidePush * (fM + 0.01f);
						}
						break;
						}
						//isize++;
						dwSize++;
						break;
						return 0;
					}
					
				}	
			}
		}
		return isize;
	}

	bool CBY_HeroProcess::CharcterMove(bool bRun)
	{
		bool bMove = false;
		float fSpeed;
		if (bRun)
		{
			fSpeed = m_pHero->GetMoveSpeed() + 10;
		}
		else
		{
			fSpeed = m_pHero->GetMoveSpeed();
		}
		 
		if (I_Input.GetKeyCheck('W'))
		{
			//if (m_pHero->GetRayCrash(RAY_FRONT) && (m_pHero->GetRayCrash(RAY_LEFT) || m_pHero->GetRayCrash(RAY_RIGHT)))
			if (m_pHero->GetRayCrash(RAY_FRONT))
			{

			}
			else
			{
				m_vMove += m_vLook * m_pHero->GetMoveSpeed() *g_SecondTime;
			}
			
			bMove = true;
		}

		if (I_Input.GetKeyCheck('S'))
		{
			//if (m_pHero->GetRayCrash(RAY_BACK) && (m_pHero->GetRayCrash(RAY_LEFT) || m_pHero->GetRayCrash(RAY_RIGHT)))
			if (m_pHero->GetRayCrash(RAY_BACK))
			{

			}
			else
			{
				m_vMove -= m_vLook * m_pHero->GetMoveSpeed() *g_SecondTime;
			}
			
			bMove = true;
		}

		if (I_Input.GetKeyCheck('A'))
		{
			//if (m_pHero->GetRayCrash(RAY_LEFT) && (m_pHero->GetRayCrash(RAY_FRONT) || m_pHero->GetRayCrash(RAY_BACK)))
			if (m_pHero->GetRayCrash(RAY_LEFT))
			{

			}
			else
			{
				m_vMove -= m_vSide * m_pHero->GetMoveSpeed() *g_SecondTime;
			}
			
			bMove = true;
		}

		if (I_Input.GetKeyCheck('D'))
		{
			//if (m_pHero->GetRayCrash(RAY_RIGHT) && (m_pHero->GetRayCrash(RAY_FRONT) || m_pHero->GetRayCrash(RAY_BACK)))
			if (m_pHero->GetRayCrash(RAY_RIGHT))
			{

			}
			else
			{
				m_vMove += m_vSide * m_pHero->GetMoveSpeed() *g_SecondTime;
			}
			
			bMove = true;
		}

		return bMove;
	}

}