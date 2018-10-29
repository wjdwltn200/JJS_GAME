#include "stdafx.h"
#include "tileMap.h"
#include "uiPopup.h"
#include "uiManager.h"
#include "enemyManager.h"


HRESULT tileMap::init(int tileX, int tileY,
	PlayerInfo * playerData, uiManager * uiMagData, enemyManager * pEnemyMag)
{
	//// Enemy 매니저 주소 초기화
	m_pEnemyMag = pEnemyMag;


	//// UI 매니저 주소 초기화
	m_pUiMag = uiMagData;
	m_pTilePopup = m_pUiMag->addPopup(NULL, NULL, NULL, NULL);

	//// 플레이어 데이터 주소 저장
	m_pPlayer = playerData;

	//// 마우스 초기화
	m_isTileClick = false;
	
	//// 타일 초기화 셋팅
	m_tileDesDaley = 0;
	m_isTilePopup = false;
	m_tileSizeX = tileX;
	m_tileSizeY = tileY;
	m_tileSizeMaxX = tileX * TILE_SIZE;
	m_tileSizeMaxY = tileY * TILE_SIZE;
	IMAGEMANAGER->addImage("TileSet", "image/inGameImg/TILE/TileSet_Terrain.bmp", 160, 128, 5, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TileShadow", "image/inGameImg/TILE/TileSet_Terrain_Shadow.bmp", 6, 32);

	//// 타일 초기화
	for (int x = 0; x < m_tileSizeX; x++)
	{
		for (int y = 0; y < m_tileSizeY; y++)
		{
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

			int TempRandom = RANDOM->getFromIntTo(1, 100);

			if ((x + 1) == (m_tileSizeX / 2) && y == 0)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = 4;
				m_tileset[x * m_tileSizeY + y].t_isStart = true;
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::START;
			}
			else if (x <= m_tileSizeX && y == (y * m_tileSizeY) || x == 0 || x == m_tileSizeX - 1 || y == m_tileSizeY - 1)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 3);
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::TOP;
			}
			else if ((x + 1) == (m_tileSizeX / 2) && y <= TILE_SETNON)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 3);
				m_tileset[x * m_tileSizeY + y].t_isAlive = false;
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::RAND;
			}
			else if (TempRandom < TILE_NON_BLOCK && y > TILE_NON_BLOCK_SET_Y)
			{
				m_tileset[x * m_tileSizeY + y].t_setImg = RANDOM->getFromIntTo(0, 4);
				m_tileset[x * m_tileSizeY + y].t_isNetDes = true;
				m_tileset[x * m_tileSizeY + y].t_enumType = tagTileType::BLOCK_NON;
			}
			else
			{
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
			if (!m_isTilePopup &&
				KEYMANAGER->isStayKeyDown(VK_RBUTTON) &&
				(IntersectRect(&tempRc, &m_tileset[x * m_tileSizeY + y].t_rc, &g_MouseRc)))
			{
				tileSetTxt(m_tileset[x * m_tileSizeY + y].t_enumType);
				m_pTilePopup->setImg(IMAGEMANAGER->findImage("TilePopup"));
				m_pTilePopup->setDaley(TILE_POPUP_COUNT);
				m_pTilePopup->setIsAilve(true);
				m_pTilePopup->setXY(m_tileset[x * m_tileSizeY + y].t_rc.left + TILE_SIZE / 2, m_tileset[x * m_tileSizeY + y].t_rc.top + TILE_SIZE / 2 - TILE_POPUP_Y_SIZE);
				m_isTilePopup = true;
			}
			else if (m_isTilePopup && KEYMANAGER->isOnceKeyUp(VK_RBUTTON))
			{
				m_isTilePopup = false;
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
					monsSetDrop(m_tileset[x * m_tileSizeY + y].t_rc.left + (TILE_SIZE / 2), m_tileset[x * m_tileSizeY + y].t_rc.top + (TILE_SIZE / 2));
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
					IMAGEMANAGER->findImage("TileSet")->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 1, TILE_SCALE, false);
				}
				else //// 파괴 가능 타일
				{
					if (m_tileset[x * m_tileSizeY + y].t_isShaking)
					{
						switch (m_tileset[x * m_tileSizeY + y].t_enumType)
						{
						case tagTileType::BLOCK:
							IMAGEMANAGER->findImage("TileSet")->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 0, TILE_SCALE, false);
							break;
						case tagTileType::BLOCK_NON:
							IMAGEMANAGER->findImage("TileSet")->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left + (RANDOM->getFromIntTo(-5, 5)), m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 3, TILE_SCALE, false);
							break;
						}
					}
					else
					{
						switch (m_tileset[x * m_tileSizeY + y].t_enumType)
						{
						case tagTileType::BLOCK:
							IMAGEMANAGER->findImage("TileSet")->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 0, TILE_SCALE, false);
							break;
						case tagTileType::BLOCK_NON:
							IMAGEMANAGER->findImage("TileSet")->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 3, TILE_SCALE, false);
							break;
						}
					}
				}
			}
			else //// 파괴된 타일
			{
				IMAGEMANAGER->findImage("TileSet")->frameRender(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, m_tileset[x * m_tileSizeY + y].t_setImg, 2, TILE_SCALE, false);
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
				sprintf_s(szText, "%d", TestTileNum);
				TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top, szText, strlen(szText));
				sprintf_s(szText, "%d, %d", m_tileset[x * m_tileSizeY + y].t_isAlive, m_tileset[x * m_tileSizeY + y].t_isShaking);
				TextOut(hdc, m_tileset[x * m_tileSizeY + y].t_rc.left, m_tileset[x * m_tileSizeY + y].t_rc.top + 12.0f, szText, strlen(szText));
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


}

void tileMap::tileSetTxt(int tileType)
{
	tagTileTxt setTxt;

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
		setTxt.t_txtName = "흙 블럭";
		setTxt.t_txtInfo = "부수다보면 무언가 튀어나오지 않을까요?";
		break;
	case tagTileType::RAND:
		setTxt.t_txtName = "돌 바닥";
		setTxt.t_txtInfo = "단단한 바닥입니다.";
		break;
	case tagTileType::BLOCK_NON:
		setTxt.t_txtName = "무지단단 블럭";
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
		break;
	}
}

void tileMap::monsSetDrop(float posX, float posY)
{
	tagEnemyData tempEnemy;
	tempEnemy.t_img = IMAGEMANAGER->findImage("enemy_00");
	tempEnemy.t_isAilve = true;
	tempEnemy.t_currHp = 10;
	tempEnemy.t_MaxHp = tempEnemy.t_currHp;
	tempEnemy.t_posX = posX;
	tempEnemy.t_posY = posY;
	tempEnemy.t_scale = 2.0f;


	m_pEnemyMag->enemyDrop(&tempEnemy);
}

tileMap::tileMap()
{
}


tileMap::~tileMap()
{
}
