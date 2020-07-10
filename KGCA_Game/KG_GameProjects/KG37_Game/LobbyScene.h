#pragma once
#include "Scene.h"

namespace KYS
{

	class LobbyScene :public Scene
	{
	public:
		LobbyScene();
		virtual ~LobbyScene();

	public:

	public:
		virtual bool Init()		override;
		virtual bool Frame()	override;
		virtual bool Render()	override;
		virtual bool Release()	override;

	public:

	private:

	};


}