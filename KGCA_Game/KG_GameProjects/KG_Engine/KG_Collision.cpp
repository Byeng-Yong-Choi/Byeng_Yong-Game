#include "KG_Collision.h"
#include "KG_Math.h"

namespace KG_COLLOSION
{
	bool ChkOBBToSegment(KG_Box pbox, KGCA37_Ray Ray, float& fm)
	{
		float t_min = -999999.0f;
		float t_max = 999999.0f;
		float  f[3], fa[3], s[3], sa[3];

		D3DXVECTOR3 vR = Ray.m_Orig- pbox.vCenter;

		for (int v = 0; v < 3; v++)
		{
			f[v] = D3DXVec3Dot(&pbox.vAxis[v], &Ray.m_Dir);
			s[v] = D3DXVec3Dot(&pbox.vAxis[v], &vR);
			fa[v] = fabs(f[v]);
			sa[v] = fabs(s[v]);

			if ((sa[v] > pbox.fExtent[v] && s[v] * f[v] >= 0.0f))
				return false;
			
			float t1 = (-s[v] - pbox.fExtent[v]) / f[v];
			float t2 = (-s[v] + pbox.fExtent[v]) / f[v];
			if (t1 > t2)
			{
				std::swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}

		float  fCross, fRhs;
		D3DXVECTOR3 vDxR;
		D3DXVec3Cross(&vDxR, &Ray.m_Dir, &vR);
		
		fCross = fabs(D3DXVec3Dot(&vDxR, &pbox.vAxis[0]));
		fRhs = pbox.fExtent[1] * fa[2] + pbox.fExtent[2] * fa[1];
		if (fCross > fRhs)
		{
			return false;
		}
		
		fCross = fabs(D3DXVec3Dot(&vDxR, &pbox.vAxis[1]));
		fRhs = pbox.fExtent[0] * fa[2] + pbox.fExtent[2] * fa[0];
		if (fCross > fRhs)
		{
			return false;
		}
		
		fCross = fabs(D3DXVec3Dot(&vDxR, &pbox.vAxis[2]));
		fRhs = pbox.fExtent[0] * fa[1] + pbox.fExtent[1] * fa[0];
		if (fCross > fRhs)
		{
			return false;
		}

		//return true;
		fm = Ray.m_fMax - t_min;

		if (fm > 0)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool ChkOBBToOBB(const KG_Box box1, const KG_Box box2)
	{
		//분리축 투영 총 15개의 분리축 사용
		//각 OBB의 X,Y,Z 와 XYZ와 XYZ의 외적 축 까지 다 합치면 6+9=15;
		D3DXVECTOR3 vDist = (box1.vCenter - box2.vCenter);

		float fD[3][3];
		float absfD[3][3];
		float AxisDist[3];
		float fRDist[4];

		//Ax
		fD[0][0] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[0]);
		fD[0][1] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[1]);
		fD[0][2] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[2]);
		AxisDist[0] = D3DXVec3Dot(&box1.vAxis[0], &vDist);
		absfD[0][0] = fabs(fD[0][0]);
		absfD[0][1] = fabs(fD[0][1]);
		absfD[0][2] = fabs(fD[0][2]);
		fRDist[0] = fabs(AxisDist[0]);
		fRDist[1] = (box2.fExtent[0] * absfD[0][0]) + (box2.fExtent[1] * absfD[0][1]) + (box2.fExtent[2] * absfD[0][2]);
		fRDist[2] = box1.fExtent[0] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;

