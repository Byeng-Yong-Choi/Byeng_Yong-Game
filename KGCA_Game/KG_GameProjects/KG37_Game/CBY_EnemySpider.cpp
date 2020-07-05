#include "CBY_EnemySpider.h"
#include "KG_SoundMgr.h"

namespace CBY
{

	bool CBY_EnemySpider::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		m_Character = std::make_shared<CBY::CBY_Character>();
		m_Character->CharacterLoad(pd3dDevice, Context, L"../../data/char/save/Mob_0_Test.txt");
		SetEffectVal(4.0f);
		//m_Bullet.Create(pd3dDevice, Context, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
		//m_Bullet.SkinLoad(L"../../data/Char/Ammo/Ammo.skn");
		//m_Bullet.BoneLoad(L"../../data/Char/Ammo/Ammo.mtr");

		//m_Bullet.SetPower(1);
		//m_Bullet.SetSpeed(25.0f);
		SetInitHp(5);
		SetHP(5);
		SetGunMonster(false);
		m_Character->SetState(0);
		SetShootLenght(30.0f);

		m_dwSoundList.resize(SOUND_COUNT);
		m_dwSoundList[SOUND_ATTACK] = I_SOUND.Load(L"../../data/Char/Monster/Spider/Sound/SpiderAttack.wav", false, 0.5f);
		m_dwSoundList[SOUND_HIT] = I_SOUND.Load(L"../../data/Char/Monster/Spider/Sound/SpiderHit.wav",false,0.5f);
		m_dwSoundList[SOUND_DIE] = I_SOUND.Load(L"../../data/Char/Monster/Spider/Sound/SpiderDie.wav", false, 0.5f);
		m_dwSoundList[SOUND_MOVE] = I_SOUND.Load(L"../../data/Char/Monster/Spider/Sound/SpiderAttack.wav", false, 0.5f);
		m_dwSoundList[SOUND_RUN] = I_SOUND.Load(L"../../data/Char/Monster/Spider/Sound/SpiderAttack.wav", false, 0.5f);
		m_dwSoundList[SOUND_RELOAD] = I_SOUND.Load(L"../../data/Char/Monster/Spider/Sound/SpiderAttack.wav", false, 0.5f);
		

		return Init();
	}


	CBY_EnemySpider::CBY_EnemySpider()
	{
		m_vCharPos = D3DXVECTOR3(30, 0, 20);
	}


	CBY_EnemySpider::~CBY_EnemySpider()
	{
	}
}

