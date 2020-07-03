#pragma once
#include <map>
#include "C_Sound.h"

class C_SoundManager
{
public:
	std::map<int, C_Sound*> list;
	std::wstring	m_szPath;
	int				m_iIndex;
	FMOD::System*  m_pSystem;
	HFONT   m_hDefaultFont;

private:
	int SoundChannel;

public:
	
	bool Release();
	static C_SoundManager& Get()
	{
		static C_SoundManager mgr;
		return mgr;
	}
	int Load(const TCHAR* filename, bool play);

private:
	C_SoundManager();
public:
	~C_SoundManager();
};

