#pragma once
#include "KG_ShapeBox.h"
#include "KG_ShapeLine.h"
namespace CBY
{

	class CBY_CharBox : public KG_ShapeBox
	{
	private:
		int m_iBoneIndex;
		KG_Box m_Box;
		std::vector<D3DXMATRIX*> m_pParmatList;
		D3DXMATRIX* m_pBindmat;
		D3DXVECTOR3 m_vMax;
		D3DXVECTOR3 m_vMin;
		D3DXVECTOR3 m_vInitPos;
		D3DXVECTOR3 m_vPos;
		D3DXVECTOR3 m_vBoxSize;
		D3DXVECTOR3 m_vInitBoxSize;
		std::vector<PNCT_VERTEX> m_VertexList;
		KG_ShapeLine m_DebugLine;

	public:
		HRESULT UpdateBoxVB();
		HRESULT CreateIndexData();
		bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName = L"../../data/shader/DefaultShader.txt", const TCHAR* TexFileName = nullptr, const CHAR* VSName = "VSmat", const CHAR* PSName = "PS") override;
		void CreateBox(int index, D3DXVECTOR3 vPos, float fXsize, float fYsize, float fZsize, D3DXMATRIX mRot);
		void SetBox(float fXsize, float fYsize, float fZsize, int id);
		void UpdateBoxAxis(D3DXMATRIX mat);
	
	public:
		virtual bool Init() override;
		virtual bool Frame() override;
		virtual bool Render() override;
		virtual bool Release() override;


	public:
		void SetParentMatrix(D3DXMATRIX* mat);
		void SetBindMatrix(D3DXMATRIX* mat);
		D3DXMATRIX GetParentMatrix(int index);
		D3DXMATRIX GetBindMatrix();
		D3DXMATRIX GetWorldMatrix(int index);
		int GetBoneIndex();
		D3DXVECTOR3 GetSize();
		D3DXVECTOR3 GetPos();


	public:
		inline void SetBoxCenter(D3DXVECTOR3 vCenter) { m_Box.vCenter = vCenter; }
		inline void SetInitPos(D3DXVECTOR3 pos)
		{
			m_vInitPos = pos;
		};
		inline D3DXVECTOR3 GetInitPos()
		{
			return m_vInitPos;
		};
		inline void SetInitBoxSize(D3DXVECTOR3 pos)
		{
			m_vInitBoxSize = pos;
		};
		inline D3DXVECTOR3 GetInitBoxSize()
		{
			return m_vInitBoxSize;
		};

		inline KG_Box GetBox()
		{
			return m_Box;
		};

	public:
		CBY_CharBox();
		virtual ~CBY_CharBox();
	};
}
