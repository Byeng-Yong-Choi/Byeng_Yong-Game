#pragma once
#include "CBY_Weapon.h"

class C_Character:public CBY_Object
{
protected:
	std::vector<std::shared_ptr<CBY_Weapon>> m_ObjectList;
	DWORD m_dwCameraSocket;
	DWORD m_dwFireSocket;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

public:

public: //object
	virtual bool ObjLoad(T_STR pszSkinLoad);
	virtual void SetObjectSocket(int iSocket, int iObjSocket, int iFireSocket,int iobj);
	virtual void ColBoxUpdate(D3DXMATRIX world);

public: //Set
	virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)override;
	inline void SetCameraSocket(DWORD dwSocket) { m_dwCameraSocket = dwSocket; }
	inline void SetFireSocket(DWORD dwSocket) { m_dwFireSocket = dwSocket; }

public: //Get
	inline DWORD GetCameraSocket() { return m_dwCameraSocket; }
	inline DWORD GetFireSocket() { return m_dwFireSocket; }

public:
	C_Character();
	virtual ~C_Character();
};

