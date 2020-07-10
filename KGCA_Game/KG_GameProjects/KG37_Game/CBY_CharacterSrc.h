#pragma once
#include "CBY_Character.h"
#include "CBY_Bullet.h"
#include "KG_Collision.h"
#include "KG_Camera.h"



namespace CBY
{
	enum CBY_SoundTrack
	{
		SOUND_HIT,
		SOUND_DIE,
		SOUND_ATTACK,
		SOUND_MOVE,
		SOUND_RUN,
		SOUND_RELOAD,
		SOUND_COUNT
	};

	class CBY_CharacterSrc
	{
	private:
		DWORD m_dwFrame;
		DWORD m_dwMoveFrame;

	protected:
		float m_fFireTime;		//발사 설정 시간
		float m_fFireSetTime;	//발사 시간까지의 계산시간
		bool m_bFireTime;
		float m_fSpeed;
		D3DXVECTOR3 m_vAfterCharPos;		//누적된 캐릭터 위치
		D3DXVECTOR3 m_vChangPos;			//프레임 구간으로 바뀐 캐릭터의 위치;
		D3DXVECTOR3 m_vCharPos;				//현 캐릭터 위치
		D3DXVECTOR3 m_vFireSocketPos;		//정해진 발사 소켓에서 더 이동될 위치값
		KG_Camera* m_pCamera;
		DWORD m_dwInitHP;
		float m_vPosHeight;
		int m_iHP;
		float m_fScale;					//캐릭터의 크기

	protected:
		std::vector<bool> m_RayCrash;	//레이 방향에 따른 충돌여부
		std::vector<DWORD> m_dwColBox;	//레이 방향에 따른 충돌된 박스

	protected:
		DWORD m_dwCharState;
		std::shared_ptr<CBY::CBY_Character> m_Character;
		CBY::CBY_Bullet m_Bullet;
		bool m_bDamge;

	public:
		std::vector<DWORD> m_dwSoundList;

	public:
		virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
		virtual bool Init();
		virtual bool Frame();
		virtual bool Render();
		virtual bool Release();

	public:
		virtual void InitSet();		//초기화 함수

	public:
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
		virtual void SetState(int i);
		virtual void SetFireTime(float ftime);
		virtual void SetHeroPos(D3DXVECTOR3 vpos);
		inline void SetMoveFrame(DWORD dwframe) { m_dwMoveFrame = dwframe; }
		inline virtual void SetRayCrash(DWORD dwRay, DWORD dwBox, bool bCrash) { m_RayCrash[dwRay] = bCrash;  m_dwColBox[dwRay] = dwBox; }
		inline virtual void SetInitPos(D3DXVECTOR3 vpos) { m_vCharPos = m_vAfterCharPos = vpos; m_Character->SetRayPos(m_vAfterCharPos); }
		inline virtual void SetEndClip(int iState, bool bclip) { m_Character->SetEndClip(iState, bclip); }
		inline virtual void SetCamera(KG_Camera* pCamera) { m_pCamera = pCamera; }
		inline virtual void SetMoveSpeed(float fSpeed){m_fSpeed = fSpeed;}
		inline virtual void SetHeight(float height) { m_vPosHeight = height; }
		inline virtual void SetFireSocketMove(D3DXVECTOR3 vfirepos) { m_vFireSocketPos = vfirepos; }
		inline virtual void SetInitHp(DWORD dwhp) { m_dwInitHP = dwhp; }
		inline virtual void SetHP(int HP) { m_iHP = HP; }
		inline virtual void SetDamage(float power) { m_bDamge = true; }
		inline virtual void SetScale(float scl) { m_fScale = scl; }

	public:
		virtual KG_Box GetCharBox();
		virtual KG_Box GetColBox(int ibox);
		virtual D3DXVECTOR3 GetColPos(int ibox);
		virtual int GetColBoxSize();
		virtual D3DXVECTOR3 GetFirePos(int i);
		virtual bool GetStateAnimationStart(int i);
		virtual bool GetStateAnimationEnd(int i);
		virtual bool GetFire(int i);
		virtual D3DXVECTOR3 GetHeroPos();
		virtual CBY_Bullet* GetBullet();
		inline virtual KGCA37_Ray GetRay(int i,int j) { return m_Character->GetRay(i,j); }
		inline virtual bool GetRayCrash(DWORD dw) { return m_RayCrash[dw]; }
		inline virtual DWORD GetRayCrashBox(DWORD dw) { return m_dwColBox[dw]; }
		inline float GetFireTime() { return m_fFireSetTime; }
		inline virtual KG_Camera* GetCamera() {return m_pCamera;}
		inline virtual float GetMoveSpeed(){return m_fSpeed;}
		inline virtual D3DXVECTOR3 GetFireSocketMove() { return m_vFireSocketPos; }
		inline virtual int GetHP() { return m_iHP; }
		inline virtual float GetScale() { return m_fScale; }
		D3DXMATRIX GetMatWorld() { return m_Character.get()->m_matWorld; }

	public:
		CBY_CharacterSrc();
		virtual ~CBY_CharacterSrc();
	};
}

