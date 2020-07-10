#include "UI_minimap.h"
#include "KG_Camera.h"
#include "CBY_CharacterSrc.h"
#include "CBY_EnemyRobot.h"
#include "CBY_EnemySpider.h"
#include "CBY_FPSCharacter.h"

KYS::UI_minimap::UI_minimap()
{
	D3DXMatrixIdentity(&_matTopView);
	D3DXMatrixIdentity(&_matTopProj);
	_recoverViewPort = nullptr;
	_boxObj = nullptr;
	_minimapAlpha = false;
}

KYS::UI_minimap::~UI_minimap()
{
}

void KYS::UI_minimap::SetValue(UI_VALUE type, int paramCount, ...)
{
	va_list ap = nullptr;
	float value[4] = { 0 };

	va_start(ap, paramCount);

	/*if (type == UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST)
	{
		_boxObj = va_arg(ap, std::shared_ptr<KG_ShapeBox>);
	}
	else
	{
		for (int count = 0; count < paramCount; count++)
		{
			value[count] = static_cast<float>(va_arg(ap, double));
		}
	}*/
	
	switch (type)
	{
	case  UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST:
	{
		_boxObj = va_arg(ap, std::shared_ptr<KG_ShapeBox>);
	}
	break;

	case  UI_VALUE::UI_VALUE_SET_RECOVER_VIEWPORT:
	{
		_recoverViewPort = va_arg(ap, D3D11_VIEWPORT*);
	}
	break;

	case  UI_VALUE::UI_VALUE_SET_CAMERA:
	{
		_camera = va_arg(ap, KG_Camera*);
	}
	break;
	case  UI_VALUE::UI_VALUE_SET_MINIMAP_ALPHA:
	{
		_minimapAlpha = !_minimapAlpha;
	}
	break;	

	case  UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST_CHARACTER:
	{
		m_Character = va_arg(ap, std::shared_ptr<CBY::CBY_FPSCharacter>);
	}
	break;

	case  UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST_ENEMY:
	{
		m_EnemyList = va_arg(ap, std::vector<std::shared_ptr<CBY::CBY_EnemyRobot>>);
		m_EnemySpiderList = va_arg(ap, std::vector<std::shared_ptr<CBY::CBY_EnemySpider>>);	
	}
	break;

	case  UI_VALUE::UI_VALUE_SET_MINIMAP_VIEW_OFFSET:
	{
		_minimapViewOffset = static_cast<float>(va_arg(ap, double));
	}
	break;
	case  UI_VALUE::UI_VALUE_ADD_MINIMAP_VIEW_OFFSET:
	{
		_minimapViewOffset += static_cast<float>(va_arg(ap, double));
	}
	break;
	}
}

bool KYS::UI_minimap::Init()
{
	//minimap
	_miniMapViewPort.Width = Winrt.right;
	_miniMapViewPort.Height = Winrt.bottom;
	_miniMapViewPort.TopLeftX = 0;
	_miniMapViewPort.TopLeftY = 0;
	_miniMapViewPort.MinDepth = 0;
	_miniMapViewPort.MaxDepth = 1.0f;

	/*_miniMap.create(
		_d3dDevice,
		_immediateContext,
		L"../../data/map/Tile20.jpg",
		L"../../data/shader/DefaultShader.hlsl");*/
	_renderTarget.Create(_device);
	//미니맵을 볼 시점
	D3DXMatrixLookAtLH(&_matTopView,
		&D3DXVECTOR3(0, 50, 0),
		&D3DXVECTOR3(0, 0, 1),
		&D3DXVECTOR3(0, 1, 0));

	D3DXMatrixPerspectiveFovLH(&_matTopProj,
		D3DX_PI * 0.5f, 1, 1, 1000);
	return false;
}

bool KYS::UI_minimap::Frame()
{
	UI_obj::Frame();

	D3DXMatrixLookAtLH(&_matTopView,
		&D3DXVECTOR3(_camera->m_Pos.x, _minimapViewOffset, _camera->m_Pos.z),
		&D3DXVECTOR3(_camera->m_Pos.x, -1, _camera->m_Pos.z),
		&D3DXVECTOR3(_camera->m_Look.x, _camera->m_Look.y, _camera->m_Look.z));


	//miniMap texture에 맵을 그린다.
	_context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CDX::ApplyBS(_context, CDX::KG_DxState::g_pAlpahBlend);
	CDX::ApplySS(_context, CDX::KG_DxState::g_pSampler);
	CDX::ApplyRS(_context, CDX::KG_DxState::g_pRSSold);

	ID3D11ShaderResourceView* const srv[1] = { NULL };
	_context->PSSetShaderResources(0, 1,
		srv);
	_renderTarget.Begin(_context, D3DXVECTOR4(0.0, 0.0, 0.0, 0.5));

	/*_boxObj->SetMatrix(nullptr,
		&_matTopView,
		&_matTopProj);
	_boxObj->Render();*/

	if (m_Character != nullptr)
	{
		m_Character->SetMatrix(nullptr,
			&_matTopView,
			&_matTopProj);
		m_Character->Render();

	}

	for (auto enemy1 : m_EnemyList)
	{

		enemy1->SetMatrix(nullptr,
			&_matTopView,
			&_matTopProj);
		enemy1->Render();
	}

	for (auto enemy2 : m_EnemySpiderList)
	{
		enemy2->SetMatrix(nullptr,
			&_matTopView,
			&_matTopProj);
		enemy2->Render();
	}
	
	//_renderTarget.saveFile(_immediateContext,L"123");
	_renderTarget.End(_context);
	return false;
}

bool KYS::UI_minimap::Render(KG_Camera * camera, ID3D11Device * _device, ID3D11DeviceContext * _context)
{
	//_miniMap.setMatrix(NULL, NULL, NULL);
	SetMatrix(NULL, NULL, NULL);
	////지정한 viewPort에 renderTarget texture를 그린다.
	_context->RSSetViewports(1, &_miniMapViewPort);
	if (_renderTarget.m_pTexture != nullptr)
	{
		ID3D11ShaderResourceView* const srv[1] = { NULL };
		//_miniMap.preRender();
		if (_minimapAlpha)
			CDX::ApplyBS(_context, CDX::KG_DxState::g_pAlpahBlend);
		PreRender();
		_context->PSSetShaderResources(0, 1,
			srv);
		_context->PSSetShaderResources(0, 1,
			_renderTarget.m_pSRV.GetAddressOf());
		PostRender();
		CDX::ApplyBS(_context, CDX::KG_DxState::g_pAlpahBlendDisable);
		//_miniMap.postRender();
	}
	//viewPort 복구
	_context->RSSetViewports(1, _recoverViewPort);

	return false;
}


