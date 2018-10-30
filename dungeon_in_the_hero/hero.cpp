#include "stdafx.h"
#include "hero.h"
#include "tileMap.h"
#include "aStarNode.h"
#include "aStarPathFinding.h"

HRESULT hero::init(tagHeroData * heroInfo, tileMap * pTileMap)
{
	//// 타일 맵 주소 초기화
	m_pTileMapMag = pTileMap;

	m_tHeroData.t_isAilve = heroInfo->t_isAilve;
	m_tHeroData.t_posX = heroInfo->t_posX + CAMERA->getCamPosX();
	m_tHeroData.t_posY = heroInfo->t_posY + CAMERA->getCamPosY();

	m_tHeroData.t_img = heroInfo->t_img;
	m_tHeroData.t_scale = heroInfo->t_scale;
	m_tHeroData.t_alphaValue = heroInfo->t_alphaValue = 255;
	m_ani.init(m_tHeroData.t_img->getWidth(), m_tHeroData.t_img->getHeight(), m_tHeroData.t_img->getFrameWidth(), m_tHeroData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(15);
	m_ani.start();

	m_tHeroData.t_currHp = heroInfo->t_currHp;
	m_tHeroData.t_MaxHp = heroInfo->t_MaxHp;
	m_tHeroData.t_currDef = heroInfo->t_currDef;

	m_tHeroData.t_moveType = heroInfo->t_moveType;
	m_tHeroData.t_moveDaley = heroInfo->t_moveDaley;

	m_tHeroData.t_attType = heroInfo->t_attType;
	m_tHeroData.t_atkDaley = heroInfo->t_atkDaley;

	m_tHeroData.t_setTileMapNum = heroInfo->t_setTileMapNum;

	m_moveDaley = HERO_MOVE_DALEY;
	m_eMoveState = eMoveState::MOVE_NUM;

	m_eMoveState = RANDOM->getFromIntTo(0, 3);
	m_isMoveAct = true;

	aStarNodeStart();
	return S_OK;
}

void hero::release()
{
}

void hero::update()
{
}

void hero::render(HDC hdc)
{
		m_tHeroData.t_img->aniRender(hdc,
		(m_tHeroData.t_posX - m_tHeroData.t_img->getFrameWidth()) - CAMERA->getCamPosX(),
		(m_tHeroData.t_posY - m_tHeroData.t_img->getFrameHeight() - IMG_TILE_SET_Y) - CAMERA->getCamPosY(),
		&m_ani, m_tHeroData.t_scale, true, m_tHeroData.t_alphaValue);
}

void hero::aStarNodeStart()
{
}

bool hero::moveRectCheck(int eMoveArrow)
{
	switch (m_eMoveState)
	{
	case eMoveState::UP:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum - 1].t_isAlive) return false;
		break;
	case eMoveState::DOWN:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum + 1].t_isAlive) return false;
		break;
	case eMoveState::LEFT:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum - (m_pTileMapMag->getTileSizeY())].t_isAlive) return false;
		break;
	case eMoveState::RIGHT:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum + (m_pTileMapMag->getTileSizeY())].t_isAlive) return false;
		break;
	}

	return true;
}

hero::hero()
{
}


hero::~hero()
{
}
