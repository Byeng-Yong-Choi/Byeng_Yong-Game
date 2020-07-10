#include "CBY_BossDragon.h"
#include "KG_SoundMgr.h"
#include "UI_objMgr.h"

namespace CBY
{

	bool CBY_BossDragon::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		m_Character = std::make_shared<CBY::CBY_Character>();
		m_Character->CharacterLoad(pd3dDevice, Context, L"../../data/char/save/BossSave.txt");
		m_Character->SetRoot(1);
		SetEffectVal(8.0f);

		SetMoveSpeed(40);
		SetGunMonster(true);
		SetShootLenght(50.0f);
		SetRadian(200.0f);
		SetScale(0.1);

		//체력 셋팅
		SetInitHp(100);
		SetHP(100);

		//스킬 셋팅
		SetSkillTime(10.0f);

		m_Character->SetState(0);

		m_dwSoundList.resize(SOUND_BOSS_COUNT);
		m_dwSoundList[SOUND_BOSS_APPEAR] = I_SOUND.Load(L"../../data/sound/Boss/cin_sight.wav", false, 0.5f);
		m_dwSoundList[SOUND_BOSS_HIT] = I_SOUND.Load(L"../../data/sound/Boss/pain9.ogg", false, 0.5f);
		m_dwSoundList[SOUND_BOSS_DIE] = I_SOUND.Load(L"../../data/sound/Boss/imp_death_03.wav", false, 0.5f);
		m_dwSoundList[SOUND_BOSS_ATTACK] = I_SOUND.Load(L"../../data/sound/Boss/attack_2.wav", false, 0.5f);
		m_dwSoundList[SOUND_BOSS_ATTACK_MOUTH] = I_SOUND.Load(L"../../data/sound/Boss/attack_01.ogg", false, 0.5f);
		m_dwSoundList[SOUND_BOSS_MOVES] = I_SOUND.Load(L"../../data/sound/Boss/Move.wav", false, 0.5f);
		m_dwSoundList[SOUND_BOSS_FLAME] = I_SOUND.Load(L"../../data/sound/Boss/fire_01.wav", false, 0.5f);;

		return Init();
	}


	CBY_BossDragon::CBY_BossDragon()
	{
	}


	CBY_BossDragon::~CBY_BossDragon()
	{
	}
}
