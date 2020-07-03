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
	std::vector<CBY_MeshSkin*> m_ObjList;
	std::vector<D3DXMATRIX>		m_matBoneBindPoss;
	std::vector<PNCT2IW_VERTEX>	m_VertexDataSkin;
	float m_fMove;
	std::wstring m_szname;


public:
	virtual bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void ConvertSkin(std::vector<PNCT2IW_VERTEX>& list);
	virtual void CreateVIData(CBY_MeshSkin* mesh);
	virtual void Draw(CBY_MeshSkin* mesh);
	virtual bool SkinCreate();
	virtual void PrePender();
	virtual void VertexDataTan(CBY_MeshSkin* mesh);
	virtual void CreateTangentSpaceVectors(D3DXVECTOR3 *v0,
		D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0,
		D3DXVECTOR2 uv1, D3DXVECTOR2 uv2,
		D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal);

public:
	virtual HRESULT CreateVertexData() override;
	virtual HRESULT CreateIndexData() override;
	virtual HRESULT CreateVertexBuffer() override;

public:
	virtual bool Render() override;
	virtual bool Release() override;

public:
	virtual void Save();
	virtual void Load();

public:
	C_SkinObj();
	virtual ~C_SkinObj();
};

