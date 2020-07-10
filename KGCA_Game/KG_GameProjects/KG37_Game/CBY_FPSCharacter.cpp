#include "CBY_FPSCharacter.h"
#include "KG_SoundMgr.h"
#include "UI_objMgr.h"
namespace CBY
{

	bool CBY_FPSCharacter::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
	{
		m_Character = std::make_shared<CBY::CBY_Character>();
		m_Character->SetRaySize(D3DXVECTOR4(10,10,3,3));
		m_Character->CharacterLoad(pd3dDevice, Context, 
			L"../../data/char/save/FPSCharacter.txt",L"../../data/shader/FPSCharacterShader.txt");

		m_Bullet.Create(pd3dDevice, Context, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
		m_Bullet.SkinLoad(L"../../data/Char/Ammo/Ammo.skn");
		m_Bullet.BoneLoad(L"../../data/Char/Ammo/Ammo.mtr");

		m_Bullet.SetPower(1);
		m_Bullet.SetSpeed(100.0f);

		//위치 및 상태 셋팅
		SetMoveSpeed(25);
		m_Character->SetState(0);
		SetScale(0.15f);
		SetCameraSocket(m_Character->GetCameraSocket());
		SetHeight(0);
		SetCameraMovePos(D3DXVECTOR3(0, 5, -8));

		//무기 셋팅
		SetFireTime(0.4);	//총알 발사 시간 셋팅
		SetAmmoSize(30);
		SetMaxAmmo(300);
		SetFireSocketMove(D3DXVECTOR3(0, -5*GetScale(), 7));
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_AMMOMAX, 1, 300);
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_MAGAZINEMAX, 1, 30);
		UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_SET_AMMO, 1, 30);

		//체력 셋팅
		SetInitHp(10);
		SetHP(10);

		//hp
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HPMAX, 1, 10.0f);
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HP, 1, 10.0f);
		//hp frame
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR_FRAME, UI_VALUE::UI_VALUE_SET_LENGTHMAX, 1, 10.0f);
		UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR_FRAME, UI_VALUE::UI_VALUE_SET_LENGTH, 1, 10.0f);
		
		//UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HPMAX, 1, 10.0f);
		//UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_SET_HP, 1, 10.0f);

		//사운드 셋팅
		m_dwSoundList.resize(SOUND_COUNT);
		m_dwSoundList[SOUND_ATTACK] = I_SOUND.Load(L"../../data/Char/FPSCharacter/Sound/shoot.mp3", false, 0.5f);
		m_dwSoundList[SOUND_HIT] = I_SOUND.Load(L"../../data/Char/FPSCharacter/Sound/Reload.mp3", false, 0.5f);
		m_dwSoundList[SOUND_DIE] = I_SOUND.Load(L"../../data/Char/FPSCharacter/Sound/NoneAmmo.wav", false, 0.5f);
		m_dwSoundList[SOUND_MOVE] = I_SOUND.Load(L"../../data/Char/FPSCharacter/Sound/Walk.mp3", false, 1.0f);
		m_dwSoundList[SOUND_RUN] = I_SOUND.Load(L"../../data/Char/FPSCharacter/Sound/Run.mp3", false, 1.0f);
		m_dwSoundList[SOUND_RELOAD] = I_SOUND.Load(L"../../data/Char/FPSCharacter/Sound/Reload.mp3", false, 0.5f);

		return Init();
	}


	CBY_FPSCharacter::CBY_FPSCharacter()
	{
	}


	CBY_FPSCharacter::~CBY_FPSCharacter()
	{
	}
}