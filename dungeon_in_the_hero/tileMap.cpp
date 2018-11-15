#include "stdafx.h"
#include "tileMap.h"
#include "uiPopup.h"
#include "uiManager.h"
#include "enemyManager.h"
#include "heroManager.h"
#include "animation.h"
#include "enemy.h"
#include "overlord.h"

HRESULT tileMap::init(int tileX, int tileY,
	PlayerInfo * playerData, uiManager * uiMagData,
	enemyManager * pEnemyMag, heroManager * pHeroMag,
	overlord * pOverlord)
{
	//tileMap 초기화
	memset(&m_tileset, 0, sizeof(m_tileset));

	//// Enemy & Hero 매니저 주소 초기화
	m_pEnemyMag = pEnemyMag;
	m_pHeroMag = pHeroMag;
	m_pOverlord = pOverlord;

	//// UI 매니저 주소 초기화
	m_pUiMag = uiMagData;
	m_pTilePopup = m_pUiMag->addPopup(NULL, NULL, NULL, NULL);

	//// 플레이어 데이터 주소 저장
	m_pPlayer = playerData;

	//// 마우스 초기화
	m_isTileClick = false;
	
	m_tileDesCurr = 0;
	m_tileDesCount = 0;
	m_isHeroStart = false;
	m_HeroStartDaley = 0;
	m_gameState = eGameState::GameReady;
	m_stageTimer = 5 * (60 * 60);
	m_stageValue = 1;
	m_fStartPopup = 10.0f;


	m_startTileX = 0;
	m_startTileY = 0;

	if (m_gameState == eGameState::GameReady &&
		!SOUNDMANAGER->isPlaying("Sound/BGM/BGM_GameReady.wav"))
		SOUNDMANAGER->play("Sound/BGM/BGM_GameReady.wav");

	//// 타일 초기화 셋팅
	m_tileDesDaley = 0;
	m_tileSizeX = tileX;
	m_tileSizeY = tileY;
	m_tileSizeMaxX = tileX * TILE_SIZE;
	m_tileSizeMaxY = tileY * TILE_SIZE;
	IMAGEMANAGER->addImage("TileSet", "image/inGameImg/TILE/TileSet_Terrain.bmp", 160, 384, 5, 12, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hero_Start_Door", "image/inGameImg/HERO/Hero_start_tile.bmp", 66, 32, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileShadow", "image/inGameImg/TILE/TileSet_Terrain_Shadow.bmp", 6, 32);

	//// list 초기화
	for (int i = 0; i < 10; i++)
	{
		m_vecHeroList.push_back(nullptr);
	}

	//// 타일 초기화
	for (int x = 0; x < m_tileSizeX; x++)
	{
		for (int y = 0; y < m_tileSizeY; y++)
		{
			m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
			m_tileset[x * m_tileSizeY + y].t_rc = RectMakeCenter(NULL, NULL, TILE_SIZE, TILE_SIZE);
			m_tileset[x * m_tileSizeY + y].t_isStart = false;
			m_tileset[x * m_tileSizeY + y].t_isShaking = false;
			m_tileset[x * m_tileSizeY + y].t_ShakingCount = 0;
			m_tileset[x * m_tileSizeY + y].t_isAlive = true;
			m_tileset[x * m_tileSizeY + y].t_tierValue = 0;
			m_tileset[x * m_tileSizeY + y].t_rc = RectMake(
				x * TILE_SIZE - CAMERA->getCamPosX(),
				y * TILE_SIZE - CAMERA->getCamPosY(),
				TILE_SIZE,
				TILE_SIZE
			);
			m_tileset[x * m_tileSizeY + y].t_setX = x;
			m_tileset[x * m_tileSizeY + y].t_setY = y;
			for (int i = 0; i < ENEMY_INFO_ARR; i++)
			{
				m_tileset[x * m_tileSizeY + y].t_enemyInfo[i] = nullptr;
			}
			for (int i = 0; i < HERO_INFO_ARR; i++)
			{
				m_tileset[x * m_tileSizeY + y].t_heroInfo[i] = nullptr;
			}
			m_tileset[x * m_tileSizeY + y].t_isOverlord = false;

			// 깊이에 따라 마나 세팅 추가
			int tempY = (m_tileSizeY / 4);
			if (y <= (tempY * 1))
			{
				m_tileset[x * m_tileSizeY + y].t_ManaValue = RANDOM->getFromIntTo(2, 4);
			}
			else if (y <= (tempY * 2))
			{
				m_tileset[x * m_tileSizeY + y].t_ManaValue = RANDOM->getFromIntTo(3, 5);
			}
			else if (y <= (tempY * 3))
			{
				m_tileset[x * m_tileSizeY + y].t_ManaValue = RANDOM->getFromIntTo(3, 6);
			}
			else if (y <= (tempY * 4))
			{
				m_tileset[x * m_tileSizeY + y].t_ManaValue = RANDOM->getFromIntTo(3, 7);
			}

			int TempRandom = RANDOM->getFromIntTo(1, 100);

			if (x == TILE_OVERLORD_X && y == TILE_OVERLORD_Y)
				m_tileset[x * m_tileSizeY + y].t_isOverlord = true;
			m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");


			if (x == ((m_tileSizeX / 2) - 1) && y == 0)
			{
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("hero_Start_Door"); 
				m_tileset[x * m_tileSizeY + y].t_setImg = 0;
				m_tileset[x * m_tileSizeY + y].t_isStart = true;
				m_tileset[x * m_tileSizeY + y].t_isNotDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::START;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
				
				m_startTileX = x;
				m_startTileY = y;

			}
			else if (x <= m_tileSizeX && y == 0)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 3);
				m_tileset[x * m_tileSizeY + y].t_isNotDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::GROUND;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}
			else if (x <= m_tileSizeX && (y == 1 && x != ((m_tileSizeX / 2) - 1)) || x == 0 || x == m_tileSizeX - 1 || y == m_tileSizeY - 1)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 3);
				m_tileset[x * m_tileSizeY + y].t_isNotDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::TOP;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}

			else if ((x + 1) == (m_tileSizeX / 2) && y <= TILE_SETNON)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 4);
				m_tileset[x * m_tileSizeY + y].t_isAlive = false;
				m_tileset[x * m_tileSizeY + y].t_isNotDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::RAND;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}
			else if (TempRandom < TILE_NON_BLOCK && y > TILE_NON_BLOCK_SET_Y)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 4);
				m_tileset[x * m_tileSizeY + y].t_isNotDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::BLOCK_NON;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}
			else
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 4);
				m_tileset[x * m_tileSizeY + y].t_isNotDes = false;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::BLOCK;
			}

			
		}
	}

	//// 카메라 최초 위치 셋팅
	CAMERA->setCamPosX((m_tileSizeMaxX - WINSIZEX - WINSTARTX) / 2);
	return S_OK;
}

