#pragma once
#include "CShapeBox.h"

class CBY_CharBox : public CShapeBox
{
private:
	int m_iBoneIndex;
	CBY_Box m_Box;
	std::vector<D3DXMATRIX*> m_pParmatList;
	D3DXMATRIX* m_pBindmat;
	D3DXVECTOR3 m_vMax;
	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vBoxSize;
	D3DXVECTOR3 m_vInitPos;
	D3DXVECTOR3 m_vInitBoxSize;
	std::vector<PNCT_VERTEX> m_VertexList;

public:
	HRESULT UpdateBoxVB();
	HRESULT CreateIndexData();
	void CreateBox(int index, D3DXVECTOR3 vPos, float fXsize, float fYsize, float fZsize, D3DXMATRIX mRot);
	void UpdateBoxAxis(D3DXMATRIX mat);


public: //set
	void SetBox(float fXsize, float fYsize, float fZsize, int id);
	void SetParentMatrix(D3DXMATRIX* mat);
	void SetBindMatrix(D3DXMATRIX* mat);
	inline void SetInitPos(D3DXVECTOR3 pos)
	{
		m_vInitPos = pos;
	};
	inline void SetInitBoxSize(D3DXVECTOR3 pos)
	{
		m_vInitBoxSize = pos;
	};



public: //get
	D3DXMATRIX GetParentMatrix(int index);
	D3DXMATRIX GetBindMatrix();
	D3DXMATRIX GetWorldMatrix(int index);
	int GetBoneIndex();
	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetPos();
	inline D3DXVECTOR3 GetInitPos()
	{
		return m_vInitPos;
	};
	inline D3DXVECTOR3 GetInitBoxSize()
	{
		return m_vInitBoxSize;
	};

public:
	CBY_CharBox();
	virtual ~CBY_CharBox();
};
