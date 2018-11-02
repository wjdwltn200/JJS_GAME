#include "stdafx.h"
#include "tileMap.h"
#include "uiPopup.h"
#include "uiManager.h"
#include "enemyManager.h"
#include "heroManager.h"


HRESULT tileMap::init(int tileX, int tileY,
	PlayerInfo * playerData, uiManager * uiMagData,
	enemyManager * pEnemyMag, heroManager * pHeroMag)
{
	//// Enemy & Hero �Ŵ��� �ּ� �ʱ�ȭ
	m_pEnemyMag = pEnemyMag;
	m_pHeroMag = pHeroMag;

	//// UI �Ŵ��� �ּ� �ʱ�ȭ
	m_pUiMag = uiMagData;
	m_pTilePopup = m_pUiMag->addPopup(NULL, NULL, NULL, NULL);

	//// �÷��̾� ������ �ּ� ����
	m_pPlayer = playerData;

	//// ���콺 �ʱ�ȭ
	m_isTileClick = false;
	
	//// Ÿ�� �ʱ�ȭ ����
	m_tileDesDaley = 0;
	m_tileSizeX = tileX;
	m_tileSizeY = tileY;
	m_tileSizeMaxX = tileX * TILE_SIZE;
	m_tileSizeMaxY = tileY * TILE_SIZE;
	IMAGEMANAGER->addImage("TileSet", "image/inGameImg/TILE/TileSet_Terrain.bmp", 160, 128, 5, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileSet_1", "image/inGameImg/TILE/TileSet_Terrain_1.bmp", 160, 128, 5, 4, true, RGB(255, 0, 255));

	
	IMAGEMANAGER->addImage("TileShadow", "image/inGameImg/TILE/TileSet_Terrain_Shadow.bmp", 6, 32);

	//// Ÿ�� �ʱ�ȭ
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
			m_tileset[x * m_tileSizeY + y].t_rc = RectMake(
				x * TILE_SIZE - CAMERA->getCamPosX(),
				y * TILE_SIZE - CAMERA->getCamPosY(),
				TILE_SIZE,
				TILE_SIZE
			);

			m_tileset[x * m_tileSizeY + y].t_setX = x;
			m_tileset[x * m_tileSizeY + y].t_setY = y;
			m_tileset[x * m_tileSizeY + y].t_enemyInfo = nullptr;

			int TempRandom = RANDOM->getFromIntTo(1, 100);

			if ((x + 1) == (m_tileSizeX / 2) && y == 0)
			{
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet");
				m_tileset[x * m_tileSizeY + y].t_setImg = 4;
				m_tileset[x * m_tileSizeY + y].t_isStart = true;
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::START;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;

				//// ��� �ӽ� ���
				tagHeroData tempHero;
				tempHero.t_img = IMAGEMANAGER->findImage("hero_00");
				tempHero.t_isAilve = true;
				tempHero.t_currHp = 10;
				tempHero.t_MaxHp = tempHero.t_currHp;
				tempHero.t_posX = m_tileset[x * m_tileSizeY + y].t_rc.left + TILE_SIZE / 2;
				tempHero.t_posY = m_tileset[x * m_tileSizeY + y].t_rc.top + TILE_SIZE / 2;
				tempHero.t_scale = 2.0f;
				tempHero.t_setTileMapNum = (x * m_tileSizeY + y);
				tempHero.t_tilePosX = x;
				tempHero.t_tilePosY = y;

				m_pHeroMag->heroDrop(&tempHero);

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
				m_tileset[x * m_tileSizeY + y].t_img = IMAGEMANAGER->findImage("TileSet_1");
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 4);
				m_tileset[x * m_tileSizeY + y].t_isNetDes = false;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::BLOCK;
				m_tileset[x * m_tileSizeY + y].t_ManaValue = RANDOM->getFromIntTo(1, 4);
			}

			
		}
	}

	//// ī�޶� ���� ��ġ ����
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
			//// Ÿ�� ����
			if (m_tileset[x * m_tileSizeY + y].t_ShakingCount >= TILE_SHAKING_COUNT)
			{
				m_tileset[x * m_tileSizeY + y].t_ShakingCount = 0;
				m_tileset[x * m_tileSizeY + y].t_isShaking = false;
			}

			if (m_tileset[x * m_tileSizeY + y].t_isShaking)
			{
				m_tileset[x * m_tileSizeY + y].t_ShakingCount++;
			}


			//// Ÿ�� ��ȣ�ۿ�
			//// Ÿ�� �˾� ���
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

			//// Ÿ�� �ı�
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
					OBJECTMANAGER->addTileDesObj(tagObjectType::ITEM_JEWEL,
						m_tileset[x * m_tileSizeY + y].t_rc.left + (TILE_SIZE / 2),
						m_tileset[x * m_tileSizeY + y].t_rc.top + (TILE_SIZE / 2), dropItemSet(tagItemType::JEWEL));
					m_pPlayer->t_TileDesEne += m_tItemInfo.t_frameY + 1;
				}
				else
				{
					monsSetDrop(m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, (x * m_tileSizeY + y), x, y);
					m_tileset[x * m_tileSizeY + y].t_ManaValue = -1;
					//monsSetDrop(tagEnemyType::Spider ,m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, (x * m_tileSizeY + y), x, y);
				}
				EFFMANAGER->play("MousePointEFF", m_tileset[x * m_tileSizeY + y].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromIntTo(-5, 5)));
				m_tileDesDaley = TILE_DES_DALEY;
			}
			//// ������ ���� Ÿ�� ����
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
	//// txt �̸� ����
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

			//// Ÿ�� ���¿� ���� ������
			if (m_tileset[x * m_tileSizeY + y].t_rc.left < -TILE_SIZE || m_tileset[x * m_tileSizeY + y].t_rc.left > WINSIZEX) continue;
			if (m_tileset[x * m_tileSizeY + y].t_rc.top < -TILE_SIZE || m_tileset[x * m_tileSizeY + y].t_rc.top > WINSIZEY) continue;

			if (m_tileset[x * m_tileSizeY + y].t_isAlive)
			{
				if (x * m_tileSizeY + y == x * m_tileSizeY + y * m_tileSizeY || x == 0 || x == m_tileSizeX - 1 || y == m_tileSizeY - 1)  //// �� �� �� Ÿ��)
				{
					m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 1, TILE_SCALE, false);
				}
				else //// �ı� ���� Ÿ��
				{
					if (m_tileset[x * m_tileSizeY + y].t_isShaking)
					{
						switch (m_tileset[x * m_tileSizeY + y].t_enumType)
						{
						case tagTileType::BLOCK:
							m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 0, TILE_SCALE, false);
							break;
						case tagTileType::BLOCK_NON:
							m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 3, TILE_SCALE, false);
							break;
						}
					}
					else
					{
						switch (m_tileset[x * m_tileSizeY + y].t_enumType)
						{
						case tagTileType::BLOCK:
							tileImgSet(x * m_tileSizeY + y, hdc);
							break;
						case tagTileType::BLOCK_NON:
							m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 3, TILE_SCALE, false);
							break;
						}
					}
				}
			}
			else //// �ı��� Ÿ��
			{
				m_tileset[x * m_tileSizeY + y].t_img->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 2, TILE_SCALE, false);
			}
			
			//// Ÿ�� �׸��� ����
			if (!m_tileset[x * m_tileSizeY + y].t_isAlive && m_tileset[((x - 1) * m_tileSizeY + y)].t_isAlive)
				IMAGEMANAGER->findImage("TileShadow")->alphaRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, TILE_ALPHA_VALUE);


			//// ����� ����
			if (g_saveData.gisTest)
			{
				Rectangle(hdc,
					m_tileset[x * m_tileSizeY + y].t_rc.left,
					m_tileset[x * m_tileSizeY + y].t_rc.top,
					m_tileset[x * m_tileSizeY + y].t_rc.right,
					m_tileset[x * m_tileSizeY + y].t_rc.bottom
				);

				//// Ÿ������ ����� ����
				sprintf_s(szText, "%d", TestTileNum);
				TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
				sprintf_s(szText, "%d, %d", m_tileset[x * m_tileSizeY + y].t_setX, m_tileset[x * m_tileSizeY + y].t_setY);
				TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top + 12.0f, szText, strlen(szText));
				//sprintf_s(szText, "%d, %d", m_tileset[x * m_tileSizeY + y].t_isAlive, m_tileset[x * m_tileSizeY + y].t_isShaking);
				//TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top + 12.0f, szText, strlen(szText));
			}
			TestTileNum++;
		}
	}


}