void tileMap::release()
{
	SAFE_DELETE(m_pEnemyMag);
}

void tileMap::update()
{
	if (m_gameState == eGameState::GameOver) return;

	tileDesEneChag();
	heroStartSys();
	keyInput();

	if (m_tileDesDaley <= 0)
	{
		m_isTileClick = false;
	}
	else
	{
		m_tileDesDaley--;
	}
	for (int x = 0; x < m_tileSizeX; x++)
	{
		for (int y = 0; y < m_tileSizeY; y++)
		{
			//// 타일 manaValue 에 맞춰 tier 세팅
			tierSet(x, y);

			//// 타일 흔들기
			if (m_tileset[x * m_tileSizeY + y].t_ShakingCount >= TILE_SHAKING_COUNT)
			{
				m_tileset[x * m_tileSizeY + y].t_ShakingCount = 0;
				m_tileset[x * m_tileSizeY + y].t_isShaking = false;
			}

			if (m_tileset[x * m_tileSizeY + y].t_isShaking)
			{
				m_tileset[x * m_tileSizeY + y].t_ShakingCount++;
			}

			//// 타일 상호작용
			//// 타일 팝업 출력
			RECT tempRc;
			// 마왕 내려놓기
			if (((m_gameState == eGameState::GameGetSet)) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) &&
				m_pOverlord->getIsSetting() &&
				(IntersectRect(&tempRc, &m_tileset[x * m_tileSizeY + y].t_rc, &g_MouseRc) &&
				(tileCheck(x, y))))
			{
				switch (RANDOM->getFromIntTo(1, 5))
				{
				case 1:
					gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "이곳의 제 자리군요?", eTxtBoxColor::Green);
					break;
				case 2:
					gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "그럼 차나 한잔?", eTxtBoxColor::Green);
					break;
				case 3:
					gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "후후 멍청한 용사들!", eTxtBoxColor::Green);
					break;
				case 4:
					gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "아늑한 곳이에요~", eTxtBoxColor::Green);
					break;
				case 5:
					gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "그럼 기다려볼까요?", eTxtBoxColor::Green);
					break;
				}

				m_pOverlord->setTileXY(
					x,
					y,
					m_tileset[x * m_tileSizeY + y].t_rc.left,
					m_tileset[x * m_tileSizeY + y].t_rc.top);

				SOUNDMANAGER->stop("Sound/BGM/BGM_UnStart.wav");
				m_gameState = eGameState::GameGo;
			}


			if (!m_pEnemyMag->getIsEnemyInfoPopup() &&
				KEYMANAGER->isStayKeyDown(VK_RBUTTON) &&
				(IntersectRect(&tempRc, &m_tileset[x * m_tileSizeY + y].t_rc, &g_MouseRc)))
			{
				tileSetTxt(m_tileset[x * m_tileSizeY + y].t_enumType, (x * m_tileSizeY + y));
				m_pTilePopup->setImg(IMAGEMANAGER->findImage("TilePopup"));
				m_pTilePopup->setDaley(POPUP_COUNT);
				m_pTilePopup->setIsAilve(true);
				m_pTilePopup->setXY(m_tileset[x * m_tileSizeY + y].t_rc.left + TILE_SIZE / 2, m_tileset[x * m_tileSizeY + y].t_rc.top + TILE_SIZE / 2 - TILE_POPUP_Y_SIZE);
			}

			//// 타일 파괴
			if ((m_gameState != eGameState::GameGetSet) &&
				m_tileDesDaley <= 0 &&
				!m_tileset[x * m_tileSizeY + y].t_isNotDes &&
				m_pPlayer->t_TileDesEne > 0 &&
				!m_isTileClick &&
				m_tileset[x * m_tileSizeY + y].t_isAlive &&
				(tileCheck(x, y)) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) &&
				(IntersectRect(&tempRc, &m_tileset[x * m_tileSizeY + y].t_rc, &g_MouseRc)))
			{
				//// tile 파괴 함수
				tileDesSys(x * m_tileSizeY + y, x, y);
			}
			//// 에너지 부족 타일 흔들기
			else if ((
				(m_tileset[x * m_tileSizeY + y].t_isNotDes || m_pPlayer->t_TileDesEne <= 0) &&
				!m_tileset[x * m_tileSizeY + y].t_isShaking &&
				!m_isTileClick && (y > 0) &&
				m_tileset[x * m_tileSizeY + y].t_isAlive &&
				(tileCheck(x, y)) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) &&
				(IntersectRect(&tempRc, &m_tileset[x * m_tileSizeY + y].t_rc, &g_MouseRc))))
			{
				m_tileset[x * m_tileSizeY + y].t_isShaking = true;
			}
			else if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
			{
				m_isTileClick = false;
				m_tileDesDaley = 0;
			}


		}
	}
}

