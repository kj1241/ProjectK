#pragma once

#include "pch.h"
#include "mMath.h"
#include "md3dUtil.h"

//이렇게 코딩하는 이유 : 도큐먼트, 책에서는 상속받아서 레이어를 구성하지만 개인적으로 생각했을 때 협업이나 코드 재사용측면에서 오브젝트 위주로 배치하는 것이 훨씬 좋다고 생각하기 때문에

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

