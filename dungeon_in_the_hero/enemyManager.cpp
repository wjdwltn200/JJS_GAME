#include "stdafx.h"
#include "enemyManager.h"
#include "enemy.h"
#include "uiManager.h"
#include "tileMap.h"

HRESULT enemyManager::init(tileMap * pTileMag, uiManager * pUiMag, bulletManager * pBulletMag)
{

	m_isAliveVec = 0;
	m_maxTileY = 0;
	m_enemyPower = 0;

	for (int i = 0; i < SET_ENEMY_VEC; i++)
	{
		enemy * tempEnemy = new enemy;
		m_vecEnemy.push_back(tempEnemy);

		m_vecTileY.push_back(nullptr);
	}

	m_pTileMap = pTileMag;
	m_pUiMag = pUiMag;
	m_pBulletMag = pBulletMag;
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
	if ((m_pTileMap->getGameState() == eGameState::GameGetSet)) return;

	int tempIsAlive = 0;
	m_maxTileY = 0;
	m_enemyPower = 0;
	m_isEnemyInfoPopup = false;
	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		if (!(*m_iter)->gettEnemyData().t_isAilve) continue;

		tempIsAlive++;
		(*m_iter)->update();
		if ((*m_iter)->gettEnemyData().t_tilePosY >= m_maxTileY)
			m_maxTileY = (*m_iter)->gettEnemyData().t_tilePosY;

		m_enemyPower += (*m_iter)->gettEnemyData().t_powerValue;
	}
	m_isAliveVec = tempIsAlive;
}

bool enemyManager::comp(enemy* a, enemy* b)
{
	return (a->gettEnemyData().t_tilePosY < b->gettEnemyData().t_tilePosY);
}

void enemyManager::render(HDC hdc)
{
	sort(m_vecEnemy.begin(), m_vecEnemy.end(), enemyManager::comp);

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

		(*m_iter)->init(enemyInfo, m_pTileMap, this, m_pBulletMag);
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
