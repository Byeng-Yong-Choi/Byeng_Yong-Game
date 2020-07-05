#include "CBY_HeroLoba.h"
#include "KG_SoundMgr.h"
#include "UI_objMgr.h"
namespace CBY
{

	bool CBY_HeroLoba::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		m_Character = std::make_shared<CBY::CBY_Character>();
		m_Character->CharacterLoad(pd3dDevice, Context, L"../../data/char/save/lobasave.txt");

		m_Bullet.Create(pd3dDevice, Context, L"../../data/shader/LobaShader.txt", nullptr, "VSSKIN", "PS");
		m_Bullet.SkinLoad(L"../../data/Char/Ammo/Ammo.skn");
		m_Bullet.BoneLoad(L"../../data/Char/Ammo/Ammo.mtr");

		m_Bullet.SetPower(1);
		m_Bullet.SetSpeed(100.0f);

		//위치 및 상태 셋팅
		SetMoveSpeed(10);
		m_Character->SetState(0);
		SetCameraSocket(14);
		SetHeight(5);
		SetCameraMovePos(D3DXVECTOR3(5, 20, -40));

		//무기 셋팅
		SetFireTime(0.4);	//총알 발사 시간 셋팅
		SetAmmoSize(7);
		SetFireSocketMove(D3DXVECTOR3(0, 0, -2));
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_AMMOMAX, 1, 7);
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_MAGAZINEMAX, 1, 7);
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_AMMO, 1, 7);

		//체력 셋팅
		SetInitHp(10);
		SetHP(10);
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HPMAX, 1, 10.0f);
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HP, 1, 10.0f);

		//사운드 셋팅
		m_dwSoundList.resize(SOUND_COUNT);
		m_dwSoundList[SOUND_ATTACK] = I_SOUND.Load(L"../../data/Char/FPS/Sound/shoot.wav", false, 0.5f);
		m_dwSoundList[SOUND_HIT] = I_SOUND.Load(L"../../data/Char/FPS/Sound/Reload.wav", false, 0.5f);
		m_dwSoundList[SOUND_DIE] = I_SOUND.Load(L"../../data/Char/FPS/Sound/NoneAmmo.wav", false, 0.5f);
		m_dwSoundList[SOUND_MOVE] = I_SOUND.Load(L"../../data/Char/FPS/Sound/Move.wav", false, 1.0f);
		m_dwSoundList[SOUND_RUN] = I_SOUND.Load(L"../../data/Char/FPS/Sound/Run.wav", false, 1.0f);
		m_dwSoundList[SOUND_RELOAD] = I_SOUND.Load(L"../../data/Char/FPS/Sound/Reload.mp3", false, 0.5f);

		return Init();
	}


	CBY_HeroLoba::CBY_HeroLoba()
	{
	}


	CBY_HeroLoba::~CBY_HeroLoba()
	{
	}
}