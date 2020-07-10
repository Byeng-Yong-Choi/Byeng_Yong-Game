#pragma once
#include "CBY_DataCtrl.h"
#include "CBY_Weapon.h"

namespace CBY
{
	enum Ray_info
	{
		RAY_SIZE =4,
		RAY_DIRSIZE=3
	};

	enum RayCast
	{
		RAY_FRONT=0,
		RAY_BACK,
		RAY_LEFT,
		RAY_RIGHT,
	};

	class CBY_Character :public CBY_Object
	{
	protected:
		int m_iRoot;
		D3DXMATRIX m_matRootPos;
		std::vector<std::shared_ptr<CBY_Weapon>> m_ObjectList;
		CBY_DataCtrl m_CharData;
		DWORD m_dwCameraSocket;
		DWORD m_dwFireSocket;
		KGCA37_Ray m_RayCast[RAY_SIZE][RAY_DIRSIZE];
		D3DXVECTOR4 m_vRaySize;
		D3DXVECTOR3 m_vRayPos;

	public:
		bool Init() override;
		bool Frame() override;
		bool Render() override;
		bool Release() override;
		virtual bool NonCharacterRender();
		virtual bool SkinLoad(T_STR pszLoad) override;

	public: //object
		virtual bool ObjLoad(T_STR pszSkinLoad);
		virtual void SetObjectSocket(int iSocket, int iObjSocket, int iFireSocket, int iobj);

	public:
		virtual bool CharacterLoad(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context ,T_STR pszLoad,
			const TCHAR* ShaderFileName= L"../../data/shader/SkinShader.txt", const TCHAR* TexFileName= nullptr, const CHAR* VSName= "VSSKIN", const CHAR* PSName= "PS");
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)override;
		virtual void ColBoxUpdate(D3DXMATRIX world);
		
	public:		//set
		virtual void SetCharBox() override;
		virtual void SetCharAction(int state, float start, float end);
		virtual void SetRootMat();
		void SetRayUpdate();
		void SetRayOrigin(D3DXVECTOR3 pos);
		inline virtual void SetRoot(DWORD dwRoot) { m_iRoot = dwRoot; SetRootMat(); }
		inline void SetRayPos(D3DXVECTOR3 pos) { m_vRayPos = pos; }		//레이의 위치를 줄 곳
		inline void SetEndClip(int iState, bool bclip) { m_StateList[iState].m_Bone.SetEndClip(bclip); }
		inline void SetCameraSocket(DWORD dwSocket) { m_dwCameraSocket = dwSocket; }
		inline void SetFireSocket(DWORD dwSocket) { m_dwFireSocket = dwSocket; }
		inline void SetRaySize(D3DXVECTOR4 vsize) { m_vRaySize = vsize; }	//front,back,left,right

	public:		//get
		D3DXVECTOR3 GetColPos(int i);
		D3DXVECTOR3 GetFirePos(int iObj);
		inline virtual int GetRoot() { return m_iRoot; }
		inline KGCA37_Ray GetRay(int i,int j) { return m_RayCast[i][j]; }
		inline KG_Box GetColBox(int i){return m_BoxList[i].GetBox();}
		inline bool GetStateAnimationStart(int i){return m_StateList[i].m_Bone.GetAniStart();}
		inline bool GetStateAnimationEnd(int i){return m_StateList[i].m_Bone.GetAniEnd();}
		inline DWORD GetCameraSocket() { return m_dwCameraSocket; }
		inline DWORD GetFireSocket() { return m_dwFireSocket; }

	public:
		CBY_Character();
		virtual ~CBY_Character();
	};
}
