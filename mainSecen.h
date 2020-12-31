#pragma once
#include "pch.h"
#include "mD3d12.h"

//일딴 win form 에서 가상 scene을 만들어 동적할당을 하여 씬을 전환시켜주는 코드 작성 예정 앞서 부모 씬을 만들어야하지만 일단 보류
class MainSecne
{
	mD3d12 mDirectX12; //directX12 파이프라인



public :
	MainSecne(int mClientWidth, int mClientHeight);
	~MainSecne();

	void InitSecen(HWND hWnd);
	void UpdateSecen();
	void OnResizeSecen();
	void DrawSecen();




};
