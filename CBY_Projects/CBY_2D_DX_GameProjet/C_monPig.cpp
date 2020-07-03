#include "C_monPig.h"

HRESULT C_monPig::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
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
//	m_Src.m_StateList[StateNUM].SetMatrix(world, view, proj);
//}

void C_monPig::TextArray(int i)
{
	m_Src->m_RtList.clear();
	switch (i)
	{
	case MONSTER_MOVE:
	{
		m_Src->m_RtList.resize(4);
		m_Src->m_RtList[0] = { 1, 137, 55, 38 };
		m_Src->m_RtList[1] = { 78, 202, 55, 38 };
		m_Src->m_RtList[2] = { 135, 202, 60, 38 };
		m_Src->m_RtList[3] = { 198, 203, 57, 36 };
	}
	break;

	case MONSTER_DAMAGE:
	{
		m_Src->m_RtList.resize(3);
		m_Src->m_RtList[0] = { 5,412,58,44 };
		m_Src->m_RtList[1] = { 66,412,58,44 };
		m_Src->m_RtList[2] = { 135,412,58,44 };
	}
	break;

	case MONSTER_DEATH:
	{
		m_Src->m_RtList.resize(2);
		m_Src->m_RtList[0] = { 9,351,57,34 };
		m_Src->m_RtList[1] = { 67,351,57,34 };
	}
	break;
	}
}

void C_monPig::TextSrc()
{
	//MONSTER_MOVE:
	m_Src->m_StateList[MONSTER_MOVE] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_MOVE]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/BlackPigcolor.bmp", "VSmat", "PSBlendMon");
	TextArray(MONSTER_MOVE);
	m_Src->m_StateList[MONSTER_MOVE]->SetRectArray(m_Src->m_RtList, 688, 529);
	m_Src->m_StateList[MONSTER_MOVE]->SetAnimation(0.1f, 0.1f);

	//MONSTER_DAMAGE:
	m_Src->m_StateList[MONSTER_DAMAGE] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_DAMAGE]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/BlackPigcolor.bmp", "VSmat", "PSBlendMon");
	TextArray(MONSTER_DAMAGE);
	m_Src->m_StateList[MONSTER_DAMAGE]->SetRectArray(m_Src->m_RtList, 688, 529);
	m_Src->m_StateList[MONSTER_DAMAGE]->SetAnimation(0.1f, MONSTER_ANIMATIO_TIME);

	//MONSTER_DEATH:
	m_Src->m_StateList[MONSTER_DEATH] = std::make_shared<C_MonsterSrc>();
	m_Src->m_StateList[MONSTER_DEATH]->CSprite::Create(m_Src->m_obj.m_pd3dDevice, m_Src->m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/MonsterSrc/BlackPigcolor.bmp", "VSmat", "PSBlendMon");
	TextArray(MONSTER_DEATH);
	m_Src->m_StateList[MONSTER_DEATH]->SetRectArray(m_Src->m_RtList, 688, 529);
	m_Src->m_StateList[MONSTER_DEATH]->SetAnimation(0.1f, MONSTER_ANIMATIO_TIME);
}

bool C_monPig::Release()
{
	m_Src->Release();
	return true;
}

C_monPig::C_monPig()
{
}


C_monPig::~C_monPig()
{
}
