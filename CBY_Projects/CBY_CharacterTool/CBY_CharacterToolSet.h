#pragma once
#include "C_Character.h"
#include "CBY_ObjectToolSet.h"


class CBY_CharacterToolSet : public C_Character
{
public:
	bool m_bViewCheck;			//state가 설정된 상태에서 애니메이션을 보는지 아니면 미리보기인지 확인
	int m_MtrViewIndex;
	int m_iBoneSelect;
	bool bColBoxRender;
	bool m_bAllBoxRender;

public:
	std::vector<std::shared_ptr<CBY_ObjectToolSet>> m_ObjectList;

public:
	bool Frame() override;
	bool Render() override;
	bool Release() override; 

public:
	virtual void SetFrameTime(int m_dwState, float start, float end);		//각 상태값 프레임 설정해줄때
	virtual void SetNoStateFrameTime(float start, float end);				//미리보기로 확인만 할때
	virtual void SetState(int dw) override;
	virtual void AniBoneLoad(T_STR pszLoad, bool bani);
	virtual void CreateCharBox(float fX, float fY, float fZ);
	virtual void SetCharBox(int id, float fX, float fY, float fZ);
	virtual void ColBoxRender();
	virtual void CreateColBox();
	virtual void SetCharBox();
	bool ObjLoad(T_STR pszSkinLoad) override;
	void SetObjectSocket(int iSocket, int iObjSocket, int iFireSocket,int iobj) override;

public:
	virtual void SetMTRApply(T_STR pszLoad, int state);

public:
	CBY_CharacterToolSet();
	virtual ~CBY_CharacterToolSet();
};


