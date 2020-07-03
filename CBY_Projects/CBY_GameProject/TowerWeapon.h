#pragma once
#include "Weapon.h"
class TowerWeapon: public Weapon
{
public:
	int WeaponSpeed;
	fPOINT monpt;
	int monsternum;
	//bool WeaponGO;
	bool Buu;
	bool Cold;
	float Ditance;			//몬스터와의 거리만큼 이펙트 출력
	int splitenum;
	float WeaponShowTime;
	float mondgr;
	int Power;


public:
	bool Frame();
	bool Render();
	void GetMonsterPosition(fPOINT mon);
	void GetMonsterNum(int i);

public:
	TowerWeapon();
	~TowerWeapon();
};

