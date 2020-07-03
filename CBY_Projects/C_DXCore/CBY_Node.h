#pragma once
#include "C_Map_shape.h"
#include "CCamera.h"
#include "CShapeLine.h"
#include "CBY_Select.h"

struct BufSplatting
{
	float fRadan;
	D3DXVECTOR3 vIntersection;
	int index;
	int cellsize;
	int iRow;
	int iCol;
};

class CNode
{
public:
	BOOL m_isLeaf;			//말단 노드인지
	DWORD m_dwDepth;		//이 노드의 깊이정도
	DWORD m_dwCorner[4];	//노드가 가르키는 정점
	CNode* m_pChild[4];		//노드의 자식노드(쿼드 트리라 4개)
	CBY_Box m_Box;
	//std::vector<DWORD> m_BoxIndexList;
	//std::vector<D3DXVECTOR3> m_BoxVertexList;
	float fPick_t;

public:
	DWORD m_dwTilenum;

public:
	std::vector<DWORD> m_IndexList;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;

public:
	CNode();
	virtual ~CNode();
};

