#pragma once
#include "header.h"
#define CTL_CHARS		31
#define SINGLE_QUOTE	39 // ( ' )
#define ZERO 1.0e-3f

class CExpGlobal
{
	CScene m_Scene;

public:
	Interface* m_pMax;
	Interval m_AnimTime;
	INode* m_pRootNode;
	TCHAR m_tmpBuffer[MAX_PATH];
	std::vector<INode*> m_pReadObjList;
	std::vector<Mtl*> m_pReadMtlList;
	std::vector<int> m_SelectNodeNumber;

	std::vector<CMesh> m_ObjectList;
	std::vector<CMtrl> m_MtlList;

	std::vector<int> m_ExpSelNodes;			//셀렉에 해당하는 것을 알아볼수 있게 저장

	std::vector<BipedVertex>  m_BipedList;

public:
	int GetFindIndex(INode* node);
	void GetSelectNode(INode* node);

public:
	bool Init(Interface* pMax);
	void Release();
	void PreProcess(INode* pNode);
	void AddObject(INode* pNode);
	void AddMtl(INode* pNode);
	bool Exporter();
	void SkinExporter();
	void MatrixExporter();
	bool Convert();
	void GetTexture(Mtl* pMtl, CMtrl& tMtrl);
	void GetMesh(INode* pnode, CMesh* pmesh);
	void SetPNCTData(INode* pnode,int Face, CMesh* pmesh, Mesh* mesh, Matrix3 tm, bool bSubMtrl,bool bipedInsert);
	TriObject* GetTriObjectFromNoode(INode* pNode, int iStartTick, BOOL& needDel);	
	void GetDecompAffine(TimeValue t, INode* pNode, AffineParts* ap);
	bool CheckForAnimation(INode* pNode, BOOL& bPos, BOOL& bRot, BOOL& bScl);		//애니메이션 존재 판단
	bool TMNegParity(Matrix3 &m);													//스케일이 축소가 뒤집어 진후 결과 확대가 된지 확인(외적과 내적으로 판단)
	Point3  GetVertexNormal(Mesh* mesh, int iFace, RVertex* rv);
	TCHAR* SaveFileDlg(TCHAR* szExt, TCHAR* szTitle);

public:
	Modifier* FindModifier(INode *nodePtr, Class_ID classID);
	void ExportPhysiqueData(INode* pNode, Modifier*Modifier);
	void ExportSkinData(INode* pNode, Modifier* Modifier);

public:
	void DumpMatrix3(Matrix3* m, D3D_MATRIX& mat);
	void Matrix3convert(Matrix3& befor, Matrix3& after);
	void DumpPoint3(Point3& pDest, Point3& pSrc);
	void DumpPosSample(INode* pNode, CMesh* pMesh);
	void DumpRotSample(INode* pNode, CMesh* pMesh);
	void DumpSclSample(INode* pNode, CMesh* pMesh);
	void DumpQuat(Quat& pDest, Quat& pSrc);

public:
	bool EqualPoint1(float p1, float p2);			//실수는 완전히 0으로 나오는게 아니라 0으로 수렴하므로 판단을 하기위한 함수
	bool EqualPoint2(Point2 p1, Point2 p2);
	bool EqualPoint3(Point3 p1, Point3 p2);
	bool EqualPoint4(Point4 p1, Point4 p2);
	bool EqualQuat(Quat p1, Quat p2);
	TCHAR*	FixupName(MSTR name);
	void   InsertWeight(DWORD dwSrc, BYTE dwBone, float fWeight);

public:
	CExpGlobal();
	~CExpGlobal();
};