void tileMap::render(HDC hdc)
{
	int TestTileNum = 0;
	//// txt 미리 셋팅
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 0, 255));

	for (int x = 0; x < m_tileSizeX; x++)
	{
		for (int y = 0; y < m_tileSizeY; y++)
		{
			m_tileset[x * m_tileSizeY + y].t_rc = RectMake(
				x * TILE_SIZE - CAMERA->getCamPosX(),
				y * TILE_SIZE - CAMERA->getCamPosY(),
				TILE_SIZE,
				TILE_SIZE
			);

			//// 타일 상태에 따른 렌더링
			
			if (m_isHeroStart && (m_tileset[x * m_tileSizeY + y].t_enumType == tagTileType::START))
				heroStart(x * m_tileSizeY + y);

			if (MY_UTIL::screenRender(m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top)) continue;

			if (m_tileset[x * m_tileSizeY + y].t_enumType == tagTileType::START)
			{
				m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, 1, 0, TILE_SCALE, false);

				if (m_gameState == eGameState::GameReady)
				{
					m_fStartPopup -= 0.5f;
					if (m_fStartPopup <= 0.0f)
						m_fStartPopup = 10.0f;
					
					IMAGEMANAGER->findImage("GameStart")->render(hdc,
						m_tileset[x * m_tileSizeY + y].t_rc.left - (IMAGEMANAGER->findImage("GameStart")->getWidth() / 2) + (TILE_SIZE / 2),
						m_tileset[x * m_tileSizeY + y].t_rc.top - (IMAGEMANAGER->findImage("GameStart")->getHeight() / 2) - (TILE_SIZE) - 20.0f + m_fStartPopup);
					m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, 0, 0, TILE_SCALE, false);
				}
			}

			if (m_tileset[x * m_tileSizeY + y].t_enumType == tagTileType::GROUND)
				m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, eTileLv::tGround, TILE_SCALE, false);

			if (m_tileset[x * m_tileSizeY + y].t_enumType == tagTileType::TOP)
				m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, eTileLv::tWall, TILE_SCALE, false);

			if (m_tileset[x * m_tileSizeY + y].t_isAlive)
			{
				if (m_tileset[x * m_tileSizeY + y].t_isShaking)
				{
					switch (m_tileset[x * m_tileSizeY + y].t_enumType)
					{
					case tagTileType::BLOCK:
						tileImgSet(x, y, hdc, true);
						break;
					case tagTileType::BLOCK_NON:
						m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, eTileLv::tBrick, TILE_SCALE, false);
						break;
					}
				}
				else
				{
					switch (m_tileset[x * m_tileSizeY + y].t_enumType)
					{
					case tagTileType::BLOCK:
						tileImgSet(x, y, hdc);
						break;
					case tagTileType::BLOCK_NON:
						m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, eTileLv::tBrick, TILE_SCALE, false);
						break;
					}
				}
			}
			else //// 파괴된 타일
			{
				m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, eTileLv::tFloor, TILE_SCALE, false);
			}
			
			//// 타일 그림자 생성
			if (!m_tileset[x * m_tileSizeY + y].t_isAlive && m_tileset[((x - 1) * m_tileSizeY + y)].t_isAlive)
				IMAGEMANAGER->findImage("TileShadow")->alphaRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, TILE_ALPHA_VALUE);


			//// 디버깅 정보
			if (g_saveData.gisTest)
			{
				Rectangle(hdc,
					m_tileset[x * m_tileSizeY + y].t_rc.left,
					m_tileset[x * m_tileSizeY + y].t_rc.top,
					m_tileset[x * m_tileSizeY + y].t_rc.right,
					m_tileset[x * m_tileSizeY + y].t_rc.bottom
				);

				//// 타일정보 디버깅 정보
				//sprintf_s(szText, "%d,%d", m_tileset[x * m_tileSizeY + y].t_setX, m_tileset[x * m_tileSizeY + y].t_setY);
				//TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
				sprintf_s(szText, "%d", (m_tileset[x * m_tileSizeY + y].t_enemyInfo[0]));
				TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
				//sprintf_s(szText, "%d", TestTileNum);
				//TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
				//sprintf_s(szText, "%d,%d", m_tileset[x * m_tileSizeY + y].t_setX, m_tileset[x * m_tileSizeY + y].t_setY);
				//TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top + 12.0f, szText, strlen(szText));
				//sprintf_s(szText, "%d, %d", m_tileset[x * m_tileSizeY + y].t_isAlive, m_tileset[x * m_tileSizeY + y].t_isShaking);
				//TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top + 12.0f, szText, strlen(szText));
			}
			TestTileNum++;
		}
	}


}

void tileMap::keyInput()
{
	//// 마우스 포인터 기준 카메라 이동
	if (g_ptMouse.x <= (WINSIZEX - WINSIZEX) + 5)
		CAMERA->setCamPosX(CAMERA->getCamPosX() - 10.0f);
	if (g_ptMouse.x >= (WINSIZEX) - 5)
		CAMERA->setCamPosX(CAMERA->getCamPosX() + 10.0f);
	if (g_ptMouse.y <= (WINSIZEY - WINSIZEY) + 5)
		CAMERA->setCamPosY(CAMERA->getCamPosY() - 10.0f);
	if (g_ptMouse.y >= (WINSIZEY) - 5)
		CAMERA->setCamPosY(CAMERA->getCamPosY() + 10.0f);

	//// 키 입력 기준 카메라 이동
	if (KEYMANAGER->isStayKeyDown('W'))
		CAMERA->setCamPosY(CAMERA->getCamPosY() - 10.0f);
	if (KEYMANAGER->isStayKeyDown('S'))
		CAMERA->setCamPosY(CAMERA->getCamPosY() + 10.0f);
	if (KEYMANAGER->isStayKeyDown('A'))
		CAMERA->setCamPosX(CAMERA->getCamPosX() - 10.0f);
	if (KEYMANAGER->isStayKeyDown('D'))
		CAMERA->setCamPosX(CAMERA->getCamPosX() + 10.0f);
}

void tileMap::heroStartSys()
{
	if (m_stageTimer > 0)
		m_stageTimer--;

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		m_stageTimer = 0;

	if (m_stageTimer <= 0 && (m_gameState == eGameState::GameReady))
	{
		switch (RANDOM->getFromIntTo(1, 5))
		{
		case 1:
			gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "꺅, 놀랬잖아!", eTxtBoxColor::Red);
			break;
		case 2:
			gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "저 문이 얼마짜린데!", eTxtBoxColor::Red);
			break;
		case 3:
			gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "용사가 나타났어요!!", eTxtBoxColor::Red);
			break;
		case 4:
			gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "기다릴 줄 모르는구나?", eTxtBoxColor::Red);
			break;
		case 5:
			gameTxtBox(IMAGEMANAGER->findImage("Overlord"), "대마왕니이이이님!!", eTxtBoxColor::Red);
			break;
		}

		CAMERA->setCamPosY(CAMERA_MAX_Y);
		CAMERA->setCamPosX((m_tileSizeMaxX - WINSIZEX - WINSTARTX) / 2);

		m_gameState = eGameState::GameGetSet;
		m_isHeroStart = true;
		m_HeroStartDaley = 1;
		m_pOverlord->setHeroValue(0);

		SOUNDMANAGER->stop("Sound/BGM/BGM_GameReady.wav");
		SOUNDMANAGER->play("Sound/BGM/BGM_GameStart.wav");

		switch (m_stageValue)
		{
		case 1:
			heroSetting(heroSet(eHeroClass::Warriors));
			break;
		case 2:
			heroSetting(heroSet(eHeroClass::Alchemist,2));
			break;
		case 3:
			heroSetting(heroSet(eHeroClass::Wizard,2));
			break;
		case 4:
			heroSetting(heroSet(eHeroClass::Alchemist, 2));
			heroSetting(heroSet(eHeroClass::Warriors, 3));
			break;
		case 5:
			heroSetting(heroSet(eHeroClass::Alchemist, 3));
			heroSetting(heroSet(eHeroClass::Warriors, 4));
			heroSetting(heroSet(eHeroClass::Wizard, 2));
			break;
		case 6:
			heroSetting(heroSet(eHeroClass::hero_0, 3));
			break;
		}
	}
	heroListStart(); 
}


