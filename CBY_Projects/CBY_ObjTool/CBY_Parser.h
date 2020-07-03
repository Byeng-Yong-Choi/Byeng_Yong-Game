#pragma once
#include "C_SkinParser.h"

class CBY_Parser
{
public:
	FILE*  m_pStream;
	TCHAR  m_pBuffer[256];
	TCHAR  m_pString[256];
	int    m_iTemp;
	CScene  m_Scene;
	std::vector<CBY_MeshSkin*> m_ObjList;
	std::vector<D3DXMATRIX> m_matBoneBindPoss;
	std::vector<C_Mtrl> m_MtlList;
	std::vector<int> m_Bone;
	std::vector<int> m_Root;

public:
	BOOL  OpenStream(const TCHAR* filename);
	BOOL  CloseStream();
	virtual bool  SkinLoad(const TCHAR* szFileName);
	virtual bool  SkinTextToBinary();
	virtual bool  SkinBinaryLoad(const TCHAR* szFileName);

	virtual bool  MtrLoad(const TCHAR* szFileName);
	virtual bool  MtrTextToBinary();
	virtual bool  MtrBinaryLoad(const TCHAR* szFileName);

	void Convert();

	void CreateVIData(CBY_MeshSkin* mesh);
	void VertexDataTan(CBY_MeshSkin* mesh);
	void CreateTangentSpaceVectors(D3DXVECTOR3 *v0,
		D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0,
		D3DXVECTOR2 uv1, D3DXVECTOR2 uv2,
		D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal);

	void Clear();
	int   GetParentIndex(C_STR szParentName);
	TCHAR* SaveFileDlg(TCHAR* szExt, TCHAR* szTitle);
	bool Release();

public:
	CBY_Parser();
	virtual ~CBY_Parser();
};

