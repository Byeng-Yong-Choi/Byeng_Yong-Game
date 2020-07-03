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

	std::vector<int> m_ExpSelNodes;			//������ �ش��ϴ� ���� �˾ƺ��� �ְ� ����

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
	bool CheckForAnimation(INode* pNode, BOOL& bPos, BOOL& bRot, BOOL& bScl);		//�ִϸ��̼� ���� �Ǵ�
	bool TMNegParity(Matrix3 &m);													//�������� ��Ұ� ������ ���� ��� Ȯ�밡 ���� Ȯ��(������ �������� �Ǵ�)
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
	bool EqualPoint1(float p1, float p2);			//�Ǽ��� ������ 0���� �����°� �ƴ϶� 0���� �����ϹǷ� �Ǵ��� �ϱ����� �Լ�
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


