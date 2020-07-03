#pragma once
#include "C_Object.h"
#define SIZE 10

class TowerCoice:public C_Object
{
public:
	HDC ChoiceDC;
	HBITMAP h_Choice;
	bool TowerClick;
	fPOINT Position;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();


public:
	TowerCoice();
	~TowerCoice();
};