void tileMap::heroListStart()
{
	if (m_gameState != eGameState::GameGo) return;
	if(!SOUNDMANAGER->isPlaying("Sound/BGM/BGM_HeroStart.wav"))
		SOUNDMANAGER->play("Sound/BGM/BGM_HeroStart.wav");

	if (m_HeroStartDaley > 0)
		m_HeroStartDaley--;

	if (m_HeroStartDaley <= 0)
	{
		for (m_iterHeroList = m_vecHeroList.begin(); m_iterHeroList != m_vecHeroList.end(); m_iterHeroList++)
		{
			if ((*m_iterHeroList) == nullptr) continue;

			m_pHeroMag->heroDrop((*m_iterHeroList));
			delete (*m_iterHeroList);
			(*m_iterHeroList) = nullptr;
			m_HeroStartDaley = HERO_START_DALEY;
			return;
		}
	}
}


tagHeroData * tileMap::heroSet(int heroClass, int lv)
{
	// 용사 개수만큼 마왕 heroValue 충전
	m_pOverlord->setHeroValue(m_pOverlord->getHeroValue() + 1);
	tagHeroData * tempHero = new tagHeroData;
	// 기본 세팅
	tempHero->t_isAilve = true;
	memset(&tempHero->t_Inven, 0, sizeof(tempHero->t_Inven));
	memset(&tempHero->t_Skill, 0, sizeof(tempHero->t_Skill));
	tempHero->t_posX = m_tileset[15 * m_tileSizeY + 0].t_rc.left + CAMERA->getCamPosX();
	tempHero->t_posY = m_tileset[15 * m_tileSizeY + 0].t_rc.top + CAMERA->getCamPosY();
	tempHero->t_tilePosX = 15;
	tempHero->t_tilePosY = 0;
	tempHero->t_scale = 2.0f;
	tempHero->t_setTileMapNum = (15 * m_tileSizeY + 0);
	tempHero->t_moveDaley = 0;
	tempHero->t_enumType = heroClass;

	switch (heroClass)
	{
	case eHeroClass::hero_0:
		tempHero->t_img_U = IMAGEMANAGER->findImage("hero_U");
		tempHero->t_img_UA = IMAGEMANAGER->findImage("hero_UA");
		tempHero->t_img_R = IMAGEMANAGER->findImage("hero_R");
		tempHero->t_img_RA = IMAGEMANAGER->findImage("hero_RA");
		tempHero->t_img_D = IMAGEMANAGER->findImage("hero_D");
		tempHero->t_img_DA = IMAGEMANAGER->findImage("hero_DA");
		tempHero->t_img_L = IMAGEMANAGER->findImage("hero_L");
		tempHero->t_img_LA = IMAGEMANAGER->findImage("hero_LA");
		tempHero->t_img_S = IMAGEMANAGER->findImage("hero_S");
		tempHero->t_img_Dead = IMAGEMANAGER->findImage("hero_Dead");

		tempHero->t_currHp = BaseHp + (lv * 50);
		tempHero->t_currMana = BaseMana + (lv * 50);
		tempHero->t_moveSpeed = BaseMoveSpeed + (lv * 0.5f);
		tempHero->t_attType = tagHeroAttType::Near;
		tempHero->t_atkPoint = BaseAtkPoint + (lv * 2);
		tempHero->t_defPoint = BaseDefPoint + (lv * 1);

		tempHero->t_Skill.t_fireWall = true;
		tempHero->t_Skill.t_ArrowMagic = true;
		tempHero->t_Skill.t_haling = true;
		tempHero->t_Skill.t_AtkBuff = true;
		tempHero->t_Skill.t_DefBuff = true;
		tempHero->t_Skill.t_HasteBuff = true;

		break;
	case eHeroClass::Alchemist:
		tempHero->t_img_U = IMAGEMANAGER->findImage("Alchemist_U");
		tempHero->t_img_UA = IMAGEMANAGER->findImage("Alchemist_UA");
		tempHero->t_img_R = IMAGEMANAGER->findImage("Alchemist_R");
		tempHero->t_img_RA = IMAGEMANAGER->findImage("Alchemist_RA");
		tempHero->t_img_D = IMAGEMANAGER->findImage("Alchemist_D");
		tempHero->t_img_DA = IMAGEMANAGER->findImage("Alchemist_DA");
		tempHero->t_img_L = IMAGEMANAGER->findImage("Alchemist_L");
		tempHero->t_img_LA = IMAGEMANAGER->findImage("Alchemist_LA");
		tempHero->t_img_S = IMAGEMANAGER->findImage("Alchemist_S");
		tempHero->t_img_Dead = IMAGEMANAGER->findImage("Alchemist_Dead");

		tempHero->t_currHp = BaseHp + (lv * 20);
		tempHero->t_currMana = BaseMana + (lv * 30);
		tempHero->t_moveSpeed = BaseMoveSpeed + (lv * 0.2f);
		tempHero->t_attType = tagHeroAttType::Near;
		tempHero->t_atkPoint = BaseAtkPoint + (lv * 1);
		tempHero->t_defPoint = BaseDefPoint + (lv * 0);

		tempHero->t_Skill.t_fireWall = true;
		tempHero->t_Skill.t_ArrowMagic = true;
		tempHero->t_Skill.t_haling = true;
		//tempHero->t_Skill.t_AtkBuff = true;
		//tempHero->t_Skill.t_DefBuff = true;
		tempHero->t_Skill.t_HasteBuff = true;

		break;
	case eHeroClass::Bard:
		tempHero->t_img_U = IMAGEMANAGER->findImage("Bard_U");
		tempHero->t_img_UA = IMAGEMANAGER->findImage("Bard_UA");
		tempHero->t_img_R = IMAGEMANAGER->findImage("Bard_R");
		tempHero->t_img_RA = IMAGEMANAGER->findImage("Bard_RA");
		tempHero->t_img_D = IMAGEMANAGER->findImage("Bard_D");
		tempHero->t_img_DA = IMAGEMANAGER->findImage("Bard_DA");
		tempHero->t_img_L = IMAGEMANAGER->findImage("Bard_L");
		tempHero->t_img_LA = IMAGEMANAGER->findImage("Bard_LA");
		tempHero->t_img_S = IMAGEMANAGER->findImage("Bard_S");
		tempHero->t_img_Dead = IMAGEMANAGER->findImage("Bard_Dead");

		tempHero->t_currHp = BaseHp + (lv * 50);
		tempHero->t_currMana = BaseMana + (lv * 50);
		tempHero->t_moveSpeed = BaseMoveSpeed + (lv * 0.5f);
		tempHero->t_attType = tagHeroAttType::Near;
		tempHero->t_atkPoint = BaseAtkPoint + (lv * 2);
		tempHero->t_defPoint = BaseDefPoint + (lv * 1);

		//tempHero->t_Skill.t_fireWall = true;
		//tempHero->t_Skill.t_ArrowMagic = true;
		//tempHero->t_Skill.t_haling = true;
		//tempHero->t_Skill.t_AtkBuff = true;
		//tempHero->t_Skill.t_DefBuff = true;
		//tempHero->t_Skill.t_HasteBuff = true;
		break;
	case eHeroClass::Theif:
		tempHero->t_img_U = IMAGEMANAGER->findImage("Theif_U");
		tempHero->t_img_UA = IMAGEMANAGER->findImage("Theif_UA");
		tempHero->t_img_R = IMAGEMANAGER->findImage("Theif_R");
		tempHero->t_img_RA = IMAGEMANAGER->findImage("Theif_RA");
		tempHero->t_img_D = IMAGEMANAGER->findImage("Theif_D");
		tempHero->t_img_DA = IMAGEMANAGER->findImage("Theif_DA");
		tempHero->t_img_L = IMAGEMANAGER->findImage("Theif_L");
		tempHero->t_img_LA = IMAGEMANAGER->findImage("Theif_LA");
		tempHero->t_img_S = IMAGEMANAGER->findImage("Theif_S");
		tempHero->t_img_Dead = IMAGEMANAGER->findImage("Theif_Dead");

		tempHero->t_currHp = BaseHp + (lv * 50);
		tempHero->t_currMana = BaseMana + (lv * 50);
		tempHero->t_moveSpeed = BaseMoveSpeed + (lv * 0.5f);
		tempHero->t_attType = tagHeroAttType::Near;
		tempHero->t_atkPoint = BaseAtkPoint + (lv * 2);
		tempHero->t_defPoint = BaseDefPoint + (lv * 1);

		//tempHero->t_Skill.t_fireWall = true;
		//tempHero->t_Skill.t_ArrowMagic = true;
		//tempHero->t_Skill.t_haling = true;
		//tempHero->t_Skill.t_AtkBuff = true;
		//tempHero->t_Skill.t_DefBuff = true;
		//tempHero->t_Skill.t_HasteBuff = true;
		break;
	case eHeroClass::Warriors:
		tempHero->t_img_U = IMAGEMANAGER->findImage("Warriors_U");
		tempHero->t_img_UA = IMAGEMANAGER->findImage("Warriors_UA");
		tempHero->t_img_R = IMAGEMANAGER->findImage("Warriors_R");
		tempHero->t_img_RA = IMAGEMANAGER->findImage("Warriors_RA");
		tempHero->t_img_D = IMAGEMANAGER->findImage("Warriors_D");
		tempHero->t_img_DA = IMAGEMANAGER->findImage("Warriors_DA");
		tempHero->t_img_L = IMAGEMANAGER->findImage("Warriors_L");
		tempHero->t_img_LA = IMAGEMANAGER->findImage("Warriors_LA");
		tempHero->t_img_S = IMAGEMANAGER->findImage("Warriors_S");
		tempHero->t_img_Dead = IMAGEMANAGER->findImage("Warriors_Dead");

		tempHero->t_currHp = BaseHp + (lv * 30);
		tempHero->t_currMana = BaseMana + (lv * 10);
		tempHero->t_moveSpeed = BaseMoveSpeed + (lv * 0.2f);
		tempHero->t_attType = tagHeroAttType::Near;
		tempHero->t_atkPoint = BaseAtkPoint + (lv * 1);
		tempHero->t_defPoint = BaseDefPoint + (lv * 1);

		//tempHero->t_Skill.t_fireWall = true;
		//tempHero->t_Skill.t_ArrowMagic = true;
		//tempHero->t_Skill.t_haling = true;
		tempHero->t_Skill.t_AtkBuff = true;
		tempHero->t_Skill.t_DefBuff = true;
		//tempHero->t_Skill.t_HasteBuff = true;
		break;
	case eHeroClass::Wizard:
		tempHero->t_img_U = IMAGEMANAGER->findImage("Wizard_U");
		tempHero->t_img_UA = IMAGEMANAGER->findImage("Wizard_UA");
		tempHero->t_img_R = IMAGEMANAGER->findImage("Wizard_R");
		tempHero->t_img_RA = IMAGEMANAGER->findImage("Wizard_RA");
		tempHero->t_img_D = IMAGEMANAGER->findImage("Wizard_D");
		tempHero->t_img_DA = IMAGEMANAGER->findImage("Wizard_DA");
		tempHero->t_img_L = IMAGEMANAGER->findImage("Wizard_L");
		tempHero->t_img_LA = IMAGEMANAGER->findImage("Wizard_LA");
		tempHero->t_img_S = IMAGEMANAGER->findImage("Wizard_S");
		tempHero->t_img_Dead = IMAGEMANAGER->findImage("Wizard_Dead");

		tempHero->t_currHp = BaseHp + (lv * 10);
		tempHero->t_currMana = BaseMana + (lv * 40);
		tempHero->t_moveSpeed = BaseMoveSpeed + (lv * 0.1f);
		tempHero->t_attType = tagHeroAttType::Near;
		tempHero->t_atkPoint = BaseAtkPoint + (lv * 1);
		tempHero->t_defPoint = BaseDefPoint + (lv * 0);

		tempHero->t_Skill.t_fireWall = true;
		tempHero->t_Skill.t_ArrowMagic = true;
		tempHero->t_Skill.t_haling = true;
		//tempHero->t_Skill.t_AtkBuff = true;
		//tempHero->t_Skill.t_DefBuff = true;
		tempHero->t_Skill.t_HasteBuff = true;
		break;
	}

	tempHero->t_MaxHp = tempHero->t_currHp;
	tempHero->t_MaxMana = tempHero->t_currMana;

	return tempHero;
	
}

