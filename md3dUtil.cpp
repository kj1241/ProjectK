#include "md3dUtil.h"



bool md3dUtil::IsKeyDown(int vkeyCode)
{
    return (GetAsyncKeyState(vkeyCode) & 0x8000) != 0;
}

UINT md3dUtil::CalcConstantBufferByteSize(UINT byteSize)
{
    // ��� ���۴� �ּ� �ϵ������ ���
       // �ݿø� �Ҵ� ũ�� (�Ϲ������� 256 ����Ʈ)
       // 256�� ���,255����ŷ
       // 256������ 2��Ʈ����
       // ex) Suppose byteSize = 300.
       // (300 + 255) & ~255
       // 555 & ~255
       // 0x022B & ~0x00ff
       // 0x022B & 0xff00
       // 0x0200
       // 512
    return (byteSize + 255) & ~255;
}

Microsoft::WRL::ComPtr<ID3DBlob> md3dUtil::LoadBinary(const std::wstring& filename)
{
    std::ifstream fin(filename, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    std::ifstream::pos_type size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);

    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));

    fin.read((char*)blob->GetBufferPointer(), size);
    fin.close();

    return blob;
}

Microsoft::WRL::ComPtr<ID3D12Resource> md3dUtil::CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

    // ���� �⺻ ���� ���ҽ��� ����
    ThrowIfFailed(device->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteSize), D3D12_RESOURCE_STATE_COMMON, nullptr,IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

    // CPU �޸� �����͸� �⺻ ���ۿ� �����Ϸ��� ���� �÷����� 
    ThrowIfFailed(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(uploadBuffer.GetAddressOf())));


    // �⺻���ۿ� �����ϴ� ������
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // ���� ������ UpdateSubresources�Լ��� �����͸� �⺻ ���ҽ��� �����ϵ��� ���� ��
    // ID3D12CommandList :: CopySubresourceRegion ����Ͽ�  CPU�޸𸮸� ���� �����Ͽ� ���ε�
    // �����ʹ� mBuffer�� ����
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

    // ���� �Լ��� ȣ�� �� �Ŀ��� uploadBuffer�� �����Ǿ� ��.
    // ���� �����ϸ� �����ϴ� ����� ���� ������� ����
    // ȣ���ڴ� ���簡 ���� �� ���� �˰� ���� uploadBuffer�� ����

    return defaultBuffer;
}

Microsoft::WRL::ComPtr<ID3DBlob> md3dUtil::CompileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target)
{
    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = S_OK;

    Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errors;
    hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

    if (errors != nullptr)
        OutputDebugStringA((char*)errors->GetBufferPointer());

    ThrowIfFailed(hr);

    return byteCode;
}
