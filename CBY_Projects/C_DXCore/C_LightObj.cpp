#include "C_LightObj.h"

bool C_LightObj::Create(LIGHTINFO& info, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = pContext;
	m_Info = info;

	m_Light.g_cAmbientLightColor = m_Info.cAmbientLightColor;
	m_Light.g_cDifuseLightColor = m_Info.cDifuseLightColor;
	m_Light.g_cSpecularLightColor = m_Info.cSpecularLightColor;
	m_vAt = D3DXVECTOR3(m_Info.vAt.x, m_Info.vAt.y, m_Info.vAt.z);
	m_vPos = D3DXVECTOR3(m_Info.LightPos.x, m_Info.LightPos.y, m_Info.LightPos.z);
	m_vinitPos= m_vPos;
	//m_vinitPos = D3DXVECTOR3(0.1,50,0.1);
	
	D3DXVec3Normalize(&m_vDir, &(m_vAt-m_vPos));
	m_vinitDir = m_vDir;

	m_Light.g_vSpot = m_Info.vSpot;

	m_Box.Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/map/baseColor.jpg", "VSmat", "PS");
	//m_Line.Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/LineShader.txt", nullptr, "VS", "PS");

	//m_Lightbuffer.Attach(CDXH::CreateConstantBuffer(m_obj.m_pd3dDevice, &m_Light, 1, sizeof(CB_LIGHT_BUFFER)));

	return true;
}

bool C_LightObj::Update(D3DXVECTOR3 pos, D3DXVECTOR3 look, float Height)
{
	m_Light.g_vEyeDir.x = look.x;
	m_Light.g_vEyeDir.y = look.y;
	m_Light.g_vEyeDir.z = look.z;
	m_Light.g_vEyeDir.w = 30;

	m_Light.g_vEyePos.x = pos.x;
	m_Light.g_vEyePos.y = pos.y;
	m_Light.g_vEyePos.z = pos.z;
	m_Light.g_vEyePos.w = 30;

	return true;
}

bool C_LightObj::Move(int iCmd)
{
	if (iCmd == 0)
	{
		D3DXMATRIX mrot;
		float radian = cosf(g_gametime)*4.0f;
		m_vPos.y = m_vinitPos.y;
		D3DXMatrixRotationY(&mrot, radian);
		D3DXVec3TransformNormal(&m_vPos, &m_vinitPos, &mrot);
	}
	else if(iCmd==1)
	{
		if (!bMove)
		{
			m_vPos.x += 50.0f*g_SecondTime;
			if (m_vPos.x > 400.0f)
			{
				bMove = true;
			}
		}
		else
		{
			m_vPos.x -= 50.0f*g_SecondTime;
			if (m_vPos.x < 0.0f)
			{
				bMove = false;
			}
		}
		m_vPos.y = m_vinitPos.y;
	}
	else if (iCmd == 2)
	{
		if (!bMove)
		{
			m_vPos.z += 50.0f*g_SecondTime;
			if (m_vPos.z > 400.0f)
			{
				bMove = true;
			}
		}
		else
		{
			m_vPos.z -= 50.0f*g_SecondTime;
			if (m_vPos.z < 0.0f)
			{
				bMove = false;
			}
		}
		m_vPos.y = m_vinitPos.y;
	}
	//D3DXVec3Normalize(&m_vDir, &(m_vAt - m_vPos));

	//D3DXVECTOR3 dir;
	//dir = m_vAt - m_vPos;
	//D3DXVec3Normalize(&m_vDir, &dir);
	//m_vAt = m_vDir + m_vPos * 1000.0f;
	return true;
}

bool C_LightObj::Frame()
{
	
	m_Light.g_vLightDir.x = m_vDir.x;
	m_Light.g_vLightDir.y = m_vDir.y;
	m_Light.g_vLightDir.z = m_vDir.z;
	m_Light.g_vLightDir.w = 10;

	m_Light.g_vLightPos.x = m_vPos.x;
	m_Light.g_vLightPos.y = m_vPos.y;
	m_Light.g_vLightPos.z = m_vPos.z;
	m_Light.g_vLightPos.w = 300;

	//m_obj.m_pContext->UpdateSubresource(m_Lightbuffer.Get(), 0, NULL, &m_Light, 0, 0);

	D3DXMATRIX mat, matsc;
	D3DXMatrixLookAtLH(&mat, &m_vPos, &m_vAt, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(&mat, NULL, &mat);
	SetMatrix(&mat, &m_Box.m_matView, &m_Box.m_matProj);

	D3DXMatrixScaling(&matsc, 20.0f, 20.0f, 20.0f);

	m_Box.m_matWorld = matsc * mat;

	m_Box.m_matWorld._41 = m_vPos.x;
	m_Box.m_matWorld._42 = m_vPos.y;
	m_Box.m_matWorld._43 = m_vPos.z;


	/*m_vDir.x = mat._31;
	m_vDir.y = mat._32;
	m_vDir.z = mat._33;

	m_vAt=-m_vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);*/
	return true;
}

bool C_LightObj::Render()
{
	m_Box.Render();
	//m_Line.SetMatrix(nullptr, &m_Box.m_matView, &m_Box.m_matProj);
	//m_Line.Draw(m_vPos, m_vAt, D3DXVECTOR4(1, 0, 1, 1));
	return true;
}

bool C_LightObj::Release()
{
	m_Box.Release();
	m_Line.Release();
	return true;
}

void C_LightObj::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_Box.SetMatrix(world, view, proj);
}

bool C_LightObj::Apply(UINT slot)
{
	//m_obj.m_pContext->VSSetConstantBuffers(slot, 1, m_Lightbuffer.GetAddressOf());
	//m_obj.m_pContext->PSSetConstantBuffers(slot, 1, m_Lightbuffer.GetAddressOf());

	return true;
}

C_LightObj::C_LightObj()
{
	bMove = false;
}


C_LightObj::~C_LightObj()
{
}
