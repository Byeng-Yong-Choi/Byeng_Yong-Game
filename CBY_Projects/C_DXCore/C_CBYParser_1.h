#pragma once
#include "C_ObjStd.h"
class C_CBYParser_1
{
public:
	FILE*  m_pStream;
	TCHAR  m_pBuffer[256];
	TCHAR  m_pString[256];
	int    m_iTemp;
	CScene  m_Scene;
	std::vector<CBY_MeshSkin*>    m_ObjList;
	std::vector<C_Mtrl>    m_MtlList;
	std::vector<D3DXMATRIX> m_matBoneBindPoss;
public:
	BOOL  OpenStream(const TCHAR* filename);
	BOOL  CloseStream();
	virtual bool  Load(const TCHAR* szFileName);
	int   GetParentIndex(C_STR szParentName);
	bool Release();
public:
	C_CBYParser_1();
	virtual ~C_CBYParser_1();
};
