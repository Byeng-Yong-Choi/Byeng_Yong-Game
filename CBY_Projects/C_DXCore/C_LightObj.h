#pragma once
#include "C_Model.h"
#include "CShapeBox.h"
#include "CShapeLine.h"

#define MAX_LIGHT 4

struct CB_LIGHT_BUFFER
{
	D3DXVECTOR4 g_cAmbientLightColor[MAX_LIGHT];
	D3DXVECTOR4 g_cDifuseLightColor[MAX_LIGHT];
	D3DXVECTOR4 g_cSpecularLightColor[MAX_LIGHT];
	D3DXVECTOR4 g_vLightDir[MAX_LIGHT];		//w=감쇠
	D3DXVECTOR4 g_vLightPos[MAX_LIGHT];
	D3DXVECTOR4 g_vEyeDir[MAX_LIGHT];
	D3DXVECTOR4 g_vEyePos[MAX_LIGHT];
	D3DXVECTOR4 g_vSpot[MAX_LIGHT];			//내부,외부,히도값
};



class C_LightObj:public C_Model
{
public:
	//Microsoft::WRL::ComPtr<ID3D11Buffer> m_Lightbuffer;
	CB_LIGHT m_Light;
	LIGHTINFO m_Info;
	D3DXVECTOR3 m_vAt;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vinitPos;
	D3DXVECTOR3 m_vDir;
	D3DXVECTOR3 m_vinitDir;
	CShapeBox m_Box;
	CShapeLine m_Line;
	float fHeight;
	bool bMove;

public:
	bool Create(LIGHTINFO& info,ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	bool Update(D3DXVECTOR3 pos, D3DXVECTOR3 look, float Height = 0.0f);
	bool Apply(UINT slot);
	bool Move(int iCmd);

public:
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj) override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	C_LightObj();
	~C_LightObj();
};

