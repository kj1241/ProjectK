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
	int mClientWidth = 1600;  //창 가로
	int mClientHeight = 900; //창 세로

	
	MainSecne *mainSence;

	mGameTimer mTimer; // 타이머

protected:
	//bool Draw(HWND hWnd);  //디바이스 초기화
	void OnResize();//디바이스 해제
	void Update();

public:
	mWindowAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow);
	~mWindowAPI();

	//윈도우 실행
	void lnitWinAPI();
	void Run();
	void ReleaseWinAPI();



	static mWindowAPI* pWinAPI;


	//윈프록
	LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


};
