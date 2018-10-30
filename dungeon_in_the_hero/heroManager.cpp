#include "stdafx.h"
#include "heroManager.h"
#include "hero.h"

HRESULT heroManager::init(tileMap * pTileMap)
{
	 m_pTileMap = pTileMap;

	return S_OK;
}

void heroManager::release()
{
	for (m_iter = m_vecHero.begin(); m_iter != m_vecHero.end(); m_iter++)
	{
		delete (*m_iter);
	}
	m_vecHero.clear();
}

void heroManager::update()
{
	for (m_iter = m_vecHero.begin(); m_iter != m_vecHero.end(); m_iter++)
	{
		(*m_iter)->update();
	}
}

void heroManager::render(HDC hdc)
{
	for (m_iter = m_vecHero.begin(); m_iter != m_vecHero.end(); m_iter++)
	{
		(*m_iter)->render(hdc);
	}
}

void heroManager::heroDrop(tagHeroData * enemyInfo)
{
	for (m_iter = m_vecHero.begin(); m_iter != m_vecHero.end(); m_iter++)
	{
		if ((*m_iter)->gettHeroData().t_isAilve) continue;

		(*m_iter)->init(enemyInfo, m_pTileMap);
		return;
	}

	hero * tempHero = new hero;
	tempHero->init(enemyInfo, m_pTileMap);
	m_vecHero.push_back(tempHero);
}

heroManager::heroManager()
{
}


heroManager::~heroManager()
{
}
