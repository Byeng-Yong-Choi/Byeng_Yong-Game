#pragma once
#include "CBY_EnemyAi.h"

namespace CBY
{
	class CBY_EnemyRobot : public CBY_EnemyAi
	{
	public:
		bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)override;

	public:
		CBY_EnemyRobot();
		virtual ~CBY_EnemyRobot();
	};
}

