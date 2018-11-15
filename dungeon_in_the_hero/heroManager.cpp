#include "stdafx.h"
#include "heroManager.h"
#include "hero.h"

HRESULT heroManager::init(tileMap * pTileMag, bulletManager * pBulletMag, overlord * pOverlord, uiManager * pUiMag)
{
	m_pUiMag = pUiMag;
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

		(*m_iter)->init(enemyInfo, m_pTileMag , m_pBulletMag, m_pOverlord, m_pUiMag);
		return;
	}

	hero * tempHero = new hero;
	tempHero->init(enemyInfo, m_pTileMag, m_pBulletMag, m_pOverlord, m_pUiMag);
	m_vecHero.push_back(tempHero);
}

#define HEROINFO_BOX_SET_X 10.0f
#define HEROINFO_BOX_SET_Y 100.0f

void heroManager::heroInfoRender(HDC hdc)
{
	int tempSizeY = 0;


	for (m_iter = m_vecHero.begin(); m_iter != m_vecHero.end(); m_iter++)
	{
		if (!(*m_iter)->gettHeroData().t_isAilve) continue;

		IMAGEMANAGER->findImage("HeroPopup")->alphaRender(hdc, HEROINFO_BOX_SET_X, ((IMAGEMANAGER->findImage("HeroPopup")->getHeight() * tempSizeY)) + HEROINFO_BOX_SET_Y, 150);

		SetBkMode(hdc, TRANSPARENT);
	

		string m_sTxt;
		switch ((*m_iter)->gettHeroData().t_enumType)
		{
		case eHeroClass::Warriors:
			m_sTxt = "전사";
			break;
		case eHeroClass::Wizard:
			m_sTxt = "마법사";
			break;
		case eHeroClass::Alchemist:
			m_sTxt = "연금술사";
			break;
		}

		image * tempImg = (*m_iter)->gettHeroData().t_img;
		tempImg->frameRender(hdc,
			HEROINFO_BOX_SET_X + (tempImg->getFrameWidth() / 2) + 15.0f,
			((IMAGEMANAGER->findImage("HeroPopup")->getHeight() * tempSizeY)) + HEROINFO_BOX_SET_Y + (tempImg->getFrameHeight() / 2) + 30.0f, 1, 1, 2.0f);

		MY_UTIL::FontOption(hdc, 22, 1000);
		SetTextColor(hdc, RGB(255, 255, 255));
		sprintf_s(szText, "%s", m_sTxt.c_str());
		TextOut(hdc, HEROINFO_BOX_SET_X + 50.0f/* - (tempTxtSize * 12)*/, ((IMAGEMANAGER->findImage("HeroPopup")->getHeight() * tempSizeY)) + HEROINFO_BOX_SET_Y + 10.0f, szText, strlen(szText));
		MY_UTIL::FontDelete(hdc);

		MY_UTIL::FontOption(hdc, 16, 500);
		SetTextColor(hdc, RGB(255, 255, 255));
		sprintf_s(szText, "[HP] %d / %d", (*m_iter)->gettHeroData().t_currHp, (*m_iter)->gettHeroData().t_MaxHp);
		TextOut(hdc, HEROINFO_BOX_SET_X + 50.0f/* - (tempTxtSize * 12)*/, ((IMAGEMANAGER->findImage("HeroPopup")->getHeight() * tempSizeY)) + HEROINFO_BOX_SET_Y + 35.0f, szText, strlen(szText));
		MY_UTIL::FontDelete(hdc);

		MY_UTIL::FontOption(hdc, 16, 500);
		SetTextColor(hdc, RGB(255, 255, 255));
		sprintf_s(szText, "[MP] %d / %d", (*m_iter)->gettHeroData().t_currMana, (*m_iter)->gettHeroData().t_MaxMana);
		TextOut(hdc, HEROINFO_BOX_SET_X + 50.0f/* - (tempTxtSize * 12)*/, ((IMAGEMANAGER->findImage("HeroPopup")->getHeight() * tempSizeY)) + HEROINFO_BOX_SET_Y + 55.0f, szText, strlen(szText));
		MY_UTIL::FontDelete(hdc);

		tempSizeY++;
	}
}

heroManager::heroManager()
{
}


heroManager::~heroManager()
{
}
