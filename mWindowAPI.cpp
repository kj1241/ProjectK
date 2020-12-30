#include "mWindowAPI.h"

mWindowAPI* mWindowAPI::pWinAPI = nullptr;


static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return mWindowAPI::pWinAPI->wndProc(hWnd, msg, wParam, lParam);
}




mWindowAPI::mWindowAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow) :hInstance(hInstance), hWnd(nullptr), name(L"projectK"), mDirectX12( mClientWidth, mClientHeight)
{
	pWinAPI = this;
}

mWindowAPI::~mWindowAPI()
{


}



void mWindowAPI::lnitWinAPI()
{
	if (hWnd)
		throw std::out_of_range("wc is fail");

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ::wndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDC_ARROW);
	wc.hCursor = LoadCursor(hInstance, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = name;
	wc.hIconSm = NULL;

	if (!RegisterClassEx(&wc))
		throw std::out_of_range("wc is fail");

	//윈도우 창 만들기


	hWnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mClientWidth, mClientHeight, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
		throw std::out_of_range("hWnd is fail");



	mDirectX12.InitD3D(hWnd);
	mDirectX12.OnResize();




}

void mWindowAPI::Run()
{

	//if (!InitD3D(hWnd))
		//throw std::out_of_range("D3D is fail");

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	/// 메시지 루프
	MSG msg = { 0 };
	/*
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	*/

	while (msg.message != WM_QUIT)
	{
		// 윈도우 메시지가 있으면 처리
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 그렇지 않으면 애니메이션
		else
		{
			/*
			if (!mAppPaused)
			{
				CalculateFrameStats();
				Update(mTimer);
				Draw(mTimer);
			}
			else
			{
				Sleep(100);
			}
			*/
			mDirectX12.Update();
			mDirectX12.Draw();


		}
	}
}

void mWindowAPI::ReleaseWinAPI()
{
	//등록된 클래스 소거
	UnregisterClass(L"ProjectK", hInstance);

}



LRESULT mWindowAPI::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		OnResize();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		Update();
		ValidateRect(hWnd, NULL);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);

	}



	return 0;
}

void mWindowAPI::OnResize()
{

}

void mWindowAPI::Update()
{
}

