#include "stdafx.h"
#include "tileMap.h"
#include "uiPopup.h"
#include "uiManager.h"
#include "enemyManager.h"
#include "heroManager.h"
#include "animation.h"

HRESULT tileMap::init(int tileX, int tileY,
	PlayerInfo * playerData, uiManager * uiMagData,
	enemyManager * pEnemyMag, heroManager * pHeroMag)
{
	//// Enemy & Hero 매니저 주소 초기화
	m_pEnemyMag = pEnemyMag;
	m_pHeroMag = pHeroMag;

	//// UI 매니저 주소 초기화
	m_pUiMag = uiMagData;
	m_pTilePopup = m_pUiMag->addPopup(NULL, NULL, NULL, NULL);

	//// 플레이어 데이터 주소 저장
	m_pPlayer = playerData;

	//// 마우스 초기화
	m_isTileClick = false;
	
	//// 타일 초기화 셋팅
	m_tileDesDaley = 0;
	m_tileSizeX = tileX;
	m_tileSizeY = tileY;
	m_tileSizeMaxX = tileX * TILE_SIZE;
	m_tileSizeMaxY = tileY * TILE_SIZE;
	IMAGEMANAGER->addImage("TileSet", "image/inGameImg/TILE/TileSet_Terrain.bmp", 160, 352, 5, 11, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addImage("TileShadow", "image/inGameImg/TILE/TileSet_Terrain_Shadow.bmp", 6, 32);

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
			m_tileset[x * m_tileSizeY + y].t_enemyInfo = nullptr;
			m_tileset[x * m_tileSizeY + y].t_heroInfo = nullptr;
			m_tileset[x * m_tileSizeY + y].t_isOverlord = false;

			// 깊이에 따라 마나 세팅 추가
			int tempY = (m_tileSizeY / 4);
			if (y <= (tempY * 1))
			{
				m_tileset[x * m_tileSizeY + y].t_ManaValue = RANDOM->getFromIntTo(3, 20);
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

			if ((x + 1) == (m_tileSizeX / 2) && y == 0)
			{
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
				m_tileset[x * m_tileSizeY + y].t_setImg = 4;
				m_tileset[x * m_tileSizeY + y].t_isStart = true;
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::START;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}
			else if (x <= m_tileSizeX && y == (y * m_tileSizeY) || x == 0 || x == m_tileSizeX - 1 || y == m_tileSizeY - 1)
			{
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 3);
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::TOP;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}
			else if ((x + 1) == (m_tileSizeX / 2) && y <= TILE_SETNON)
			{
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 3);
				m_tileset[x * m_tileSizeY + y].t_isAlive = false;
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::RAND;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}
			else if (TempRandom < TILE_NON_BLOCK && y > TILE_NON_BLOCK_SET_Y)
			{
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 4);
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::BLOCK_NON;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
			}
			else
			{
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 4);
				m_tileset[x * m_tileSizeY + y].t_isNetDes = false;
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
			tierSet(x * m_tileSizeY + y);

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
			if (m_tileDesDaley <= 0 &&
				!m_tileset[x * m_tileSizeY + y].t_isNetDes &&
				m_pPlayer->t_TileDesEne > 0 &&
				!m_isTileClick &&
				m_tileset[x * m_tileSizeY + y].t_isAlive &&
				(!m_tileset[(x * m_tileSizeY + y) - 1].t_isAlive ||
					!m_tileset[(x * m_tileSizeY + y) + 1].t_isAlive ||
					!m_tileset[((x - 1) * m_tileSizeY + y)].t_isAlive ||
					!m_tileset[((x + 1) * m_tileSizeY + y)].t_isAlive) &&
				KEYMANAGER->isStayKeyDown(VK_LBUTTON) &&
				(IntersectRect(&tempRc, &m_tileset[x * m_tileSizeY + y].t_rc, &g_MouseRc)))
			{
				m_pTilePopup->setIsAilve(false);
				m_tileset[x * m_tileSizeY + y].t_isAlive = false;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::RAND;
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
				m_isTileClick = true;
				m_pPlayer->t_TileDesEne--;
				for (int i = 0; i < 6; i++)
				{
					OBJECTMANAGER->addTileDesObj(tagObjectType::TILE_DIS,
						m_tileset[x * m_tileSizeY + y].t_rc.left + (TILE_SIZE / 2),
						m_tileset[x * m_tileSizeY + y].t_rc.top + (TILE_SIZE / 2), m_tItemInfo);
				}

				if (RANDOM->getInt(100) <= 10)
				{
					SOUNDMANAGER->play("Sound/SE/getGam.wav");
					
					OBJECTMANAGER->addTileDesObj(tagObjectType::ITEM_JEWEL,
						m_tileset[x * m_tileSizeY + y].t_rc.left + (TILE_SIZE / 2),
						m_tileset[x * m_tileSizeY + y].t_rc.top + (TILE_SIZE / 2), dropItemSet(tagItemType::JEWEL));
					m_pPlayer->t_TileDesEne += m_tItemInfo.t_frameY + 1;
				}
				else
				{
					monsSetDrop(m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, (x * m_tileSizeY + y), x, y);
					m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
				}
				EFFMANAGER->play("MousePointEFF", m_tileset[x * m_tileSizeY + y].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromIntTo(-5, 5)));
				m_tileDesDaley = TILE_DES_DALEY;
			}
			//// 에너지 부족 타일 흔들기
			else if ((
				(m_tileset[x * m_tileSizeY + y].t_isNetDes || m_pPlayer->t_TileDesEne <= 0 ) &&
				!m_tileset[x * m_tileSizeY + y].t_isShaking &&
				!m_isTileClick && (y > 0) &&
				m_tileset[x * m_tileSizeY + y].t_isAlive &&
				(!m_tileset[(x * m_tileSizeY + y) - 1].t_isAlive ||
					!m_tileset[(x * m_tileSizeY + y) + 1].t_isAlive ||
					!m_tileset[((x - 1) * m_tileSizeY + y)].t_isAlive ||
					!m_tileset[((x + 1) * m_tileSizeY + y)].t_isAlive) &&
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
			if (m_tileset[x * m_tileSizeY + y].t_rc.left < -TILE_SIZE || m_tileset[x * m_tileSizeY + y].t_rc.left > WINSIZEX) continue;
			if (m_tileset[x * m_tileSizeY + y].t_rc.top < -TILE_SIZE || m_tileset[x * m_tileSizeY + y].t_rc.top > WINSIZEY) continue;

			if (m_tileset[x * m_tileSizeY + y].t_isAlive)
			{
				if (x * m_tileSizeY + y == x * m_tileSizeY + y * m_tileSizeY || x == 0 || x == m_tileSizeX - 1 || y == m_tileSizeY - 1)  //// 각 면 끝 타일)
				{
					m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 0, TILE_SCALE, false);
				}
				else //// 파괴 가능 타일
				{
					if (m_tileset[x * m_tileSizeY + y].t_isShaking)
					{
						switch (m_tileset[x * m_tileSizeY + y].t_enumType)
						{
						case tagTileType::BLOCK:
							tileImgSet(x, y, hdc, true);
							break;
						case tagTileType::BLOCK_NON:
							m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 1, TILE_SCALE, false);
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
							m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 1, TILE_SCALE, false);
							break;
						}
					}
				}
			}
			else //// 파괴된 타일
			{
				m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 2, TILE_SCALE, false);
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
				sprintf_s(szText, "%d", m_tileset[x * m_tileSizeY + y].t_heroInfo);
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
		CAMERA->setCamPosY(CAMERA->getCamPosY() - 3.0f);
	if (KEYMANAGER->isStayKeyDown('S'))
		CAMERA->setCamPosY(CAMERA->getCamPosY() + 3.0f);
	if (KEYMANAGER->isStayKeyDown('A'))
		CAMERA->setCamPosX(CAMERA->getCamPosX() - 3.0f);
	if (KEYMANAGER->isStayKeyDown('D'))
		CAMERA->setCamPosX(CAMERA->getCamPosX() + 3.0f);

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		//// 용사 임시 드롭
		tagHeroData tempHero;
		tempHero.t_img_U = IMAGEMANAGER->findImage("hero_00_U");
		tempHero.t_img_UA = IMAGEMANAGER->findImage("hero_00_UA");
		tempHero.t_img_R = IMAGEMANAGER->findImage("hero_00_R");
		tempHero.t_img_RA = IMAGEMANAGER->findImage("hero_00_RA");
		tempHero.t_img_D = IMAGEMANAGER->findImage("hero_00_D");
		tempHero.t_img_DA = IMAGEMANAGER->findImage("hero_00_DA");
		tempHero.t_img_L = IMAGEMANAGER->findImage("hero_00_L");
		tempHero.t_img_LA = IMAGEMANAGER->findImage("hero_00_LA");
		tempHero.t_img_Dead = IMAGEMANAGER->findImage("hero_00_Dead");

		tempHero.t_isAilve = true;
		tempHero.t_currHp = 200;
		tempHero.t_MaxHp = tempHero.t_currHp;
		tempHero.t_posX = m_tileset[15 * m_tileSizeY + 0].t_rc.left + CAMERA->getCamPosX();
		tempHero.t_posY = m_tileset[15 * m_tileSizeY + 0].t_rc.top + CAMERA->getCamPosY();
		tempHero.t_tilePosX = 15;
		tempHero.t_tilePosY = 0;
		tempHero.t_scale = 2.0f;
		tempHero.t_moveSpeed = 1.0f;
		tempHero.t_moveDaley = 0;
		tempHero.t_setTileMapNum = (15 * m_tileSizeY + 0);
		tempHero.t_atkPoint = 10;
		tempHero.t_defPoint = 1;

		m_pHeroMag->heroDrop(&tempHero);
	}

}

