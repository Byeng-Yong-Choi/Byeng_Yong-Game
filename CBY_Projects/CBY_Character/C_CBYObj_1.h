#pragma once
#include "C_CBYParser_1.h"

class C_CBYObj_1 : public C_Model
{
public:
	CScene m_Scene;
	C_CBYParser_1 m_Loader;
	float m_fElapseTick;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneWorld;
	std::vector<PNCTIW_VERTEX>	m_VertexDataSkin;

public:
	virtual bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void Convert(std::vector<PNCTIW_VERTEX>& list);
	virtual bool AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype);
	virtual void CreateVIData(CMeshSkin& mesh);
	virtual void Draw(CMeshSkin& mesh);

public:
	virtual HRESULT CreateVertexData() override;
	virtual HRESULT CreateIndexData() override;
	virtual HRESULT CreateInputLayout() override;
	virtual HRESULT CreateVertexBuffer() override;
	virtual HRESULT CreateConstantBuffer() override;

public:
	virtual bool Render() override;
	virtual bool Frame() override;
	virtual bool Release() override;

public:
	C_CBYObj_1();
	virtual ~C_CBYObj_1();
};

