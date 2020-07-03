#pragma once
//타워
#define TOWERCHOICENUM 3				//타워 선택 수
#define TOWERNUM 3						//타워 종류
#define WEAPONNUM 3						//무기 종류
#define TOWERSPEED 0.8f					//타워 공격 속도
#define EFFECTSHOWTIME 0.3f				//이펙트 터지는 시간
#define POSITIONNUM 12					//타워 설치 가능 장소 갯수

//플레이어
#define GAMELIFE 5					//플레이어의 체력
#define STARTMONEY 100					//초기 시작 돈

//몬스터
#define MONSTERLEVEL 6					//몬스터 증가수와 체력 증가수치
#define MONSTERNUM 10					//초기 몬스터 수
#define MONSTERDEATHMONEY 10			//몬스터 죽였을시 얻는 돈의 양
#define HP 13							//몬스터 초기 체력
#define MONSTERSPPED 150				//몬스터 이동속도 150
#define MONSTERMAKETIME 1.5f			//몬스터 생성 후 다음 몬스터 생성까지의 시간 1.5
#define NEXTLEVEL 8.0f					//다음 레벨 몬스터 출현까지 걸리는 시간

//Tower 공격력
#define POWER 2							//타워 공격력


extern float g_Money;