#include "pch.h"
#include "mWindowAPI.h"

//#pragma comment(lib,"d3dcompiler.lib")
//#pragma comment(lib, "D3D12.lib")
//#pragma comment(lib, "dxgi.lib")

//#pragma comment(lib,"d3d9.lib")
//#pragma comment(lib,"d3dx9.lib")
//#pragma comment(lib,"winmm.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    
    try
    {
        mWindowAPI projectK(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
        projectK.lnitWinAPI();
        projectK.Run();
        projectK.ReleaseWinAPI();
    }

    catch (exception& e)
    {
        MessageBox(nullptr, nullptr, L"HR Failed", MB_OK);
        return 0;
    }



    return 0;
}

/*
최근 vs 업데이트 이후 error 처리
1) LNK2019 error
c/c++->전처기리 정의 win32; 추가
속성 ->링커-> 시스템 콘솔에서 윈도우로 변경

2) error C2102: '&' requires l-value 처리
vs에서 보내온 말 함수의 리턴값을 주소로 만드는 r-value 값을 i-value 처리하는 것은 부적잘한 코드.
/w4(level 4) 의 경고수준으로 error c4238 동반 따라서 최신 업데이트에서는 프로젝트 /permissive가 걸려있어서 error 발생
c/c++ ->  Lange 준수모드를 /permissive 에서 no로 설정해줘야함
  개인적인 생각으로 api에 근본적 문제가 있다고 생각함. api 에서는 주소값을 받아서 처리해야되는데 현제 경고수준으로는 주소값을 받아올수 없기때문임.


*/