void tileMap::tileSetTxt(int tileType, int tileNum)
{
	tagTileTxt setTxt;
	setTxt.t_TileMana = m_tileset[tileNum].t_ManaValue;

	switch (tileType)
	{
	case tagTileType::GROUND:
		setTxt.t_txtName = "저세상 바닥";
		setTxt.t_txtInfo = "던전 밖의 무서운 흙더미 입니다!";
		break;
	case tagTileType::START:
		setTxt.t_txtName = "던전 입구";
		setTxt.t_txtInfo = "용사가 나타나는 입구 입니다!";
		break;
	case tagTileType::TOP:
		setTxt.t_txtName = "던전 외벽";
		setTxt.t_txtInfo = "인간들은 절대 뚫을 수 없는 마법의 벽입니다 아참, 마왕님도 불가능합니다.";
		break;
	case tagTileType::BLOCK:
		switch (m_tileset[tileNum].t_tierValue)
		{
		case eTileEnemy::Slime:
			setTxt.t_txtName = "액괴 소환석";
			setTxt.t_txtInfo = "이 녀석의 액체는 피부 미용에 탁월하다고 합니다.";
			setTxt.t_TileEnemyName = "액괴";
			break;
		case eTileEnemy::Bug:
			setTxt.t_txtName = "볼레 소환석";
			setTxt.t_txtInfo = "인간계 곤충과는 차원이 다른 녀석이죠!";
			setTxt.t_TileEnemyName = "볼레";
			break;
		case eTileEnemy::Lizardman:
			setTxt.t_txtName = "꼬복이 소환석";
			setTxt.t_txtInfo = "어디서 주웠는지 방패까지 갖춘 강력한 보병 입니다!";
			setTxt.t_TileEnemyName = "꼬복이";
			break;
		case eTileEnemy::Lili:
			setTxt.t_txtName = "릴리 소환석";
			setTxt.t_txtInfo = "천방지축이라, 용사를 괴롭히는데는 최고죠!";
			setTxt.t_TileEnemyName = "릴리";
			break;
		case eTileEnemy::Demon:
			setTxt.t_txtName = "덩치 소환석";
			setTxt.t_txtInfo = "무섭고 강력한 녀석입니다만.. 너무 커서 문제죠. 좁은 길은 못나갑니다.";
			setTxt.t_TileEnemyName = "데몬";
			break;
		case eTileEnemy::Mamon:
			setTxt.t_txtName = "외눈박이 소환석";
			setTxt.t_txtInfo = "요녀석의 강력한 춉 공격은 상대를 기절시킬 정도라더군요.";
			setTxt.t_TileEnemyName = "외눈박이";
			break;
		case eTileEnemy::Lady:
			setTxt.t_txtName = "부끄마마 소환석";
			setTxt.t_txtInfo = "낯가림이 심해서 용사가 다가오면 숨어버린다고 하더군요.";
			setTxt.t_TileEnemyName = "외눈박이";
			break;
		default:
			setTxt.t_txtName = "흙 블럭";
			setTxt.t_txtInfo = "그저 흙냄새만 나는 평범한 흙블럭 입니다";
			setTxt.t_TileEnemyName = "없음";
			break;
		}

		break;
	case tagTileType::RAND:
		setTxt.t_txtName = "돌 바닥";
		setTxt.t_txtInfo = "단단한 바닥입니다.";
		break;
	case tagTileType::BLOCK_NON:
		setTxt.t_txtName = "무지단단";
		setTxt.t_txtInfo = "이건 마왕님도 어쩔 수 없을 것 같습니다..";
		break;
	}
	
	m_pTilePopup->setTxtInfo(setTxt);
}

