// CBY_ObjSoket.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_ObjSoket.h"
#include "afxdialogex.h"


// CBY_ObjSoket 대화 상자

IMPLEMENT_DYNAMIC(CBY_ObjSoket, CDialogEx)

CBY_ObjSoket::CBY_ObjSoket(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
	, m_Yaw(_T(""))
	, m_Pitch(_T(""))
	, m_MoveX(_T(""))
	, m_MoveY(_T(""))
	, m_MoveZ(_T(""))
	, m_Scale(_T(""))
	, m_Roll(_T(""))
{

}

CBY_ObjSoket::~CBY_ObjSoket()
{
}

void CBY_ObjSoket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_BoneList);
	DDX_Text(pDX, IDC_EDIT1, m_Yaw);
	DDX_Text(pDX, IDC_EDIT2, m_Pitch);
	DDX_Text(pDX, IDC_EDIT5, m_MoveX);
	DDX_Text(pDX, IDC_EDIT6, m_MoveY);
	DDX_Text(pDX, IDC_EDIT7, m_MoveZ);
	DDX_Text(pDX, IDC_EDIT8, m_Scale);
	DDX_Text(pDX, IDC_EDIT9, m_Roll);
}


BEGIN_MESSAGE_MAP(CBY_ObjSoket, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CBY_ObjSoket::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_ObjSoket::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_ObjSoket::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_ObjSoket::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON4, &CBY_ObjSoket::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CBY_ObjSoket::OnBnClickedButton5)
END_MESSAGE_MAP()


// CBY_ObjSoket 메시지 처리기


void CBY_ObjSoket::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM selitem = m_BoneList.GetSelectedItem();

	m_Selectstr = m_BoneList.GetItemText(selitem);
	*pResult = 0;
}


void CBY_ObjSoket::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_BoneList.DeleteAllItems();
	m_hRoot.clear();
	m_hChild.clear();

	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character->m_ObjectList[0]!=nullptr)
	{
		DWORD dwRootsize = pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_iRootList.size();
		for (int i = 0; i < dwRootsize; i++)
		{
			DWORD dwRootNum = pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_iRootList[i];
			std::string st = pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_ObjectList[dwRootNum]->m_szName.c_str();
			CString string(st.c_str());

			HTREEITEM root = m_BoneList.InsertItem(string, NULL, TVI_ROOT);
			m_hRoot.push_back(root);

			TreeChildSet(root, pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_ObjectList[dwRootNum]);
		}
	}
	
}

void CBY_ObjSoket::TreeChildSet(HTREEITEM parent, CBY_MeshSkin* obj)
{
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	DWORD ChildSize = obj->m_Child.size();

	for (DWORD dwchild = 0; dwchild < ChildSize; dwchild++)
	{
		std::string st = obj->m_Child[dwchild]->m_szName.c_str();
		CString string(st.c_str());

		HTREEITEM child = m_BoneList.InsertItem(string, parent, TVI_LAST);
		m_hChild.push_back(child);
		TreeChildSet(child, obj->m_Child[dwchild]);
	}
}


void CBY_ObjSoket::OnBnClickedSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character->m_ObjectList[0])
	{
		for (int i = 0; i < pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_ObjectList.size(); i++)
		{
			if (m_Selectstr == pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_ObjectList[i]->m_szName.c_str())
			{
				pApp->m_main.m_Character->m_ObjectList[0]->SetObjSocke(i);
				return;
			}
		}
	}
}


void CBY_ObjSoket::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CBY_ObjSoket::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character->m_ObjectList[0])
	{
		for (int i = 0; i < pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_ObjectList.size(); i++)
		{
			if (m_Selectstr == pApp->m_main.m_Character->m_ObjectList[0]->m_Bone->m_ObjectList[i]->m_szName.c_str())
			{
				pApp->m_main.m_Character->m_ObjectList[0]->SetFireSocket(i);
				return;
			}
		}
	}
}


void CBY_ObjSoket::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character->m_ObjectList[0])
	{
		float fx, fy,fz;
		fx = _ttof(m_MoveX);
		fy = _ttof(m_MoveY);
		fz = _ttof(m_MoveZ);

		float fYaw, fPitch, fRoll;
		fYaw = _ttof(m_Yaw);
		fPitch = _ttof(m_Pitch);
		fRoll = _ttof(m_Roll);

		float fscale;
		fscale = _ttof(m_Scale);
		
		pApp->m_main.SetObjectMove(fx, fy, fz);

		pApp->m_main.SetObjectRot(fYaw, fPitch, fRoll);

		pApp->m_main.SetObjectScale(fscale);
	}
	UpdateData(FALSE);
}