void tileMap::keyInput()
{
	//// ���콺 ������ ���� ī�޶� �̵�
	if (g_ptMouse.x <= (WINSIZEX - WINSIZEX) + 5)
		CAMERA->setCamPosX(CAMERA->getCamPosX() - 10.0f);
	if (g_ptMouse.x >= (WINSIZEX) - 5)
		CAMERA->setCamPosX(CAMERA->getCamPosX() + 10.0f);
	if (g_ptMouse.y <= (WINSIZEY - WINSIZEY) + 5)
		CAMERA->setCamPosY(CAMERA->getCamPosY() - 10.0f);
	if (g_ptMouse.y >= (WINSIZEY) - 5)
		CAMERA->setCamPosY(CAMERA->getCamPosY() + 10.0f);

	//// Ű �Է� ���� ī�޶� �̵�
	if (KEYMANAGER->isStayKeyDown('W'))
		CAMERA->setCamPosY(CAMERA->getCamPosY() - 3.0f);
	if (KEYMANAGER->isStayKeyDown('S'))
		CAMERA->setCamPosY(CAMERA->getCamPosY() + 3.0f);
	if (KEYMANAGER->isStayKeyDown('A'))
		CAMERA->setCamPosX(CAMERA->getCamPosX() - 3.0f);
	if (KEYMANAGER->isStayKeyDown('D'))
		CAMERA->setCamPosX(CAMERA->getCamPosX() + 3.0f);


}

