#include "CBY_MeshSkin.h"



CBY_MeshSkin::CBY_MeshSkin()
{
	m_Parent = nullptr;
}


CBY_MeshSkin::~CBY_MeshSkin()
{
	m_BoneBox.Release();
	subMeshSkin.clear();
	listSkin.clear();
	vblistSkin.clear();
	iblistSkin.clear();
	for (int i = 0; i < m_Child.size(); i++)
	{
		m_Child[i] = nullptr;
	}
	m_Child.clear();
	m_Parent=nullptr;
}
