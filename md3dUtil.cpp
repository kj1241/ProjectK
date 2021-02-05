#include "md3dUtil.h"



bool md3dUtil::IsKeyDown(int vkeyCode)
{
    return (GetAsyncKeyState(vkeyCode) & 0x8000) != 0;
}

UINT md3dUtil::CalcConstantBufferByteSize(UINT byteSize)
{
    // 상수 버퍼는 최소 하드웨어의 배수
       // 반올림 할당 크기 (일반적으로 256 바이트)
       // 256의 배수,255마스킹
       // 256이하의 2비트저장
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

    // 실제 기본 버퍼 리소스를 생성
    ThrowIfFailed(device->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteSize), D3D12_RESOURCE_STATE_COMMON, nullptr,IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

    // CPU 메모리 데이터를 기본 버퍼에 복사하려면 힙에 올려야함 
    ThrowIfFailed(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(uploadBuffer.GetAddressOf())));


    // 기본버퍼에 복사하는 데이터
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // 높은 수준의 UpdateSubresources함수로 데이터를 기본 리소스에 복사하도록 도움 줌
    // ID3D12CommandList :: CopySubresourceRegion 사용하여  CPU메모리를 힙에 복사하여 업로드
    // 데이터는 mBuffer에 복사
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

    // 위의 함수가 호출 된 후에도 uploadBuffer는 유지되야 함.
    // 실제 복사하를 수행하는 명령은 아직 실행되지 않음
    // 호출자는 복사가 실행 된 것을 알고 나면 uploadBuffer를 해제

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
