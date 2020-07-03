#include "C_Map_shape.h"

float C_Map_shape::GetHeightMap(int iRow, int iCol)
{
	return m_VerTex[iRow*m_iNumRows + iCol].p.y;
}

float C_Map_shape::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}

float C_Map_shape::GetHeight(float fX, float fZ)
{
	float fCellX = m_iNumCellCols * fCellDistance / 2.0f + fX;
	float fCellZ = m_iNumCellRows * fCellDistance / 2.0f - fZ;
	fCellX /= fCellDistance;
	fCellZ /= fCellDistance;

	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);			//floorf ���� �ִ� ���� ���(����ȭ)

	float A = GetHeightMap(fCellZ, fCellX);
	float B = GetHeightMap(fCellZ, fCellX+1);
	float C = GetHeightMap(fCellZ+1, fCellX);
	float D = GetHeightMap(fCellZ+1, fCellX+1);		//4���� ���� ���̰��� ������

	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;		//�Ǽ����� ������ ���� �Ҽ����� ������ �츮���� �ʿ��� �� ���� ���� �� �ִ�.

	float fHeight = 0.0f;
	if ((fDeltaX + fDeltaZ) <= 1.0f)
	{
		float uy = B - A;// * fDeltaX;
		float vy = C - A;// * fDeltaZ;
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	else
	{
		float uy = C - D;// * (1.0f - fDeltaX);
		float vy = B - D;// *(1.0f - fDeltaZ);
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}

	return fHeight;
}

void C_Map_shape::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)		//��� ����
{
	if (world != nullptr)
	{
		m_matWorld = *world;
	}
	if (view != nullptr)
	{
		m_matView = *view;
	}
	if (proj != nullptr)
	{
		m_matProj = *proj;
	}

	D3DXMatrixTranspose(&m_ConData.World, &m_matWorld);
	D3DXMatrixTranspose(&m_ConData.View, &m_matView);
	D3DXMatrixTranspose(&m_ConData.Proj, &m_matProj);


	if (m_obj.iCBBufferType == CB_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE mss;
		if (SUCCEEDED(m_obj.m_pContext->Map(m_obj.m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mss)))
		{
			CONDATA* pData = (CONDATA*)mss.pData;
			memcpy(pData, &m_ConData, sizeof(CONDATA));
			m_obj.m_pContext->Unmap(m_obj.m_pConstantBuffer.Get(), 0);
		}
	}
	else
	{
		m_obj.m_pContext->UpdateSubresource(m_obj.m_pConstantBuffer.Get(), 0, NULL, &m_ConData, 0, 0);
	}

}

HRESULT C_Map_shape::Load(C_MapDesc& mapdesc, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_MapDesc = mapdesc;
	m_iNumCols = m_MapDesc.iNumCols;
	m_iNumRows = m_MapDesc.iNumRows;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumFace = m_iNumCellCols * m_iNumCellRows * 2;
	fCellDistance = m_MapDesc.fCellDistance;

	
	//hr = Create(pd3dDevice, Context, m_MapDesc.strShaderFile.c_str(), m_MapDesc.strTextureFile.c_str(), "VSmat", "PSALPHA");
	hr = Create(pd3dDevice, Context, m_MapDesc.strShaderFile.c_str(), m_MapDesc.strTextureFile.c_str(), m_MapDesc.str_VSname.c_str() , m_MapDesc.str_PSname.c_str());
	if (hr)
	{
		return hr;
	}

	return hr;
}

