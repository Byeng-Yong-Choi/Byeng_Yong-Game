#pragma once
#include "CBYCore.h"
#include "CModelViewCamera.h"
#include "CBY_BackViewCamera.h"
#include "CBY_Object.h"
#include "CBY_Parser.h"
#include "CBY_ObjectTest.h"


class CBY_ToolMain:public CBYCore
{
public:
	std::shared_ptr<CModelViewCamera> m_ModelCamera;
	std::shared_ptr<CBY_BackViewCamera> m_BackCamera;
	std::shared_ptr<CBY_Object> m_Object;
	std::shared_ptr<CBY_ObjectTest> m_TestObj;
	CBY_Parser m_Parser;
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Lightbuffer;
	CB_LIGHT m_Light;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	void SkinAdd(T_STR SkinLoad);
	void MTRAdd(T_STR MTRLoad);;
	void ObjCreate(T_STR SkinLoad, T_STR MTRLoad);
	void TestCreate(T_STR SkinLoad, T_STR MTRLoad);

public:
	void SkinParser(T_STR Load);
	void SkinBLoad(T_STR Load);
	void MtrParser(T_STR Load);
	void MtrBLoad(T_STR Load);

public:
	CBY_ToolMain();
	virtual ~CBY_ToolMain();
};

