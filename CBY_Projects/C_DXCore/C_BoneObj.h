#pragma once
#include "C_CBYObj_1.h"
#include "C_BoneParser.h"

class C_BoneObj:public C_CBYObj_1
{
public:
	C_BoneParser m_ObjLoader;
	CBY_Box m_CharBox;

public:
	virtual bool BoneCreate();
	virtual bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context) override;
	virtual void Convert(std::vector<PNCT2IW_VERTEX>& list);
	virtual void Update(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList);
	virtual void MTRUpdate(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList);
	virtual void ObjUpdate(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList, D3DXMATRIX* parmat, int socket);
	virtual bool AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype, float fETick);

public:
	C_BoneObj();
	virtual ~C_BoneObj();
};