tagItemData tileMap::dropItemSet(int itemType)
{
	switch (itemType)
	{
	case tagItemType::JEWEL:
		m_tItemInfo.t_frameX = RANDOM->getFromIntTo(0, 4);
		m_tItemInfo.t_frameY = RANDOM->getFromIntTo(0, 6);
		return m_tItemInfo;
	}
	return m_tItemInfo;
}

void tileMap::monsSetDrop(float posX, float posY, int setTileNum, int tileX, int tileY)
{
	int tempTierValue = m_tileset[setTileNum].t_tierValue;

	//// 기본 셋팅
	tagEnemyData tempEnemy;
	tempEnemy.t_isAilve = true;
	tempEnemy.t_posX = posX + CAMERA->getCamPosX();
	tempEnemy.t_posY = posY + CAMERA->getCamPosY();
	tempEnemy.t_tilePosX = tileX;
	tempEnemy.t_tilePosY = tileY;
	tempEnemy.t_setTileMapNum = setTileNum;

	// TileLv에 따른 처리
	if (tempTierValue == 0)
	{
		SOUNDMANAGER->play("Sound/SE/BarracudaJoe.wav");
		return;
	}

	else if (tempTierValue == eTileEnemy::Slime)
	{
		SOUNDMANAGER->play("Sound/SE/Slime.wav");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Slime_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Slime_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Slime_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Slime_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Slime_00_Dead");

		tempEnemy.t_currHp = 25;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 1.5f;
		tempEnemy.t_moveSpeed = RANDOM->getFromFloatTo(0.5f, 0.8f);
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 3;
		tempEnemy.t_defPoint = 0;
		tempEnemy.t_FoodChainLv = 0;
		tempEnemy.t_currMana = RANDOM->getFromIntTo(1, 2);
		tempEnemy.t_enumType = tagEnemyType::Slime;
		tempEnemy.t_powerValue = 15;
	}
	else if (tempTierValue == eTileEnemy::Bug)
	{
		SOUNDMANAGER->play("Sound/SE/Bug.wav");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Bug_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Bug_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Bug_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Bug_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Bug_00_Dead");

		tempEnemy.t_currHp = 50;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0;
		tempEnemy.t_moveSpeed = 0.8f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 3;
		tempEnemy.t_defPoint = 0;
		tempEnemy.t_FoodChainLv = 1;
		tempEnemy.t_currMana = RANDOM->getFromIntTo(1, 1);
		tempEnemy.t_enumType = tagEnemyType::Bug;
		tempEnemy.t_powerValue = 20;

	}
	else if (tempTierValue == eTileEnemy::Lizardman)
	{
		SOUNDMANAGER->play("Sound/SE/Lizardman.wav");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Lizardman_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Lizardman_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Lizardman_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Lizardman_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Lizardman_00_Dead");

		tempEnemy.t_currHp = 65;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 8;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 2;
		tempEnemy.t_enumType = tagEnemyType::Lizardman;
		tempEnemy.t_powerValue = 35;

		
	}
	else if (tempTierValue == eTileEnemy::Lili)
	{
		SOUNDMANAGER->play("Sound/SE/Lili.wav");

		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Lili_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Lili_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Lili_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Lili_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Lili_00_Dead");

		tempEnemy.t_currHp = 35;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.7f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 3;
		tempEnemy.t_defPoint = 0;
		tempEnemy.t_FoodChainLv = 2;
		tempEnemy.t_enumType = tagEnemyType::Lili;
		tempEnemy.t_powerValue = 30;

	}
	else if (tempTierValue == eTileEnemy::Mamon)
	{
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Mamon_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Mamon_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Mamon_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Mamon_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Mamon_00_Dead");

		tempEnemy.t_currHp = 100;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 20;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 3;
		tempEnemy.t_enumType = tagEnemyType::Mamon;
		tempEnemy.t_powerValue = 50;

	}
	else if (tempTierValue == eTileEnemy::Lady)
	{
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Lady_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Lady_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Lady_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Lady_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Lady_00_Dead");

		tempEnemy.t_currHp = 110;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 20;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 4;
		tempEnemy.t_enumType = tagEnemyType::Lady;
		tempEnemy.t_powerValue = 100;

	}
	else if (tempTierValue == eTileEnemy::Demon)
	{
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Demon_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Demon_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Demon_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Demon_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Demon_00_Dead");

		tempEnemy.t_currHp = 150;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 20;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 4;
		tempEnemy.t_enumType = tagEnemyType::Demon;
		tempEnemy.t_powerValue = 100;

	}



	m_pEnemyMag->enemyDrop(&tempEnemy);
}

