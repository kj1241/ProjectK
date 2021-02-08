#pragma once

#include "pch.h"
#include "mMath.h"
#include "md3dUtil.h"

/*
2021.02.08
���� ����߿� �ִ� ��: DirectX12 ������ new�� �Ἥ �����ϴ� Directx9�� �ٸ��� GPU�� CPU�� ����ϱ� ���� �ʱ�ȭ�� ���������ο� ���� �־ ������.
���� �̷����� ������ �������鿡�� ���� �ؽ���, ���ý����� ���� ��Ƽ� �ѹ��� �����ϴ� �͵� �±���.
������ ��������̶� �۾� �ϰų� ���� ���鿡�� ���� �����ؾߵǴ� �͵� �±���.
��� �͵� Ʋ���� �ʰ� �̰� ������ �ٸ��� �̱� ������ ����� ����� �ʿ�.

�߰� ����ΰ� ��Ƽ �����带 �̿��Ͽ� �Ѹ��� �ִ°� �ھ� �Ҵ��ϴ°� �´��� �����.
���ݸ� �� ����غ���.

*/

class Box
{

	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildBoxGeometry();
	void BuildPSO();



public:

	Box(HINSTANCE hInstance);
	Box(const Box& rhs) = delete;
	Box& operator=(const Box& rhs) = delete;
	~Box();

	bool InitBox();
	void UpdateBox();
	void DrawBox();

	void OnResizeBox();




};

