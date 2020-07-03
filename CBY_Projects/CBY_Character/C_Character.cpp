#include "C_Character.h"
#include "C_CharMgr.h"

bool C_Character::SkinLoad(T_STR pszLoad)
{
	int index, list;
	index = I_CHARACTER.SkinLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);
	m_SkinObjIndex.push_back(index);

	list = m_ObjList.size();
	m_ObjList.push_back(std::make_shared<C_SkinObj>());
	*m_ObjList[list] = *I_CHARACTER.m_SkinLoadList[index];

	return true;
}

bool C_Character::BoneLoad(T_STR pszLoad)
{
	m_BoneIndex = I_CHARACTER.BoneLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);

	m_Bone = std::make_shared<C_BoneObj>();
	*m_Bone = *I_CHARACTER.m_BoneLoadList[m_BoneIndex];

	return true;
}

bool C_Character::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName, const TCHAR* TexFileName, const CHAR* VSName, const CHAR* PSName)
{
	HRESULT hr = S_OK;

	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;

	if (FAILED(LoadShader(ShaderFileName, VSName, PSName)))
	{
		return false;
	}

	if (FAILED(CreateConstantBuffer()))
	{
		return false;
	}

	if (FAILED(CreateInputLayout()))
	{
		return false;
	}
	if (FAILED(LoadTexture(TexFileName)))
	{
		return false;
	}
	Init();
	return true;
}

HRESULT C_Character::CreateInputLayout()
{
	HRESULT hr = S_OK;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION_OBJ",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iElementCount = sizeof(layout) /
		sizeof(layout[0]);
	m_obj.m_pd3dDevice->CreateInputLayout(
		layout,
		iElementCount,
		m_obj.m_pVertexCode->GetBufferPointer(),
		m_obj.m_pVertexCode->GetBufferSize(),
		&m_obj.m_pVertexLayout);
	return hr;
}
HRESULT C_Character::CreateConstantBuffer()
{
	C_Model::CreateConstantBuffer();

	HRESULT hr = S_OK;
	ZeroMemory(&m_cbBoneWorld, sizeof(CBConstBoneWorld));

	m_pCBConstBoneWorld.Attach(
		CDXH::CreateConstantBuffer(
			m_obj.m_pd3dDevice,
			nullptr, 1,
			sizeof(CBConstBoneWorld),
			true));
	return hr;
}

bool C_Character::Init()
{
	for (int iBone = 0; iBone < 255; iBone++)
	{
		D3DXMatrixIdentity(&m_pMatrixList[iBone]);
	}
	return true;
}

bool C_Character::Frame()
{
	m_Bone->Update(0, 0, m_fElapseTick, m_pMatrixList);

	for (int iBone = 0; iBone<m_ObjList[0]->m_Loader.m_matBoneBindPoss.size(); iBone++)		
	{
		D3DXMATRIX mat = m_ObjList[0]->m_Loader.m_matBoneBindPoss[iBone] * m_pMatrixList[iBone];		//바인드 포즈는 공통이다
		D3DXMatrixTranspose(&m_cbBoneWorld.g_matConstBoneWorld[iBone], &mat);
	}
	
	D3D11_MAPPED_SUBRESOURCE mapResource;
	m_obj.m_pContext->Map(m_pCBConstBoneWorld.Get(),0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	memcpy(mapResource.pData, &m_cbBoneWorld, sizeof(CBConstBoneWorld));
	m_obj.m_pContext->Unmap(m_pCBConstBoneWorld.Get(), 0);

	return true;
}

bool C_Character::Render()
{
	m_obj.m_pContext->VSSetConstantBuffers(1, 1, m_pCBConstBoneWorld.GetAddressOf());
	PrePender();
	for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
	{
		m_ObjList[iObj]->Render();
	}
	return true;
}

bool C_Character::Release()
{
	for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
	{
		m_ObjList[iObj]->Release();
	}
	m_Bone->Release();
	return true;
}


C_Character::C_Character()
{
	m_fElapseTick = 0;
}


C_Character::~C_Character()
{
}

void C_Character::PrePender()
{
	m_obj.m_pContext->VSSetShader(m_obj.m_pVS, NULL, 0);
	m_obj.m_pContext->PSSetShader(m_obj.m_pPS, NULL, 0);

	m_obj.m_pContext->VSSetConstantBuffers(0, 1, m_obj.m_pConstantBuffer.GetAddressOf());
	m_obj.m_pContext->PSSetConstantBuffers(0, 1, m_obj.m_pConstantBuffer.GetAddressOf());
	m_obj.m_pContext->IASetInputLayout(m_obj.m_pVertexLayout);


	m_obj.m_pContext->PSSetShaderResources(0, 1, &m_obj.m_pSRV);
}