void tileMap::tileImgSet(int posX, int posY, HDC hdc, bool isShaking)
{
	int tempTileXY = posX * m_tileSizeY + posY;
	int tempX = 0;
	if (isShaking)
		tempX = (RANDOM->getFromIntTo(-5, 5));

	int tileX[3] = { -1, 0, +1 };
	int tileY[3] = { -1, 0, +1 };

	if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::NonEnemy)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, eTileLv::tNoEnemy, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Slime)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, eTileLv::tSlime, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Bug)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, eTileLv::tBug, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Lizardman)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, eTileLv::tLizardman, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Lili)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[posX * m_tileSizeY + posY].t_setImg, eTileLv::tLili, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Mamon)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[posX * m_tileSizeY + posY].t_setImg, eTileLv::tMamon, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Lady)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[posX * m_tileSizeY + posY].t_setImg, eTileLv::tLady, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Demon)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[posX * m_tileSizeY + posY].t_setImg, eTileLv::tDemon, TILE_SCALE, false);
	}

	return;
}

void tileMap::tierSet(int tileX, int tileY)
{
	int tempTileXY = tileX * m_tileSizeY + tileY;
	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_0)
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::NonEnemy;

	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_1)
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::Slime;

	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_2)
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::Bug;

	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_3)
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::Lizardman;

	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_4)
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::Lili;

	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_5)
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::Mamon;

	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_6 && LadyTileCheck(tileX, tileY))
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::Lady;

	if (m_tileset[tempTileXY].t_ManaValue >= TILE_LV_6 && DemonTileCheck(tileX, tileY))
		m_tileset[tempTileXY].t_tierValue = eTileEnemy::Demon;


	return;
}

void tileMap::tileDesSys(int tileValue, int tileX, int tileY)
{
	m_pTilePopup->setIsAilve(false);
	m_tileset[tileValue].t_isAlive = false;
	m_tileset[tileValue].t_enumType = tagTileType::RAND;
	m_tileset[tileValue].t_img = IMAGEMANAGER->findImage("TileSet");
	m_isTileClick = true;
	m_pPlayer->t_TileDesEne--;
	for (int i = 0; i < 6; i++)
	{
		OBJECTMANAGER->addTileDesObj(tagObjectType::TILE_DIS,
			m_tileset[tileValue].t_rc.left + (TILE_SIZE / 2),
			m_tileset[tileValue].t_rc.top + (TILE_SIZE / 2), m_tItemInfo);
	}

	tileDesSe();
	tileItemGet(tileValue);

	monsSetDrop(m_tileset[tileValue].t_rc.left, m_tileset[tileValue].t_rc.top, (tileValue), tileX, tileY);
	m_tileset[tileValue].t_ManaValue = -1;
	EFFMANAGER->play("MousePointEFF", m_tileset[tileValue].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromIntTo(-5, 5)), m_tileset[tileValue].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromIntTo(-5, 5)));



	m_tileDesDaley = TILE_DES_DALEY;


}

void tileMap::tileDesSe()
{
	switch (RANDOM->getInt(3))
	{
	case 0:
		SOUNDMANAGER->play("Sound/SE/Het_0.wav");
		break;
	case 1:
		SOUNDMANAGER->play("Sound/SE/Het_1.wav");
		break;
	case 2:
		SOUNDMANAGER->play("Sound/SE/Het_2.wav");
		break;
	default:
		break;
	}
}

void tileMap::tileItemGet(int tileValue, bool isGet)
{
	if (RANDOM->getInt(100) <= 10 || isGet)
	{
		SOUNDMANAGER->play("Sound/SE/getGam.wav");

		OBJECTMANAGER->addTileDesObj(tagObjectType::ITEM_JEWEL,
			m_tileset[tileValue].t_rc.left + (TILE_SIZE / 2),
			m_tileset[tileValue].t_rc.top + (TILE_SIZE / 2), dropItemSet(tagItemType::JEWEL));
		m_tileDesCurr += m_tItemInfo.t_frameY + 1;
	}
}

void tileMap::tileDesEneChag()
{
	if (m_tileDesCurr <= 0) return;

	if (m_tileDesCurr > 0)
	{
		m_tileDesCount--;
		
		if (m_tileDesCount <= 0)
		{
			m_tileDesCurr--;
			m_pPlayer->t_TileDesEne += 1;
			m_tileDesCount = TILE_DES_SET_COUNT;
		}
	}


}

void tileMap::enemyArrNullptrList(int tileMapValue, tagEnemyData * enemyData)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (!(m_tileset[tileMapValue].t_enemyInfo[i] == enemyData)) continue;

		m_tileset[tileMapValue].t_enemyInfo[i] = nullptr;
		return;
	}

	return;
}

