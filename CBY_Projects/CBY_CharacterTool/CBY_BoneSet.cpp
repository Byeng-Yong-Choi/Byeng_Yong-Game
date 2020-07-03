// CBY_BoneSet.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_BoneSet.h"
#include "afxdialogex.h"


// CBY_BoneSet 대화 상자

IMPLEMENT_DYNAMIC(CBY_BoneSet, CDialogEx)

CBY_BoneSet::CBY_BoneSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_XSize(_T(""))
	, m_YSize(_T(""))
	, m_ZSize(_T(""))
	, m_iDirection(0)
	, m_dirRadio(0)
{

}

CBY_BoneSet::~CBY_BoneSet()
{
}

void CBY_BoneSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_XSize);
	DDX_Text(pDX, IDC_EDIT2, m_YSize);
	DDX_Text(pDX, IDC_EDIT5, m_ZSize);
	DDX_Radio(pDX, IDC_RADIO1, (int&)m_dirRadio);
}


BEGIN_MESSAGE_MAP(CBY_BoneSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_BoneSet::OnBnClickedButton1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CBY_BoneSet::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_BoneSet::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_BoneSet::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CBY_BoneSet::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_BUTTON5, &CBY_BoneSet::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON6, &CBY_BoneSet::OnBnClickedColBoxRender)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO6, &CBY_BoneSet::RadioCtrl)
	ON_BN_CLICKED(IDC_BUTTON7, &CBY_BoneSet::OnBnClickedColBoxAllRender)
	ON_BN_CLICKED(IDC_BUTTON8, &CBY_BoneSet::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CBY_BoneSet::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CBY_BoneSet::OnBnClickedButton10)
END_MESSAGE_MAP()


// CBY_BoneSet 메시지 처리기

void CBY_BoneSet::RadioCtrl(UINT id)
{
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();

	m_iDirection = m_dirRadio;
	
	UpdateData(FALSE);
}


void CBY_BoneSet::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_TreeCtrl.DeleteAllItems();
	m_hRoot.clear();
	m_hChild.clear();

	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		DWORD dwRootsize = pApp->m_main.m_Character->m_Bone->m_iRootList.size();
		for (int i = 0; i < dwRootsize; i++)
		{
			DWORD dwRootNum = pApp->m_main.m_Character->m_Bone->m_iRootList[i];
			std::string st = pApp->m_main.m_Character->m_Bone->m_ObjectList[dwRootNum]->m_szName.c_str();
			CString string(st.c_str());
			
			HTREEITEM root = m_TreeCtrl.InsertItem(string, NULL, TVI_ROOT);
			m_hRoot.push_back(root);

			TreeChildSet(root, pApp->m_main.m_Character->m_Bone->m_ObjectList[dwRootNum]);
		}
	}
}

void CBY_BoneSet::TreeChildSet(HTREEITEM parent, CBY_MeshSkin* obj)
{
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	DWORD ChildSize = obj->m_Child.size();

	for (DWORD dwchild = 0; dwchild < ChildSize; dwchild++)
	{
		std::string st = obj->m_Child[dwchild]->m_szName.c_str();
		CString string(st.c_str());

		HTREEITEM child = m_TreeCtrl.InsertItem(string, parent, TVI_LAST);
		m_hChild.push_back(child);
		TreeChildSet(child, obj->m_Child[dwchild]);
	}
}


BOOL CBY_BoneSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBY_BoneSet::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM selitem = m_TreeCtrl.GetSelectedItem();
	
	m_Selectstr = m_TreeCtrl.GetItemText(selitem);
	*pResult = 0;
}


void CBY_BoneSet::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		for (int i = 0; i < pApp->m_main.m_Character->m_Bone->m_ObjectList.size(); i++)
		{
			if (m_Selectstr == pApp->m_main.m_Character->m_Bone->m_ObjectList[i]->m_szName.c_str())
			{
				pApp->m_main.m_Character->m_iBoneSelect = i;
				return;
			}
		}
	}
}


void CBY_BoneSet::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.m_Character->m_iBoneSelect = -1;
		pApp->m_main.m_Character->bColBoxRender = false;
	}
}


void CBY_BoneSet::OnBnClickedCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	float fX, fY, fZ;
	fX = _ttof(m_XSize);
	fY = _ttof(m_YSize);
	fZ = _ttof(m_ZSize);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.m_Character->CreateCharBox(fX, fY, fZ);
	}
	UpdateData(FALSE);
}


void CBY_BoneSet::OnBnClickedApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	float fX, fY, fZ;
	fX = _ttof(m_XSize);
	fY = _ttof(m_YSize);
	fZ = _ttof(m_ZSize);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.m_Character->SetCharBox(m_iDirection, fX, fY, fZ);
	}
	UpdateData(FALSE);
}


void CBY_BoneSet::OnBnClickedColBoxRender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.m_Character->bColBoxRender = true;
	}
}


void CBY_BoneSet::OnBnClickedColBoxAllRender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		if (!pApp->m_main.m_Character->m_bAllBoxRender)
		{
			pApp->m_main.m_Character->m_bAllBoxRender = true;
		}
		else
		{
			pApp->m_main.m_Character->m_bAllBoxRender = false;
		}
	}
}


void CBY_BoneSet::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		int iSel = pApp->m_main.m_Character->m_iBoneSelect;
		pApp->m_main.ObjSocketSet(iSel,0);
	}
}


void CBY_BoneSet::OnBnClickedButton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		int iSel = pApp->m_main.m_Character->m_iBoneSelect;
		pApp->m_main.SetCharCameraSocket(iSel);
	}
}


void CBY_BoneSet::OnBnClickedButton10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		int iSel = pApp->m_main.m_Character->m_iBoneSelect;
		pApp->m_main.SetWeaponFireSocket(iSel);
	}
}
