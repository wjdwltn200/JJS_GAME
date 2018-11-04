#include "stdafx.h"
#include "enemyManager.h"
#include "enemy.h"
#include "uiManager.h"

HRESULT enemyManager::init(tileMap * pTileMag, uiManager * pUiMag)
{
	for (int i = 0; i < SET_ENEMY_VEC; i++)
	{
		enemy * tempEnemy = new enemy;
		m_vecEnemy.push_back(tempEnemy);
	}
	m_pTileMap = pTileMag;
	m_pUiMag = pUiMag;
	m_pEnemyPopup = pUiMag->addPopup(NULL, NULL, NULL, NULL);
	m_isEnemyInfoPopup = false;
	return S_OK;
}

void enemyManager::release()
{
	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		delete (*m_iter);
	}
	m_vecEnemy.clear();
}

void enemyManager::update()
{
	m_isEnemyInfoPopup = false;
	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		if (!(*m_iter)->gettEnemyData().t_isAilve) continue;

		(*m_iter)->update();
	}
}

void enemyManager::render(HDC hdc)
{
	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		if (!(*m_iter)->gettEnemyData().t_isAilve) continue;

		(*m_iter)->render(hdc);
	}
}

void enemyManager::enemyDrop(tagEnemyData * enemyInfo)
{
	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		if ((*m_iter)->gettEnemyData().t_isAilve) continue;

		(*m_iter)->init(enemyInfo, m_pTileMap, this);
		return;
	}

	//enemy * tempEnemy = new enemy;
	//tempEnemy->init(enemyInfo, m_pTileMap, this);
	//m_vecEnemy.push_back(tempEnemy);
}

enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}
