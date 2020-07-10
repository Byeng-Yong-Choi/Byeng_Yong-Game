#pragma once
#include "Scene.h"


namespace KYS
{

	class TitleScene :public Scene
	{
	public:
		TitleScene();
		virtual ~TitleScene();

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

