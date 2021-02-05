#pragma once
#include "mUploadBuffer.h"
#include "md3dUtil.h"
#include "mMath.h"


struct ObjectConstants
{
    DirectX::XMFLOAT4X4 World = mMath::Identity4x4();
};

struct PassConstants
{
    DirectX::XMFLOAT4X4 View = mMath::Identity4x4();
    DirectX::XMFLOAT4X4 InvView = mMath::Identity4x4();
    DirectX::XMFLOAT4X4 Proj = mMath::Identity4x4();
    DirectX::XMFLOAT4X4 InvProj = mMath::Identity4x4();
    DirectX::XMFLOAT4X4 ViewProj = mMath::Identity4x4();
    DirectX::XMFLOAT4X4 InvViewProj = mMath::Identity4x4();
    DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
    float cbPerObjectPad1 = 0.0f;
    DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
    DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
    float NearZ = 0.0f;
    float FarZ = 0.0f;
    float TotalTime = 0.0f;
    float DeltaTime = 0.0f;
};

struct Vertex
{
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT4 Color;
};

// 프레임일 때 CPU가 명령 목록을 작성하는 데 필요한 리소스를 저장
struct FrameResource
{
public:

    FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT waveVertCount);
    FrameResource(const FrameResource& rhs) = delete;
    FrameResource& operator=(const FrameResource& rhs) = delete;
    ~FrameResource();



    //변수로 숨겨야 하지 않을까 고민..... 일딴 보류


    //GPU가 명령 처리를 완료 할 때까지 할당자를 재설정 할 수 없음. 따라서 각 프레임에는 자체 할당자가 필요
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

    //GPU가 명령 처리를 완료 할 때까지 cbuffer를 업데이트 할 수 없음, 각 프레임에는 자체 cbuffer가 필요
    std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
    std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

    // GPU 처리가 완료 될 때까지 동적 정점 버퍼를 업데이트 할수 없음. 각 프레임에는 고유 한 것이 필요
    std::unique_ptr<UploadBuffer<Vertex>> WavesVB = nullptr;

   // 이 펜스 지점까지 명령을 표시하기위한 펜스 값, 프레임 리소스가 GPU 사용중인지 확인 할수 있음
    UINT64 Fence = 0;
};