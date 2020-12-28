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
�ֱ� vs ������Ʈ ���� error ó��
1) LNK2019 error
c/c++->��ó�⸮ ���� win32; �߰�
�Ӽ� ->��Ŀ-> �ý��� �ֿܼ��� ������� ����

2) error C2102: '&' requires l-value ó��
vs���� ������ �� �Լ��� ���ϰ��� �ּҷ� ����� r-value ���� i-value ó���ϴ� ���� �������� �ڵ�.
/w4(level 4) �� ���������� error c4238 ���� ���� �ֽ� ������Ʈ������ ������Ʈ /permissive�� �ɷ��־ error �߻�
c/c++ ->  Lange �ؼ���带 /permissive ���� no�� �����������
  �������� �������� api�� �ٺ��� ������ �ִٰ� ������. api ������ �ּҰ��� �޾Ƽ� ó���ؾߵǴµ� ���� ���������δ� �ּҰ��� �޾ƿü� ���⶧����.


*/