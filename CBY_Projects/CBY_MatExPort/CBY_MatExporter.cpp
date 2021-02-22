#include "header.h"
#include "CExpGlobal.h"
#include "resource.h"
#define CBY_MatEXPORT_CLASS_ID Class_ID(0x4ccb46c6, 0x312e5b58)

class C_MatExport : public UtilityObj
{
public:
	HWND m_hPanel;		//패널 핸들
	CExpGlobal m_Exp;

public:
	//Constructor/Destructor
	C_MatExport();
	~C_MatExport();
	virtual void BeginEditParams(Interface *ip, IUtil *iu);
	virtual void EndEditParams(Interface *ip, IUtil *iu);
	virtual void SelectionSetChanged(Interface *ip, IUtil *iu);
	virtual void DeleteThis();
	virtual void SetStartupParam(MSTR param);
	static C_MatExport* GetInstance() {
		static C_MatExport theExp;
		return &theExp;
	}

private:
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#define MAT_EXPORT C_MatExport::GetInstance()


class CBYSkinExportClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) { return new C_MatExport(); }
	virtual const TCHAR *	ClassName() { return _T("C_SkinExporter"); }
	virtual SClass_ID SuperClassID() { return UTILITY_CLASS_ID; }
	virtual Class_ID ClassID() { return CBY_MatEXPORT_CLASS_ID; }
	virtual const TCHAR* Category() { return _T("C_MatExportClassDesc"); }

	virtual const TCHAR* InternalName() { return _T("CBYMatExportClassDesc"); }
	virtual HINSTANCE HInstance() { return hInstance; }


};


ClassDesc2* GetExportDesc() {
	static CBYSkinExportClassDesc C_SkinExport;
	return &C_SkinExport;
}


C_MatExport::C_MatExport()
{

}

C_MatExport::~C_MatExport()
{

}

INT_PTR CALLBACK C_MatExport::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		//MessageBox(NULL, L"a", L"b", MB_OK);
	}break;

	case WM_DESTROY:
	{
		//MessageBox(NULL, L"a", L"b", MB_OK);
	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
		{
			MAT_EXPORT->m_Exp.MatrixExporter();
		}break;
		}
	}break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		GetCOREInterface()->RollupMouseMessage(hWnd, msg, wParam, lParam);
		break;
	default:
		return 0;
	}
	return 1;
}

void C_MatExport::BeginEditParams(Interface *ip, IUtil *iu)
{
	m_hPanel = ip->AddRollupPage(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), DlgProc, _T("C_Utility"), 0);
}

void C_MatExport::EndEditParams(Interface *ip, IUtil *iu)
{
	ip->DeleteRollupPage(m_hPanel);
}

void C_MatExport::SelectionSetChanged(Interface *ip, IUtil *iu)
{
	if (ip->GetSelNodeCount() <= 0)return;				//선택된게 없다.
	MAT_EXPORT->m_Exp.Release();									//메모리 초기화
	if (MAT_EXPORT->m_Exp.Init(ip))
	{
		MAT_EXPORT->m_Exp.Convert();
	}

	for (int iObj = 0; iObj < ip->GetSelNodeCount(); iObj++)
	{
		INode* pNode = ip->GetSelNode(iObj);			//선택된 노드 호출
		MAT_EXPORT->m_Exp.GetSelectNode(pNode);						//익스포터로 노드를 보내주고 원하는 작업을 한다.
	}
}

void C_MatExport::DeleteThis()
{

}

void C_MatExport::SetStartupParam(MSTR param)
{

}



