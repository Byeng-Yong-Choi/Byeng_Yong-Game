#pragma once
#include <map>
#include "C_Sound.h"

const int g_iMaxSound = 32;

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
	int Load(const TCHAR* filename, bool play, float fvol);
	bool Init();
	bool Frame();

private:
	C_SoundManager();
public:
	~C_SoundManager();
};

#define I_SOUND C_SoundManager::Get()
