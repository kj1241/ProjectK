#pragma once
//#ifndef MWINDOWAPI
//#define MWIDOWAPI
//#endif
#include "pch.h"
#include "mD3d12.h"

using namespace std;

class mWindowAPI
{
protected:
	HINSTANCE  hInstance;
	HWND hWnd;
	LPCTSTR name;
	int mClientWidth = 800;  //창 가로
	int mClientHeight = 600; //창 세로

	mD3d12 mDirectX12;

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
