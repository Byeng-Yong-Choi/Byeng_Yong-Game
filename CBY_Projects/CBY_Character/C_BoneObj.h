#pragma once
#include "C_CBYObj_1.h"
class C_BoneObj:public C_CBYObj_1
{
public:
	std::wstring m_szname;

public:
	bool BoneCreate();
	bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context) override;
	void Update(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList);
	bool AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype, float fETick);

public:
	C_BoneObj();
	virtual ~C_BoneObj();
};

