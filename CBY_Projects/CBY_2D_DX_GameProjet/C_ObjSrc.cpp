#include "C_ObjSrc.h"
HRESULT C_ObjSrc::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;


	return hr;
}

void C_ObjSrc::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_SrcList[StateNUM]->SetMatrix(world, view, proj);
	D3DXVECTOR3 pos = D3DXVECTOR3(world->_41, world->_42, world->_44);
	RectSet(pos);
}

void C_ObjSrc::RectSet(D3DXVECTOR3 pos)
{

	m_Center.x = 0 + pos.x;
	m_Center.y = 0 + pos.y;
	m_Center.z = 0 + pos.z;			//센터값에서 이동값 더함

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;
}

void C_ObjSrc::SetAniTime(float Time)
{

}
bool C_ObjSrc::Init()
{
	return true;
}


bool C_ObjSrc::Frame()
{
	m_SrcList[StateNUM]->Frame();
	return true;
}

bool C_ObjSrc::Render()
{
	m_SrcList[StateNUM]->Render();
	return true;
}

void C_ObjSrc::VertexData()
{
	m_vertexdata.resize(4);

	m_vertexdata[0].p = D3DXVECTOR3(-29.0f, 29.0f, 0.5f);
	m_vertexdata[1].p = D3DXVECTOR3(29.0f, 29.0f, 0.5f);
	m_vertexdata[2].p = D3DXVECTOR3(-29.0f, -29.0f, 0.5f);
	m_vertexdata[3].p = D3DXVECTOR3(29.0f, -29.0f, 0.5f);				//정점 좌표 입력(직교 투영)	

	m_vertexdata[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_vertexdata[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_vertexdata[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_vertexdata[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_vertexdata[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);			//정점 컬러값 입력

	m_vertexdata[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_vertexdata[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_vertexdata[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_vertexdata[3].t = D3DXVECTOR2(1.0f, 1.0f);			//정점 텍스쳐 좌표

	m_Center.x = 0;
	m_Center.y = 0;
	m_Center.z = 0;

	m_Size.x = abs((m_vertexdata[0].p.x - m_vertexdata[1].p.x) / 2);
	m_Size.y = abs((m_vertexdata[2].p.y - m_vertexdata[0].p.y) / 2);
	m_Size.z = abs((m_vertexdata[0].p.z - m_vertexdata[1].p.z) / 2);

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;
}

void C_ObjSrc::TextSrc()
{
	
}

void C_ObjSrc::TextArray(int a)
{

}

bool C_ObjSrc::Release()
{
	m_rtlist.clear();
	m_vertexdata.clear();
	return true;
}


C_ObjSrc::C_ObjSrc()
{
	D3DXMatrixIdentity(&m_mat);
	D3DXMatrixIdentity(&m_world);
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(&m_proj);
}

C_ObjSrc::~C_ObjSrc()
{
}