		//Ay
		fD[1][0] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[0]);
		fD[1][1] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[1]);
		fD[1][2] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[2]);
		AxisDist[1] = D3DXVec3Dot(&box1.vAxis[1], &vDist);
		absfD[1][0] = fabs(fD[1][0]);
		absfD[1][1] = fabs(fD[1][1]);
		absfD[1][2] = fabs(fD[1][2]);
		fRDist[0] = fabs(AxisDist[1]);
		fRDist[1] = (box2.fExtent[0] * absfD[1][0]) + (box2.fExtent[1] * absfD[1][1]) + (box2.fExtent[2] * absfD[1][2]);
		fRDist[2] = box1.fExtent[1] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;

		//Az
		fD[2][0] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[0]);
		fD[2][1] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[1]);
		fD[2][2] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[2]);
		AxisDist[2] = D3DXVec3Dot(&box1.vAxis[2], &vDist);
		absfD[2][0] = fabs(fD[2][0]);
		absfD[2][1] = fabs(fD[2][1]);
		absfD[2][2] = fabs(fD[2][2]);
		fRDist[0] = fabs(AxisDist[2]);
		fRDist[1] = (box2.fExtent[0] * absfD[2][0]) + (box2.fExtent[1] * absfD[2][1]) + (box2.fExtent[2] * absfD[2][2]);
		fRDist[2] = box1.fExtent[2] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;


		//Bx
		fRDist[0] = fabs(D3DXVec3Dot(&box2.vAxis[0], &vDist));
		fRDist[1] = (box1.fExtent[0] * absfD[0][0]) + (box1.fExtent[1] * absfD[1][0]) + (box1.fExtent[2] * absfD[2][0]);
		fRDist[2] = box2.fExtent[0] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;

		//By
		fRDist[0] = fabs(D3DXVec3Dot(&box2.vAxis[1], &vDist));
		fRDist[1] = (box1.fExtent[0] * absfD[0][1]) + (box1.fExtent[1] * absfD[1][1]) + (box1.fExtent[2] * absfD[2][1]);
		fRDist[2] = box2.fExtent[1] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;


		//Bz
		fRDist[0] = fabs(D3DXVec3Dot(&box2.vAxis[2], &vDist));
		fRDist[1] = (box1.fExtent[0] * absfD[0][2]) + (box1.fExtent[1] * absfD[1][2]) + (box1.fExtent[2] * absfD[2][2]);
		fRDist[2] = box2.fExtent[2] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;


		//AxBx
		fRDist[0] = fabs((AxisDist[2] * fD[1][0]) - (AxisDist[1] * fD[2][0]));
		fRDist[1] = box1.fExtent[1] * absfD[2][0] + box1.fExtent[2] * absfD[1][0];
		fRDist[2] = box2.fExtent[1] * absfD[0][2] + box2.fExtent[2] * absfD[0][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		//AxBy
		fRDist[0] = fabs((AxisDist[2] * fD[1][1]) - (AxisDist[1] * fD[2][1]));
		fRDist[1] = box1.fExtent[1] * absfD[2][1] + box1.fExtent[2] * absfD[1][1];
		fRDist[2] = box2.fExtent[0] * absfD[0][2] + box2.fExtent[2] * absfD[0][0];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;

		//AxBz
		fRDist[0] = fabs((AxisDist[2] * fD[1][2]) - (AxisDist[1] * fD[2][2]));
		fRDist[1] = box1.fExtent[1] * absfD[2][2] + box1.fExtent[2] * absfD[1][2];
		fRDist[2] = box2.fExtent[1] * absfD[0][0] + box2.fExtent[0] * absfD[0][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		//AyBx
		fRDist[0] = fabs((AxisDist[0] * fD[2][0]) - (AxisDist[2] * fD[0][0]));
		fRDist[1] = box1.fExtent[0] * absfD[2][0] + box1.fExtent[2] * absfD[0][0];
		fRDist[2] = box2.fExtent[1] * absfD[1][2] + box2.fExtent[2] * absfD[1][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		//AyBy
		fRDist[0] = fabs((AxisDist[0] * fD[2][1]) - (AxisDist[2] * fD[0][1]));
		fRDist[1] = box1.fExtent[0] * absfD[2][1] + box1.fExtent[2] * absfD[0][1];
		fRDist[2] = box2.fExtent[0] * absfD[1][2] + box2.fExtent[2] * absfD[1][0];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		//AyBz
		fRDist[0] = fabs((AxisDist[0] * fD[2][2]) - (AxisDist[2] * fD[0][2]));
		fRDist[1] = box1.fExtent[0] * absfD[2][2] + box1.fExtent[2] * absfD[0][2];
		fRDist[2] = box2.fExtent[1] * absfD[1][0] + box2.fExtent[0] * absfD[1][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		//AzBx
		fRDist[0] = fabs((AxisDist[0] * fD[1][0]) - (AxisDist[1] * fD[0][0]));
		fRDist[1] = box1.fExtent[0] * absfD[1][0] + box1.fExtent[1] * absfD[0][0];
		fRDist[2] = box2.fExtent[1] * absfD[2][2] + box2.fExtent[2] * absfD[2][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		//AzBy
		fRDist[0] = fabs((AxisDist[0] * fD[1][1]) - (AxisDist[1] * fD[0][1]));
		fRDist[1] = box1.fExtent[0] * absfD[1][1] + box1.fExtent[1] * absfD[0][1];
		fRDist[2] = box2.fExtent[0] * absfD[2][2] + box2.fExtent[2] * absfD[2][0];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		//AzBz
		fRDist[0] = fabs((AxisDist[0] * fD[1][2]) - (AxisDist[1] * fD[0][2]));
		fRDist[1] = box1.fExtent[0] * absfD[1][2] + box1.fExtent[1] * absfD[0][2];
		fRDist[2] = box2.fExtent[1] * absfD[2][0] + box2.fExtent[0] * absfD[2][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;


		return true;
	}

	bool ChkOBBToOBBAndDirection(const KG_Box box1, const KG_Box box2, D3DXVECTOR3& vCenDir, float& fM)
	{
		//분리축 투영 총 15개의 분리축 사용
		//각 OBB의 X,Y,Z 와 XYZ와 XYZ의 외적 축 까지 다 합치면 6+9=15;
		D3DXVECTOR3 vDist = (box1.vCenter - box2.vCenter);

		vCenDir = vDist;
		D3DXVec3Normalize(&vCenDir, &vCenDir);

		float fD[3][3];
		float absfD[3][3];
		float AxisDist[3];
		float fRDist[4];

		float fBeginM;

		//Ax
		fD[0][0] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[0]);
		fD[0][1] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[1]);
		fD[0][2] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[2]);
		AxisDist[0] = D3DXVec3Dot(&box1.vAxis[0], &vDist);
		absfD[0][0] = fabs(fD[0][0]);
		absfD[0][1] = fabs(fD[0][1]);
		absfD[0][2] = fabs(fD[0][2]);
		fRDist[0] = fabs(AxisDist[0]);
		fRDist[1] = (box2.fExtent[0] * absfD[0][0]) + (box2.fExtent[1] * absfD[0][1]) + (box2.fExtent[2] * absfD[0][2]);
		fRDist[2] = box1.fExtent[0] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;
		fBeginM = fRDist[2] - fRDist[0];
		if (fBeginM == 0)
			fBeginM += 0.1f;
		fM = fBeginM;
		

		//Ay
		fD[1][0] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[0]);
		fD[1][1] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[1]);
		fD[1][2] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[2]);
		AxisDist[1] = D3DXVec3Dot(&box1.vAxis[1], &vDist);
		absfD[1][0] = fabs(fD[1][0]);
		absfD[1][1] = fabs(fD[1][1]);
		absfD[1][2] = fabs(fD[1][2]);
		fRDist[0] = fabs(AxisDist[1]);
		fRDist[1] = (box2.fExtent[0] * absfD[1][0]) + (box2.fExtent[1] * absfD[1][1]) + (box2.fExtent[2] * absfD[1][2]);
		fRDist[2] = box1.fExtent[1] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;
		fBeginM = fRDist[2] - fRDist[0];
		if ((fBeginM !=0 ) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//Az
		fD[2][0] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[0]);
		fD[2][1] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[1]);
		fD[2][2] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[2]);
		AxisDist[2] = D3DXVec3Dot(&box1.vAxis[2], &vDist);
		absfD[2][0] = fabs(fD[2][0]);
		absfD[2][1] = fabs(fD[2][1]);
		absfD[2][2] = fabs(fD[2][2]);
		fRDist[0] = fabs(AxisDist[2]);
		fRDist[1] = (box2.fExtent[0] * absfD[2][0]) + (box2.fExtent[1] * absfD[2][1]) + (box2.fExtent[2] * absfD[2][2]);
		fRDist[2] = box1.fExtent[2] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;
		fBeginM = fRDist[2] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//Bx
		fRDist[0] = fabs(D3DXVec3Dot(&box2.vAxis[0], &vDist));
		fRDist[1] = (box1.fExtent[0] * absfD[0][0]) + (box1.fExtent[1] * absfD[1][0]) + (box1.fExtent[2] * absfD[2][0]);
		fRDist[2] = box2.fExtent[0] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;
		fBeginM = fRDist[2] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//By
		fRDist[0] = fabs(D3DXVec3Dot(&box2.vAxis[1], &vDist));
		fRDist[1] = (box1.fExtent[0] * absfD[0][1]) + (box1.fExtent[1] * absfD[1][1]) + (box1.fExtent[2] * absfD[2][1]);
		fRDist[2] = box2.fExtent[1] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;
		fBeginM = fRDist[2] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//Bz
		fRDist[0] = fabs(D3DXVec3Dot(&box2.vAxis[2], &vDist));
		fRDist[1] = (box1.fExtent[0] * absfD[0][2]) + (box1.fExtent[1] * absfD[1][2]) + (box1.fExtent[2] * absfD[2][2]);
		fRDist[2] = box2.fExtent[2] + fRDist[1];
		if (fRDist[0] > fRDist[2])return false;
		fBeginM = fRDist[2] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AxBx
		fRDist[0] = fabs((AxisDist[2] * fD[1][0]) - (AxisDist[1] * fD[2][0]));
		fRDist[1] = box1.fExtent[1] * absfD[2][0] + box1.fExtent[2] * absfD[1][0];
		fRDist[2] = box2.fExtent[1] * absfD[0][2] + box2.fExtent[2] * absfD[0][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AxBy
		fRDist[0] = fabs((AxisDist[2] * fD[1][1]) - (AxisDist[1] * fD[2][1]));
		fRDist[1] = box1.fExtent[1] * absfD[2][1] + box1.fExtent[2] * absfD[1][1];
		fRDist[2] = box2.fExtent[0] * absfD[0][2] + box2.fExtent[2] * absfD[0][0];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AxBz
		fRDist[0] = fabs((AxisDist[2] * fD[1][2]) - (AxisDist[1] * fD[2][2]));
		fRDist[1] = box1.fExtent[1] * absfD[2][2] + box1.fExtent[2] * absfD[1][2];
		fRDist[2] = box2.fExtent[1] * absfD[0][0] + box2.fExtent[0] * absfD[0][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AyBx
		fRDist[0] = fabs((AxisDist[0] * fD[2][0]) - (AxisDist[2] * fD[0][0]));
		fRDist[1] = box1.fExtent[0] * absfD[2][0] + box1.fExtent[2] * absfD[0][0];
		fRDist[2] = box2.fExtent[1] * absfD[1][2] + box2.fExtent[2] * absfD[1][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AyBy
		fRDist[0] = fabs((AxisDist[0] * fD[2][1]) - (AxisDist[2] * fD[0][1]));
		fRDist[1] = box1.fExtent[0] * absfD[2][1] + box1.fExtent[2] * absfD[0][1];
		fRDist[2] = box2.fExtent[0] * absfD[1][2] + box2.fExtent[2] * absfD[1][0];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AyBz
		fRDist[0] = fabs((AxisDist[0] * fD[2][2]) - (AxisDist[2] * fD[0][2]));
		fRDist[1] = box1.fExtent[0] * absfD[2][2] + box1.fExtent[2] * absfD[0][2];
		fRDist[2] = box2.fExtent[1] * absfD[1][0] + box2.fExtent[0] * absfD[1][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AzBx
		fRDist[0] = fabs((AxisDist[0] * fD[1][0]) - (AxisDist[1] * fD[0][0]));
		fRDist[1] = box1.fExtent[0] * absfD[1][0] + box1.fExtent[1] * absfD[0][0];
		fRDist[2] = box2.fExtent[1] * absfD[2][2] + box2.fExtent[2] * absfD[2][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AzBy
		fRDist[0] = fabs((AxisDist[0] * fD[1][1]) - (AxisDist[1] * fD[0][1]));
		fRDist[1] = box1.fExtent[0] * absfD[1][1] + box1.fExtent[1] * absfD[0][1];
		fRDist[2] = box2.fExtent[0] * absfD[2][2] + box2.fExtent[2] * absfD[2][0];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		//AzBz
		fRDist[0] = fabs((AxisDist[0] * fD[1][2]) - (AxisDist[1] * fD[0][2]));
		fRDist[1] = box1.fExtent[0] * absfD[1][2] + box1.fExtent[1] * absfD[0][2];
		fRDist[2] = box2.fExtent[1] * absfD[2][0] + box2.fExtent[0] * absfD[2][1];
		fRDist[3] = fRDist[1] + fRDist[2];
		if (fRDist[0] > fRDist[3])return false;
		fBeginM = fRDist[3] - fRDist[0];
		if ((fBeginM != 0) && fBeginM < fM)
		{
			fM = fBeginM;
		}

		GetBoxNormal(box1, box2, vCenDir);

		return true;
	}

	void GetBoxNormal(KG_Box box1, KG_Box box2, D3DXVECTOR3& vCenDir)
	{
		vCenDir = (box1.vCenter - box2.vCenter);
		D3DXVec3Normalize(&vCenDir, &vCenDir);

		float fradian = D3DXVec3Dot(&vCenDir, &box2.vAxis[2]) /
			(D3DXVec3Length(&vCenDir) * D3DXVec3Length(&box2.vAxis[2]));

		fradian = acosf(fradian);
		fradian = D3DXToDegree(fradian);	//0~180
		float fy = -((vCenDir.x * box2.vAxis[2].z) - (vCenDir.z * box2.vAxis[2].x)); //외적의 y성분을 이용한 0~360으로 바꾸기

		fradian = (fy > 0.0f) ? fradian : (360.0f - fradian);
		float fZtheta = (box2.fZtheta / 2.0f);
		float fXtheta = box2.fXtheta;


		if (0 <= fradian && fradian <= fZtheta)
		{
			vCenDir = box2.vAxis[2];
		}
		else if (fZtheta < fradian && fradian < fZtheta + fXtheta)
		{
			vCenDir = -box2.vAxis[0];
		}
		else if (fZtheta + fXtheta <= fradian && fradian <= (3 * fZtheta + fXtheta))
		{
			vCenDir = -box2.vAxis[2];
		}
		else if ((3 * fZtheta + fXtheta) < fradian && fradian < ((3 * fZtheta) + (2 * fXtheta)))
		{
			vCenDir = box2.vAxis[0];
		}
		else
		{
			vCenDir = box2.vAxis[2];
		}
	}

	void GetBoxToSegmentNormal(KG_Box box, KGCA37_Ray Ray,D3DXVECTOR3& vCenDir)
	{
		vCenDir = Ray.m_Orig - box.vCenter;
		D3DXVec3Normalize(&vCenDir, &vCenDir);
		float fradian = D3DXVec3Dot(&vCenDir, &box.vAxis[2]) /
			(D3DXVec3Length(&vCenDir) * D3DXVec3Length(&box.vAxis[2]));

		fradian = acosf(fradian);
		fradian = D3DXToDegree(fradian);	//0~180
		float fy = -((vCenDir.x * box.vAxis[2].z) - (vCenDir.z * box.vAxis[2].x)); //외적의 y성분을 이용한 0~360으로 바꾸기

		fradian = (fy > 0.0f) ? fradian : (360.0f - fradian);
		float fZtheta = (box.fZtheta / 2.0f);
		float fXtheta = box.fXtheta;


		if (0 <= fradian && fradian <= fZtheta)
		{
			vCenDir = box.vAxis[2];
			
		}
		else if (fZtheta < fradian && fradian < fZtheta + fXtheta)
		{
			vCenDir = -box.vAxis[0];
			
		}
		else if (fZtheta + fXtheta <= fradian && fradian <= (3 * fZtheta + fXtheta))
		{
			vCenDir = -box.vAxis[2];
			
		}
		else if ((3 * fZtheta + fXtheta) < fradian && fradian < ((3 * fZtheta) + (2 * fXtheta)))
		{
			vCenDir = box.vAxis[0];
		
		}
		else
		{
			vCenDir = box.vAxis[2];
			/*D3DXVECTOR3 vM = Ray.m_Orig + (Ray.m_Dir*Ray.m_fMax);
			vM = vM - box.vCenter;
			fM = box.fExtent[2] - D3DXVec3Dot(&box.vAxis[2], &vM);*/
		}
	}


	bool ChkOBBToRay(KG_Box* pbox, KGCA37_Ray ray)
	{
		float t_min = -999999.0f;
		float t_max = 999999.0f;
		float  f[3], fa[3], s[3], sa[3];

		D3DXVECTOR3 vR = ray.m_Orig - pbox->vCenter;

		for (int v = 0; v < 3; v++)
		{
			f[v] = D3DXVec3Dot(&pbox->vAxis[v], &ray.m_Dir);
			s[v] = D3DXVec3Dot(&pbox->vAxis[v], &vR);
			fa[v] = fabs(f[v]);
			sa[v] = fabs(s[v]);

			if (sa[v] > pbox->fExtent[v] && s[v] * f[v] >= 0.0f)
				return false;

			float t1 = (-s[v] - pbox->fExtent[v]) / f[v];
			float t2 = (-s[v] + pbox->fExtent[v]) / f[v];
			if (t1 > t2)
			{
				std::swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}

		return true;
	}
}

KG_Collision::KG_Collision()
{
}


KG_Collision::~KG_Collision()
{
}
