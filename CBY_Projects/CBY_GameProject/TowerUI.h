#pragma once
#include "C_Object.h"

class TowerUI:public C_Object
{
public:
	bool Click;
	bool splite;
	float ClickTime;

public:
	bool Init();
	bool Frame();
	bool Render();	
	bool Release();
	//bool GetRECT(RECT rt1);

public:
	TowerUI();
	~TowerUI();
};

