#include "UI_obj.h"
#include "KG_Camera.h"

KYS::UI_obj::UI_obj()
{
	D3DXMatrixIdentity(&_parentMatWorld);
	_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	_rotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_uiType = UI_TYPE::UI_TYPE_NONE;
}

KYS::UI_obj::~UI_obj()
{
}


void KYS::UI_obj::UpdateData()
{
	D3DXMATRIX matScale, matRotate, matTrans;
	D3DXMATRIX parentMatRotate, parentMatTrans;
	D3DXQUATERNION parentQuaterRotate;
	D3DXVECTOR3 parentScale, parentTrans;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotate);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&parentMatRotate);
	D3DXMatrixIdentity(&parentMatTrans);

	float rotX = D3DXToRadian(_rotate.x);
	float rotY = D3DXToRadian(_rotate.y);
	float rotZ = D3DXToRadian(_rotate.z);
	D3DXMatrixIdentity(&matRotate);
	D3DXMatrixRotationX(&matRotate, rotX);
	D3DXMatrixRotationY(&matRotate, rotY);
	D3DXMatrixRotationZ(&matRotate, rotZ);

	D3DXMatrixScaling(&matScale, _scale.x, _scale.y, _scale.z);
	
	matTrans._41 = m_vPos.x +_scale.x;
	matTrans._42 = m_vPos.y - _scale.y ;
	matTrans._43 = m_vPos.z;

	//부모 행렬분해
	D3DXMatrixDecompose(&parentScale, &parentQuaterRotate, &parentTrans, &_parentMatWorld);
	D3DXMatrixRotationQuaternion(&parentMatRotate, &parentQuaterRotate);
	parentMatTrans._41 = parentTrans.x - parentScale.x;
	parentMatTrans._42 = parentTrans.y + parentScale.y;
	//parentMatTrans._43 = parentTrans.z;

	m_matWorld = matScale * matRotate * matTrans * parentMatRotate * parentMatTrans;
}

void KYS::UI_obj::SetValue(UI_VALUE type,int paramCount, ...)
{
	va_list ap = nullptr;
	float result = 0;

	result = va_arg(ap, float);
}

void KYS::UI_obj::SetType(UI_TYPE type, UI_VALUE value, int paramCount, ...)
{
}

bool KYS::UI_obj::Render(KG_Camera * camera, ID3D11Device * _device, ID3D11DeviceContext * _context)
{
	CDX::ApplyBS(_context, CDX::KG_DxState::g_pAlpahBlend);

	UI_obj::Render();

	CDX::ApplyBS(_context, CDX::KG_DxState::g_pAlpahBlendDisable);

	return false;
}

bool KYS::UI_obj::Init()
{
	return false;
}

bool KYS::UI_obj::Frame()
{
	UpdateData();
	return false;
}

bool KYS::UI_obj::Render()
{
	m_obj.PrePender();
	m_obj.PostPender();
	return false;
}

bool KYS::UI_obj::Release()
{
	return false;
}

