#include "CBY_EnemyRobot.h"

namespace CBY
{

	bool CBY_EnemyRobot::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		m_Character = std::make_shared<CBY::CBY_Character>();
		m_Character->CharacterLoad(pd3dDevice, Context, L"../../data/char/save/Robot.txt");
		SetEffectVal(4.0f);
		m_Bullet.Create(pd3dDevice, Context, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
		m_Bullet.SkinLoad(L"../../data/Char/Ammo/Ammo.skn");
		m_Bullet.BoneLoad(L"../../data/Char/Ammo/Ammo.mtr");
		m_Bullet.SetEnemy(true);

		m_Bullet.SetPower(1);
		m_Bullet.SetSpeed(40.0f);
		SetInitHp(5);
		SetHP(5);
		SetGunMonster(true);
		SetShootLenght(30.0f);
		m_Character->SetState(0);

		return Init();
	}


	CBY_EnemyRobot::CBY_EnemyRobot()
	{
	}


	CBY_EnemyRobot::~CBY_EnemyRobot()
	{
	}
}

