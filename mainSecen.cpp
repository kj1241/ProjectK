#include "mainSecen.h"

MainSecne::MainSecne(int mClientWidth, int mClientHeight):mDirectX12(mClientWidth, mClientHeight)
{
}

MainSecne::~MainSecne()
{
}

void MainSecne::InitSecen(HWND hWnd)
{
	mDirectX12.InitD3D(hWnd);
	
}

void MainSecne::UpdateSecen()
{
	mDirectX12.Update();
}

void MainSecne::OnResizeSecen()
{
	mDirectX12.OnResize();
}

void MainSecne::DrawSecen()
{
	mDirectX12.Draw();
}
