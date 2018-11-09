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
	//// Enemy & Hero ¸Å´ÏÀú ÁÖ¼Ò ÃÊ±âÈ­
	m_pEnemyMag = pEnemyMag;
	m_pHeroMag = pHeroMag;

	//// UI ¸Å´ÏÀú ÁÖ¼Ò ÃÊ±âÈ­
	m_pUiMag = uiMagData;
	m_pTilePopup = m_pUiMag->addPopup(NULL, NULL, NULL, NULL);

	//// ÇÃ·¹ÀÌ¾î µ¥ÀÌÅÍ ÁÖ¼Ò ÀúÀå
	m_pPlayer = playerData;

	//// ¸¶¿ì½º ÃÊ±âÈ­
	m_isTileClick = false;
	
	//// Å¸ÀÏ ÃÊ±âÈ­ ¼ÂÆÃ
	m_tileDesDaley = 0;
	m_tileSizeX = tileX;
	m_tileSizeY = tileY;
	m_tileSizeMaxX = tileX * TILE_SIZE;
	m_tileSizeMaxY = tileY * TILE_SIZE;
	IMAGEMANAGER->addImage("TileSet", "image/inGameImg/TILE/TileSet_Terrain.bmp", 160, 352, 5, 11, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addImage("TileShadow", "image/inGameImg/TILE/TileSet_Terrain_Shadow.bmp", 6, 32);

	//// Å¸ÀÏ ÃÊ±âÈ­
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

			// ±íÀÌ¿¡ µû¶ó ¸¶³ª ¼¼ÆÃ Ãß°¡
			int tempY = (m_tileSizeY / 4);
			if (y <= (tempY * 1))
			{
				m_tileset[x * m_tileSizeY + y].t_ManaValue = RANDOM->getFromIntTo(2, 20);
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

	//// Ä«¸Þ¶ó ÃÖÃÊ À§Ä¡ ¼ÂÆÃ
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
			//// Å¸ÀÏ manaValue ¿¡ ¸ÂÃç tier ¼¼ÆÃ
			tierSet(x * m_tileSizeY + y);

			//// Å¸ÀÏ Èçµé±â
			if (m_tileset[x * m_tileSizeY + y].t_ShakingCount >= TILE_SHAKING_COUNT)
			{
				m_tileset[x * m_tileSizeY + y].t_ShakingCount = 0;
				m_tileset[x * m_tileSizeY + y].t_isShaking = false;
			}

			if (m_tileset[x * m_tileSizeY + y].t_isShaking)
			{
				m_tileset[x * m_tileSizeY + y].t_ShakingCount++;
			}

			//// Å¸ÀÏ »óÈ£ÀÛ¿ë
			//// Å¸ÀÏ ÆË¾÷ Ãâ·Â
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

			//// Å¸ÀÏ ÆÄ±«
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
				//// tile ÆÄ±« ÇÔ¼ö
				tileDesSys(x * m_tileSizeY + y, x, y);
			}
			//// ¿¡³ÊÁö ºÎÁ· Å¸ÀÏ Èçµé±â
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
	//// txt ¹Ì¸® ¼ÂÆÃ
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

			//// Å¸ÀÏ »óÅÂ¿¡ µû¸¥ ·»´õ¸µ
			
			if (MY_UTIL::screenRender(m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top)) continue;

			if (m_tileset[x * m_tileSizeY + y].t_isAlive)
			{
				if (x * m_tileSizeY + y == x * m_tileSizeY + y * m_tileSizeY || x == 0 || x == m_tileSizeX - 1 || y == m_tileSizeY - 1)  //// °¢ ¸é ³¡ Å¸ÀÏ)
				{
					m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 0, TILE_SCALE, false);
				}
				else //// ÆÄ±« °¡´É Å¸ÀÏ
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
			else //// ÆÄ±«µÈ Å¸ÀÏ
			{
				m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 2, TILE_SCALE, false);
			}
			
			//// Å¸ÀÏ ±×¸²ÀÚ »ý¼º
			if (!m_tileset[x * m_tileSizeY + y].t_isAlive && m_tileset[((x - 1) * m_tileSizeY + y)].t_isAlive)
				IMAGEMANAGER->findImage("TileShadow")->alphaRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, TILE_ALPHA_VALUE);


			//// µð¹ö±ë Á¤º¸
			if (g_saveData.gisTest)
			{
				Rectangle(hdc,
					m_tileset[x * m_tileSizeY + y].t_rc.left,
					m_tileset[x * m_tileSizeY + y].t_rc.top,
					m_tileset[x * m_tileSizeY + y].t_rc.right,
					m_tileset[x * m_tileSizeY + y].t_rc.bottom
				);

				//// Å¸ÀÏÁ¤º¸ µð¹ö±ë Á¤º¸
				//sprintf_s(szText, "%d,%d", m_tileset[x * m_tileSizeY + y].t_setX, m_tileset[x * m_tileSizeY + y].t_setY);
				//TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
				//sprintf_s(szText, "%d", m_tileset[x * m_tileSizeY + y].t_heroInfo);
				//TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
				sprintf_s(szText, "%d", TestTileNum);
				TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
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
	//// ¸¶¿ì½º Æ÷ÀÎÅÍ ±âÁØ Ä«¸Þ¶ó ÀÌµ¿
	if (g_ptMouse.x <= (WINSIZEX - WINSIZEX) + 5)
		CAMERA->setCamPosX(CAMERA->getCamPosX() - 10.0f);
	if (g_ptMouse.x >= (WINSIZEX) - 5)
		CAMERA->setCamPosX(CAMERA->getCamPosX() + 10.0f);
	if (g_ptMouse.y <= (WINSIZEY - WINSIZEY) + 5)
		CAMERA->setCamPosY(CAMERA->getCamPosY() - 10.0f);
	if (g_ptMouse.y >= (WINSIZEY) - 5)
		CAMERA->setCamPosY(CAMERA->getCamPosY() + 10.0f);

	//// Å° ÀÔ·Â ±âÁØ Ä«¸Þ¶ó ÀÌµ¿
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
		//// ¿ë»ç ÀÓ½Ã µå·Ó
		tagHeroData tempHero;
		tempHero.t_img_U = IMAGEMANAGER->findImage("hero_00_U");
		tempHero.t_img_UA = IMAGEMANAGER->findImage("hero_00_UA");
		tempHero.t_img_R = IMAGEMANAGER->findImage("hero_00_R");
		tempHero.t_img_RA = IMAGEMANAGER->findImage("hero_00_RA");
		tempHero.t_img_D = IMAGEMANAGER->findImage("hero_00_D");
		tempHero.t_img_DA = IMAGEMANAGER->findImage("hero_00_DA");
		tempHero.t_img_L = IMAGEMANAGER->findImage("hero_00_L");
		tempHero.t_img_LA = IMAGEMANAGER->findImage("hero_00_LA");
		tempHero.t_img_S = IMAGEMANAGER->findImage("hero_00_S");
		tempHero.t_img_Dead = IMAGEMANAGER->findImage("hero_00_Dead");

		tempHero.t_isAilve = true;
		tempHero.t_currHp = 200;
		tempHero.t_MaxHp = tempHero.t_currHp;
		tempHero.t_currMana = 1000;
		tempHero.t_MaxMana = tempHero.t_currMana;

		tempHero.t_posX = m_tileset[15 * m_tileSizeY + 0].t_rc.left + CAMERA->getCamPosX();
		tempHero.t_posY = m_tileset[15 * m_tileSizeY + 0].t_rc.top + CAMERA->getCamPosY();
		tempHero.t_tilePosX = 15;
		tempHero.t_tilePosY = 0;
		tempHero.t_scale = 2.0f;
		tempHero.t_moveSpeed = 1.0f;
		tempHero.t_moveDaley = 0;
		tempHero.t_setTileMapNum = (15 * m_tileSizeY + 0);

		tempHero.t_attType = tagHeroAttType::Near;
		tempHero.t_atkPoint = 10;
		tempHero.t_defPoint = 1;

		memset(&tempHero.t_Inven, 0, sizeof(tempHero.t_Inven));
		memset(&tempHero.t_Skill, 0, sizeof(tempHero.t_Skill));
		tempHero.t_Skill.t_fireWall = true;
		//tempHero.t_Skill.t_ArrowMagic = true;
		tempHero.t_Skill.t_haling = true;
		//tempHero.t_Skill.t_AtkBuff = true;
		//tempHero.t_Skill.t_DefBuff= true;
		tempHero.t_Skill.t_HasteBuff = true;

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
		setTxt.t_txtName = "´øÀü ÀÔ±¸";
		setTxt.t_txtInfo = "°Ç¹æÁø ¿ë»ç´Â ÀÌÂÊ¿¡¼­ µîÀåÇÕ´Ï´Ù!";
		break;
	case tagTileType::TOP:
		setTxt.t_txtName = "´øÀü ¿Üº®";
		setTxt.t_txtInfo = "ÀÎ°£µéÀº Àý´ë ¶ÕÀ» ¼ö ¾ø´Â ¸¶¹ýÀÇ º®ÀÔ´Ï´Ù ¾ÆÂü, ¸¶¿Õ´Ôµµ ºÒ°¡´ÉÇÕ´Ï´Ù.";
		break;
	case tagTileType::BLOCK:
		switch (m_tileset[tileNum].t_tierValue)
		{
		case eTileEnemy::Slime:
			setTxt.t_txtName = "¾×±« ¼ÒÈ¯¼®";
			setTxt.t_txtInfo = "ÀÌ ³à¼®ÀÇ ¾×Ã¼´Â ÇÇºÎ ¹Ì¿ë¿¡ Å¹¿ùÇÏ´Ù°í ÇÕ´Ï´Ù.";
			setTxt.t_TileEnemyName = "¾×±«";
			break;
		case eTileEnemy::Bug:
			setTxt.t_txtName = "º¼·¹ ¼ÒÈ¯¼®";
			setTxt.t_txtInfo = "ÀÎ°£°è °ïÃæ°ú´Â Â÷¿øÀÌ ´Ù¸¥ ³à¼®ÀÌÁÒ!";
			setTxt.t_TileEnemyName = "º¼·¹";
			break;
		case eTileEnemy::Lizardman:
			setTxt.t_txtName = "²¿º¹ÀÌ ¼ÒÈ¯¼®";
			setTxt.t_txtInfo = "¾îµð¼­ ÁÖ¿ü´ÂÁö ¹æÆÐ±îÁö °®Ãá °­·ÂÇÑ º¸º´ ÀÔ´Ï´Ù!";
			setTxt.t_TileEnemyName = "²¿º¹ÀÌ";
			break;
		case eTileEnemy::Lili:
			setTxt.t_txtName = "¸±¸® ¼ÒÈ¯¼®";
			setTxt.t_txtInfo = "Ãµ¹æÁöÃàÀÌ¶ó, ¿ë»ç¸¦ ±«·ÓÈ÷´Âµ¥´Â ÃÖ°íÁÒ!";
			setTxt.t_TileEnemyName = "¸±¸®";
			break;
		case eTileEnemy::Demon:
			setTxt.t_txtName = "µ¢Ä¡ ¼ÒÈ¯¼®";
			setTxt.t_txtInfo = "¹«¼·°í °­·ÂÇÑ ³à¼®ÀÔ´Ï´Ù¸¸.. ³Ê¹« Ä¿¼­ ¹®Á¦ÁÒ. Á¼Àº ±æÀº ¸ø³ª°©´Ï´Ù.";
			setTxt.t_TileEnemyName = "µ¥¸ó";
			break;
		case eTileEnemy::Mamon:
			setTxt.t_txtName = "¿Ü´«¹ÚÀÌ ¼ÒÈ¯¼®";
			setTxt.t_txtInfo = "¿ä³à¼®ÀÇ °­·ÂÇÑ ­h °ø°ÝÀº »ó´ë¸¦ ±âÀý½ÃÅ³ Á¤µµ¶ó´õ±º¿ä.";
			setTxt.t_TileEnemyName = "¿Ü´«¹ÚÀÌ";
			break;
		case eTileEnemy::Lady:
			setTxt.t_txtName = "ºÎ²ô¸¶¸¶ ¼ÒÈ¯¼®";
			setTxt.t_txtInfo = "³¸°¡¸²ÀÌ ½ÉÇØ¼­ ¿ë»ç°¡ ´Ù°¡¿À¸é ¼û¾î¹ö¸°´Ù°í ÇÏ´õ±º¿ä.";
			setTxt.t_TileEnemyName = "¿Ü´«¹ÚÀÌ";
			break;
		default:
			setTxt.t_txtName = "Èë ºí·°";
			setTxt.t_txtInfo = "±×Àú Èë³¿»õ¸¸ ³ª´Â Æò¹üÇÑ Èëºí·° ÀÔ´Ï´Ù";
			setTxt.t_TileEnemyName = "¾øÀ½";
			break;
		}

		break;
	case tagTileType::RAND:
		setTxt.t_txtName = "µ¹ ¹Ù´Ú";
		setTxt.t_txtInfo = "´Ü´ÜÇÑ ¹Ù´ÚÀÔ´Ï´Ù.";
		break;
	case tagTileType::BLOCK_NON:
		setTxt.t_txtName = "¹«Áö´Ü´Ü";
		setTxt.t_txtInfo = "ÀÌ°Ç ¸¶¿Õ´Ôµµ ¾îÂ¿ ¼ö ¾øÀ» °Í °°½À´Ï´Ù..";
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

	//// ±âº» ¼ÂÆÃ
	tagEnemyData tempEnemy;
	tempEnemy.t_isAilve = true;
	tempEnemy.t_posX = posX + CAMERA->getCamPosX();
	tempEnemy.t_posY = posY + CAMERA->getCamPosY();
	tempEnemy.t_tilePosX = tileX;
	tempEnemy.t_tilePosY = tileY;
	tempEnemy.t_setTileMapNum = setTileNum;

	// TileLv¿¡ µû¸¥ Ã³¸®
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

		tempEnemy.t_currHp = 65;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_atkPoint = 8;
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

void tileMap::tileItemGet(int tileValue)
{
	if (RANDOM->getInt(100) <= 10)
	{
		SOUNDMANAGER->play("Sound/SE/getGam.wav");

		OBJECTMANAGER->addTileDesObj(tagObjectType::ITEM_JEWEL,
			m_tileset[tileValue].t_rc.left + (TILE_SIZE / 2),
			m_tileset[tileValue].t_rc.top + (TILE_SIZE / 2), dropItemSet(tagItemType::JEWEL));
		m_pPlayer->t_TileDesEne += m_tItemInfo.t_frameY + 1;
	}
}

tileMap::tileMap()
{
}


tileMap::~tileMap()
{
}
