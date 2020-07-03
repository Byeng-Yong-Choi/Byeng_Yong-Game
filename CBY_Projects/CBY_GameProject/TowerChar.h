#pragma once
#include "C_Actor.h"

enum charstate
{
	STATE_NOMAL = 0,
	STATE_ATTACKER
};

class TowerChar:public C_Actor
{
public:
	int splitenum;
	float CharTime;
	float CharShowTime;
	bool WeaponGo;				//타워 모션중 공격체 발사
	int state;					//타워 상태
	bool LevelUP;				//타워 레벨업 유무
	int power;					//타워 공격력
	int price;					//타워 가격
	fSphere charfsp;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool GetRECT(vector<RECT>& rtList, float showtime);
	void GetSphere(float x, float y, float i);

public:
	TowerChar();
	~TowerChar();
};

