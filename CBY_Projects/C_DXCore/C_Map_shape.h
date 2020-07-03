#pragma once
#include "CShapeMap.h"

struct C_MapDesc
{
	int	iNumCols;
	int	iNumRows;
	float fCellDistance;
	float fScaleHeight;
	T_STR strShaderFile;
	T_STR strTextureFile;
	C_STR str_VSname;
	C_STR str_PSname;

	
	/*inline ~C_MapDesc()
	{
		strShaderFile=nullptr;
		strTextureFile = nullptr;
		str_VSname = nullptr;
		str_PSname = nullptr;
	}*/
};

class C_Map_shape:public CShapeMap
{
public:
	int m_imapsize;
	C_MapDesc m_MapDesc;
	int	m_iNumFace;
	int	m_iNumCols;
	int	m_iNumRows;
	int	m_iNumCellCols;
	int	m_iNumCellRows;
	float fCellDistance;
	float fScaleHeight;
	
public:
	std::vector<float> m_fHeightsize;

public:
	void MapSize(int a);
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj) override;
	HRESULT Load(C_MapDesc& mapdesc, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	HRESULT HeightMapLoad(T_STR filename, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	HRESULT CBYHeightMapLoad(T_STR filename, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	HRESULT CreateVertexData() override;
	HRESULT CreateIndexData() override;
	float GetHeight(float fX, float fY);
	float GetHeightMap(int iRow, int iCol);
	float Lerp(float fStart, float fEnd, float fTangent);
	//HRESULT UpdateMap();

public:
	C_Map_shape();
	~C_Map_shape();
};

