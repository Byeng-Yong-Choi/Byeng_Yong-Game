#pragma once
#include "Cstd.h"
#include "C_ObjStd.h"

static std::vector<std::string> aseSections = { "SCENE ","MATERIAL_LIST ", "GEOMOBJECT ","HELPEROBJECT " };


static std::vector<std::string> MaterialSections = { "MATERIAL ","SUBMATERIAL ","NUMSUBMTLS","MAP_SUBNO ","BITMAP ", "GEOMOBJECT ","HELPEROBJECT " };


static std::vector<std::string> GeomObjectSections = { "NODE_NAME ","NODE_PARENT ","NODE_TM ", "MESH ","MATERIAL_REF ", "TM_ANIMATION " };

enum aseSectionType 
{
	SCENE = 0,
	MATERIAL,
	GEOMOBJECT,
	HELPEROBJECT
};
enum aseMaterialType
{ 
	NEWMATERIAL = 0, 
	NEWSUBMATERIAL,
	SUBNUMCOUNT,
	SUBNO,
	TEXTURE,
	GEOM_OBJECT,
	HELPER_OBJECT
};

enum aseObjectType
{ 
	NODENAME = 0,
	PARENTNAME,
	TM,
	MESH,
	REF,
	ANIMATION
};

struct CFace
{
	D3DXVECTOR3 p[3];
};


struct CGeomObject
{
	CHAR szName[256];
	CHAR szParent[256];
	D3DXVECTOR3 v[4];

	int  iNumPosVertex;
	int  iNumFace;
	std::vector<D3DXVECTOR3>   PosVertexList;
	std::vector<CIndex>        PosFaceList;

	int  iNumUVVertex;
	int  iNumUVFace;
	std::vector<D3DXVECTOR2>   UVVertexList;
	std::vector<CIndex>        UVFaceList;

	int  iNumColorVertex;
	int  iNumColorFace;
	std::vector<D3DXVECTOR3>   ColorVertexList;
	std::vector<CIndex>        ColorFaceList;

	std::vector<CFace>		  NormalVertexList;

	std::vector<CAnimationTrack>  AniTrack_Pos;
	std::vector<CAnimationTrack>  AniTrack_Rotate;
	std::vector<CAnimationTrack>  AniTrack_Scale;

	int  m_iRef;
	std::vector<CGeomObject> subObj;
	
	CGeomObject()
	{
		m_iRef = -1;
		iNumPosVertex = 0;
		iNumFace = 0;
		iNumUVVertex = 0;
		iNumUVFace = 0;
		iNumColorVertex = 0;
		iNumColorFace = 0;
		ZeroMemory(szName, sizeof(char) * 256);
		ZeroMemory(szParent, sizeof(char) * 256);
	}
};

class C_ASEParser
{
public:
	CScene m_Scene;
	std::vector<C_Mtrl> m_MtrlList;
	std::vector<CGeomObject> m_ObjectList;
	std::vector<C_STR> m_ListTokens;
	TCHAR* m_pwcMeshData;
	CHAR* m_pmbMeshData;
	int m_iTokenindex;
	std::string m_smbTokenData;
	CHAR m_TempString[256];

public:
	bool Load(const TCHAR* pszfilename);
	bool BeginToken();		//m_ListTokens를 채우기위한 함수 (함수의 목적)
	int SearchTokenArray(std::vector<std::string>& asesections);
	bool GetNextLine();
	bool GetData(const CHAR* pString, VOID* pData, INT dataType, BOOL bLoop = TRUE);		//DEFAULT값은 찾을때까지 돌아라
	const CHAR* GetSearchString(const CHAR* pString, BOOL bLoop);
	bool GetValue(VOID* pData, INT dataType);
	int  GetParentIndex(C_STR szParentName);
	bool Release();

public:
	void LoadScene();
	void LoadMaterial();
	void LoadMaterialData(int i,int iSub);
	void LoadGeomobject();

public:
	template <typename OutputIterator>
	void Tokenize(const C_STR& text, const C_STR& token, OutputIterator ListData);		//리스트로 끊어서 사용하는것이 목적

public:
	C_ASEParser();
	virtual ~C_ASEParser();
};

