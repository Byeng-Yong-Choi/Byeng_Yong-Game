#pragma once
#include "Cstd.h"

class CBYWindow
{
public:
	WNDCLASSEX wn;				//������ Ŭ���� Ȯ������� �̿��� ����
	HWND m_Hwnd;				//������ �������� �ڵ�(���� �Ѱ��� �����츸 �̿��ϹǷ� ��ü���� ���� �Ѱ��� �ڵ鸸 ����)
	HINSTANCE hInstance;		//�����쿡 ���� �ν��Ͻ� �������� �����ѹ�
	//RECT m_ClientRt;

public:
	bool InitWindow(const TCHAR* WindowClass, const TCHAR* WindowName);
	virtual bool ResizeDevice(UINT width,UINT height);
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND m_hEdit;
	HWND m_hButton;

public:
	CBYWindow();
	virtual ~CBYWindow();
};

