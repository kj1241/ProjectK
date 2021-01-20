#pragma once
#include "pch.h"

/// <summary>
///  ���ø� ��� �и� (����, ����) �� �ؾߵ� .h ���ϰ� .hpp �ϵ� �߿��� �� �ƴ����� ���ķ� �̷�
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

        // ��� ���� ��Ҵ� 256 ����Ʈ�� ��� ������
        // T�ϵ��� ��� ������ �� �� �� �ֱ� ����
        //m * 256 ����Ʈ ������ �� n * 256 ����Ʈ ����.
        // typedef ����ü D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // UINT64 OffsetInBytes; 256�� ���
        // UINT   SizeInBytes;   256�� ���
        // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
        if (isConstantBuffer)
            mElementByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(T));

        ThrowIfFailed(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mUploadBuffer)));

        ThrowIfFailed(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

        // ���ҽ��� �� ����� ������ ������ ���� �� �ʿ䰡 ����, GPU���� ������� ���ҽ� (����ȭ ����� ����ؾ� ��)
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
