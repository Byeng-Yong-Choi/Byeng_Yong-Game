#pragma once
#include "C_ASEParser.h"

class C_ASEObj :public C_Model
{
public:
	CScene m_Scene;
	C_ASEParser m_Loader;
	std::vector<PNCT_VERTEX> m_FaceVertexData;
	std::vector<DWORD> m_FaceIndexData;
	std::vector<CMesh> m_ObjectList;
	float m_fElapseTick;

public:
	bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void Convert(std::vector<PNCT_VERTEX>& list);
	void SetVertexData(CMesh& mesh, int obj, int face);
	void MeshmatWorld(D3DXMATRIX& meshworld, D3DXVECTOR3* pos);
	void CreateVIData(CMesh& mesh);
	void Draw(CMesh& mesh);

public:
	HRESULT CreateVertexData() override;
	HRESULT CreateIndexData() override;
	HRESULT CreateConstantBuffer() override;
	bool AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype);
	bool Render() override;
	bool Frame() override;
	bool Release() override;
	
public:
	C_ASEObj();
	virtual ~C_ASEObj();
};

