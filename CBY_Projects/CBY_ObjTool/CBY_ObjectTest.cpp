#include "stdafx.h"
#include "CBY_ObjectTest.h"
#include "CBY_TestMgr.h"


bool CBY_ObjectTest::SkinLoad(T_STR pszLoad)
{
	int index, list;
	index = I_TEST.SkinLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);
	m_SkinObjIndex.push_back(index);

	list = m_ObjList.size();
	m_ObjList.push_back(std::make_shared<C_SkinObj>());
	*m_ObjList[list] = *I_TEST.m_SkinLoadList[index];

	//SetCharBox();
	return true;
}

bool CBY_ObjectTest::BoneLoad(T_STR pszLoad)
{
	m_BoneIndex = I_TEST.BoneLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);

	m_BindBone = std::make_shared<C_BoneObj>();
	*m_BindBone = *I_TEST.m_BoneLoadList[m_BoneIndex];

	m_Bone = m_BindBone.get();

	m_Scene = m_BindBone->m_Scene;
	return true;
}

CBY_ObjectTest::CBY_ObjectTest()
{
}


CBY_ObjectTest::~CBY_ObjectTest()
{
}
