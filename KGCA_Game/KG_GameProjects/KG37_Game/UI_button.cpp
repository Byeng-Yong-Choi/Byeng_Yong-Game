#include "UI_button.h"
#include "SceneMgr.h"
KYS::UI_button::UI_button()
{
}

KYS::UI_button::~UI_button()
{
}

bool KYS::UI_button::Frame()
{
	UI_obj::Frame();

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(g_hwnd, &cursorPos);

	SHORT key = I_Input.KeyCheck(VK_LBUTTON);
	if (key == KEY_PUSH)
	{
		float convertX, convertY;
		convertX = ((m_vPos.x / (Winrt.right/2)) +1) /2.0f;
		convertY = (((m_vPos.y / (Winrt.bottom/2)) - 1) / 2.0f) * - 1;
		convertX = convertX * Winrt.right;
		convertY = convertY * Winrt.bottom;

		if ((convertX <= cursorPos.x && (convertX + _scale.x *2 ) >= cursorPos.x)
			&&
			(convertY <= cursorPos.y && (convertY + _scale.y * 2) >= cursorPos.y))
		{
			SCENE_MGR.SetNextSceneType(SCENE_TYPE::SCENE_BATTLE);
//			SCENE_MGR.SetNextSceneType(SCENE_TYPE::SCENE_BOSS);
		}
	}

	return false;
}