void tileMap::tileSetTxt(int tileType, int tileNum)
{
	tagTileTxt setTxt;
	setTxt.t_TileMana = m_tileset[tileNum].t_ManaValue;

	switch (tileType)
	{
	case tagTileType::START:
		setTxt.t_txtName = "���� �Ա�";
		setTxt.t_txtInfo = "�ǹ��� ���� ���ʿ��� �����մϴ�!";
		break;
	case tagTileType::TOP:
		setTxt.t_txtName = "���� �ܺ�";
		setTxt.t_txtInfo = "�ΰ����� ���� ���� �� ���� ������ ���Դϴ� ����, ���մԵ� �Ұ����մϴ�.";
		break;
	case tagTileType::BLOCK:
		setTxt.t_txtName = "�� ��";
		setTxt.t_txtInfo = "�μ��ٺ��� ���� Ƣ����� �������?";
		if (setTxt.t_TileMana < 3)
		{
			setTxt.t_TileEnemyName = "��";
		}
		break;
	case tagTileType::RAND:
		setTxt.t_txtName = "�� �ٴ�";
		setTxt.t_txtInfo = "�ܴ��� �ٴ��Դϴ�.";
		break;
	case tagTileType::BLOCK_NON:
		setTxt.t_txtName = "�����ܴ� ��";
		setTxt.t_txtInfo = "�̰� ���մԵ� ��¿ �� ���� �� �����ϴ�..";
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
		break;
	}
}

