#pragma once
#include "CBY_QuadTree.h"

typedef std::vector<DWORD> dwIndexList;

class CBY_StaticData
{
public:
	DWORD dwLevel;
	std::vector<dwIndexList> m_LODIndexList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>>	IndexBufferList;

public:
	CBY_StaticData();
	~CBY_StaticData();
};

class CBY_QuadTreeIndex:public CBY_QuadTree
{
public:
	std::vector<CBY_StaticData*> m_StaticDataList;		//Level마다 가질 스태틱 lod정보

public:
	void CreateLODIndexData(int maxcells);
	HRESULT CreateIndexBuffer(CBY_StaticData* pdata);

public:
	bool Release() override;

public:
	CBY_QuadTreeIndex();
	virtual ~CBY_QuadTreeIndex();
};

