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

// �������� �� CPU�� ��� ����� �ۼ��ϴ� �� �ʿ��� ���ҽ��� ����
struct FrameResource
{
public:

    FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT waveVertCount);
    FrameResource(const FrameResource& rhs) = delete;
    FrameResource& operator=(const FrameResource& rhs) = delete;
    ~FrameResource();



    //������ ���ܾ� ���� ������ ���..... �ϵ� ����


    //GPU�� ��� ó���� �Ϸ� �� ������ �Ҵ��ڸ� �缳�� �� �� ����. ���� �� �����ӿ��� ��ü �Ҵ��ڰ� �ʿ�
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

    //GPU�� ��� ó���� �Ϸ� �� ������ cbuffer�� ������Ʈ �� �� ����, �� �����ӿ��� ��ü cbuffer�� �ʿ�
    std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
    std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

    // GPU ó���� �Ϸ� �� ������ ���� ���� ���۸� ������Ʈ �Ҽ� ����. �� �����ӿ��� ���� �� ���� �ʿ�
    std::unique_ptr<UploadBuffer<Vertex>> WavesVB = nullptr;

   // �� �潺 �������� ����� ǥ���ϱ����� �潺 ��, ������ ���ҽ��� GPU ��������� Ȯ�� �Ҽ� ����
    UINT64 Fence = 0;
};