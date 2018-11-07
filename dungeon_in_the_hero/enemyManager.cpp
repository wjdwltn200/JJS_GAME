#include "stdafx.h"
#include "enemyManager.h"
#include "enemy.h"
#include "uiManager.h"
#include "tileMap.h"

HRESULT enemyManager::init(tileMap * pTileMag, uiManager * pUiMag, bulletManager * pBulletMag)
{

	m_isAliveVec = 0;
	m_maxTileY = 0;

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
	int tempIsAlive = 0;

	m_maxTileY = 0;
	m_isEnemyInfoPopup = false;
	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		if (!(*m_iter)->gettEnemyData().t_isAilve) continue;

		tempIsAlive++;
		(*m_iter)->update();
		if ((*m_iter)->gettEnemyData().t_tilePosY >= m_maxTileY)
			m_maxTileY = (*m_iter)->gettEnemyData().t_tilePosY;
	}
	m_isAliveVec = tempIsAlive;
}

bool enemyManager::comp(enemy* a, enemy* b)
{
	return (a->gettEnemyData().t_tilePosY < b->gettEnemyData().t_tilePosY);
}

void enemyManager::render(HDC hdc)
{
	// stdafx.h 에서 전역으로 선언했음으로 해당 함수 에서 사용되는 함수 또한 전역으로 선언되어야 한다?
	sort(m_vecEnemy.begin(), m_vecEnemy.end(), enemyManager::comp);

	//tileXYsort();

	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		if (!(*m_iter)->gettEnemyData().t_isAilve) continue;

		(*m_iter)->render(hdc);
	}

	//int tempIsAliveCount = 0;
	//for (int y = 0; y < m_pTileMap->getTileSizeY(); y++)
	//{
	//	// 살아있는 만큼 찾으면 탈출
	//	if (tempIsAliveCount == m_isAliveVec) break;
	//	int tempIsAliveY = 0;

	//	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	//	{
	//		if (tempIsAliveY == m_isAliveVec) break;
	//		if (!(*m_iter)->gettEnemyData().t_isAilve) continue;
	//		tempIsAliveY++;
	//		if ((*m_iter)->gettEnemyData().t_tilePosY != y) continue;
	//		tempIsAliveCount++;
	//		(*m_iter)->render(hdc);
	//	}
	//}
}

void enemyManager::tileXYsort()
{
	//if (m_isAliveVec < 2) return;
	//int tempIsAlivValue = 0;

	//for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	//{
	//	if (tempIsAlivValue == m_isAliveVec) break;
	//	if ((*m_iter)->gettEnemyData().t_isAilve)
	//	{
	//		if (tempIsAlivValue == m_isAliveVec) break;
	//		for (m_iterTileY = m_vecTileY.begin(); m_iterTileY != m_vecTileY.end(); m_iterTileY++)
	//		{
	//			if ((*m_iterTileY) != nullptr) continue;
	//			{
	//				(*m_iterTileY) = (*m_iter);
	//				(*m_iter) = nullptr;
	//				tempIsAlivValue++;
	//				break;
	//			}
	//		}
	//		
	//	}
	//}

	//tempIsAlivValue = 0;
	//for (int i = 0; i < m_pTileMap->getTileSizeY(); i++)
	//{
	//	if (tempIsAlivValue == m_isAliveVec) return;
	//	
	//	for (m_iterTileY = m_vecTileY.begin(); m_iterTileY != m_vecTileY.end(); m_iterTileY++)
	//	{
	//		if (tempIsAlivValue == m_isAliveVec) return;

	//		if ((*m_iterTileY) == nullptr) continue;
	//		if ((*m_iterTileY)->gettEnemyData().t_tilePosY == i)
	//		{
	//			for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	//			{
	//				if ((*m_iter) != nullptr) continue;

	//				(*m_iter) = (*m_iterTileY);
	//				(*m_iterTileY) = nullptr;
	//				tempIsAlivValue++;
	//				break;
	//			}
	//		}
	//	}
	//}


	vector<enemy*>::iterator	nextIter;
	enemy * currIter;
	int tempA, tempB;

	for (m_iter = m_vecEnemy.begin(); m_iter != m_vecEnemy.end(); m_iter++)
	{
		// 임시 보관
		currIter = (*m_iter);
		// 앞 vec를 가져온다
		nextIter = m_iter++;

		tempA = (*m_iter)->gettEnemyData().t_tilePosY;
		tempB = (*nextIter)->gettEnemyData().t_tilePosY;

		if ((*m_iter)->gettEnemyData().t_tilePosY < (*nextIter)->gettEnemyData().t_tilePosY)
		{
			(*m_iter) = (*nextIter);
			(*nextIter) = currIter;
		}

		tempA = (*m_iter)->gettEnemyData().t_tilePosY;
		tempB = (*nextIter)->gettEnemyData().t_tilePosY;
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
