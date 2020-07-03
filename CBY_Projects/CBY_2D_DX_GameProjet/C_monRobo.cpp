#include "C_monRobo.h"

HRESULT C_monRobo::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;
	m_Src = std::make_shared<C_MonsterSrc>();
	m_Src->m_Pos = D3DXVECTOR3(-(Winrt.right / 2.03f), -(Winrt.bottom / 11.61f) + (Winrt.bottom / 8.0f), 0.0f);
	m_Src->m_obj.m_pd3dDevice = pd3dDevice;
	m_Src->m_obj.m_pContext = Context;
	m_Src->m_StateList.resize(MONSTER_COUNT);
	TextSrc();
	m_Src->m_iHp = PIG_HP;

	m_Src->SetState(MONSTER_MOVE);

	m_Src->m_Size = m_Src->m_StateList[0]->m_Size;

	return hr;
}

//void C_monPig::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
//{
//	m_Src.m_StateList[StateNUM]->SetMatrix(world, view, proj);
//}

void C_monRobo::TextArray(int i)
{
	m_Src->m_RtList.clear();
	switch (i)
	{
	case MONSTER_MOVE:
	{
		m_Src->m_RtList.resize(8);
		m_Src->m_RtList[0] = { 1, 70, 47, 51 };
		m_Src->m_RtList[1] = { 63, 70, 47, 51 };
		m_Src->m_RtList[2] = { 119,70, 47, 51 };
		m_Src->m_RtList[3] = { 174,70, 47, 51 };
		m_Src->m_RtList[4] = { 232,70, 47, 51 };
		m_Src->m_RtList[5] = { 288,70, 47, 51 };
		m_Src->m_RtList[6] = { 345,70, 47, 51 };
		m_Src->m_RtList[7] = { 402,70, 47, 51 };
	}
	break;

	case MONSTER_DAMAGE:
	{
		m_Src->m_RtList.resize(3);
		m_Src->m_RtList[0] = { 0,139,47,51 };
		m_Src->m_RtList[1] = { 61,139,47,51 };
		m_Src->m_RtList[2] = { 122,139,47,51 };
	}
	break;

	case MONSTER_DEATH:
	{
		m_Src->m_RtList.resize(3);
		m_Src->m_RtList[0] = { 178,139,51,51 };
		m_Src->m_RtList[1] = { 240,139,47,51 };
		m_Src->m_RtList[2] = { 299,136,51,57 };
	}
	break;
	}
}

void C_monRobo::TextSrc()
{
	//MONSTER_MOVE:
	m_Src->m_StateList[MONSTER_MOVE] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_MOVE]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/robocolor.bmp", "VSmat", "PSBlendMon");
	TextArray(MONSTER_MOVE);
	m_Src->m_StateList[MONSTER_MOVE]->SetRectArray(m_Src->m_RtList, 450, 263);
	m_Src->m_StateList[MONSTER_MOVE]->SetAnimation(0.1f, 0.1f);

	//MONSTER_DAMAGE:
	m_Src->m_StateList[MONSTER_DAMAGE] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_DAMAGE]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/robocolor.bmp", "VSmat", "PSBlendMon");
	TextArray(MONSTER_DAMAGE);
	m_Src->m_StateList[MONSTER_DAMAGE]->SetRectArray(m_Src->m_RtList, 450, 263);
	m_Src->m_StateList[MONSTER_DAMAGE]->SetAnimation(0.1f, MONSTER_ANIMATIO_TIME);

	//MONSTER_DEATH:
	m_Src->m_StateList[MONSTER_DEATH] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_DEATH]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/robocolor.bmp", "VSmat", "PSBlendMon");
	TextArray(MONSTER_DEATH);
	m_Src->m_StateList[MONSTER_DEATH]->SetRectArray(m_Src->m_RtList, 450, 263);
	m_Src->m_StateList[MONSTER_DEATH]->SetAnimation(0.1f, MONSTER_ANIMATIO_TIME);
}

bool C_monRobo::Release()
{
	m_Src->Release();
	return true;
}

C_monRobo::C_monRobo()
{
}


C_monRobo::~C_monRobo()
{
}
