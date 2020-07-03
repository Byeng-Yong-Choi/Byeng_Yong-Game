#pragma once
#include "CBYCore.h"
#include "C_LightObj.h"
#include "CBY_QuadTree.h"
#include "CBY_Minimap.h"
#include "CBY_MRT.h"
#include "CShapeMap.h"
#include "CShapeBox.h"
#include "CBY_DxRT.h"
#include "CBY_Shadow.h"

enum HeightState
{
	HE_UP = 1,
	HE_DOWN = -1,
	HE_NOMAL = 0,
	HE_FALSE = 3,
	HE_SPLATTING = 4
};

class Sample :public CBY_MRT
{
public:
	std::shared_ptr<CBY_QuadTree> m_QuadTree;
	std::shared_ptr<C_Map_shape> m_Map;
	std::shared_ptr<CCamera> m_Camera;
	std::shared_ptr<CBY_Minimap> m_MiniMap;
	std::shared_ptr<CBY_Shadow> m_Shadow;
	D3D11_VIEWPORT	m_vp;

	//CShapeMap box;
	CShapeBox box;
	CShapeBox boxlight;
	float t;
	
	//C_LightObj m_Light;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Lightbuffer;
	CB_LIGHT m_Light;

public:
	int m_iRow;
	int m_iCol;
	int m_iMode;
	bool bButton;
	
public:
	//splatting
	ID3D11ShaderResourceView* m_pSptSRV[4];
	ID3D11ShaderResourceView* m_pAlphaSRV;
	DWORD m_dwSptSRVnum;
	DWORD m_dwSptSRV;
	ID3D11Texture2D* m_pAlphaTexture;
	ID3D11Texture2D* m_pCopyMap;
	ID3D11Texture2D* m_pHeightMap;
	D3DXMATRIX m_LTM[2];

	//tile
	ID3D11ShaderResourceView* m_pSRV[3];
	DWORD m_dwSRVnum;		//ÃÑ °³¼ö
	DWORD m_dwSRV;				//ÇöÀç ¹øÈ£

public:
	D3DXMATRIX m_matTopView;
	D3DXMATRIX m_matTopProj;
	D3DXMATRIX m_matTexture;

public:
	bool bTile;
	bool bSplatting;

public:
	ID3D11VertexShader* m_pVS;					//Á¤Àû ½¦ÀÌ´õ
	ID3D11PixelShader* m_pPS;					//ÇÈ¼¿ ½¦ÀÌ´õ
	ID3DBlob* m_pVertexCode;
	ID3DBlob* m_pPixelCode;

public:							//CS
	BufSplatting m_buf;
	ID3D11ComputeShader*        m_pCS;
	ID3D11Buffer*               m_pBuf0;
	ID3D11ShaderResourceView*   m_pBufSRV;
	ID3D11UnorderedAccessView*  m_pBufResultUAV;
	ID3D11ShaderResourceView* m_pBufResultSRV;


public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release() override;
	bool Create(int width, int height, int size, int quality, TCHAR* szTextur = nullptr);
	bool SetCS();
	void MiniMapCopy();
	void SetRelease();

public:
	void SetRadian(UINT i);

public:
	void SetHeightMode(int mode);

public:
	bool Tile(TCHAR* szTextur, int quality);

public:
	void SetTexture();
	void CopyTexture(ID3D11Texture2D* ptext);
	bool Splatting(TCHAR* szTextur, int quality);
	void SplattingRender();
	HRESULT LoadShader(const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName);
	HRESULT SaveFile(T_STR name,DWORD type);


public:
	void SetLTM(D3DXMATRIX* world);
	void Shadow(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);

public:
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public:
	Sample();
	virtual ~Sample();
};

