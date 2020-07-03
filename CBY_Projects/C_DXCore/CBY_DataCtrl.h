#pragma once
#include "CBYCore.h"
#include "C_Character.h"

struct ObjInfo;

struct MTRInfo
{
	T_STR MtrName;
	int State;
	inline MTRInfo()
	{
		State = -2;
	}
};

struct cby_CharData
{
	DWORD m_CharType;
	std::vector<T_STR> SknNameList;
	std::vector<MTRInfo> MtrList;
	DWORD dwStateNum;
	std::vector<Char_State_Info> StateList;
	CBY_CharBox CharBox;
	std::vector<CBY_CharBox> ColBoxList;

	std::vector<ObjInfo> ObjList;

	DWORD m_dwWeapon;
	DWORD m_dwFireSocket;
	DWORD m_dwCameraSocket;

	inline cby_CharData()
	{
		dwStateNum = 0;
		m_dwWeapon = 0;
		m_dwCameraSocket = 0;
		m_dwFireSocket = 0;
	}
};

struct ObjInfo
{
	cby_CharData objdata;
	int Socket;
	int ObjSocket;
	int FireSocket;
	D3DXVECTOR3 vpos;
	D3DXVECTOR3 vRot;
	float fScale;

	inline ObjInfo()
	{
		Socket=0;
		ObjSocket=0;
		FireSocket=0;
		vpos = {0,0,0};
		vRot = { 0,0,0 };
		fScale=0.0f;
	}
};

class CBY_DataCtrl
{
public:
	bool Save(const TCHAR* pszSave);
	bool Load(const TCHAR* pszName);
	TCHAR* SaveFileDlg(TCHAR* szExt, TCHAR* szTitle);

public:
	cby_CharData m_data;

public:
	CBY_DataCtrl();
	virtual ~CBY_DataCtrl();
};

