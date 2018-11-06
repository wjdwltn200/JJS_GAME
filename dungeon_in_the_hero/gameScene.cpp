#include "stdafx.h"
#include "gameScene.h"
#include "enemyManager.h"
#include "heroManager.h"
#include "bulletManager.h"
#include "bullet.h"
#include "hero.h"

HRESULT gameScene::init()
{
	//// EffImgSet
	EFFMANAGER->addEffect("tileDes", "image/inGameImg/EFF/Tile_Des.bmp", 512, 384, 512 / 4, 384 / 3, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("MousePointEFF", "image/inGameImg/EFF/EXP_EFF_1.bmp", 238, 34, 238 / 7, 34 / 1, 15, 100, 0.0f, true);
	EFFMANAGER->addEffect("Hit_Eff_0", "image/inGameImg/EFF/Hit_EFF_0.bmp", 133, 34, 133 / 4, 34/ 1, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("Mana_Eat_0", "image/inGameImg/EFF/MANA_EAT_EFF_0.bmp", 252, 36, 252/ 7, 36 / 1, 10, 10, 0.0f, true);
	EFFMANAGER->addEffect("Mana_Eat_1", "image/inGameImg/EFF/MANA_EAT_EFF_1.bmp", 252, 36, 252 / 7, 36 / 1, 10, 10, 0.0f, true);


	// 노란색
	EFFMANAGER->addEffect("EXP_EFF_1", "image/inGameImg/EFF/EXP_EFF_1.bmp", 238, 34, 238 / 7, 34 / 1, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("EXP_EFF_2", "image/inGameImg/EFF/EXP_EFF_2.bmp", 224, 32, 224 / 7, 32 / 1, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("EXP_EFF_3", "image/inGameImg/EFF/EXP_EFF_3.bmp", 210, 24, 210 / 7, 24 / 1, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("EXP_EFF_4", "image/inGameImg/EFF/EXP_EFF_4.bmp", 238, 30, 238 / 7, 30 / 1, 15, 5, 0.0f, true);
	// 파란색
	EFFMANAGER->addEffect("EXP_EFF_5", "image/inGameImg/EFF/EXP_EFF_5.bmp", 238, 33, 238 / 7, 33 / 1, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("EXP_EFF_6", "image/inGameImg/EFF/EXP_EFF_6.bmp", 238, 31, 238 / 7, 31 / 1, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("EXP_EFF_7", "image/inGameImg/EFF/EXP_EFF_7.bmp", 168, 23, 168 / 7, 23 / 1, 15, 5, 0.0f, true);
	EFFMANAGER->addEffect("EXP_EFF_8", "image/inGameImg/EFF/EXP_EFF_8.bmp", 112, 16, 112 / 7, 16 / 1, 15, 5, 0.0f, true);


	EFFMANAGER->addEffect("Enemy_Eat", "image/inGameImg/EFF/EAT_EFF.bmp", 238, 34, 238 / 7, 34 / 1, 30, 10, 0.0f, true);
	//EFFMANAGER->addEffect("Enemy_Eat_R", "image/inGameImg/EFF/EAT_EFF_R.bmp", 238, 34, 238 / 7, 34 / 1, 30, 10, 0.0f, true);

	//// UiImgSet
	IMAGEMANAGER->addImage("mousePoint", "image/inGameImg/UI/MousePoint.bmp", 112, 49, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("NumberPont", "image/inGameImg/UI/NumberPont.bmp", 80, 60, 10, 6, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_0", "image/inGameImg/EFF/TileSet_Terrain_DesSet0.bmp", 12, 11, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_1", "image/inGameImg/EFF/TileSet_Terrain_DesSet1.bmp", 16, 16, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileSet_Terrain_DesSet_2", "image/inGameImg/EFF/TileSet_Terrain_DesSet2.bmp", 23, 22, 1, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("Item_Jewel", "image/inGameImg/EFF/Item_JewelSet.bmp", 75, 105, 5, 7, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TilePopup", "image/inGameImg/UI/TilePopup.bmp", 157, 95, 1, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("EnemyPopup", "image/inGameImg/UI/EnemyPopup.bmp", 170, 110, 1, 1, true, RGB(255, 0, 255));

	//// BuuletImgSet
	IMAGEMANAGER->addImage("Bullet_0", "image/inGameImg/EFF/bullet_0.bmp", 104, 26, 4, 1, true, RGB(255, 0, 255));



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
	IMAGEMANAGER->addImage("Flower_00_RA", "image/inGameImg/ENEMY/Flower/Flower_0_RIGHT_ATK.bmp", 192, 19, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Flower_00_LA", "image/inGameImg/ENEMY/Flower/Flower_0_LEFT_ATK.bmp", 192, 19, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Flower_00_Dead", "image/inGameImg/ENEMY/Flower/Flower_0_Dead.bmp", 360, 19, 15, 1, true, RGB(255, 0, 255));
	//슬라임 꽃 변형
	IMAGEMANAGER->addImage("FlowerV2_00_R", "image/inGameImg/ENEMY/FlowerV2/FlowerV2_0_RIGHT.bmp", 288, 27, 12, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("FlowerV2_00_L", "image/inGameImg/ENEMY/FlowerV2/FlowerV2_0_LEFT.bmp", 288, 27, 12, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("FlowerV2_00_RA", "image/inGameImg/ENEMY/FlowerV2/FlowerV2_0_RIGHT_ATK.bmp", 380, 31, 10, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("FlowerV2_00_LA", "image/inGameImg/ENEMY/FlowerV2/FlowerV2_0_LEFT_ATK.bmp", 380, 31, 10, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("FlowerV2_00_Dead", "image/inGameImg/ENEMY/FlowerV2/FlowerV2_0_Dead.bmp", 1008, 45, 18, 1, true, RGB(255, 0, 255));
	//볼레레
	IMAGEMANAGER->addImage("Bug_00_R", "image/inGameImg/ENEMY/Bug/Bug_0_RIGHT.bmp", 192, 18, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_L", "image/inGameImg/ENEMY/Bug/Bug_0_LEFT.bmp", 192, 18, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_RA", "image/inGameImg/ENEMY/Bug/Bug_0_RIGHT_ATK.bmp", 288, 18, 12, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_LA", "image/inGameImg/ENEMY/Bug/Bug_0_LEFT_ATK.bmp", 288, 18, 12, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Bug_00_Dead", "image/inGameImg/ENEMY/Bug/Bug_0_DEAD.bmp", 219, 30, 9, 1, true, RGB(255, 0, 255));
	//볼레레 변형
	IMAGEMANAGER->addImage("BugV2_00_R", "image/inGameImg/ENEMY/BugV2/BugV2_0_RIGHT.bmp", 272, 36, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BugV2_00_L", "image/inGameImg/ENEMY/BugV2/BugV2_0_LEFT.bmp", 272, 36, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BugV2_00_RA", "image/inGameImg/ENEMY/BugV2/BugV2_0_RIGHT_ATK.bmp", 136, 144, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BugV2_00_LA", "image/inGameImg/ENEMY/BugV2/BugV2_0_LEFT_ATK.bmp", 136, 144, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BugV2_00_Dead", "image/inGameImg/ENEMY/BugV2/BugV2_0_DEAD.bmp", 170, 72, 5, 2, true, RGB(255, 0, 255));
	//리자드맨
	IMAGEMANAGER->addImage("Lizardman_00_R", "image/inGameImg/ENEMY/Lizardman/Lizardman_0_RIGHT.bmp", 160, 104, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lizardman_00_L", "image/inGameImg/ENEMY/Lizardman/Lizardman_0_LEFT.bmp", 160, 104, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lizardman_00_RA", "image/inGameImg/ENEMY/Lizardman/Lizardman_0_RIGHT_ATK.bmp", 287, 24, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lizardman_00_LA", "image/inGameImg/ENEMY/Lizardman/Lizardman_0_LEFT_ATK.bmp", 287, 24, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lizardman_00_Dead", "image/inGameImg/ENEMY/Lizardman/Lizardman_0_DEAD.bmp", 164, 123, 4, 3, true, RGB(255, 0, 255));
	//릴리
	IMAGEMANAGER->addImage("Lili_00_R", "image/inGameImg/ENEMY/Lili/Lili_0_RIGHT.bmp", 264, 30, 11, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_L", "image/inGameImg/ENEMY/Lili/Lili_0_LEFT.bmp", 264, 30, 11, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_RA", "image/inGameImg/ENEMY/Lili/Lili_0_RIGHT_ATK.bmp", 241, 30, 10, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_LA", "image/inGameImg/ENEMY/Lili/Lili_0_LEFT_ATK.bmp", 241, 30, 10, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Lili_00_Dead", "image/inGameImg/ENEMY/Lili/Lili_0_DEAD.bmp", 255, 31, 15, 1, true, RGB(255, 0, 255));

	//데몬
	IMAGEMANAGER->addImage("Demon_00_R", "image/inGameImg/ENEMY/Demon/Demon_0_RIGHT.bmp", 880, 39, 22, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Demon_00_L", "image/inGameImg/ENEMY/Demon/Demon_0_LEFT.bmp", 880, 39, 22, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Demon_00_RA", "image/inGameImg/ENEMY/Demon/Demon_0_RIGHT_ATK.bmp", 741, 38, 13, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Demon_00_LA", "image/inGameImg/ENEMY/Demon/Demon_0_LEFT_ATK.bmp", 741, 38, 13, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Demon_00_Dead", "image/inGameImg/ENEMY/Demon/Demon_0_DEAD.bmp", 201, 272, 4, 8, true, RGB(255, 0, 255));

	//// heroImgSet
	IMAGEMANAGER->addImage("hero_00_U", "image/inGameImg/HERO/hero_0/hero_0_UP.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_R", "image/inGameImg/HERO/hero_0/hero_0_RIGHT.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_D", "image/inGameImg/HERO/hero_0/hero_0_DOWN.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_L", "image/inGameImg/HERO/hero_0/hero_0_LEFT.bmp", 36, 18, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_Dead", "image/inGameImg/HERO/hero_0/hero_0_Dead.bmp", 72, 18, 4, 1, true, RGB(255, 0, 255));


	IMAGEMANAGER->addImage("hero_00_UA", "image/inGameImg/HERO/hero_0/hero_0_UP_ATK.bmp", 72, 37, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_RA", "image/inGameImg/HERO/hero_0/hero_0_RIGHT_ATK.bmp", 140, 18, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_DA", "image/inGameImg/HERO/hero_0/hero_0_DOWN_ATK.bmp", 72, 37, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_00_LA", "image/inGameImg/HERO/hero_0/hero_0_LEFT_ATK.bmp", 140, 18, 4, 1, true, RGB(255, 0, 255));


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

	//// bullet 매니저 동적할당
	m_pBulletMag = new bulletManager;
	m_pBulletMag->init();

	//// Enemy 매니저 동적할당
	m_pEnemyMag = new enemyManager;
	m_pEnemyMag->init(m_MapTile, m_pUiMag, m_pBulletMag);

	//// Hero 매니저 동적할당
	m_pHeroMag = new heroManager;
	m_pHeroMag->init(m_MapTile);

	//// 플레이어 셋팅
	m_tPlayer = new PlayerInfo;
	m_tPlayer->t_TileDesEne = 100;

	m_MapTile->init(32, 64, m_tPlayer, m_pUiMag, m_pEnemyMag, m_pHeroMag);
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
	SAFE_DELETE(m_pBulletMag);
}

void gameScene::update()
{
	m_pEnemyMag->update();
	m_pHeroMag->update();
	m_MapTile->update();
	m_pBulletMag->update();
	RectCol();
	CAMERA->update(0.0f, CAMERA_MAX_Y, m_MapTile->gettileMaxValueX() - WINSIZEX, m_MapTile->gettileMaxValueY() - WINSIZEY);
	m_pUiMag->update();
	OBJECTMANAGER->update();
	EFFMANAGER->update();
}

void gameScene::render(HDC hdc)
{
	m_imgTopBg->render(hdc, 0 - CAMERA->getCamPosX(), 0 - CAMERA->getCamPosY() + CAMERA_MAX_Y);
	m_MapTile->render(hdc);
	m_pHeroMag->render(hdc);
	m_pEnemyMag->render(hdc);
	m_pBulletMag->render(hdc);
	EFFMANAGER->render(hdc);
	OBJECTMANAGER->render(hdc);
	m_pUiMag->render(hdc);
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

void gameScene::RectCol()
{
	RECT temp_rc;

	// bulletMag 정보
	std::vector<bullet*> vBullet = m_pBulletMag->getVecBullet();
	std::vector<bullet*>::iterator iterBullet;
	// heroMag 정보
	std::vector<hero*> vHero = m_pHeroMag->getVecHero();
	std::vector<hero*>::iterator iterHero;
	// enemyMag 정보
	std::vector<enemy*> vEnemy = m_pEnemyMag->getVecEnemy();
	std::vector<enemy*>::iterator iterEnemy;

	for (iterBullet = vBullet.begin(); iterBullet != vBullet.end(); iterBullet++) // bulletVector;
	{
		if (!(*iterBullet)->gettBulletInfo().t_isAilve) continue;
		RECT tempBullet = (*iterBullet)->gettBulletInfo().t_rc;

		for (iterHero = vHero.begin(); iterHero != vHero.end(); iterHero++)
		{
			RECT tempHero = (*iterHero)->gettHeroData().t_rc;

			// enemy의 Bullet이 Hero와 충돌
			if ((*iterBullet)->gettBulletInfo().t_isAilve &&
				((*iterBullet)->gettBulletInfo().t_master == tagMaster::Enemy &&
				IntersectRect(&temp_rc, &tempBullet, &tempHero)))
			{
				(*iterHero)->setDamgePoint((*iterBullet)->gettBulletInfo().t_atkPoint);
				(*iterBullet)->setIsAlive(false);
				HitEff(tempHero.left, tempHero.top);
			}
		}
	}
}

void gameScene::HitEff(int x, int y)
{
	switch (RANDOM->getInt(4))
	{
	case 0:
		EFFMANAGER->play("EXP_EFF_5",
			x + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)),
			y + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)));
		break;
	case 1:
		EFFMANAGER->play("EXP_EFF_6",
			x + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)),
			y + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)));
		break;
	case 2:
		EFFMANAGER->play("EXP_EFF_7",
			x + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)),
			y + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)));
		break;
	case 3:
		EFFMANAGER->play("EXP_EFF_8",
			x + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)),
			y + (TILE_SIZE / 2) + (RANDOM->getFromIntTo(-10, 10)));
		break;
	default:
		break;
	}
}

gameScene::gameScene()
{
}


gameScene::~gameScene()
{
}
