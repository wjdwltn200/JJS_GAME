#include "stdafx.h"
#include "heroManager.h"
#include "hero.h"

HRESULT heroManager::init(tileMap * pTileMag, bulletManager * pBulletMag, overlord * pOverlord)
{
	 m_pTileMag = pTileMag;
	 m_pBulletMag = pBulletMag;
	 m_pOverlord = pOverlord;
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

		(*m_iter)->init(enemyInfo, m_pTileMag , m_pBulletMag, m_pOverlord);
		return;
	}

	hero * tempHero = new hero;
	tempHero->init(enemyInfo, m_pTileMag, m_pBulletMag, m_pOverlord);
	m_vecHero.push_back(tempHero);
}

heroManager::heroManager()
{
}


heroManager::~heroManager()
{
}
