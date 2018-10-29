#include "stdafx.h"
#include "enemy.h"
#include "animation.h"


HRESULT enemy::init(tagEnemyData* enemyInfo)
{
	m_tEnemyData.t_isAilve = enemyInfo->t_isAilve;
	m_tEnemyData.t_posX = enemyInfo->t_posX;
	m_tEnemyData.t_posY = enemyInfo->t_posY;

	m_tEnemyData.t_img = enemyInfo->t_img;
	m_tEnemyData.t_scale = enemyInfo->t_scale;
	m_tEnemyData.t_alphaValue = enemyInfo->t_alphaValue = 255;
	m_ani.init(m_tEnemyData.t_img->getWidth(), m_tEnemyData.t_img->getHeight(), m_tEnemyData.t_img->getFrameWidth(), m_tEnemyData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(15);

	m_tEnemyData.t_currHp = enemyInfo->t_currHp;
	m_tEnemyData.t_MaxHp = enemyInfo->t_MaxHp;
	m_tEnemyData.t_currDef = enemyInfo->t_currDef;

	m_tEnemyData.t_moveType = enemyInfo->t_moveType;
	m_tEnemyData.t_moveDaley = enemyInfo->t_moveDaley;

	m_tEnemyData.t_attType = enemyInfo->t_attType;
	m_tEnemyData.t_atkDaley = enemyInfo->t_atkDaley;


	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{

	m_ani.frameUpdate();
}

void enemy::render(HDC hdc)
{
	m_tEnemyData.t_img->aniRender(hdc, m_tEnemyData.t_posX, m_tEnemyData.t_posY, &m_ani, m_tEnemyData.t_scale, true, m_tEnemyData.t_alphaValue);
}

enemy::enemy()
{
}


enemy::~enemy()
{
}
