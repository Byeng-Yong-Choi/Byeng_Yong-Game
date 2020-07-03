#include "C_BitMapManager.h"

C_BitMap* C_BitMapManager::Load(const TCHAR* filename)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename, Drive, Dir, name, ext);

	std::wstring nameext = name;
	nameext += ext;

	std::map<std::wstring, C_BitMap*>::iterator iter;
	iter = list.find(nameext);
	if (iter != list.end())
	{
		return (C_BitMap*)iter->second;
	}

	std::wstring loadfile = Dir;
	loadfile = m_Path+filename;
	
	C_BitMap* data = new C_BitMap;
	data->Init();

	if (data->Load(loadfile.c_str()))
	{
		list.insert(std::make_pair(nameext, data));
		return list[nameext];
	}

	delete data;

	return nullptr;
}


bool C_BitMapManager::Init()
{
	return true;
}

bool C_BitMapManager::Frame()
{
	return true;
}

bool C_BitMapManager::Render()
{
	return true;
}

bool C_BitMapManager::Release()
{
	std::map<std::wstring, C_BitMap*>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		C_BitMap* pBitMap= iter->second;
		pBitMap->Release();
		delete pBitMap;
	}
	list.clear();
	return true;
}


C_BitMapManager::C_BitMapManager()
{
	m_Path = L"../../data/";
	BitMap_Index = -1;
}


C_BitMapManager::~C_BitMapManager()
{
	Release();
}
