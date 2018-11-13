#include "stdafx.h"
#include "overlord.h"
#include "tileMap.h"

HRESULT overlord::init(tileMap * pTileMap)
{
	m_pTileMap = pTileMap;
	m_tOverlord.t_alphaValue = 255;
	m_tOverlord.t_img = IMAGEMANAGER->findImage("Overlord");
	m_ani.init(	m_tOverlord.t_img->getWidth(), m_tOverlord.t_img->getHeight(), m_tOverlord.t_img->getFrameWidth(), m_tOverlord.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(15);
	m_ani.start();

	m_tOverlord.t_isAlive = true;
	m_tOverlord.t_isGet = false;

	m_tOverlord.t_posX = pTileMap->getTileSetPoint()[15 * pTileMap->getTileSizeY() + 4].t_rc.left;
	m_tOverlord.t_posY = pTileMap->getTileSetPoint()[15 * pTileMap->getTileSizeY() + 4].t_rc.top;
	m_tOverlord.t_moveEndX = m_tOverlord.t_posX;
	m_tOverlord.t_moveEndY = m_tOverlord.t_posY;

	m_tOverlord.t_scale = 2.0f;
	m_tOverlord.t_tilePosX = TILE_OVERLORD_X;
	m_tOverlord.t_tilePosY = TILE_OVERLORD_Y;

	memset(&m_tOverlord.t_rc, 0, sizeof(m_tOverlord.t_rc));

	m_isSetting = false;

	return S_OK;
}

void overlord::release()
{
}

void overlord::update()
{

	if (!SOUNDMANAGER->isPlaying("Sound/BGM/BGM_GameStart.wav") &&
		(m_pTileMap->getGameState() == eGameState::GameGetSet) &&
		!SOUNDMANAGER->isPlaying("Sound/BGM/BGM_UnStart.wav"))
		SOUNDMANAGER->play("Sound/BGM/BGM_UnStart.wav", 0.5f);

	RECT tempRc;
	if (!SOUNDMANAGER->isPlaying("Sound/BGM/BGM_GameStart.wav") &&
		(m_pTileMap->getGameState() == eGameState::GameGetSet) &&
		KEYMANAGER->isOnceKeyDown(VK_RBUTTON) &&
		(IntersectRect(&tempRc, &m_tOverlord.t_rc, &g_MouseRc)))
	{
		m_isSetting = true;
	}

	m_ani.frameUpdate();
}

void overlord::render(HDC hdc)
{
	if (m_isSetting)
	{
		m_tOverlord.t_rc = RectMake(
			g_MouseRc.left - (TILE_SIZE / 2),
			g_MouseRc.top - (TILE_SIZE / 2),
			TILE_SIZE,
			TILE_SIZE);

		m_tOverlord.t_posX = g_MouseRc.left;
		m_tOverlord.t_posY = g_MouseRc.top;
	}
	else
	{
		m_tOverlord.t_rc = RectMake(
			m_tOverlord.t_posX - CAMERA->getCamPosX(),
			m_tOverlord.t_posY - CAMERA->getCamPosY(),
			TILE_SIZE,
			TILE_SIZE);
	}


	// 스크린 랜더 조건
	if (!(m_tOverlord.t_rc.left < -TILE_SIZE || m_tOverlord.t_rc.left > WINSIZEX) &&
		!(m_tOverlord.t_rc.top < -TILE_SIZE || m_tOverlord.t_rc.top > WINSIZEY))
	{
		if (m_isSetting)
		{
			m_tOverlord.t_img->aniRender(hdc,
				(m_tOverlord.t_posX + (TILE_SIZE / 2) - (m_tOverlord.t_img->getFrameWidth() * m_tOverlord.t_scale) / 2),
				((m_tOverlord.t_posY + TILE_SIZE) - (m_tOverlord.t_img->getFrameHeight() * m_tOverlord.t_scale)),
				&m_ani, m_tOverlord.t_scale, true, m_tOverlord.t_alphaValue);
		}
		else
		{
			m_tOverlord.t_img->aniRender(hdc,
				(m_tOverlord.t_posX + (TILE_SIZE / 2) - (m_tOverlord.t_img->getFrameWidth() * m_tOverlord.t_scale) / 2) - CAMERA->getCamPosX(),
				((m_tOverlord.t_posY + TILE_SIZE) - (m_tOverlord.t_img->getFrameHeight() * m_tOverlord.t_scale)) - CAMERA->getCamPosY(),
				&m_ani, m_tOverlord.t_scale, true, m_tOverlord.t_alphaValue);
		}
	}

	if (g_saveData.gisTest)
	{
		Rectangle(hdc, m_tOverlord.t_rc.left, m_tOverlord.t_rc.top, m_tOverlord.t_rc.right, m_tOverlord.t_rc.bottom);

		char szText[256];
		sprintf_s(szText, "%d,%d", m_tOverlord.t_tilePosX, m_tOverlord.t_tilePosY);
		TextOut(hdc, m_tOverlord.t_posX - CAMERA->getCamPosX(), m_tOverlord.t_posY - CAMERA->getCamPosY(), szText, strlen(szText));
	}
}

void overlord::setTileXY(int tileX, int tileY, float posX, float posY)
{
	m_tOverlord.t_tilePosX = tileX;
	m_tOverlord.t_tilePosY = tileY;
	m_tOverlord.t_posX = posX + CAMERA->getCamPosX();
	m_tOverlord.t_posY = posY + CAMERA->getCamPosY();

	m_isSetting = false;
	return;
}

overlord::overlord()
{
}


overlord::~overlord()
{
}
