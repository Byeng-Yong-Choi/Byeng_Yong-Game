#pragma once
#include "CBY_Weapon.h"

class CBY_ObjectToolSet : public CBY_Weapon
{
public:
	bool m_bViewCheck;			//state�� ������ ���¿��� �ִϸ��̼��� ������ �ƴϸ� �̸��������� Ȯ��
	int m_MtrViewIndex;
	int m_iBoneSelect;
	bool bColBoxRender;
	bool m_bAllBoxRender;
	D3DXVECTOR3 m_vObjectMove;
	D3DXVECTOR3 m_vRot;
	float m_fScale;

public:
	bool Frame() override;
	bool Render() override;
	bool Update(D3DXMATRIX* parmat);

public:
	virtual void AniBoneLoad(T_STR pszLoad, bool bani);
	virtual void CreateCharBox(float fX, float fY, float fZ);
	virtual void ColBoxRender();
	virtual void CreateColBox();


public:	//Set
	virtual void SetFrameTime(int m_dwState, float start, float end);		//�� ���°� ������ �������ٶ�
	virtual void SetNoStateFrameTime(float start, float end);				//�̸������ Ȯ�θ� �Ҷ�
	virtual void SetState(int dw) override;
	virtual void SetCharBox(int id, float fX, float fY, float fZ);
	virtual void SetCharBox();
	virtual void SetMTRApply(T_STR pszLoad, int state);
	inline void SetMovePos(D3DXVECTOR3 pos) { m_vMovePos = pos; }
	inline void SetRot(D3DXVECTOR3 rot) { m_vRot = rot; }
	inline void SetScale(float scl) { m_fScale = scl; }

public:
	inline D3DXVECTOR3 GetRot() { return m_vRot; }
	inline float GetScale() { return m_fScale; }

public:
	CBY_ObjectToolSet();
	virtual ~CBY_ObjectToolSet();
};

