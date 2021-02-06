#include "Box.h"

bool Box::InitBox()
{
    //�Ϸ� ���̸��

    //�ʱ�ȭ�� ���� ��� ����� �缳��
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    BuildDescriptorHeaps();
    BuildConstantBuffers();


    BuildRootSignature();
    BuildShadersAndInputLayout();
    BuildBoxGeometry();
    BuildPSO();

    //�ʱ�ȭ ����� ����.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // �ʱ�ȭ �Ϸ�ɶ����� ��ٸ�
    FlushCommandQueue();


}

void Box::UpdateBox()
{
    // ����ǥ�� ��ī��Ʈ��ǥ�� ��ȯ
    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);

    // ���Ʈ���� ����
    DirectX::XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    DirectX::XMVECTOR target = XMVectorZero();
    DirectX::XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    DirectX::XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);

    DirectX::XMMATRIX world = XMLoadFloat4x4(&mWorld);
    DirectX::XMMATRIX proj = XMLoadFloat4x4(&mProj);
    DirectX::XMMATRIX worldViewProj = world * view * proj;

    // �ֽ� ���� �� ������Ʈ ����� ��� ���۷� ������Ʈ
    ObjectConstants objConstants;
    XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
    mObjectCB->CopyData(0, objConstants);

}

void Box::DrawBox()
{
    //��ɰ� ���õ� �޸� ����
    //���� ��� ����� GPU���� ���� �Ϸ� �� ��쿡�� �缳�� ����
    ThrowIfFailed(mDirectCmdListAlloc->Reset());

    // ��� ����� ExecuteCommendList�� ���ؼ� ��� ��⿭�� �߰� ���� �缳�� ����
    // ��� ��� �����ϸ� �޸� ����
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));

    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // ���ҽ� ��뷮�� ���� ���� ��ȯ�� ��Ÿ��
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // ���� ���� ���� ����
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // �������� ���� ����
    mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

    ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
    mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    mCommandList->IASetVertexBuffers(0, 1, &mBoxGeo->VertexBufferView());
    mCommandList->IASetIndexBuffer(&mBoxGeo->IndexBufferView());
    mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

    mCommandList->DrawIndexedInstanced( mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);

    // ���ҽ� ��뷮�� ���� ���� ��ȯ�� ��Ÿ��
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // ��� ��� �Ϸ�
    ThrowIfFailed(mCommandList->Close());

    // ������ ��� ť�� �߰�
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // �ĸ���ۿ� ������� ��ü
    ThrowIfFailed(mSwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;


    //������ �Ϸ�ɶ����� ��ٸ�
    //��Ƽ�����忡�� ���ľߵ�
    FlushCommandQueue();
}

void Box::OnResizeBox()
{
    //�Ϸ� ���̸��
    //âũ�Ⱑ �����Ǹ� ������ ���� ������ �ٽ� �׷��� �Ǳ� ������
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * mMath::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void Box::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 1;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.NodeMask = 0;
    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,IID_PPV_ARGS(&mCbvHeap)));

}

void Box::BuildConstantBuffers()
{
    mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1, true);

    UINT objCBByteSize = md3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
    //���� i��° ��ü ��� ���ۿ� ���� ������
    int boxCBufIndex = 0;
    cbAddress += boxCBufIndex * objCBByteSize;

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    cbvDesc.BufferLocation = cbAddress;
    cbvDesc.SizeInBytes = md3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    md3dDevice->CreateConstantBufferView(&cbvDesc,mCbvHeap->GetCPUDescriptorHandleForHeapStart());

}

void Box::BuildRootSignature() //?
{
    //���̴� ���α׷����� �Ϲ������� ���ҽ��� �Է����� �ʿ�(��� ����)
    //���̴��� �Լ��� �����ϸ� �Է� ���ҽ��� �Լ� �Ű�����, ��Ʈ������ �Լ��� ����

    // ��Ʈ �Ű������� ���̺� ��Ʈ ������, �Ǵ� ��Ʈ ���.
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];

    // CBV�� ���� ���̺� ����
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

    //��Ʈ������ ��Ʈ �Ű������� �迭
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

   //���� ������۷� ������ ������ ������ ����Ű�� ���� ���� ��Ʈ ����
    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

    if (errorBlob != nullptr)
    {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    ThrowIfFailed(hr);

    ThrowIfFailed(md3dDevice->CreateRootSignature( 0, serializedRootSig->GetBufferPointer(),  serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
}

void Box::BuildShadersAndInputLayout()
{
  //  HRESULT hr = S_OK;

    mvsByteCode = md3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
    mpsByteCode = md3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

    mInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
}

void Box::BuildBoxGeometry()
{
    std::array<Vertex, 8> vertices =
    {
        Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
        Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
        Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
        Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
        Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
        Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
        Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
    };

    std::array<std::uint16_t, 36> indices =
    {
        // ����
        0, 1, 2,
        0, 2, 3,

        // �ĸ�
        4, 6, 5,
        4, 7, 6,

        // ����
        4, 5, 1,
        4, 1, 0,

        // ������
        3, 2, 6,
        3, 6, 7,

        // ��
        1, 5, 6,
        1, 6, 2,

        // �Ʒ�
        4, 0, 3,
        4, 3, 7
    };

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

    mBoxGeo = std::make_unique<MeshGeometry>();
    mBoxGeo->Name = "boxGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
    CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
    CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

    mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),  mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

    mBoxGeo->VertexByteStride = sizeof(Vertex);
    mBoxGeo->VertexBufferByteSize = vbByteSize;
    mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
    mBoxGeo->IndexBufferByteSize = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    mBoxGeo->DrawArgs["box"] = submesh;
}

void Box::BuildPSO() //�����
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
    psoDesc.pRootSignature = mRootSignature.Get();
    psoDesc.VS =
    {
        reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
        mvsByteCode->GetBufferSize()
    };
    psoDesc.PS =
    {
        reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
        mpsByteCode->GetBufferSize()
    };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = mBackBufferFormat;
    psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat = mDepthStencilFormat;
    ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}
