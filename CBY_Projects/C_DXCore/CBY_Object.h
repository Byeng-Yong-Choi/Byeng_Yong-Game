#pragma once
#include "C_Model.h"
#include "C_BoneObj.h"
#include "C_SkinObj.h"

enum CBY_CHARDATATYPE
{
	CHAR_FRAMETYPE = 0,
	CHAR_MTRTYPE
};

struct Char_State_Info
{
	int m_iStartFrame;
	int m_iEndFrame;

	C_BoneObj m_Bone;
	inline Char_State_Info()
	{
		m_iStartFrame = 0;
		m_iEndFrame = 0;
	}
};


class CBY_Object :public C_Model
{
protected:
	DWORD m_dwAniType;
	int m_iRoot;
	float m_fElapseTick;
	DWORD m_dwState;
	std::vector<Char_State_Info> m_StateList;
	CBY_CharBox m_CharBox;
	std::vector<CBY_CharBox> m_BoxList;

public:
	float m_iStartFrame;
	float m_iEndFrame;

public:
	CScene m_Scene;
	D3DXMATRIX m_pMatrixList[255];
	CBConstBoneWorld m_cbBoneWorld;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneWorld;
	ID3D11ShaderResourceView* m_pTextView;
	ID3D11ShaderResourceView* m_pNomalView;
	ID3D11ShaderResourceView* m_pSpecView;


public:
	std::vector<std::shared_ptr<C_SkinObj>> m_ObjList;
	std::shared_ptr<C_BoneObj> m_BindBone;
	C_BoneObj* m_Bone;
	std::vector<int> m_SkinObjIndex;
	int m_BoneIndex;

public:
	virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName,
		const TCHAR* TexFileName, const CHAR* VSName, const CHAR* PSName) override;
	virtual HRESULT CreateConstantBuffer() override;
	virtual HRESULT CreateInputLayout() override;
	virtual bool PreRender();
	virtual void CharPostRender();
	virtual void BoneBoxRender(CBY_MeshSkin* obj, int index);	//ĳ������ ���� ���

public:
	virtual bool SkinLoad(T_STR pszLoad);
	virtual bool BoneLoad(T_STR pszLoad);
	virtual void SetStateNum(int statenum);
	virtual void SetFrameTime(int m_dwState, float start, float end);
	virtual void SetMTRApply(T_STR pszLoad, int state);		//ĳ���� ���¼���

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Release();
	virtual bool Render();

public:
	virtual void SetState(int dw);							//ĳ���� ���� ����
	virtual int GetStateNum();								//ĳ������ ���¹�ȣ �޾ƿ���
	virtual void SetCharRootBone(int ibone);				//ĳ������ ��ġ������ ���� ��Ʈ������Ʈ ����
	virtual D3DXVECTOR3 GetCharPos();						//ĳ������ ��ġ �ޱ�


public:				//Box
	virtual void SetCharBox();
	virtual CBY_CharBox GetCharBox();
	virtual void SetColBoxList(std::vector<CBY_CharBox>& boxlist);
	virtual CBY_CharBox SetColBoxList(int ibox);
	virtual int GetColBoxSize();
	virtual void CreateColBox();

public:
	CBY_Object();
	virtual ~CBY_Object();
};

