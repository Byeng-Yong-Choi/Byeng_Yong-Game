#pragma once
#include "KG_Std.h"

namespace KG_COLLOSION
{
	bool ChkOBBToOBB(const KG_Box box1, const KG_Box box2);
	bool ChkOBBToOBBAndDirection(const KG_Box box1, const KG_Box box2, D3DXVECTOR3& vCenDir ,float& fM);
	bool ChkOBBToRay(KG_Box* pbox, KGCA37_Ray ray);
	void GetBoxNormal(KG_Box box1, KG_Box box2, D3DXVECTOR3& vCenDir);
	void GetBoxToSegmentNormal(KG_Box box, KGCA37_Ray Ray,D3DXVECTOR3& vCenDir);
	bool ChkOBBToSegment(KG_Box pbox, KGCA37_Ray Ray, float& fm);
}

class KG_Collision
{
public:
	KG_Collision();
	virtual ~KG_Collision();
};

