#include "stdafx.h"
#include "gameScene.h"


HRESULT gameScene::init()
{
	//// 마우스 감추기
	ShowCursor(FALSE);
	m_imgMouseCur = IMAGEMANAGER->addImage("MouseCursor", "image/inGameImg/UI/MousePoint.bmp", 112, 49, 2, 1, true, RGB(255, 0, 255));

	RECT rc = RectMake(WINS_PRISON_MIN_X, WINS_PRISON_MIN_Y, WINS_PRISON_MAX_X, WINS_PRISON_MAX_Y);
	ClipCursor(&rc);
	//// UI 매니저 동적할당
	m_pUiMag = new uiManager;

	//// 플레이어 셋팅
	m_tPlayer = new PlayerInfo;
	m_tPlayer->t_TileDesEne = 100;

	m_MapTile = new tileMap;
	m_MapTile->init(32, 42, m_tPlayer, m_pUiMag);
	CAMERA->setCamPosY(CAMERA_MAX_Y);
	
	EFFMANAGER->addEffect("tileDes", "image/inGameImg/EFF/Tile_Des.bmp", 512, 384, 512 / 4, 384 / 3, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("MousePointEFF", "image/inGameImg/EFF/EXP_EFF_1.bmp", 238, 34, 238 / 7, 34 / 1, 15, 5, 0.0f, true);
	
	IMAGEMANAGER->addImage("mousePoint", "image/inGameImg/UI/MousePoint.bmp", 112, 49, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("NumberPont", "image/inGameImg/UI/NumberPont.bmp", 80, 60, 10, 6, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("TilePopup", "image/inGameImg/UI/TilePopup.bmp", 157, 95, 1, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_0", "image/inGameImg/EFF/TileSet_Terrain_DesSet0.bmp", 12, 11, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_1", "image/inGameImg/EFF/TileSet_Terrain_DesSet1.bmp", 16, 16, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_2", "image/inGameImg/EFF/TileSet_Terrain_DesSet2.bmp", 23, 22, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Item_Jewel", "image/inGameImg/EFF/Item_JewelSet.bmp", 75, 105, 5, 7, true, RGB(255, 0, 255));

	m_imgTopBg = IMAGEMANAGER->addImage("inGameTopBG", "image/inGameImg/BG/InGame_Top_Bg.bmp", m_MapTile->gettileMaxValueX(), TOP_IMG_SIZE_Y);


	return S_OK;
}

void gameScene::release()
{
	SAFE_DELETE(m_MapTile);
	SAFE_DELETE(m_tPlayer);
	SAFE_DELETE(m_pUiMag);
}

void gameScene::update()
{
	m_MapTile->update();
	CAMERA->update(0.0f, CAMERA_MAX_Y, m_MapTile->gettileMaxValueX() - WINSIZEX, m_MapTile->gettileMaxValueY() - WINSIZEY);
	m_pUiMag->update();
	OBJECTMANAGER->update();
	EFFMANAGER->update();
}

void gameScene::render(HDC hdc)
{
	m_imgTopBg->render(hdc, 0 - CAMERA->getCamPosX(), 0 - CAMERA->getCamPosY() + CAMERA_MAX_Y);
	m_MapTile->render(hdc);
	m_pUiMag->render(hdc);
	OBJECTMANAGER->render(hdc);
	EFFMANAGER->render(hdc);
	screenUi(hdc);

	if (g_saveData.gisTest)
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 0, 255));
		//// 카메라 좌표 디버깅 정보
		sprintf_s(szText, "CAM_X : %.3f", CAMERA->getCamPosX());
		TextOut(hdc, WINSIZEX - 120, 10, szText, strlen(szText));
		sprintf_s(szText, "Mouse_X : %d", g_ptMouse.x);
		TextOut(hdc, WINSIZEX - 120, 30, szText, strlen(szText));
		sprintf_s(szText, "Mouse_Y : %d", g_ptMouse.y);
		TextOut(hdc, WINSIZEX - 120, 50, szText, strlen(szText));
		sprintf_s(szText, "PlayerEne : %d", m_tPlayer->t_TileDesEne);
		TextOut(hdc, WINSIZEX - 120, 70, szText, strlen(szText));
	}
}

void gameScene::screenUi(HDC hdc)
{
	//// 채굴 파워 UI
	if (m_tPlayer->t_TileDesEne <= MOUSE_FOINT_CANG_VALUE)
	{
		MY_UTIL::NumberPont(hdc, m_tPlayer->t_TileDesEne, MOUSE_FOINT_SET_NUMSIZE, g_ptMouse.x + MOUSE_FOINT_SET_X, g_ptMouse.y - MOUSE_FOINT_SET_Y, 1, 2.0f);
	}
	else
	{
		MY_UTIL::NumberPont(hdc, m_tPlayer->t_TileDesEne, MOUSE_FOINT_SET_NUMSIZE, g_ptMouse.x + MOUSE_FOINT_SET_X, g_ptMouse.y - MOUSE_FOINT_SET_Y, 2, 2.0f);
	}

	//// 마우스 커서
	if (m_MapTile->getIsTileClivk())
	{
		m_imgMouseCur->frameRender(hdc, g_ptMouse.x - MOUSE_CURSOR_SET_SIZE, g_ptMouse.y - MOUSE_CURSOR_SET_SIZE, 1, 0, 2.0f, false);
	}
	else
	{
		m_imgMouseCur->frameRender(hdc, g_ptMouse.x - MOUSE_CURSOR_SET_SIZE, g_ptMouse.y - MOUSE_CURSOR_SET_SIZE, 0, 0, 2.0f, false);
	}
}

gameScene::gameScene()
{
}


gameScene::~gameScene()
{
}
