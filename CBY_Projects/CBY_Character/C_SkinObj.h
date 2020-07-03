#pragma once
#include "C_SkinParser.h"
#include "C_BoneObj.h"

#define MAX_BONE_MATRICES 255
struct CBConstBoneWorld
{
	D3DXMATRIX g_matConstBoneWorld[MAX_BONE_MATRICES];
};

class C_SkinObj : public C_Model
{
public:
	C_BoneObj m_Bone;
	CScene m_Scene;
	C_SkinParser m_Loader;
	float m_fElapseTick;
	CBConstBoneWorld m_cbBoneWorld;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneWorld;
	D3DXMATRIX m_pMatrixList[MAX_BONE_MATRICES];
	std::vector<PNCTIW_VERTEX>	m_VertexDataSkin;
	float m_fMove;
	std::wstring m_szname;


public:
	bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void ConvertSkin(std::vector<PNCTIW_VERTEX>& list);
	void CreateVIData(CMeshSkin& mesh);
	void Draw(CMeshSkin& mesh);
	bool SkinCreate();
	void PrePender();

public:
	HRESULT CreateVertexData() override;
	HRESULT CreateIndexData() override;
	HRESULT CreateVertexBuffer() override;

public:
	bool Render() override;
	bool Release() override;

public:
	C_SkinObj();
	virtual ~C_SkinObj();
};