void tileMap::tileSetTxt(int tileType, int tileNum)
{
	tagTileTxt setTxt;
	setTxt.t_TileMana = m_tileset[tileNum].t_ManaValue;

	switch (tileType)
	{
	case tagTileType::START:
		setTxt.t_txtName = "던전 입구";
		setTxt.t_txtInfo = "건방진 용사는 이쪽에서 등장합니다!";
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
	}
	else if (tempTierValue == eTileEnemy::Lizardman)
	{
		SOUNDMANAGER->play("Sound/SE/Lizardman.wav");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Lizardman_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Lizardman_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Lizardman_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Lizardman_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Lizardman_00_Dead");

		tempEnemy.t_currHp = 10;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 20;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 3;
		tempEnemy.t_enumType = tagEnemyType::Lizardman;
		
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
		tempEnemy.t_scale = 1.5f;
		tempEnemy.t_moveSpeed = 0.7f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 3;
		tempEnemy.t_defPoint = 0;
		tempEnemy.t_FoodChainLv = 2;
		tempEnemy.t_enumType = tagEnemyType::Lili;
	}
	else if (tempTierValue == eTileEnemy::Mamon)
	{
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Mamon_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Mamon_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Mamon_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Mamon_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Mamon_00_Dead");

		tempEnemy.t_currHp = 200;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 20;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 3;
		tempEnemy.t_enumType = tagEnemyType::Mamon;
	}
	else if (tempTierValue == eTileEnemy::Lady)
	{
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Lady_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Lady_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Lady_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Lady_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Lady_00_Dead");

		tempEnemy.t_currHp = 200;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 20;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 3;
		tempEnemy.t_enumType = tagEnemyType::Lady;
	}
	else if (tempTierValue == eTileEnemy::Demon)
	{
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Demon_00_R");
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Demon_00_L");
		tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Demon_00_RA");
		tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Demon_00_LA");
		tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Demon_00_Dead");

		tempEnemy.t_currHp = 200;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 20;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 3;
		tempEnemy.t_enumType = tagEnemyType::Demon;
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
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, 3, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Slime)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, 4, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Bug)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, 5, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Lizardman)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[tempTileXY].t_setImg, 6, TILE_SCALE, false);
	}
	else if (m_tileset[tempTileXY].t_tierValue == eTileEnemy::Lili)
	{
		m_tileset[tempTileXY].t_img->frameRender(hdc, m_tileset[tempTileXY].t_rc.left + tempX, m_tileset[tempTileXY].t_rc.top, m_tileset[posX * m_tileSizeY + posY].t_setImg, 7, TILE_SCALE, false);

		//for (int x = 0; x < 3; x++)
		//{
		//	for (int y = 0; y < 3; y++)
		//	{
		//		if (x == 1 && y == 1) break;

		//		if (m_tileset[(posX + tileX[x]) * m_tileSizeY + (posY + tileY[y])].t_isAlive) return;

		//		if (x == 2 && y == 2)
		//			m_tileset[posX * m_tileSizeY + posY].t_img->frameRender(hdc,
		//				m_tileset[posX * m_tileSizeY + posY].t_rc.left + tempX,
		//				m_tileset[posX * m_tileSizeY + posY].t_rc.top,
		//				RANDOM->getInt(4), 8, TILE_SCALE, false);
		//	}
		//}
	}

	return;
}

void tileMap::tierSet(int tileNum)
{
	if (m_tileset[tileNum].t_ManaValue >= TILE_LV_0)
		m_tileset[tileNum].t_tierValue = eTileEnemy::NonEnemy;

	if (m_tileset[tileNum].t_ManaValue >= TILE_LV_1)
		m_tileset[tileNum].t_tierValue = eTileEnemy::Slime;

	if (m_tileset[tileNum].t_ManaValue >= TILE_LV_2)
		m_tileset[tileNum].t_tierValue = eTileEnemy::Bug;

	if (m_tileset[tileNum].t_ManaValue >= TILE_LV_3)
		m_tileset[tileNum].t_tierValue = eTileEnemy::Lizardman;

	if (m_tileset[tileNum].t_ManaValue >= TILE_LV_4)
		m_tileset[tileNum].t_tierValue = eTileEnemy::Lili;

	//if (m_tileset[tileNum].t_ManaValue >= TILE_LV_4)
	//	m_tileset[tileNum].t_tierValue = eTileEnemy::Demon;

	return;
}

tileMap::tileMap()
{
}


tileMap::~tileMap()
{
}
