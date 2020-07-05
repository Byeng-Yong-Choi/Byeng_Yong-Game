#include "UI_objMgr.h"
#include "UI_hpPanel.h"
#include "UI_weaponPanel.h"
#include "UI_hp_bar.h"
#include "UI_Ammo_font.h"
#include "UI_characterPortrait.h"
#include "UI_weaponPortrait.h"
#include "UI_hp_bar_Inner.h"
#include "UI_hp_bar_frame.h"

KYS::UI_objMgr::UI_objMgr()
{
}

KYS::UI_objMgr::~UI_objMgr()
{
}

bool KYS::UI_objMgr::Init()
{
	std::shared_ptr<KYS::UI_hpPanel> hpPanel = nullptr;
	std::shared_ptr<KYS::UI_weaponPanel> weaponPanel = nullptr;
	std::shared_ptr<KYS::UI_hp_bar_Inner> _hpBarInner = nullptr;
	std::shared_ptr<KYS::UI_hp_bar> _hpBar = nullptr;
	std::shared_ptr<KYS::UI_hp_bar_frame> _hpBar_frame = nullptr;
	std::shared_ptr<KYS::UI_Ammo_font> _ammoFont[5] = { nullptr, };
	std::shared_ptr<KYS::UI_characterPortrait> characterPortrait = nullptr;
	std::shared_ptr<KYS::UI_weaponPortrait> weaponPortrait = nullptr;

	D3DXMATRIX parentWorld;
	D3DXMatrixIdentity(&parentWorld);

	parentWorld._41 = _camera->m_orthoPos.x;
	parentWorld._42 = _camera->m_orthoPos.y;
	parentWorld._43 = _camera->m_orthoPos.z;


	//hpPanel
	hpPanel = std::make_shared<KYS::UI_hpPanel>();
	hpPanel->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/character_state_panel.png", "VS", "PS");
	hpPanel->SetPos(D3DXVECTOR3(-Winrt.right / 2.0f, -Winrt.bottom / 2.0f + 120, 1005));
	//hpPanel->SetPos(D3DXVECTOR3(1000,1000, 1005));
	hpPanel->SetScale(D3DXVECTOR3(200.0f, 40.0f, 0.0f));
	hpPanel->SetUiType(UI_TYPE::UI_TYPE_HP_PANEL);
	hpPanel->SetParentMatWorld(parentWorld);

	_hpBarInner = std::make_shared<KYS::UI_hp_bar_Inner>();
	_hpBarInner->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/hp_bar_damaged.png", "VS", "PS");
	_hpBarInner->SetPos(D3DXVECTOR3(125, -50, 1003));
	_hpBarInner->SetScale(D3DXVECTOR3(120.0f, 8.0f, 0.0f));
	_hpBarInner->SetUiType(UI_TYPE::UI_TYPE_HP_BAR_INNER);

	_hpBar = std::make_shared<KYS::UI_hp_bar>();
	_hpBar->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/hp_bar.png", "VS", "PS");
	_hpBar->SetPos(D3DXVECTOR3(125, -50, 1002));
	_hpBar->SetScale(D3DXVECTOR3(120.0f, 8.0f, 0.0f));
	_hpBar->SetUiType(UI_TYPE::UI_TYPE_HP_BAR);

	_hpBar_frame = std::make_shared<KYS::UI_hp_bar_frame>();
	_hpBar_frame->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/hp_bar_frame.png", "VS", "PS");
	_hpBar_frame->SetPos(D3DXVECTOR3(125, -50, 1003));
	_hpBar_frame->SetScale(D3DXVECTOR3(120.0f, 8.0f, 0.0f));
	_hpBar_frame->SetUiType(UI_TYPE::UI_TYPE_HP_BAR_FRAME);

	hpPanel->AddUI(_hpBarInner.get());
	hpPanel->AddUI(_hpBar.get());
	hpPanel->AddUI(_hpBar_frame.get());
	
	// weaponPanel
	weaponPanel = std::make_shared<KYS::UI_weaponPanel>();
	weaponPanel->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/weapon_state_panel.png", "VS", "PS");
	weaponPanel->SetScale(D3DXVECTOR3(200.0f, 40.0f, 0.0f));
	weaponPanel->SetPos(D3DXVECTOR3(Winrt.right / 2.0f - weaponPanel->GetScale().x * 2.0f, -Winrt.bottom / 2.0f + 120, 1005));
	weaponPanel->SetUiType(UI_TYPE::UI_TYPE_WEAPON_PANEL);
	weaponPanel->SetParentMatWorld(parentWorld);

	//ammoFont
	Sprite_Info info;
	info._animType = false; //uv
	info._heightCount = 1;
	info._widthCount = 10;
	info._heightSize = 50;
	info._widthSize = 500;

	_ammoFont[0] = std::make_shared<KYS::UI_Ammo_font>();
	_ammoFont[0]->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/font_number.png", "VS", "PS");
	_ammoFont[0]->SetScale(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	_ammoFont[0]->SetPos(D3DXVECTOR3(265, -10, 1004));
	//순서 지켜야함.
	_ammoFont[0]->SetTexSpriteInfo(info);
	_ammoFont[0]->SetUiType(UI_TYPE::UI_TYPE_AMMO_FONT);
	_ammoFont[0]->Init();

	//_ammoFont->SetValue(UI_VALUE::UI_VALUE_SET_AMMO, 1, AMMOFONT_MAX);

	_ammoFont[1] = std::make_shared<KYS::UI_Ammo_font>();
	_ammoFont[1]->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/font_number.png", "VS", "PS");
	_ammoFont[1]->SetScale(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	_ammoFont[1]->SetPos(D3DXVECTOR3(240, -10, 1004));
	//순서 지켜야함.
	_ammoFont[1]->SetTexSpriteInfo(info);
	_ammoFont[1]->SetUiType(UI_TYPE::UI_TYPE_AMMO_FONT);
	_ammoFont[1]->Init();

	_ammoFont[2] = std::make_shared<KYS::UI_Ammo_font>();
	_ammoFont[2]->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/font_number.png", "VS", "PS");
	_ammoFont[2]->SetScale(D3DXVECTOR3(20.0f, 15.0f, 0.0f));
	_ammoFont[2]->SetPos(D3DXVECTOR3(290, -40, 1004));
	//순서 지켜야함.
	_ammoFont[2]->SetTexSpriteInfo(info);
	_ammoFont[2]->SetUiType(UI_TYPE::UI_TYPE_AMMO_FONT);
	_ammoFont[2]->Init();

	_ammoFont[3] = std::make_shared<KYS::UI_Ammo_font>();
	_ammoFont[3]->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/font_number.png", "VS", "PS");
	_ammoFont[3]->SetScale(D3DXVECTOR3(20.0f, 15.0f, 0.0f));
	_ammoFont[3]->SetPos(D3DXVECTOR3(265, -40, 1004));
	//순서 지켜야함.
	_ammoFont[3]->SetTexSpriteInfo(info);
	_ammoFont[3]->SetUiType(UI_TYPE::UI_TYPE_AMMO_FONT);
	_ammoFont[3]->Init();

	_ammoFont[4] = std::make_shared<KYS::UI_Ammo_font>();
	_ammoFont[4]->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/font_number.png", "VS", "PS");
	_ammoFont[4]->SetScale(D3DXVECTOR3(20.0f, 15.0f, 0.0f));
	_ammoFont[4]->SetPos(D3DXVECTOR3(240, -40, 1004));
	//순서 지켜야함.
	_ammoFont[4]->SetTexSpriteInfo(info);
	_ammoFont[4]->SetUiType(UI_TYPE::UI_TYPE_AMMO_FONT);
	_ammoFont[4]->Init();

	weaponPanel->SetAmmoObj(_ammoFont[0].get());
	weaponPanel->SetAmmoObj(_ammoFont[1].get());
	weaponPanel->SetAmmoMaxObj(_ammoFont[2].get());
	weaponPanel->SetAmmoMaxObj(_ammoFont[3].get());
	weaponPanel->SetAmmoMaxObj(_ammoFont[4].get());

	//portrait
	characterPortrait = std::make_shared<KYS::UI_characterPortrait>();
	characterPortrait->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl",
		L"../../data/ui/apex_ui/loba_portrait.png", "VS", "PS");
	characterPortrait->SetScale(D3DXVECTOR3(80.0f, 35.0f, 0.0f));
	characterPortrait->SetPos(D3DXVECTOR3(0,-5, 1004));
	characterPortrait->SetUiType(UI_TYPE::UI_TYPE_CHARACTER_PORTRAIT);

	weaponPortrait = std::make_shared<KYS::UI_weaponPortrait>();
	weaponPortrait->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl",
		L"../../data/ui/apex_ui/weapon_portrait.png", "VS", "PS");
	weaponPortrait->SetScale(D3DXVECTOR3(80.0f, 30.0f, 0.0f));
	weaponPortrait->SetPos(D3DXVECTOR3(40, -10, 1004));
	weaponPortrait->SetUiType(UI_TYPE::UI_TYPE_WEAPON_PORTRAIT);



	



	for (int index = 0; index < 5; index++)
	{
		weaponPanel->AddUI(_ammoFont[index].get());
	}
	hpPanel->AddUI(characterPortrait.get());
	weaponPanel->AddUI(weaponPortrait.get());

	//등록 
	_uiList.push_back(hpPanel);
	_uiList.push_back(weaponPanel);
	_uiList.push_back(_hpBar_frame);
	_uiList.push_back(_hpBarInner);
	_uiList.push_back(_hpBar);

	for (int index = 0; index < 5; index++)
	{
		_uiList.push_back(_ammoFont[index]);
	}
	_uiList.push_back(characterPortrait);
	_uiList.push_back(weaponPortrait);
	

	return false;
}

