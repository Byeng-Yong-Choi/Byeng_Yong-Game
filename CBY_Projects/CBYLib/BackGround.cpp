#include "BackGround.h"

bool BackGround::Init()
{
	C_Object::Init();
	return true;
}

bool BackGround::Frame()
{
	return true;
}

bool BackGround::Render()
{
	C_Object::Render();
	return true;
}

bool BackGround::Release()
{
	C_Object::Release();
	return true;
}

BackGround::BackGround()
{
}


BackGround::~BackGround()
{
}
