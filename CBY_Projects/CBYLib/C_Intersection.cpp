#include "C_Intersection.h"

float C_Intersection::GetDistance(float ax, float ay, float bx,float by)
{
	float Distance;
	Distance=sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by));
	return Distance;
}

fSphere C_Intersection::RecttoSphere(fRECT rt1)
{
	fPOINT rt1Center;
	fSphere rt1Sphere;
	rt1Center = Center(rt1);
	float distance = GetDistance(rt1Center.x, rt1Center.y, rt1.right, rt1.bottom);
	rt1Sphere.fpt.x = rt1Center.x;
	rt1Sphere.fpt.y = rt1Center.y;
	rt1Sphere.radius = distance;

	return rt1Sphere;
}

float C_Intersection::Distance(fPOINT pt1, fPOINT pt2)
{
	float distance = sqrt((pt1.x-pt2.x)*(pt1.x - pt2.x) + (pt1.y-pt2.y) * (pt1.y - pt2.y));
	return distance;
}

fPOINT C_Intersection::Center(fRECT rt1)
{
	float PointX = (rt1.right + rt1.left) / 2.0f;
	float PointY = (rt1.top + rt1.bottom) / 2.0f;

	return { PointX, PointY };
}

bool C_Intersection::collision(fRECT rt1, fRECT rt2)
{
	fPOINT fp1;
	fPOINT fp2;
	float distanceX;
	float distanceY;
	float width;
	float height;

	fp1 = Center(rt1);
	fp2 = Center(rt2);

	distanceX = abs(fp1.x - fp2.x);
	distanceY = abs(fp1.y - fp2.y);
	width = (((float)rt1.right - (float)rt1.left) / 2) + (((float)rt2.right - (float)rt2.left) / 2);
	height = (((float)rt1.bottom - (float)rt1.top) / 2) + (((float)rt2.bottom - (float)rt2.top) / 2);

	if (width > distanceX && height > distanceY)
	{
		return true;
	}
	else
		return false;
}

bool C_Intersection::Spherecollision(fSphere sh1, fSphere sh2)
{
	float truedistance = Distance(sh1.fpt, sh2.fpt);
	float radiusdistance = sh1.radius + sh2.radius;

	if (truedistance > radiusdistance)
	{
		return false;
	}
	else
		return true;
}



C_Intersection::C_Intersection()
{
}


C_Intersection::~C_Intersection()
{
}
