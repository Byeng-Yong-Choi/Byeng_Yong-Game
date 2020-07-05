#pragma once
#include"JH_BaseObj.h"
#include"CBY_Object.h"
namespace JH {

	enum OBJ_FLAG
	{
		PATH = 1,
		WALL,
	};
	class JH_MapObj
	{
		T_STR	m_FileName;
		T_STR	m_SkinFileName;
		T_STR	m_BoneFileName;
		void*	m_pNode;
		DWORD	m_dwFlag;

		std::shared_ptr<CBY::CBY_Object> m_Object;
		int				m_ID;
		//int				m_iObjIndex;
		int				m_iQuadIndex;
	public:
		//int					GetObjIndex() { return m_iObjIndex; };
		//void				SetObjIndex(int Index) { m_iObjIndex = Index; };
		inline int					GetID() { return m_ID; }
		inline void				SetID(int Index) { m_ID = Index; }
		inline T_STR				GetName() { return m_FileName; }
		inline void				SetName(T_STR FileName) { m_FileName = FileName; }
		inline T_STR				GetSkinName() { return m_SkinFileName; }
		inline void				SetSkinName(const T_STR FileName) { m_SkinFileName = FileName; }
		inline T_STR				GetBoneName() { return m_BoneFileName; }
		inline void				SetBoneName(const T_STR FileName) { m_BoneFileName = FileName; }
		inline void*				GetNode() { return m_pNode; }
		inline void				SetNode(void* pNode) { m_pNode = pNode; }
		inline DWORD				GetFlag() { return m_dwFlag; }
		inline void				SetFlag(DWORD dwFlag) { m_dwFlag = dwFlag; }

		inline void				SetObj(const std::shared_ptr<CBY::CBY_Object> Obj) { m_Object = Obj; }
		std::shared_ptr<CBY::CBY_Object> GetObj() { return m_Object; }



		inline int					GetQuadIndex() { return m_iQuadIndex; }
		inline void				SetQuadIndex(int Index) { m_iQuadIndex = Index; }
		JH_MapObj();
		virtual ~JH_MapObj();
	};

}