HRESULT C_Map_shape::HeightMapLoad(T_STR filename, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	D3DX11_IMAGE_INFO imageinfo;							//�ε� ������ ������ �޾ƿ� ����ü
	D3DX11_IMAGE_LOAD_INFO loadinfo;						//�̹��� ������ ��� ������ ���� �Է�
	ZeroMemory(&loadinfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadinfo.Usage = D3D11_USAGE_STAGING;					//�̹����� CPU���� ����
	loadinfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;		//�̹����� cpu�� �а� ���� ����
	loadinfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			//�̹��� ���� 32��Ʈ�� ���� ����
	loadinfo.pSrcInfo = &imageinfo;							//�̹��� ���� ������ ���� �ּ� �Է�
	loadinfo.MipLevels = 1;									//�̹����� �Ӹ� ����

	ID3D11Resource* pResource = nullptr;				//�̹��� �ε��Ҷ� �ҷ��� ����
	hr = D3DX11CreateTextureFromFile(pd3dDevice, filename.c_str(), &loadinfo, NULL, &pResource, NULL);
	if (hr != S_OK)
	{
		return hr;
	}

	ID3D11Texture2D* pTexture2D = nullptr;				//���ҽ��� 2d�ؽ��ķ� ��ȯ�Ͽ� ����ϱ� ���� �̿�
	hr = pResource->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D);
	if (hr != S_OK)
	{
		return hr;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);			//�ؽ����� ������ ����ũ�� �޾� ��� loadinfo�� �ٸ��� ���� ���� ũ�⸦ �˼� �����Ƿ� ����Ѵ�

	m_fHeightsize.resize(desc.Height*desc.Width);		//���� ũ�⸦ �Է����� ������ ���� ����Ʈ

	D3D11_MAPPED_SUBRESOURCE MapFaceDest;
	if (SUCCEEDED(Context->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MapFaceDest)))
	{
		UCHAR* pTexels = (UCHAR*)MapFaceDest.pData;
		for (UINT row = 0; row < desc.Height; row++)
		{
			UINT RowBegin = row * MapFaceDest.RowPitch;		//�ܿ� �����Ͱ� �����Ƿ� �׸�ŭ ������ �� �ְ� �̷��� ����� ������Ѵ�.
			for (UINT col = 0; col < desc.Width; col++)
			{
				UINT ColBegin = col * 4;		//RGBA�̹Ƿ� 4����Ʈ�� �������� �� ���� ���� �����̴�.
				UINT Red = pTexels[ColBegin + RowBegin + 0];
				m_fHeightsize[row*desc.Width + col] = Red;
			}
		}
		Context->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
	}

	m_iNumCols = desc.Width;
	m_iNumRows = desc.Height;

	pResource->Release();
	pTexture2D->Release();

	return hr;
}

HRESULT C_Map_shape::CBYHeightMapLoad(T_STR filename, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	D3DX11_IMAGE_INFO imageinfo;							//�ε� ������ ������ �޾ƿ� ����ü
	D3DX11_IMAGE_LOAD_INFO loadinfo;						//�̹��� ������ ��� ������ ���� �Է�
	ZeroMemory(&loadinfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadinfo.Usage = D3D11_USAGE_STAGING;					//�̹����� CPU���� ����
	loadinfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;		//�̹����� cpu�� �а� ���� ����
	loadinfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			//�̹��� ���� 32��Ʈ�� ���� ����
	loadinfo.pSrcInfo = &imageinfo;							//�̹��� ���� ������ ���� �ּ� �Է�
	loadinfo.MipLevels = 1;									//�̹����� �Ӹ� ����

	ID3D11Resource* pResource = nullptr;				//�̹��� �ε��Ҷ� �ҷ��� ����
	hr = D3DX11CreateTextureFromFile(pd3dDevice, filename.c_str(), &loadinfo, NULL, &pResource, NULL);
	if (hr != S_OK)
	{
		return hr;
	}

	ID3D11Texture2D* pTexture2D = nullptr;				//���ҽ��� 2d�ؽ��ķ� ��ȯ�Ͽ� ����ϱ� ���� �̿�
	hr = pResource->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D);
	if (hr != S_OK)
	{
		return hr;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);			//�ؽ����� ������ ����ũ�� �޾� ��� loadinfo�� �ٸ��� ���� ���� ũ�⸦ �˼� �����Ƿ� ����Ѵ�

	m_fHeightsize.resize(desc.Height*desc.Width);		//���� ũ�⸦ �Է����� ������ ���� ����Ʈ

	D3D11_MAPPED_SUBRESOURCE MapFaceDest;
	if (SUCCEEDED(Context->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MapFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MapFaceDest.pData;
		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{
				int pixel = y * MapFaceDest.RowPitch + (x * 4);
				BYTE* R = &pDestBytes[pixel];
				BYTE* G = &pDestBytes[pixel + 1];
				
				if (*R != 0)
				{
					m_fHeightsize[y*desc.Width + x] = *R;
				}
				else if (*G != 0)
				{
					m_fHeightsize[y*desc.Width + x] = -(*G);
				}
			}
		}
		Context->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
	}

	m_iNumCols = desc.Width;
	m_iNumRows = desc.Height;
	pResource->Release();
	pTexture2D->Release();
}

