#include "stdafx.h"
#include "mainGame.h"
#include "loadingScene.h"
#include "titleScene.h"
#include "gameScene.h"

void mainGame::setBackBuffer()
{
	m_pBackBuffer = new image;
	m_pBackBuffer->init(WINSIZEX, WINSIZEY);
}

HRESULT mainGame::init()
{
	hdc = GetDC(g_hWnd);

	//HANDLE hTimer = (HANDLE)SetTimer(g_hWnd, 1, 10, NULL);
	KEYMANAGER->init();
	IMAGEMANAGER->init();
	TIMEMANAGER->init();
	SCENEMANAGER->init();
	EFFMANAGER->init();
	OBJECTMANAGER->init();

	setBackBuffer();

	// 필요한 리소스 미리 로드
	
	//m_pTileMapScene = new tileMap;
	//SCENEMANAGER->addScene("tileMap", m_pTileMapScene);
	m_pLoadingScene = new loadingScene;
	SCENEMANAGER->addLoadingScene("loading", m_pLoadingScene);

	m_pTitleScene = new titleScene;
	SCENEMANAGER->addScene("title", m_pTitleScene);

	m_pinGameScene = new gameScene;
	SCENEMANAGER->addScene("inGame", m_pinGameScene);
	
	SCENEMANAGER->changeScene("inGame");

	return S_OK;
}

void mainGame::release()
{
	// 씬 매니저 내부(SCENEMANAGER->release())에서 처리하므로
	// 여기서는 삭제
	//m_pBattleScene->release();
	//SAFE_DELETE(m_pBattleScene);

	//m_pTitleScene->release();
	//SAFE_DELETE(m_pTitleScene);

	SAFE_DELETE(m_pBackBuffer);

	//KillTimer(g_hWnd, 1);
	ReleaseDC(g_hWnd, hdc);

	TXTDATA->release();
	KEYMANAGER->release();
	IMAGEMANAGER->release();
	TIMEMANAGER->release();
	SCENEMANAGER->release();
	EFFMANAGER->release();

	OBJECTMANAGER->release();

	TXTDATA->releaseSingleton();
	KEYMANAGER->releaseSingleton();
	IMAGEMANAGER->releaseSingleton();
	TIMEMANAGER->releaseSingleton();
	SCENEMANAGER->releaseSingleton();
	CAMERA->releaseSingleton();
	EFFMANAGER->releaseSingleton();
	OBJECTMANAGER->releaseSingleton();
}

LRESULT mainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	//case WM_MOUSEHWHEEL:
	//	if ((SHORT)HIWORD(wParam) > 0)
	//	{ // 0 보다 크면 위로 올라감
	//		m_pEditorScene_RE->WheelEvent(1, wParam);
	//	}
	//	else
	//	{ // 0 보다 작으면 밑으로 내려감
	//		m_pEditorScene_RE->WheelEvent(-1, wParam);
	//	}
	//	break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_COMMAND:
		break;
	}


	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void mainGame::update()
{
	InvalidateRect(g_hWnd, NULL, false);

	g_MouseRc = RectMakeCenter(g_ptMouse.x, g_ptMouse.y, 2, 2);

	// 테스트 코드
	if (KEYMANAGER->isOnceKeyDown('T'))
	{
		if (g_saveData.gisTest)
		{
			g_saveData.gisTest = false;
		}
		else
		{
			g_saveData.gisTest = true;
		}
	}

	SCENEMANAGER->update();
}

void mainGame::render()
{
	HDC backDC = m_pBackBuffer->getMemDC();





	// titleScene
	//m_pTitleScene->render(backDC);

	// battleScene
	//m_pBattleScene->render(backDC);

	SCENEMANAGER->render(backDC);

	m_pBackBuffer->render(hdc, 0, 0);

	if (g_saveData.gisTest)
		TIMEMANAGER->render(hdc);
}

mainGame::mainGame()
{
}


mainGame::~mainGame()
{
}
