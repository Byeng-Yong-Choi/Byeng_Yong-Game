#include "C_SoundManager.h"

int C_SoundManager::Load(const TCHAR* filename,bool play)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename, Drive, Dir, name, ext);

	std::wstring nameExt = name;
	nameExt += ext;

	std::map<int, C_Sound*>::iterator iter;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i]->m_szName == nameExt)
		{
			return i;
		}
	}

	std::wstring loadfile = Dir;
	loadfile = m_szPath + filename;

	C_Sound* pData = new C_Sound;
	pData->m_szName = nameExt;

	pData->Init();
	if (pData->Load(m_pSystem, loadfile.c_str()) >= 0)
	{
		list.insert(std::make_pair(++m_iIndex, pData));
		//pData->m_iIndex = m_iIndex;
		return m_iIndex;
	}
	delete pData;
	return m_iIndex;
}

bool C_SoundManager::Release()
{
	for (int i = 0; i < SoundChannel; i++)
	{
		C_Sound* temp = list[i];
		temp->Release();
		delete temp;
	}
	list.clear();
	return true;
}

C_SoundManager::C_SoundManager()
{
	SoundChannel = -1;
	m_szPath = L"../../data/";
}


C_SoundManager::~C_SoundManager()
{
	Release();
}
