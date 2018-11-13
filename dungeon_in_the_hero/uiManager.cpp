#include "stdafx.h"
#include "uiManager.h"


HRESULT uiManager::init()
{
	return S_OK;
}

void uiManager::release()
{
	for (m_ObjectsIter = m_vecUiObjects.begin(); m_ObjectsIter != m_vecUiObjects.end(); m_ObjectsIter++)
	{
		delete (*m_ObjectsIter);
	}
	m_vecUiObjects.clear();

	for (m_PopupIter = m_vecUiPopup.begin(); m_PopupIter != m_vecUiPopup.end(); m_PopupIter++)
	{
		delete (*m_PopupIter);
	}
	m_vecUiPopup.clear();
}

void uiManager::update()
{
	for (m_ObjectsIter = m_vecUiObjects.begin(); m_ObjectsIter != m_vecUiObjects.end(); m_ObjectsIter++)
	{
		if (!(*m_ObjectsIter)->getisAlive()) continue;

		(*m_ObjectsIter)->update();
	}

	for (m_PopupIter = m_vecUiPopup.begin(); m_PopupIter != m_vecUiPopup.end(); m_PopupIter++)
	{
		if (!(*m_PopupIter)->getIsAilve()) continue;

		(*m_PopupIter)->update();
	}
}

void uiManager::render(HDC hdc)
{
	for (m_ObjectsIter = m_vecUiObjects.begin(); m_ObjectsIter != m_vecUiObjects.end(); m_ObjectsIter++)
	{
		if (!(*m_ObjectsIter)->getisAlive()) continue;

		(*m_ObjectsIter)->render(hdc);
	}

	for (m_PopupIter = m_vecUiPopup.begin(); m_PopupIter != m_vecUiPopup.end(); m_PopupIter++)
	{
		if (!(*m_PopupIter)->getIsAilve()) continue;
		if ((*m_PopupIter)->getImg() == NULL) continue;


		(*m_PopupIter)->render(hdc);
	}
}

void uiManager::addHitTxt(float dam, float posX, float posY)
{
	for (m_ObjectsIter = m_vecUiObjects.begin(); m_ObjectsIter != m_vecUiObjects.end(); m_ObjectsIter++)
	{
		if ((*m_ObjectsIter)->getisAlive()) continue;

		(*m_ObjectsIter)->init(dam, posX, posY);
		return;
	}

	uiTxt * tempUiTxt = new uiTxt;
	tempUiTxt->init(dam, posX, posY);
	m_vecUiObjects.push_back(tempUiTxt);
}

uiPopup * uiManager::addPopup(image * img, float posX, float posY, int DesDaley)
{
	uiPopup * tempUiPopup = new uiPopup;
	tempUiPopup->init(img, posX, posY, DesDaley);
	m_vecUiPopup.push_back(tempUiPopup);

	return tempUiPopup;
}

uiManager::uiManager()
{
}


uiManager::~uiManager()
{
}