#pragma once
#include "pch.h"
#include "mD3d12.h"

//�ϵ� win form ���� ���� scene�� ����� �����Ҵ��� �Ͽ� ���� ��ȯ�����ִ� �ڵ� �ۼ� ���� �ռ� �θ� ���� ������������ �ϴ� ����
class MainSecne
{
	mD3d12 mDirectX12; //directX12 ����������



public :
	MainSecne(int mClientWidth, int mClientHeight);
	~MainSecne();

	void InitSecen(HWND hWnd);
	void UpdateSecen();
	void OnResizeSecen();
	void DrawSecen();




};
