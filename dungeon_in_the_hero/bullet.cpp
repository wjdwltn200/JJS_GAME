#include "stdafx.h"
#include "bullet.h"
#include "tileMap.h"


HRESULT bullet::init(tagBullet * bulletInfo)
{
	//// bullet strect 초기화
	memset(&m_tBulletInfo, 0, sizeof(m_tBulletInfo));

	m_tBulletInfo = *bulletInfo;

	m_tBulletInfo.t_img = bulletInfo->t_img;
	m_ani.init(m_tBulletInfo.t_img->getWidth(), m_tBulletInfo.t_img->getHeight(), m_tBulletInfo.t_img->getFrameWidth(), m_tBulletInfo.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(15);
	m_ani.start();

	m_tBulletInfo.t_alphaValue = 255;
	memset(&m_tBulletInfo.t_rc, 0, sizeof(m_tBulletInfo.t_rc));

	return S_OK;
}

void bullet::release()
{
}

void bullet::update()
{
	if (!m_tBulletInfo.t_isAilve) return;

	movement();
	m_ani.frameUpdate();
}

void bullet::render(HDC hdc)
{
	if (!m_tBulletInfo.t_isAilve) return;

	if (g_saveData.gisTest)
	{
		Rectangle(hdc, m_tBulletInfo.t_rc.left, m_tBulletInfo.t_rc.top, m_tBulletInfo.t_rc.right, m_tBulletInfo.t_rc.bottom);

		//char szText[256];
		//sprintf_s(szText, "%d,%d", m_tBulletInfo.t_currMana, m_moveDaley);
		//TextOut(hdc, m_tEnemyData.t_posX - CAMERA->getCamPosX(), m_tEnemyData.t_posY - CAMERA->getCamPosY(), szText, strlen(szText));
	}

	// 스크린 랜더 조건
	if (!(m_tBulletInfo.t_rc.left < -TILE_SIZE || m_tBulletInfo.t_rc.left > WINSIZEX) &&
		!(m_tBulletInfo.t_rc.top < -TILE_SIZE || m_tBulletInfo.t_rc.top > WINSIZEY))
	{
		m_tBulletInfo.t_img->aniRender(hdc,
			(m_tBulletInfo.t_posX + (TILE_SIZE / 2) - (m_tBulletInfo.t_img->getFrameWidth() * m_tBulletInfo.t_scale) / 2) - CAMERA->getCamPosX(),
			((m_tBulletInfo.t_posY + TILE_SIZE) - (m_tBulletInfo.t_img->getFrameHeight() * m_tBulletInfo.t_scale)) - CAMERA->getCamPosY(),
			&m_ani, m_tBulletInfo.t_scale, true, m_tBulletInfo.t_alphaValue);
	}

	m_tBulletInfo.t_rc = RectMakeCenter(
		(m_tBulletInfo.t_posX + TILE_SIZE / 2) - CAMERA->getCamPosX(),
		(m_tBulletInfo.t_posY + TILE_SIZE / 2) - CAMERA->getCamPosY(),
		TILE_SIZE / 2,
		TILE_SIZE / 2);
}

void bullet::movement()
{
	switch (m_tBulletInfo.t_moveArrow)
	{
	case eMoveState::UP:
		m_tBulletInfo.t_posY -= m_tBulletInfo.t_moveSpeed;
		break;
	case eMoveState::RIGHT:
		m_tBulletInfo.t_posX += m_tBulletInfo.t_moveSpeed;
		break;
	case eMoveState::DOWN:
		m_tBulletInfo.t_posY += m_tBulletInfo.t_moveSpeed;
		break;
	case eMoveState::LEFT:
		m_tBulletInfo.t_posX -= m_tBulletInfo.t_moveSpeed;
		break;
	default:
		break;
	}
	m_tBulletInfo.t_range -= m_tBulletInfo.t_moveSpeed;
	if (m_tBulletInfo.t_range <= 0.0f)
		m_tBulletInfo.t_isAilve = false;
}

bullet::bullet()
{
}


bullet::~bullet()
{
}