bool KYS::UI_objMgr::Frame()
{
	for (auto Obj : _uiList)
	{
		Obj->Frame();
	}
	return false;
}

bool KYS::UI_objMgr::Render(KG_Camera * camera)
{
	CDX::ApplyDSS(_context, CDX::KG_DxState::g_pDSSDisable);
	for (auto Obj : _uiList)
	{
		Obj->SetMatrix(nullptr,
		&camera->m_orthoView ,
		&camera->m_OrthoProj);
		
		Obj->Render(camera,_device,_context);
	}
	CDX::ApplyDSS(_context, CDX::KG_DxState::g_pDSS);

	return false;
}

bool KYS::UI_objMgr::Release()
{
	return false;
}

void KYS::UI_objMgr::BossUi()
{
	std::shared_ptr<KYS::UI_hpPanel> bossHpPanel = nullptr;
	std::shared_ptr<KYS::UI_hp_bar> bossHpBar = nullptr;
	std::shared_ptr<KYS::UI_hp_bar_Inner> bossHpBarInner = nullptr;
	std::shared_ptr<KYS::UI_hp_bar_frame> bossHpBar_frame = nullptr;
	std::shared_ptr<KYS::UI_characterPortrait> bossPortrait = nullptr;

	D3DXMATRIX parentWorld;
	D3DXMatrixIdentity(&parentWorld);

	parentWorld._41 = _camera->m_orthoPos.x;
	parentWorld._42 = _camera->m_orthoPos.y;
	parentWorld._43 = _camera->m_orthoPos.z;


	//Boss
	bossHpPanel = std::make_shared<KYS::UI_hpPanel>();
	bossHpPanel->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", nullptr, "VS", "PS");
	bossHpPanel->SetPos(D3DXVECTOR3(-250, 300, 1005));
	//hpPanel->SetPos(D3DXVECTOR3(1000,1000, 1005));
	bossHpPanel->SetScale(D3DXVECTOR3(200.0f, 40.0f, 0.0f));
	bossHpPanel->SetUiType(UI_TYPE::UI_TYPE_BOSS_HP_PANEL);
	bossHpPanel->SetParentMatWorld(parentWorld);

	bossHpBarInner = std::make_shared<KYS::UI_hp_bar_Inner>();
	bossHpBarInner->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/Hp_line.png", "VS", "PS");
	bossHpBarInner->SetPos(D3DXVECTOR3(155, -40, 1003));
	bossHpBarInner->SetScale(D3DXVECTOR3(200.0f, 16.0f, 0.0f));
	bossHpBarInner->SetUiType(UI_TYPE::UI_TYPE_BOSS_HP_BAR_INNER);

	bossHpBar_frame = std::make_shared<KYS::UI_hp_bar_frame>();
	bossHpBar_frame->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/Hp_frame.png", "VS", "PS");
	bossHpBar_frame->SetPos(D3DXVECTOR3(155, -40, 1004));
	bossHpBar_frame->SetScale(D3DXVECTOR3(200.0f, 16.0f, 0.0f));
	bossHpBar_frame->SetUiType(UI_TYPE::UI_TYPE_BOSS_HP_BAR_FRAME);

	bossHpBar = std::make_shared<KYS::UI_hp_bar>();
	bossHpBar->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/hp_bar.png", "VS", "PS");
	bossHpBar->SetPos(D3DXVECTOR3(125, -50, 1002));
	bossHpBar->SetScale(D3DXVECTOR3(120.0f, 8.0f, 0.0f));
	bossHpBar->SetUiType(UI_TYPE::UI_TYPE_BOSS_HP_BAR);

	bossPortrait = std::make_shared<KYS::UI_characterPortrait>();
	bossPortrait->Create(_device, _context, L"../../data/shader/DefaultShader.hlsl",
		L"../../data/ui/apex_ui/boss_portrait.png", "VS", "PS");
	bossPortrait->SetScale(D3DXVECTOR3(80.0f, 50.0f, 0.0f));
	bossPortrait->SetPos(D3DXVECTOR3(0, -5, 1004));
	bossPortrait->SetUiType(UI_TYPE::UI_TYPE_BOSS_PORTRAIT);

	bossHpPanel->AddUI(bossHpBarInner.get());
	//bossHpPanel->AddUI(bossHpBar.get());
	bossHpPanel->AddUI(bossHpBar_frame.get());
	bossHpPanel->AddUI(bossPortrait.get());


	_uiList.push_back(bossHpPanel);
	_uiList.push_back(bossHpBar_frame);
	_uiList.push_back(bossHpBarInner);
	_uiList.push_back(bossHpBar);

	_uiList.push_back(bossPortrait);
}

void KYS::UI_objMgr::add(std::shared_ptr<UI_obj> obj)
{
	if (obj == nullptr)
		return;
	std::shared_ptr<KYS::UI_obj> findObj = nullptr;
	findObj = find(findObj);

	if (findObj == nullptr)
		_uiList.push_back(obj);

	return;

}

std::shared_ptr<KYS::UI_obj> KYS::UI_objMgr::find(UI_TYPE type)
{
	if (type == UI_TYPE::UI_TYPE_NONE)
		return nullptr;

	std::shared_ptr<KYS::UI_obj> reObj = nullptr;
	for (auto obj : _uiList)
	{
		if (obj->GetUiType() == type )
			return reObj = obj;
	}

	return reObj;
}

std::shared_ptr<KYS::UI_obj> KYS::UI_objMgr::find(std::shared_ptr<UI_obj> inObj)
{
	if (inObj == nullptr)
		return nullptr;

	std::shared_ptr<KYS::UI_obj> reObj = nullptr;
	for (auto obj : _uiList)
	{
		if (obj == inObj)
			reObj = inObj;
	}
	return reObj;
}

void KYS::UI_objMgr::ClearUiList()
{
	_uiList.clear();
}