void tileMap::enemyArrInList(int tileMapValue, tagEnemyData * enemyData)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_enemyInfo[i] != nullptr) continue;
		
		m_tileset[tileMapValue].t_enemyInfo[i] = enemyData;
		return;
	}

	return;
}

bool tileMap::enemyArrIsList(int tileMapValue, tagEnemyData * enemyData)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_enemyInfo[i] == nullptr) continue;
		if (m_tileset[tileMapValue].t_enemyInfo[i] == enemyData) return true;
	}

	return false;
}

tagEnemyData * tileMap::enemyArrOutList(int tileMapValue)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_enemyInfo[i] == nullptr) continue;

		return m_tileset[tileMapValue].t_enemyInfo[i];
	}

	return nullptr;
}

void tileMap::enemyBackMove(int tileMapValue, tagHeroData * pHero)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_enemyInfo[i] == nullptr) continue;

		m_tileset[tileMapValue].t_enemyInfo[i]->t_pEnemy->backMoveArrow(pHero->t_tilePosX, pHero->t_tilePosY);
	}

	return;
}

bool tileMap::enemyFoodChain(int tileMapValue, tagEnemyData * pEnemyDate)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_enemyInfo[i] == nullptr) continue;

		if (m_tileset[tileMapValue].t_enemyInfo[i]->t_FoodChainLv < pEnemyDate->t_FoodChainLv)
		{
			m_tileset[tileMapValue].t_enemyInfo[i]->t_currHp = 0;
			pEnemyDate->t_currHp += m_tileset[tileMapValue].t_enemyInfo[i]->t_MaxHp;
			return true;
		}
	}

	return false;
}

void tileMap::HeroArrNullptrList(int tileMapValue, tagHeroData * HeroData)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (!(m_tileset[tileMapValue].t_heroInfo[i] == HeroData)) continue;

		m_tileset[tileMapValue].t_heroInfo[i] = nullptr;
		return;
	}

	return;
}



void tileMap::HeroArrInList(int tileMapValue, tagHeroData * HeroData)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_heroInfo[i] != nullptr) continue;

		m_tileset[tileMapValue].t_heroInfo[i] = HeroData;
		return;
	}

	return;
}

bool tileMap::HeroArrIsList(int tileMapValue, tagHeroData * HeroData)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_heroInfo[i] == nullptr) continue;
		if (m_tileset[tileMapValue].t_heroInfo[i] == HeroData) return true;
	}

	return false;
}

tagHeroData * tileMap::HeroArrOutList(int tileMapValue)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_heroInfo[i] == nullptr) continue;

		return m_tileset[tileMapValue].t_heroInfo[i];
	}

	return nullptr;
}

void tileMap::HeroSkillAtk(int tileMapValue, tagHeroData * pHeroData)
{
	for (int i = 0; i < ENEMY_INFO_ARR; i++)
	{
		if (m_tileset[tileMapValue].t_enemyInfo[i] == nullptr) continue;

		m_tileset[tileMapValue].t_enemyInfo[i]->t_damgePoint += pHeroData->t_Skill.t_fireWallData.t_damage;
	}

	return;
}

bool tileMap::tileCheck(int tileX, int tileY)
{
	if (tileX == 0 || tileY == 0 || tileX == m_tileSizeX || tileY == m_tileSizeY) return false;

	int tempX[2] = { -1, +1 };
	int tempY[2] = { -1, +1 };

	for (int x = 0; x < 2; x++)
	{
		if (!m_tileset[(tileX + tempX[x]) * m_tileSizeY + (tileY)].t_isAlive) return true;
		for (int y = 0; y < 2; y++)
		{
			if (!m_tileset[(tileX) * m_tileSizeY + (tileY + tempY[y])].t_isAlive) return true;
		}
	}

	return false;
}

bool tileMap::DemonTileCheck(int tileX, int tileY)
{
	int tempX[3] = { -1, 0, 1 };
	int tempY[3] = { -1, 0, 1 };

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (x == 1 && y == 1) continue;
			if (m_tileset[(tileX + tempX[x]) * m_tileSizeY + (tileY + tempY[y])].t_isAlive) return false;
		}
	}

	return true;
}

bool tileMap::LadyTileCheck(int tileX, int tileY)
{
	int tempX[3] = { -1, 0, 1 };
	int tempY[3] = { -1, 0, 1 };

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (y == 1 && x == 1) continue;

			if (y == 1 && x != 1)
				if (m_tileset[(tileX + tempX[x])* m_tileSizeY + (tileY + tempY[y])].t_isAlive) return false;
			if (y != 1 && x == 1)
				if (m_tileset[(tileX + tempX[x])* m_tileSizeY + (tileY + tempY[y])].t_isAlive) return false;
			if (y != 1 && x != 1)
				if (!m_tileset[(tileX + tempX[x])* m_tileSizeY + (tileY + tempY[y])].t_isAlive) return false;
		}
	}

	return true;
}

void tileMap::heroStart(int tileValue)
{
	m_isHeroStart = false;
	SOUNDMANAGER->play("Sound/SE/HeroOpen_0.wav");
	EFFMANAGER->play("hero_Start",
		(m_tileset[tileValue].t_rc.left) + (TILE_SIZE / 2),
		(m_tileset[tileValue].t_rc.top) + (TILE_SIZE / 2 / 2));
	for (int i = 0; i < 20; i++)
	{
		OBJECTMANAGER->addTileDesObj(tagObjectType::HERO_START,
			m_tileset[tileValue].t_rc.left + (TILE_SIZE / 2),
			m_tileset[tileValue].t_rc.top + (TILE_SIZE / 2), m_tItemInfo);
	}

	for (int x = 0; x < m_tileSizeX; x++)
	{
		for (int y = 0; y < m_tileSizeY; y++)
		{
				m_tileset[x * m_tileSizeY + y].t_isShaking = true;
		}
	}
}

void tileMap::heroSetting(tagHeroData * heroData)
{
	for (m_iterHeroList = m_vecHeroList.begin(); m_iterHeroList != m_vecHeroList.end(); m_iterHeroList++)
	{
		if ((*m_iterHeroList) != nullptr) continue;
		(*m_iterHeroList) = heroData;
		break;
	}
}

void tileMap::gameTxtBox(image * img, string txt, int txtCol)
{
	string temp = txt;
	m_pUiMag->addTxtBox(img, temp, true, txtCol);
}


tileMap::tileMap()
{
}


tileMap::~tileMap()
{
}
