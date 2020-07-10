#pragma once
#include "Scene.h"


namespace KYS
{

	class GameEnd :public Scene
	{
	public:
		GameEnd();
		virtual ~GameEnd();

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
