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
	BOOL m_isLeaf;			//���� �������
	DWORD m_dwDepth;		//�� ����� ��������
	DWORD m_dwCorner[4];	//��尡 ����Ű�� ����
	CNode* m_pChild[4];		//����� �ڽĳ��(���� Ʈ���� 4��)
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

