#include "stdafx.h"
#include "enemyManager.h"
#include "enemy.h"

HRESULT enemyManager::init(tileMap * pTileMag)
{
	m_pTileMap = pTileMag;

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

		(*m_iter)->init(enemyInfo, m_pTileMap);
		return;
	}

	enemy * tempEnemy = new enemy;
	tempEnemy->init(enemyInfo, m_pTileMap);
	m_vecEnemy.push_back(tempEnemy);
}

enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}
