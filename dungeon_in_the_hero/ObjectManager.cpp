#include "stdafx.h"
#include "ObjectManager.h"
#include "subObject.h"

HRESULT ObjectManager::init()
{

	return S_OK;
}

void ObjectManager::release()
{
	for (m_iter = m_vecSubObjects.begin(); m_iter != m_vecSubObjects.end(); m_iter++)
	{
		delete (*m_iter);
	}
	m_vecSubObjects.clear();
}

void ObjectManager::update()
{
	for (m_iter = m_vecSubObjects.begin(); m_iter != m_vecSubObjects.end(); m_iter++)
	{
		if (!(*m_iter)->getisAlive()) continue;

		(*m_iter)->update();
	}
}

void ObjectManager::render(HDC hdc)
{
	for (m_iter = m_vecSubObjects.begin(); m_iter != m_vecSubObjects.end(); m_iter++)
	{
		if (!(*m_iter)->getisAlive()) continue;

		(*m_iter)->render(hdc);
	}
}

void ObjectManager::addTileDesObj(int typeNum, float posX, float posY, tagItemData itemData)
{
	for (m_iter = m_vecSubObjects.begin(); m_iter != m_vecSubObjects.end(); m_iter++)
	{
		if ((*m_iter)->getisAlive()) continue;
			(*m_iter)->init(typeNum, posX, posY, itemData);
			return;
	}

	subObject * tempObj = new subObject;
	tempObj->init(typeNum, posX, posY, itemData);
	m_vecSubObjects.push_back(tempObj);
}

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}
