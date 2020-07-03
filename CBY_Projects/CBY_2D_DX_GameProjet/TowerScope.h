#pragma once
#include "Cstd.h"
class TowerScope
{
public:
	HDC EllipseDC;
	HBRUSH hh;
	HBITMAP aa;
	float ScorpeScale;
	fPOINT Pt;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void ScopeSetPosition(float x, float y);
	void SetScorpeScale(float x);

public:
	TowerScope();
	~TowerScope();
};

