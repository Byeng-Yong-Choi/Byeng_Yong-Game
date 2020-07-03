#pragma once
#include <map>
#include <string>
#include "C_BitMap.h"
class C_BitMapManager
{
private:
	int BitMap_Index;
	std::wstring  m_Path;
	std::map<std::wstring, C_BitMap*> list;

public:
	static C_BitMapManager& get()
	{
		static C_BitMapManager mgr;
		return mgr;
	}
	C_BitMap* Load(const TCHAR* filename);

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	C_BitMapManager();
	~C_BitMapManager();
};

