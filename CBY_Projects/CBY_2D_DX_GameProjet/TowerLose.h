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
	bool Render();									//�⺻ �̹��� ���
	bool Release();

public:
	TowerLose();
	~TowerLose();
};

