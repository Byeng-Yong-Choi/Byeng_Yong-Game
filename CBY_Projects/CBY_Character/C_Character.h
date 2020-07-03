#pragma once
#include "C_Model.h"
#include "C_BoneObj.h"
#include "C_SkinObj.h"

class C_Character:public C_Model
{
public:
	float m_fElapseTick;
	CScene m_Scene;
	D3DXMATRIX m_pMatrixList[255];
	CBConstBoneWorld m_cbBoneWorld;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneWorld;
	ID3D11ShaderResourceView* m_pTextView;


public:
	std::vector<std::shared_ptr<C_SkinObj>> m_ObjList;
	std::shared_ptr<C_BoneObj> m_Bone;
	std::vector<int> m_SkinObjIndex;
	int m_BoneIndex;

public:
	bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName, const TCHAR* TexFileName, const CHAR* VSName, const CHAR* PSName) override;
	HRESULT CreateConstantBuffer() override;
	HRESULT CreateInputLayout() override;
	void PrePender();

public:
	bool SkinLoad(T_STR pszLoad);
	bool BoneLoad(T_STR pszLoad);
	
public:
	bool Init();
	bool Frame();
	bool Release();
	bool Render();

public:
	C_Character();
	virtual ~C_Character();
};