void tileMap::monsSetDrop(float posX, float posY, int setTileNum, int tileX, int tileY)
{
	tagEnemyData tempEnemy;
	
	if (m_tileset[setTileNum].t_ManaValue >= 6)
	{
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Daemon_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Daemon_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Daemon_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 100;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.7f;
		tempEnemy.t_moveDaley = 120;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 10;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_enumType = tagEnemyType::Daemon;
	}
	else if (m_tileset[setTileNum].t_ManaValue >= 5)
	{
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Bat_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Bat_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Bat_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 100;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.7f;
		tempEnemy.t_moveDaley = 10;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 10;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_enumType = tagEnemyType::Spider;
	}
	else if (m_tileset[setTileNum].t_ManaValue >= 4)
	{
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Rat_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Rat_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Rat_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 10;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 1.0f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 10;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_FoodChainLv = 1;
		tempEnemy.t_enumType = tagEnemyType::Rat;
		
	}
	else if (m_tileset[setTileNum].t_ManaValue <= 3)
	{
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Slime_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Slime_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Slime_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 30;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 0;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 3;
		tempEnemy.t_defPoint = 0;
		tempEnemy.t_FoodChainLv = 0;
		tempEnemy.t_enumType = tagEnemyType::Slime;
	}

	/*switch (monType)
	{
	case tagEnemyType::Skeleton:
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Skeleton_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Skeleton_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Skeleton_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 50;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 1.0f;
		tempEnemy.t_moveDaley = 60;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 5;
		tempEnemy.t_defPoint = 0;
		tempEnemy.t_enumType = monType;
		break;
	case tagEnemyType::Slime:
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Slime_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Slime_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Slime_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 30;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.5f;
		tempEnemy.t_moveDaley = 5;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 3;
		tempEnemy.t_defPoint = 0;
		tempEnemy.t_enumType = monType;
		break;
	case tagEnemyType::Daemon:

		break;
	case tagEnemyType::Bat:

		break;
	case tagEnemyType::Rat:
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Rat_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Rat_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Rat_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 100;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.7f;
		tempEnemy.t_moveDaley = 10;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 10;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_enumType = monType;
		break;
	case tagEnemyType::Spider:
		tempEnemy.t_img_L = IMAGEMANAGER->findImage("Spider_00_L");
		tempEnemy.t_img_R = IMAGEMANAGER->findImage("Spider_00_R");
		tempEnemy.t_img_D = IMAGEMANAGER->findImage("Spider_00_D");
		tempEnemy.t_isAilve = true;
		tempEnemy.t_currHp = 100;
		tempEnemy.t_MaxHp = tempEnemy.t_currHp;
		tempEnemy.t_posX = posX + CAMERA->getCamPosX();
		tempEnemy.t_posY = posY + CAMERA->getCamPosY();
		tempEnemy.t_tilePosX = tileX;
		tempEnemy.t_tilePosY = tileY;
		tempEnemy.t_scale = 2.0f;
		tempEnemy.t_moveSpeed = 0.7f;
		tempEnemy.t_moveDaley = 10;
		tempEnemy.t_setTileMapNum = setTileNum;
		tempEnemy.t_atkPoint = 10;
		tempEnemy.t_defPoint = 3;
		tempEnemy.t_enumType = monType;
		break;
	}*/


	m_pEnemyMag->enemyDrop(&tempEnemy);
}

void tileMap::tileImgSet(int tileNum, HDC hdc)
{
	if (m_tileset[tileNum].t_ManaValue <= 3)
		m_tileset[tileNum].t_img->frameRender(hdc, m_tileset[tileNum].t_rc.left, m_tileset[tileNum].t_rc.top, m_tileset[tileNum].t_setImg, 0, TILE_SCALE, false);
	
	if (m_tileset[tileNum].t_ManaValue >= 4)
		m_tileset[tileNum].t_img->frameRender(hdc, m_tileset[tileNum].t_rc.left, m_tileset[tileNum].t_rc.top, m_tileset[tileNum].t_setImg, 1, TILE_SCALE, false);
	
	if (m_tileset[tileNum].t_ManaValue >= 5)
		m_tileset[tileNum].t_img->frameRender(hdc, m_tileset[tileNum].t_rc.left, m_tileset[tileNum].t_rc.top, m_tileset[tileNum].t_setImg, 2, TILE_SCALE, false);
	
	if (m_tileset[tileNum].t_ManaValue >= 6)
		m_tileset[tileNum].t_img->frameRender(hdc, m_tileset[tileNum].t_rc.left, m_tileset[tileNum].t_rc.top, m_tileset[tileNum].t_setImg, 3, TILE_SCALE, false);
}

tileMap::tileMap()
{
}


tileMap::~tileMap()
{
}