HRESULT C_Map_shape::CreateVertexData()
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);
	m_VerTex.resize(m_iNumCols * m_iNumRows);

	float fHalfCols = (m_iNumCols - 1) / 2.0f;
	float fHalfRows = (m_iNumRows - 1) / 2.0f;
	float ftxOffsetU = 1.0f / (m_iNumCols - 1);
	float ftxOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			m_VerTex[iVertexIndex].p.x = (iCol - fHalfCols)		* m_MapDesc.fCellDistance;
			m_VerTex[iVertexIndex].p.z = -((iRow - fHalfRows)	* m_MapDesc.fCellDistance);
			if (m_fHeightsize.size() > 0)
			{
				m_VerTex[iVertexIndex].p.y = m_fHeightsize[iVertexIndex] * m_MapDesc.fScaleHeight;
			}
			else
			{
				m_VerTex[iVertexIndex].p.y = 0.0f;
			}
			m_VerTex[iVertexIndex].n = D3DXVECTOR3(0, 0, 0);
			m_VerTex[iVertexIndex].c = D3DXVECTOR4(1, 1, 1, 1);
			m_VerTex[iVertexIndex].t = D3DXVECTOR2(ftxOffsetU * iCol, ftxOffsetV * iRow);
		}
	}

	return hr;
}

HRESULT C_Map_shape::CreateIndexData()
{
	HRESULT hr = S_OK;

	m_IndexData.resize(m_iNumFace * 3);

	int iCurIndex = 0;
	int iFace = 0;
	for (int iRow = 0; iRow < m_iNumCellRows ; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols ; iCol++)
		{
			//0	1    4   
			//2	   3 5
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexData[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexData[iCurIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexData[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexData[iCurIndex + 3] = m_IndexData[iCurIndex + 2];
			m_IndexData[iCurIndex + 4] = m_IndexData[iCurIndex + 1];
			m_IndexData[iCurIndex + 5] = m_IndexData[iCurIndex + 2] + 1;

			D3DXVECTOR3 E0, E1, facenomal;
			E0 = m_VerTex[m_IndexData[iCurIndex + 1]].p - m_VerTex[m_IndexData[iCurIndex + 0]].p;
			E1 = m_VerTex[m_IndexData[iCurIndex + 2]].p - m_VerTex[m_IndexData[iCurIndex + 0]].p;
			D3DXVec3Cross(&facenomal, &E0, &E1);
			D3DXVec3Normalize(&facenomal, &facenomal);
			m_VerTex[m_IndexData[iCurIndex + 0]].n += facenomal;
			m_VerTex[m_IndexData[iCurIndex + 1]].n += facenomal;
			m_VerTex[m_IndexData[iCurIndex + 2]].n += facenomal;


			E0 = m_VerTex[m_IndexData[iCurIndex + 4]].p - m_VerTex[m_IndexData[iCurIndex + 3]].p;
			E1 = m_VerTex[m_IndexData[iCurIndex + 5]].p - m_VerTex[m_IndexData[iCurIndex + 3]].p;
			D3DXVec3Cross(&facenomal, &E0, &E1);
			D3DXVec3Normalize(&facenomal, &facenomal);
			m_VerTex[m_IndexData[iCurIndex + 3]].n += facenomal;
			m_VerTex[m_IndexData[iCurIndex + 4]].n += facenomal;
			m_VerTex[m_IndexData[iCurIndex + 5]].n += facenomal;
			

			iCurIndex += 6;
		}
	}

	D3DXVECTOR3 Light(0, -1, 0);

	for (int v = 0; v < m_VerTex.size(); v++)
	{
		D3DXVec3Normalize(&m_VerTex[v].n, &m_VerTex[v].n);

		float fDot = D3DXVec3Dot(&-Light, &m_VerTex[v].n);
		m_VerTex[v].c.x = fDot;
		m_VerTex[v].c.y = fDot;
		m_VerTex[v].c.z = fDot;
		//m_VerTex[v].c.w = fDot;
	}

	return hr;
}

//HRESULT C_Map_shape::UpdateMap()
//{
//	m_VerTex.clear();
//	m_IndexData.clear();
//	CreateVertexData();
//	CreateIndexData();
//	m_obj.m_pContext->UpdateSubresource(m_obj.m_pVertexBuffer, 0, nullptr, &m_VerTex.at(0), 0, 0);
//	m_obj.m_pContext->UpdateSubresource(m_obj.m_pIndexBuffer, 0, nullptr, &m_IndexData.at(0), 0, 0);
//}


void C_Map_shape::MapSize(int a)
{
	m_imapsize = a;
}


C_Map_shape::C_Map_shape()
{
	m_imapsize = 0;
}


C_Map_shape::~C_Map_shape()
{
	m_fHeightsize.clear();
	m_MapDesc.strShaderFile.clear();
	m_MapDesc.strTextureFile.clear();
	m_MapDesc.str_PSname.clear();
	m_MapDesc.str_VSname.clear();
}
