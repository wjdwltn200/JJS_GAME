#include "stdafx.h"
#include "uiManager.h"


HRESULT uiManager::init(int vecMaxSize)
{
	m_vecUIObjects.reserve(vecMaxSize);



	return E_NOTIMPL;
}

void uiManager::release()
{
	for (m_iter = m_vecUIObjects.begin(); m_iter != m_vecUIObjects.end(); m_iter++)
	{
		delete (*m_iter);
	}
	m_vecUIObjects.clear();
}

void uiManager::update()
{
	for (m_iter = m_vecUIObjects.begin(); m_iter != m_vecUIObjects.end(); m_iter++)
	{
		if (!(*m_iter)->getisAlive()) continue;

		(*m_iter)->update();
	}
}

void uiManager::render(HDC hdc)
{
	for (m_iter = m_vecUIObjects.begin(); m_iter != m_vecUIObjects.end(); m_iter++)
	{
		if (!(*m_iter)->getisAlive()) continue;

		(*m_iter)->render(hdc);
	}
}

void uiManager::addHitTxt(float dam, float posX, float posY)
{
	for (m_iter = m_vecUIObjects.begin(); m_iter != m_vecUIObjects.end(); m_iter++)
	{
		if ((*m_iter)->getisAlive()) continue;

		(*m_iter)->init(dam, posX, posY);
		return;
	}

	uiTxt * tempUiTxt = new uiTxt;
	tempUiTxt->init(dam, posX, posY);
	m_vecUIObjects.push_back(tempUiTxt);
}

uiManager::uiManager()
{
}


uiManager::~uiManager()
{
}