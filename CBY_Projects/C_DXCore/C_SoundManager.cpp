#include "C_SoundManager.h"

int C_SoundManager::Load(const TCHAR* filename,bool play, float fvol)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename, Drive, Dir, name, ext);

	std::wstring nameExt = name;
	nameExt += ext;

	std::map<int, C_Sound*>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		if (iter->second->m_szName == nameExt)
		{
			return iter->first;
		}
	}

	std::wstring loadfile = Dir;
	loadfile = m_szPath + filename;

	C_Sound* pData = new C_Sound;
	pData->m_szName = nameExt;
	pData->fVolume = fvol;

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

bool C_SoundManager::Init()
{
	FMOD::System_Create(&m_pSystem);
	FMOD_RESULT ret = m_pSystem->init(g_iMaxSound, FMOD_INIT_NORMAL, 0);
	if (ret != FMOD_OK)
	{
		return false;
	}
	return true;
}

bool C_SoundManager::Frame()
{
	m_pSystem->update();

	return true;
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
	m_iIndex = 0;
	m_szPath = L"../../data/sound/";
}


C_SoundManager::~C_SoundManager()
{
	Release();
}
