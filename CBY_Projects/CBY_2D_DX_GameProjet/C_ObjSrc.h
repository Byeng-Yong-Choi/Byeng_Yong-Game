#pragma once
#include "CShapeObject.h"

enum ChoiceState
{
	CHOICE_STANDBY = 0,
	CHOICE_LEVELUP_STATE,
	CHOICE_COUNT
};

class C_ObjSrc
{
public:
	std::vector<RECT> m_RtList;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;

public:
	D3DXMATRIX m_mat;
	D3DXMATRIX m_world;
	D3DXMATRIX m_view;
	D3DXMATRIX m_proj;
	D3DXVECTOR3 m_pos;			//캐릭터의 위치 변화량
	D3DXVECTOR3 m_Center;		//캐릭터의 센터 위치
	D3DXVECTOR3 m_Size;			//센터에서 더해줄 크기이다
	fRECT m_fRect;

public:
	T_STR_VECTOR m_str;
	std::vector<RECT> m_rtlist;
	std::vector<PNCT_VERTEX> m_vertexdata;
	RECT rtsrc;
	int StateNUM;

public:
	std::vector<std::shared_ptr<CShapeObject>> m_SrcList;

public:
	void SetAniTime(float Time);

public:
	virtual HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	virtual void VertexData();
	virtual void TextSrc();
	virtual void TextArray(int a);
	void RectSet(D3DXVECTOR3 pos);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	C_ObjSrc();
	~C_ObjSrc();
};

