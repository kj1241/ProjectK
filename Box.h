#pragma once

#include "pch.h"
#include "mMath.h"
#include "md3dUtil.h"

//�̷��� �ڵ��ϴ� ���� : ��ť��Ʈ, å������ ��ӹ޾Ƽ� ���̾ ���������� ���������� �������� �� �����̳� �ڵ� �������鿡�� ������Ʈ ���ַ� ��ġ�ϴ� ���� �ξ� ���ٰ� �����ϱ� ������

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

