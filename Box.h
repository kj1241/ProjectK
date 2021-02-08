#pragma once

#include "pch.h"
#include "mMath.h"
#include "md3dUtil.h"

/*
2021.02.08
지금 고민중에 있는 것: DirectX12 에서는 new를 써서 관리하던 Directx9과 다르게 GPU와 CPU를 사용하기 위해 초기화를 파이프라인에 집어 넣어서 관리함.
물론 이런점이 디자인 패턴측면에서 보면 텍스쳐, 버택스정보 등을 모아서 한번에 관리하는 것도 맞긴함.
하지만 여려사람이랑 작업 하거나 재사용 측면에서 보면 독립해야되는 것도 맞긴함.
어느 것도 틀리지 않고 이건 설계의 다른점 이기 때문에 충분한 고민이 필요.

추가 고민인건 멀티 쓰래드를 이용하여 뿌릴수 있는걸 코어 할당하는게 맞는지 고민중.
조금만 더 고민해보기.

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

