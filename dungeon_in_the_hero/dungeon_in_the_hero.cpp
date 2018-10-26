// 20180814.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "dungeon_in_the_hero.h"// 헤더파일 포함
#include "mainGame.h"



HINSTANCE	g_hInstance;	// 프로그램 인스턴스 핸들
HWND		g_hWnd;			// 윈도우 핸들
HFONT		g_hFont;
HFONT		g_OldFont;		// 폰트 설정
LPTSTR		g_lpszClass = (LPTSTR)TEXT("용사 따위가 날 잡아갈 수있을리가 없잖아");
RECT		g_MouseRc = { 0,0,0,0 };
POINT		g_ptMouse = { 0, 0 };
mainGame	g_mainGame;

// save data
SAVE_DATA	g_saveData;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void SetWindowSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstacne,
	LPSTR _lpszCmdParam, int nCmdShow)
{
	g_hInstance = _hInstance;


	MSG message;

	WNDCLASS wndClass;

	wndClass.cbClsExtra = 0;		// 클래스 여분 메모리
	wndClass.cbWndExtra = 0;		// 윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	// 백그라운드 색상
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// 커서
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// 아이콘
	wndClass.hInstance = _hInstance;								// 인스턴스 핸들
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						// 윈도우 프로시저
	wndClass.lpszClassName = g_lpszClass;							// 클래스 이름
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDC_MY20180814);								// 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						// 윈도우 스타일

	RegisterClass(&wndClass);

#ifdef FULL_SCREEN
	DEVMODE	dm;

	ZeroMemory(&dm, sizeof(DEVMODE));
	dm.dmBitsPerPel = 32;
	dm.dmPelsWidth = 1920;
	dm.dmPelsHeight = 1080;
	dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH
		| DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

	// 전체화면 실패하면 다시 되돌린다
	if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		ChangeDisplaySettings(&dm, 0);
	}

	// 윈도우 생성
	g_hWnd = CreateWindow(g_lpszClass,
		g_lpszClass,
		WS_POPUPWINDOW | WS_MAXIMIZE,
		WINSTARTX,
		WINSTARTY,
		WINSIZEX,
		WINSIZEY,
		NULL,
		(HMENU)NULL,
		_hInstance,
		NULL);

	// 화면 작업 사이즈 영역 계산
	SetWindowSize(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#else

	// 윈도우 생성
	g_hWnd = CreateWindow(g_lpszClass,
		g_lpszClass,
		WS_OVERLAPPEDWINDOW,
		WINSTARTX,
		WINSTARTY,
		WINSIZEX,
		WINSIZEY,
		NULL,
		(HMENU)NULL,
		_hInstance,
		NULL);

	// 화면 작업 사이즈 영역 계산
	SetWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);
#endif
	// 윈도우 출력
	ShowWindow(g_hWnd, nCmdShow);

	if (FAILED(g_mainGame.init()))
	{
		return 0;
	}

	// 메시지큐에 있는 메시지 처리
	// 메시지가 발생할 때까지 기다린다 (대기)
	//while (GetMessage(&message, 0, 0, 0))
	//{
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	while (true)
	{
		// 메시지가 발생할 때만 반응한다
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)	break;

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIMEMANAGER->update(60.0f);

			g_mainGame.update();
			g_mainGame.render();
		}
	}

	g_mainGame.release();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}

void SetWindowSize(int x, int y, int width, int height)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(g_hWnd, NULL,
		x, y,
		(rc.right - rc.left), (rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}