#pragma once

#include "targetver.h"
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <utilapi.h>
#include <max.h>
#include <stdmat.h>
#include <decomp.h>
#include <bipexp.h>
#include <phyexp.h>
#include <iskin.h>
#include <vector>



////�ʿ� ����ü �� Ŭ����

enum ASE_NODE_TYPE
{
	GEOM_MESH = 0,
	HELPER_DUMMY,
	HELPER_BONE,
	BYPED,
};


struct BipedVertex
{
	int   iNumWeight;
	std::vector<BYTE>   idList;
	std::vector<float>  weightList;
};

#ifdef __cplusplus
typedef struct _D3D_MATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
}D3D_MATRIX, *LPD3D_MATRIX;
#else
typedef struct _D3D_MATRIX D3D_MATRIX, *LPD3D_MATRIX;
#endif

struct PNCTIW_VERTEX
{
	Point3 p;
	Point3 n;
	Point4 c;
	Point2 t;
	float i[4];
	float w[4];
	bool operator == (const PNCTIW_VERTEX& v)
	{
		if (p == v.p && c == v.c && t == v.t && n == v.n)
		{
			return true;
		}
		return false;
	}
	PNCTIW_VERTEX() {
		this->i[0] = 0;
		this->i[1] = 0;
		this->i[2] = 0;
		this->i[3] = 0;
		this->w[0] = 0;
		this->w[1] = 0;
		this->w[2] = 0;
		this->w[3] = 0;
	}
	PNCTIW_VERTEX(Point3 p, Point3 n, Point4 c, Point2 t,
		Point4 i, Point4 w)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
		this->i[0] = i.x;
		this->i[1] = i.y;
		this->i[2] = i.z;
		this->i[3] = i.w;
		this->w[0] = w.x;
		this->w[1] = w.y;
		this->w[2] = w.z;
		this->w[3] = w.w;

	}
};
struct CScene
{
	int iVersion;		// ����
	int	iFirstFrame;    // ���� ������
	int	iLastFrame;     // ������ ������
	int	iFrameSpeed;    // 1�ʴ� ������ ����(30)
	int	iTickPerFrame;  // 1�������� ƽ ��(160)
	int iNumMesh;		// �޽�������Ʈ ����
	int iNumMaterial;   // ��Ʈ���͸��� ����
	int iMaxWeight;		// ���� �� ����ġ
	int iBindPose;		// ���ε� ���� ���ϸ��̼� ����
};

struct CSubMap
{
	int  iSubNo;
	MSTR szTextureName;
};
struct CMtrl
{
	std::vector<CSubMap> texList;
	std::vector<CMtrl> subMtrl;
};
struct CAnimTrack
{
	int iTick;
	Point3 p;
	Quat   q;
};

struct CBY_Bone
{
	Box3	m_Box;
	Point3	m_Pos;
};

class CMesh
{
public:
	int			m_iType;
	int			m_iBone;
	int			m_iRoot;
	MSTR        m_szName;
	MSTR        m_szParentName;
	int			m_iTexIndex;
	UINT        m_iBaseVertex;
	UINT        m_iNumVertex;
	UINT		m_iBaseIndex;
	UINT		m_iNumIndex;
	CBY_Bone	m_Bone;

	int         m_iParentIndex;
	D3D_MATRIX  m_matWorld;
	D3D_MATRIX  m_matInverse;

	Point3		m_vAnimScale;
	Quat		m_qAnimScaleRotation;
	Quat		m_qAnimRotation;
	Point3		m_vAnimPos;

	std::vector<CAnimTrack> posTrack;
	std::vector<CAnimTrack> rotTrack;
	std::vector<CAnimTrack> sclTrack;
	std::vector<CMesh> subMesh;
	std::vector<PNCTIW_VERTEX> list;
	std::vector<PNCTIW_VERTEX> vblist;
	std::vector<DWORD> iblist;

	CMesh()
	{
		m_iRoot = -1;
		m_iBone = -1;
		m_iParentIndex = -1;
		m_iTexIndex = -1;
		m_iBaseVertex = 0;
		m_iNumVertex = 0;
		m_iBaseIndex = 0;
		m_iNumIndex = 0;
		m_iType = GEOM_MESH;
	}
};