#pragma once
//#ifndef MWINDOWAPI
//#define MWIDOWAPI
//#endif
#include "pch.h"
#include "mainSecen.h"
#include "mGameTime.h"

using namespace std;

class mWindowAPI
{
protected:
	HINSTANCE  hInstance;
	HWND hWnd;
	LPCTSTR name;
	int mClientWidth = 1600;  //â ����
	int mClientHeight = 900; //â ����

	
	MainSecne *mainSence;

	mGameTimer mTimer; // Ÿ�̸�

protected:
	//bool Draw(HWND hWnd);  //����̽� �ʱ�ȭ
	void OnResize();//����̽� ����
	void Update();

public:
	mWindowAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow);
	~mWindowAPI();

	//������ ����
	void lnitWinAPI();
	void Run();
	void ReleaseWinAPI();



	static mWindowAPI* pWinAPI;


	//������
	LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


};
