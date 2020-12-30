#pragma once
#include "pch.h"

using namespace std;

class mD3d12
{
	HWND hWnd;

	int mClientWidth = 0;//클라이언트 가로크기
	int mClientHeight = 0; //클라이언트 세로높이

	int gNumFrames = 3;

	//파이프라인 오브젝트
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> gRTVDescriptorHeap;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	UINT gCurrentBackBufferIndex;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	UINT dxgiFactoryFlags = 0;

	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	UINT m4xMsaaQuality = 0;//4x msaa 확인
	bool m4xMsaaState = false;

	UINT64 mCurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	//파생생성자 정의
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;





	
	
	void EnableDebugLayer();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void LoadPipeline();

public:
	mD3d12(int mClientWidth, int mClientHeight);

	bool InitD3D(HWND hWnd);
	void Update();  //
	void Draw(); //그리기

	void OnResize();

};