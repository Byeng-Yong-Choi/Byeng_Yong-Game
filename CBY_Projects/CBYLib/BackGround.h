#pragma once
#include "C_Object.h"
class BackGround:public C_Object
{

public:
	bool Init();
	bool Frame();
	bool Render();									//기본 이미지 출력
	bool Release();

public:
	BackGround();
	~BackGround();
};

