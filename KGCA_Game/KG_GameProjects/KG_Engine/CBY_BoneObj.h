#pragma once
#include "CBY_Obj.h"
#include "CBY_BoneParser.h"

namespace CBY
{
	class CBY_BoneObj :public CBY_Obj
	{
	public:
		CBY_BoneParser m_ObjLoader;
		KG_Box m_CharBox;
		bool m_bAniStart;
		bool m_bAniEnd;
		bool m_bAniEndClip;

	public:
		bool BoneCreate();
		bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context) override;
		void Convert(std::vector<PNCT2IW_VERTEX>& list);
		void Update(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList);
		void MTRUpdate(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList);
		void ObjUpdate(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList, D3DXMATRIX* parmat, int socket);
		bool AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype, float fETick);

	public:
		inline void SetEndClip(bool bclip)		//애니메이션 종료후 정지 고정
		{
			m_bAniEndClip = bclip;
			if (!bclip)
			{
				m_bAniEnd = false;
				m_fElapseTick = 0.0f;
			}
		}
		inline bool  GetAniStart(){return m_bAniStart;}				//애니메이션 시작인지 반환
		inline bool GetAniEnd(){return m_bAniEnd;}					//애니메이션 끝났는지 반환

	public:
		CBY_BoneObj();
		virtual ~CBY_BoneObj();
	};
}

