#pragma once
#include "CBYCore.h"
#include "C_Clientsrc.h"

class main:public CBYCore
{
public:
	C_Clientsrc m_Client;
	HWND		 m_hEdit;
	HWND		 m_hButton;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void SendMSGClick() override;
public:
	main();
	~main();
};

CBY_RUN(main, CBY_Client, CBY_Client);
