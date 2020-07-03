#pragma once
#include "Cstd.h"
#include "fmod.h"
#include "fmod.hpp"
//#define MAX_SOUNDLIST 32

class C_Sound
{
public:
	std::wstring  m_szName;
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pChannel;
	int m_SoundList;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	
public:
	bool Load(FMOD::System*  pSystem,const TCHAR* SoundFile);
	bool Play(FMOD::System*  pSystem);
	bool Pause();
	bool Volume(float ChangeVolume, bool UP);

public:
	C_Sound();
	~C_Sound();
};

