#include "stdafx.h"
#include "gameScene.h"
#include "enemyManager.h"
#include "heroManager.h"

HRESULT gameScene::init()
{
	//// EffImgSet
	EFFMANAGER->addEffect("tileDes", "image/inGameImg/EFF/Tile_Des.bmp", 512, 384, 512 / 4, 384 / 3, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("MousePointEFF", "image/inGameImg/EFF/EXP_EFF_1.bmp", 238, 34, 238 / 7, 34 / 1, 15, 100, 0.0f, true);
	EFFMANAGER->addEffect("Enemy_Die", "image/inGameImg/EFF/Enemy_Die.bmp", 512, 384, 512 / 4, 384 / 3, 15, 5, 0.0f, true);
	//EFFMANAGER->addEffect("Enemy_Eat", "image/inGameImg/EFF/EAT_EFF.bmp", 238, 34, 238 / 7, 34 / 1, 30, 10, 0.0f, true);
	//EFFMANAGER->addEffect("Enemy_Eat_R", "image/inGameImg/EFF/EAT_EFF_R.bmp", 238, 34, 238 / 7, 34 / 1, 30, 10, 0.0f, true);



	//// UiImgSet
	IMAGEMANAGER->addImage("mousePoint", "image/inGameImg/UI/MousePoint.bmp", 112, 49, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("NumberPont", "image/inGameImg/UI/NumberPont.bmp", 80, 60, 10, 6, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_0", "image/inGameImg/EFF/TileSet_Terrain_DesSet0.bmp", 12, 11, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_1", "image/inGameImg/EFF/TileSet_Terrain_DesSet1.bmp", 16, 16, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_2", "image/inGameImg/EFF/TileSet_Terrain_DesSet2.bmp", 23, 22, 1, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("Item_Jewel", "image/inGameImg/EFF/Item_JewelSet.bmp", 75, 105, 5, 7, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TilePopup", "image/inGameImg/UI/TilePopup.bmp", 157, 95, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("EnemyPopup", "image/inGameImg/UI/EnemyPopup.bmp", 157, 95, 1, 1, true, RGB(255, 0, 255));


	//// enemyImgSet
	//뼈다귀
	/*IMAGEMANAGER->addImage("Skeleton_00_L", "image/inGameImg/ENEMY/Skeleton_0_L.bmp", 48, 16, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Skeleton_00_R", "image/inGameImg/ENEMY/Skeleton_0_R.bmp", 48, 16, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Skeleton_00_D", "image/inGameImg/ENEMY/Skeleton_0_D.bmp", 16, 16, 1, 1, true, RGB(255, 0, 255));*/
	//슬라임
	IMAGEMANAGER->addImage("Slime_00_R", "image/inGameImg/ENEMY/Slime/Slime_0_RIGHT.bmp",442, 20, 13, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Slime_00_L", "image/inGameImg/ENEMY/Slime/Slime_0_LEFT.bmp", 442, 20, 13, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Slime_00_RA", "image/inGameImg/ENEMY/Slime/Slime_0_RIGHT_ATK.bmp", 476, 21, 14, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Slime_00_LA", "image/inGameImg/ENEMY/Slime/Slime_0_LEFT_ATK.bmp", 476, 21, 14, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Slime_00_Dead", "image/inGameImg/ENEMY/Slime/Slime_0_Dead.bmp", 340, 20, 10, 1, true, RGB(255, 0, 255));
	//슬라임 꽃
	IMAGEMANAGER->addImage("Flower_00_R", "image/inGameImg/ENEMY/Flower/Flower_0_RIGHT.bmp", 192, 19, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Flower_00_L", "image/inGameImg/ENEMY/Flower/Flower_0_LEFT.bmp", 192, 19, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Flower_00_RA", "image/inGameImg/ENEMY/SFlowerlime/Flower_0_RIGHT_ATK.bmp", 192, 19, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Flower_00_LA", "image/inGameImg/ENEMY/Flower/Flower_0_LEFT_ATK.bmp", 192, 19, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Flower_00_Dead", "image/inGameImg/ENEMY/Flower/Flower_0_Dead.bmp", 360, 19, 15, 1, true, RGB(255, 0, 255));
	//볼레레
	IMAGEMANAGER->addImage("Bug_00_R", "image/inGameImg/ENEMY/Bug/Bug_0_RIGHT.bmp", 192, 18, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_L", "image/inGameImg/ENEMY/Bug/Bug_0_LEFT.bmp", 192, 18, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_RA", "image/inGameImg/ENEMY/Bug/Bug_0_RIGHT_ATK.bmp", 288, 18, 12, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_LA", "image/inGameImg/ENEMY/Bug/Bug_0_LEFT_ATK.bmp", 288, 18, 12, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_Dead", "image/inGameImg/ENEMY/Bug/Bug_0_DEAD.bmp", 219, 30, 9, 1, true, RGB(255, 0, 255));
	//릴리
	IMAGEMANAGER->addImage("Lili_00_R", "image/inGameImg/ENEMY/Lili/Lili_0_RIGHT.bmp", 264, 30, 11, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_L", "image/inGameImg/ENEMY/Lili/Lili_0_LEFT.bmp", 264, 30, 11, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_RA", "image/inGameImg/ENEMY/Lili/Lili_0_RIGHT_ATK.bmp", 241, 30, 10, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_LA", "image/inGameImg/ENEMY/Lili/Lili_0_LEFT_ATK.bmp", 241, 30, 10, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_Dead", "image/inGameImg/ENEMY/Lili/Lili_0_DEAD.bmp", 255, 31, 15, 1, true, RGB(255, 0, 255));

	//// heroImgSet
	IMAGEMANAGER->addImage("hero_00_U", "image/inGameImg/HERO/hero_0/hero_0_UP.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_R", "image/inGameImg/HERO/hero_0/hero_0_RIGHT.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_D", "image/inGameImg/HERO/hero_0/hero_0_DOWN.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_L", "image/inGameImg/HERO/hero_0/hero_0_LEFT.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));


	//// player
	IMAGEMANAGER->addImage("player_00", "image/inGameImg/HERO/Player.bmp", 72, 25, 4, 1, true, RGB(255, 0, 255));


	//// 마우스 감추기
	ShowCursor(FALSE);
	m_imgMouseCur = IMAGEMANAGER->addImage("MouseCursor", "image/inGameImg/UI/MousePoint.bmp", 112, 49, 2, 1, true, RGB(255, 0, 255));

	RECT rc = RectMake(WINS_PRISON_MIN_X, WINS_PRISON_MIN_Y, WINS_PRISON_MAX_X, WINS_PRISON_MAX_Y);
	ClipCursor(&rc);
	//// UI 매니저 동적할당
	m_pUiMag = new uiManager;

	//// 맵타일 우선 동적할당
	m_MapTile = new tileMap;

	//// Enemy 매니저 동적할당
	m_pEnemyMag = new enemyManager;
	m_pEnemyMag->init(m_MapTile, m_pUiMag);

	//// Hero 매니저 동적할당
	m_pHeroMag = new heroManager;
	m_pHeroMag->init(m_MapTile);

	//// 플레이어 셋팅
	m_tPlayer = new PlayerInfo;
	m_tPlayer->t_TileDesEne = 100;

	m_MapTile->init(32, 42, m_tPlayer, m_pUiMag, m_pEnemyMag, m_pHeroMag);
	CAMERA->setCamPosY(CAMERA_MAX_Y);

	m_imgTopBg = IMAGEMANAGER->addImage("inGameTopBG", "image/inGameImg/BG/InGame_Top_Bg.bmp", m_MapTile->gettileMaxValueX(), TOP_IMG_SIZE_Y);

	return S_OK;
}

void gameScene::release()
{
	SAFE_DELETE(m_MapTile);
	SAFE_DELETE(m_tPlayer);
	SAFE_DELETE(m_pUiMag);
	SAFE_DELETE(m_pEnemyMag);
	SAFE_DELETE(m_pHeroMag);
}

void gameScene::update()
{
	m_pEnemyMag->update();
	m_pHeroMag->update();
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
	m_pEnemyMag->render(hdc);
	m_pHeroMag->render(hdc);
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
