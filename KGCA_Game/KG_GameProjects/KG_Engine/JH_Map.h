#pragma once

#include"KG_ShapeMap.h"
#include"KG_ShapeLine.h"
#include"KG_TextureMgr.h"
#include"NormalMap.h"
#include"KG_SkyBox.h"
#include"LightMgr.h"
#include"KG_Camera.h"
namespace JH
{


	struct CB_SPT
	{
		D3DXVECTOR4 MapSubData;//x:splattTexture Num
		CB_SPT()
		{
			MapSubData = D3DXVECTOR4(0, 0, 0, 0);
		}
	};
	struct SplattTextureDesc
	{
		int TexNum;
		float Alpha;
	};
	struct MapDesc
	{
		int iColumNum;
		int iRowNum;
		const TCHAR* TextureFileName;
		const TCHAR* ShaderFileName;
		const TCHAR* pNormMapFileName;
		const TCHAR* pHeightFileName;
		float fSellDistance;
		float fScaleHeight;
		//�ؽ�Ʈ����
		//���̴� 
		//�� ����
		//��
		//��
		//���� �� ��

	};

	class JH_Map :public KG_Model
	{
	public:
		bool	m_bMapEdit;

		std::shared_ptr<KG_SkyBox>										 m_SkyBox;
		std::map<int, CTexture*>										 m_vSplattTextureList;
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	 m_vSplattSRVList;
		//
		CB_SPT m_CBSubData;
		Microsoft::WRL::ComPtr<ID3D11Buffer>							 m_CBSub;
	public:
		//NormalMap
		std::vector<D3DXVECTOR3>										 m_TangentList;
		NormalMap														 m_NormalMap;
		D3DXMATRIX														 m_matNormal;

		Microsoft::WRL::ComPtr<ID3D11Buffer>							 m_pTangentVB;
		CTexture*														 m_pTexture;
		int																 m_iTexNum;
		T_STR															 m_pNormMapFileName;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>				 m_pNormSrv;

		const TCHAR*													m_pSPTAFile;

	public:
		D3DXVECTOR3 m_vEyePos;
		MapDesc m_MapDesc;
		int m_iColumNum;
		int m_iRowNum;
		int m_iVertices;
		int m_iCellCol;
		int m_iCellRow;
		int m_iCellCount;
		T_STR  m_TextureFileName;
		T_STR  m_ShaderFileName;
		T_STR  m_HegithFileName;
		T_STR m_LightFileName;
		float m_fCellDistance;
		float m_fScaleHeight;

		D3DXVECTOR3 m_CharPos;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pCopySrv;
	public:
		std::vector<float> m_vHeightList;
	public:
		float GetHeight(float fX, float fZ);
		float GetHeightMap(int iRow, int iCol);
		HRESULT Load(ID3D11Device* pD3D11Device, ID3D11DeviceContext* pD3D11DeviceContext);
		HRESULT LoadMap(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName = L"../../data/shader/DefaultShader.txt", const TCHAR* TexFileName = nullptr, const CHAR* VSName = "VS", const CHAR* PSName = "PS");
		HRESULT CreateHeightMap(ID3D11Device* pD3D11Device, ID3D11DeviceContext* pD3D11DeviceContext, const TCHAR* TextureFileName);
		void SetMapDesc(const TCHAR* TextureFileName,
			const TCHAR* ShaderFileName,
			int ColumNum,
			int RowNum,
			float fSellDistance,
			float fScaleHeight);
		void    SetMatrix(D3DXMATRIX* matWorld,
			D3DXMATRIX* matView,
			D3DXMATRIX* matProj)override;
		void UpdateConstantBuffer(ID3D11Buffer* pConstantBuffer, void* Data);
		float Lerp(float fStart, float fEnd, float fTangent);
		//�븻�� ���� ���� ���

	//�� ���� �Լ�
		INT AddSplattTexture(const TCHAR* pFileName, int Num, float Alpha = 0.0f);
		D3DXVECTOR3 GetCharPos(){ return m_CharPos; }
		void		SetCharPos(D3DXVECTOR3 vPos) { m_CharPos = vPos; }
	public:
		bool RenderSet(KG_Camera* Camera);
	public:
		HRESULT CreateInputLayout()override;
		HRESULT	LoadTexture(const TCHAR* pszTexFileName)override;
		bool	UpdateTangentBuffer();
		bool Frame()override;
		bool Release()override;

	public:

		HRESULT		CreateVertexData();
		HRESULT		CreateIndexData();
	public:
		void ShadowRenderSet();
		bool NoneLightRenderSet(KG_Camera* pCamera);
	public:
		JH_Map();
		virtual ~JH_Map();
	};
}