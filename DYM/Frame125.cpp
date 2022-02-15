// Frame125.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Frame125.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);		// ������ â ��Ÿ���� �����ϱ� ���� �뵵�� �Լ�
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,			// �޸𸮿� �Ҵ�Ǵ� ��ü(���� �����ǰ� �ִ� ���α׷��� �ǹ�)
                     _In_opt_ HINSTANCE hPrevInstance,	// ���� ����Ǿ��� �ν��Ͻ� �ڵ��� �Ѿ�´�, ���� ��� null, ���� ȣȯ���� ���� ������
                     _In_ LPWSTR    lpCmdLine,			// TSTR : �����ڵ�, TCHAR���� STRING Ŭ����, LP : �����͸� ����, 
                     _In_ int       nCmdShow)			// ������ â�� ��Ÿ�� �μ� ���� �Ѿ���� �Ű� ����
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FRAME125, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRAME125));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
    while (GetMessage(&msg, nullptr, 0, 0))		// ������� �޼��� ť��� �ڷᱸ�� ���¸� ���ϰ� �ִ�.
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) // ����Ű�� �ش��ϴ� ��ɾ �ƴ϶��
        {
            TranslateMessage(&msg);		// Ű���� �޽����� �����Ͽ� ���α׷����� ���� ����� �� �ֵ��� ���ִ� �Լ�
            DispatchMessage(&msg);		// �ý��� �޼��� ť���� ���� �޼����� ���α׷��� �޼��� ó�� �Լ�(WndProc)�� �����ϴ� �Լ�
        }
    }

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRAME125));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;// MAKEINTRESOURCEW(IDC_FRAME125);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   RECT rcWindow{ 0, 0, 800, 600 };

   HWND hWnd = CreateWindowW(szWindowClass,				// �ռ� ������ ������ Ŭ������ ������ Ŭ���� �̸��� �־��ش�.
							   szTitle,					// ������ â ����� ����� ���ڿ�
							   WS_OVERLAPPEDWINDOW,		// ������� �ϴ� ������ â ��Ÿ�� ����, �⺻ ������ ����ڵ��� ���� ���ϴ� ������ ����� �ǹ��ϴ� FLAG ��
							   CW_USEDEFAULT,			// �����ϴ� â�� LEFT, TOP ��ġ
							   0, 
							   rcWindow.right - rcWindow.left,						// (�̰͸� �߿�!!!!)�����Ǵ� â�� ���� ������, ���� ������
							   rcWindow.bottom - rcWindow.top,
							   nullptr,					// �θ������찡 �ִٸ� �θ� �������� �ڵ� ����, �����ϱ� NULL
							   nullptr,					// �����쿡�� ����� �޴��� �ڵ��� ����, �������Ϳ� ����� �޴� ���¸� ����Ϸ��� NULL
							   hInstance,				// �����츦 ����� ��ü, ���α׷��� �ڵ��� �����Ѵ�.
							   nullptr);				// �ü���� Ư���� �������� ����ϹǷ� �ǵ帱 ���� ����.

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

RECT	rc{ 300, 300, 400, 400 };
list<RECT>		BulletList;

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 0, 0, 0);			// Ÿ�̸Ӹ� ��ġ�ϴ� �Լ�
											// 1���� : �ڵ� , 2���� : Ÿ�̸��� id, 
											// 3���� : Ÿ�̸� �ֱ� 1/1000 ��,4���� : null�� ��� wm_timer �޼����� �߻���Ŵ
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, 0, true);			// ������ ���� �Լ�, �� �Լ��� ȣ��Ǵ� ��� wm_paint �޼����� �߻�
												// 2���� : ������ �ȿ��� ������ ���� ����, 0�� ��� ������ ��ü ����
		break;									// 3���� : false�� ��� ���� �׸��� �κи� ����, true�� ��� �׷��� ���� �ʴ� �κе� ����

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);		// (handle device context) : ��ȭ��

			//MoveToEx(hdc, 100, 100, NULL);			// ������ ��ġ�κ��� �׸��⸦ �����ϰڴٰ� �ϴ� �Լ�
			//LineTo(hdc, 200, 100);
			//LineTo(hdc, 200, 200);
			//LineTo(hdc, 100, 200);
			//LineTo(hdc, 100, 100);


			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			/*for (list<RECT>::iterator iter = BulletList.begin();
				iter != BulletList.end(); ++iter)*/
			for(auto& iter : BulletList)
			{
				/*Ellipse(hdc, (*iter).left, (*iter).top, (*iter).right, (*iter).bottom);
				(*iter).top		-= 10;
				(*iter).bottom	-= 10;*/

				Ellipse(hdc, iter.left, iter.top, iter.right, iter.bottom);
				iter.top -= 10;
				iter.bottom -= 10;
			}
				


			//LineTo(hdc, 200, 200);
			//LineTo(hdc, 200, 300);


			// GDI(graphic device interface) : ȭ��ó�� �׷����� ���, �׸�, ���� ��� ��¿� ����

            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			rc.left -= 10;
			rc.right -= 10;
			break;

		case VK_RIGHT:
			rc.left += 10;
			rc.right += 10;
			break;

		case VK_UP:
			rc.top -= 10;
			rc.bottom -= 10;
			break;

		case VK_DOWN:
			rc.top += 10;
			rc.bottom += 10;
			break;

		case VK_SPACE:
			BulletList.push_back(rc);
			break;

		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}

		break;

    case WM_DESTROY:
		KillTimer(hWnd, 0);
        PostQuitMessage(0);		
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
