#pragma once
#include "CBY_MapLOD.h"

class CBY_QuadTree:public CBY_MapLOD
{
public:
	int iPicknum;
	bool m_bPick;
	CNode* m_pRootNode;
	C_Map_shape* m_pMap;
	CCamera* m_pCamera;
	std::vector<CNode*>  m_DrawNodeList;
	std::vector<CNode*>  m_PickNodeList;
	CShapeLine    m_AxisLine;
	CBY_Select* m_pSelect;
	D3DXVECTOR3 m_vIntersection;

public:
	float m_Radian;
	
public:																							//쿼드트리 생성
	virtual bool Build(C_Map_shape* pMap, CCamera* pCamera);
	virtual bool BuildTree(CNode* pNode);
	virtual bool ReleaseTree(CNode* pNode);
	virtual CNode* CreateNode(CNode* pParent, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	virtual bool SubDivide(CNode* pNode);
	virtual void CreateIndexData(CNode* pNode);
	virtual void CreateBB(CNode* pNode);
	virtual void UpdateBBY(CNode* pNode);
		
public:																//그릴 노드 찾아 뿌리기
	void NonPreRender();
	void Draw(CNode* pNode, ID3D11DeviceContext*	pContext);
	void DrawFindNode(CNode* pNode);
	void VisibleNode(CNode* pNode);
	void		DrawLine(CNode* pNode, bool bpick);
	void		DrawNodeLine();

public:															//피킹
	void PickingSelect();
	void GetSelect(CBY_Select* select);
	bool MousePicking();
	bool GetPickingData(CNode* pNode);
	void FacePick();

public:
	void TileSet(DWORD num);									//싱글 타일

public:
	void SetRadian(float rad);									//스플래팅
	void Splatting(ID3D11Texture2D* text,int i);
	void SplattingBufSet(BufSplatting* buf, int i);

public:															//지형 올리기 내리기
	void SetHeightMap(int mode);
	void SetHeight(CNode* pNode,int mode);
	void UpdateNomal(CNode* pNode);
	void SaveheightMap(ID3D11Texture2D* text);

public:
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	CBY_QuadTree();
	virtual ~CBY_QuadTree();
};

