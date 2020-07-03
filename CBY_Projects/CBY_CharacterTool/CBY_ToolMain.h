#pragma once
#include "CBY_DataCtrl.h"
#include "C_DebugCamera.h"
#include "C_SkinObj.h"
#include "C_CBYObj_1.h"
#include "CModelViewCamera.h"
#include "CBY_BackViewCamera.h"
#include "CBY_CharacterCamera.h"
#include "CBY_SkyBox.h"
#include "CBY_QuadTree.h"
#include "CBY_Shadow.h"
#include "CBY_CharacterToolSet.h"


class CBY_ToolMain:public CBYCore
{
public:
	std::shared_ptr<CModelViewCamera> m_ModelCamera;
	std::shared_ptr<CBY_BackViewCamera> m_BackCamera;
	std::shared_ptr<CBY_CharacterCamera> m_CharCamera;
	std::shared_ptr<CBY_CharacterToolSet> m_Character;
	std::shared_ptr<CBY_SkyBox> m_SkyBox;
	std::shared_ptr<C_Map_shape> m_Map;
	std::shared_ptr<CBY_QuadTree> m_QuadTree;
	std::shared_ptr<CBY_Shadow> m_Shadow;
	bool m_bPlay;
	bool m_bObjRender;
	float fCharFront;
	float fCharSide;
	CBY_DataCtrl m_Data;
	fPOINT m_Center;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Lightbuffer;
	CB_LIGHT m_Light;

public:
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_CharRot;
	D3DXMATRIX m_LTM;
	float t;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

public:
	void Create(T_STR SkinLoad, T_STR MTRLoad);
	void ObjCreate(T_STR SkinLoad, T_STR MTRLoad);
	void ObjSocketSet(int socket, int objnum);
	void SkinAdd(T_STR SkinLoad);
	void MTRAdd(T_STR MTRLoad);
	void AniMTRAdd(T_STR MTRLoad,bool bani);
	void ObjectAniMTRAdd(T_STR MTRLoad, bool bani);
	void AniMTRApply(T_STR MTRLoad, int state);
	void ObjectAniMTRApply(T_STR MTRLoad, int state);
	void LoadCreate();
	void SetShadow();

public:
	void SetCharAction(int state, float start, float end);
	void GetCharAction(int state);
	void SetCharacterPlay();
	void SetClientCenter(int x, int y);
	void SetCharCameraSocket(DWORD dwSocket);
	void SetWeaponFireSocket(DWORD dwSocket);

public:	//오브젝트 셋팅
	void SetObjectMove(float fx, float fy, float fz);
	void SetObjectRot(float Yaw, float Pitch, float Roll);
	void SetObjectScale(float Scl);

public:
	bool Save(const TCHAR* pszSave);
	bool Load(const TCHAR* pszName);

public:
	CBY_ToolMain();
	virtual ~CBY_ToolMain();
};

