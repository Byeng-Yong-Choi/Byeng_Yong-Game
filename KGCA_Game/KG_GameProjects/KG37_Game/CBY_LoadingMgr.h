#pragma once
#include "KG_ShapeObject.h"
namespace CBY
{
	class CBY_LoadingMgr :public Singleton<CBY_LoadingMgr>
	{
	public:


	public:
		bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
		bool Init();
		bool Frame();
		bool Render();
		bool Realse();

	public:
		CBY_LoadingMgr();
		virtual ~CBY_LoadingMgr();
	};

#define CBY_LOADING CBY_LoadingMgr::GetInstance()
}

