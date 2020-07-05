#pragma once
#include "KG_Model.h"

class KG_ShapeBox : public KG_Model
{
public:
	KG_Model m_Model;
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName = L"../../data/shader/DefaultShader.txt", const TCHAR* TexFileName = nullptr, const CHAR* VSName = "VSmat", const CHAR* PSName = "PS") override;

public:
	virtual bool Init() override; 
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	KG_ShapeBox();
	~KG_ShapeBox();
};

