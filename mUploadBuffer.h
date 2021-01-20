#pragma once
#include "pch.h"

/// <summary>
///  템플릿 헤더 분리 (선언, 구현) 를 해야됨 .h 파일과 .hpp 일딴 중요한 게 아님으로 차후로 미룸
/// 
/// </summary>
/// <typeparam name="T"></typeparam>

template<typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : mIsConstantBuffer(isConstantBuffer)
    {
        mElementByteSize = sizeof(T);

        // 상수 버퍼 요소는 256 바이트의 배수 여야함
        // T하드웨어가 상수 데이터 만 볼 수 있기 때문
        //m * 256 바이트 오프셋 및 n * 256 바이트 길이.
        // typedef 구조체 D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // UINT64 OffsetInBytes; 256의 배수
        // UINT   SizeInBytes;   256의 배수
        // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
        if (isConstantBuffer)
            mElementByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(T));

        ThrowIfFailed(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mUploadBuffer)));

        ThrowIfFailed(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

        // 리소스를 다 사용할 때까지 매핑을 해제 할 필요가 없음, GPU에서 사용중인 리소스 (동기화 기술을 사용해야 함)
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer();

    ID3D12Resource* Resource()const
    {
        return mUploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
    }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
    BYTE* mMappedData = nullptr;

    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;
};



template<typename T>
inline UploadBuffer<T>::~UploadBuffer()
{
    if (mUploadBuffer != nullptr)
        mUploadBuffer->Unmap(0, nullptr);

    mMappedData = nullptr;
}
