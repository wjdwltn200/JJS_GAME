#include "stdafx.h"
#include "bulletManager.h"
#include "bullet.h"

HRESULT bulletManager::init()
{
	// bullet는 미리 생성해둔다
	for (int i = 0; i < SET_BULLET_VEC; i++)
	{
		bullet * tempBullet = new bullet;
		m_vecBullet.push_back(tempBullet);
	}

	return S_OK;
}

void bulletManager::release()
{
	for (m_iterBullet = m_vecBullet.begin(); m_iterBullet != m_vecBullet.end(); m_iterBullet++)
	{
		delete (*m_iterBullet);
	}
	m_vecBullet.clear();
}

void bulletManager::update()
{
	for (m_iterBullet = m_vecBullet.begin(); m_iterBullet != m_vecBullet.end(); m_iterBullet++)
	{
		if (!(*m_iterBullet)->gettBulletInfo().t_isAilve) continue;

		(*m_iterBullet)->update();
	}
}

void bulletManager::render(HDC hdc)
{
	for (m_iterBullet = m_vecBullet.begin(); m_iterBullet != m_vecBullet.end(); m_iterBullet++)
	{
		if (!(*m_iterBullet)->gettBulletInfo().t_isAilve) continue;

		(*m_iterBullet)->render(hdc);
	}
}

void bulletManager::addBullet(tagBullet * bulletInfo)
{
	for (m_iterBullet = m_vecBullet.begin(); m_iterBullet != m_vecBullet.end(); m_iterBullet++)
	{
		if ((*m_iterBullet)->gettBulletInfo().t_isAilve) continue;

		(*m_iterBullet)->init(bulletInfo);
		return;
	}
	return;
}

bulletManager::bulletManager()
{
}


bulletManager::~bulletManager()
{
}
