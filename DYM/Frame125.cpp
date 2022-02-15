// Frame125.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Frame125.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);		// 윈도우 창 스타일을 변경하기 위한 용도의 함수
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,			// 메모리에 할당되는 실체(현재 구동되고 있는 프로그램을 의미)
                     _In_opt_ HINSTANCE hPrevInstance,	// 전에 실행되었떤 인스턴스 핸들이 넘어온다, 없는 경우 null, 단지 호환성을 위해 존재함
                     _In_ LPWSTR    lpCmdLine,			// TSTR : 유니코드, TCHAR만든 STRING 클래스, LP : 포인터를 뜻함, 
                     _In_ int       nCmdShow)			// 실행한 창의 스타일 인수 값이 넘어오는 매개 변수
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FRAME125, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRAME125));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))		// 윈도우는 메세지 큐라는 자료구조 형태를 지니고 있다.
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) // 단축키에 해당하는 명령어가 아니라면
        {
            TranslateMessage(&msg);		// 키보드 메시지를 가공하여 프로그램에서 쉽게 사용할 수 있도록 해주는 함수
            DispatchMessage(&msg);		// 시스템 메세지 큐에서 꺼낸 메세지를 프로그램의 메세지 처리 함수(WndProc)로 전달하는 함수
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rcWindow{ 0, 0, 800, 600 };

   HWND hWnd = CreateWindowW(szWindowClass,				// 앞서 정의한 윈도우 클래스의 전달한 클래스 이름을 넣어준다.
							   szTitle,					// 윈도우 창 상단의 출력할 문자열
							   WS_OVERLAPPEDWINDOW,		// 만들고자 하는 윈도우 창 스타일 지정, 기본 값으로 사용자들이 흔히 접하는 윈도우 모양을 의미하는 FLAG 값
							   CW_USEDEFAULT,			// 생성하는 창의 LEFT, TOP 위치
							   0, 
							   rcWindow.right - rcWindow.left,						// (이것만 중요!!!!)생성되는 창의 가로 사이즈, 세로 사이즈
							   rcWindow.bottom - rcWindow.top,
							   nullptr,					// 부모윈도우가 있다면 부모 윈도우의 핸들 지정, 없으니깐 NULL
							   nullptr,					// 윈도우에서 사용할 메뉴의 핸들을 지정, 레지스터에 등록한 메뉴 형태를 사용하려면 NULL
							   hInstance,				// 윈도우를 만드는 주체, 프로그램의 핸들을 지정한다.
							   nullptr);				// 운영체제가 특수한 목적으로 사용하므로 건드릴 일이 없다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 0, 0, 0);			// 타이머를 설치하는 함수
											// 1인자 : 핸들 , 2인자 : 타이머의 id, 
											// 3인자 : 타이머 주기 1/1000 초,4인자 : null인 경우 wm_timer 메세지를 발생시킴
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, 0, true);			// 윈도우 갱신 함수, 이 함수가 호출되는 경우 wm_paint 메세지를 발생
												// 2인자 : 윈도우 안에서 갱신할 범위 지정, 0인 경우 윈도우 전체 영역
		break;									// 3인자 : false인 경우 새로 그리는 부분만 갱신, true인 경우 그려져 있지 않는 부분도 갱신

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
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
            HDC hdc = BeginPaint(hWnd, &ps);		// (handle device context) : 도화지

			//MoveToEx(hdc, 100, 100, NULL);			// 지정한 위치로부터 그리기를 시작하겠다고 하는 함수
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


			// GDI(graphic device interface) : 화면처리 그래픽을 담당, 그림, 글자 모두 출력에 관여

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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
