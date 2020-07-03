#pragma once

class TowerLose
{
public:
	float AlphaNum;
	int Death;
	int ShowTime;

public:
	bool Init();
	bool Frame();
	bool Render();									//기본 이미지 출력
	bool Release();

public:
	TowerLose();
	~TowerLose();
};

