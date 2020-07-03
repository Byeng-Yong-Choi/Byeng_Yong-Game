#include "C_Boss.h"

HRESULT C_Boss::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;
	m_Src = std::make_shared<C_MonsterSrc>();
	m_Src->m_Pos = D3DXVECTOR3(-(Winrt.right / 2.03f), -(Winrt.bottom / 11.61f) + (Winrt.bottom / 8.0f), 0.0f);
	m_Src->m_obj.m_pd3dDevice = pd3dDevice;
	m_Src->m_obj.m_pContext = Context;
	m_Src->m_StateList.resize(MONSTER_COUNT);
	TextSrc();
	m_Src->m_iHp = PIG_HP * 10;

	m_Src->SetState(MONSTER_MOVE);

	m_Src->m_Size = m_Src->m_StateList[0]->m_Size;

	return hr;
}

//void C_monPig::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
//{
//	m_Src.m_StateList[StateNUM].SetMatrix(world, view, proj);
//}

void C_Boss::TextArray(int i)
{
	m_Src->m_RtList.clear();
	switch (i)
	{
	case MONSTER_MOVE:
	{
		m_Src->m_RtList.resize(4);
		m_Src->m_RtList[0] = { 0, 91, 51, 79 };
		m_Src->m_RtList[1] = { 55, 91, 51, 79 };
		m_Src->m_RtList[2] = { 110, 91, 51, 79 };
		m_Src->m_RtList[3] = { 165, 91, 51, 79 };
	}
	break;

	case MONSTER_DAMAGE:
	{
		m_Src->m_RtList.resize(5);
		m_Src->m_RtList[0] = { 0,178,51,79 };
		m_Src->m_RtList[1] = { 55,178,51,79 };
		m_Src->m_RtList[2] = { 110,178,51,79 };
		m_Src->m_RtList[3] = { 165,178,51,79 };
		m_Src->m_RtList[4] = { 220,178,51,79 };
	}
	break;

	case MONSTER_DEATH:
	{
		m_Src->m_RtList.resize(15);
		m_Src->m_RtList[0] = { 2,625,66,79 };
		m_Src->m_RtList[1] = { 126,625,51,79 };
		m_Src->m_RtList[2] = { 181,625,51,79 };
		m_Src->m_RtList[3] = { 236,625,51,79 };
		m_Src->m_RtList[4] = { 291,625,51,79 };
		m_Src->m_RtList[5] = { 346,625,51,79 };
		m_Src->m_RtList[6] = { 401,625,51,79 };
		m_Src->m_RtList[7] = { 456,625,51,79 };
		m_Src->m_RtList[8] = { 511,625,51,79 };
		m_Src->m_RtList[9] = { 566,625,51,79 };
		m_Src->m_RtList[10] = { 621,625,51,79 };
		m_Src->m_RtList[11] = { 676,625,51,79 };
		m_Src->m_RtList[12] = { 731,625,51,79 };
		m_Src->m_RtList[13] = { 786,625,51,79 };
		m_Src->m_RtList[14] = { 841,625,51,79 };

	}
	break;
	}
}

void C_Boss::TextSrc()
{
	//MONSTER_MOVE:
	m_Src->m_StateList[MONSTER_MOVE] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_MOVE]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/Boss.png", "VSmat", "PSBlendMon");
	TextArray(MONSTER_MOVE);
	m_Src->m_StateList[MONSTER_MOVE]->SetRectArray(m_Src->m_RtList, 800, 800);
	m_Src->m_StateList[MONSTER_MOVE]->SetAnimation(0.1f, 0.1f);

	//MONSTER_DAMAGE:
	m_Src->m_StateList[MONSTER_DAMAGE] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_DAMAGE]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/Boss.png", "VSmat", "PSBlendMon");
	TextArray(MONSTER_DAMAGE);
	m_Src->m_StateList[MONSTER_DAMAGE]->SetRectArray(m_Src->m_RtList, 800, 800);
	m_Src->m_StateList[MONSTER_DAMAGE]->SetAnimation(0.1f, 0.2f);

	//MONSTER_DEATH:
	m_Src->m_StateList[MONSTER_DEATH] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_DEATH]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/Boss.png", "VSmat", "PSBlendMon");
	TextArray(MONSTER_DEATH);
	m_Src->m_StateList[MONSTER_DEATH]->SetRectArray(m_Src->m_RtList, 800, 800);
	m_Src->m_StateList[MONSTER_DEATH]->SetAnimation(0.1f, 0.2f);
}

bool C_Boss::Release()
{
	m_Src->Release();
	return true;
}

C_Boss::C_Boss()
{
}


C_Boss::~C_Boss()
{
}