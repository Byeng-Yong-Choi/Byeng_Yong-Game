#pragma once
#include "Cstd.h"
#include "C_ObjStd.h"

static std::vector<std::string> CBYaseSections = { "SCENE ","MAT ", "OBJECT " };


static std::vector<std::string> CBYMaterialSections = { "MATERIAL ","SUBMATERIAL ","TEXTURE" };


static std::vector<std::string> CBYGeomObjectSections = { "OBJECT ","NAME ", "MATWORLD", "SUBMESH ","VERTEX_POSITION ","VERTEX_NOMAL ","VERTEX_COLOR ", "VERTEX_TEXTURE ","ANIMATION " };

enum cbySectionType
{
	CBYSCENE = 0,
	CBYMATERIAL,
	CBYGEOMOBJECT,
};
enum cbyMaterialType
{
	NEWMAT = 0,
	NEWSUBMAT,
	TEXT,
};

enum cbyObjectType
{
	OBJECT = 0,
	NAME,
	WORLD,
	SUBMESH,
	GE_POSITION,
	NOMAL,
	COLOR,
	UV,
	ANI
};


class C_CBYParser
{
public:
	CScene m_Scene;
	std::vector<CBY_Mesh> m_Mesh;
	std::vector<C_Mtrl> m_MtrlList;
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
	void LoadMaterialData(int i, int iSub);
	void LoadGeomobject();

public:
	template <typename OutputIterator>
	void Tokenize(const C_STR& text, const C_STR& token, OutputIterator ListData);		//리스트로 끊어서 사용하는것이 목적

public:
	C_CBYParser();
	virtual ~C_CBYParser();
};


