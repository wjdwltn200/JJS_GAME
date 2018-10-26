#include "stdafx.h"
#include "subObject.h"

HRESULT subObject::init(int typeNum, float posX, float posY)
{
	m_isAlive = true;

	switch (typeNum)
	{
	case tagObjectType::TILE_DIS:
		int temp = RANDOM->getFromIntTo(0, 2);
		switch (temp)
		{
		case 0:
			m_imgObj = IMAGEMANAGER->findImage("TileSet_Terrain_DesSet_0");
			break;
		case 1:
			m_imgObj = IMAGEMANAGER->findImage("TileSet_Terrain_DesSet_1");
			break;
		case 2:
			m_imgObj = IMAGEMANAGER->findImage("TileSet_Terrain_DesSet_2");
			break;
		}
		m_fAngle = RANDOM->getFromFloatTo(-180.0f, 180.0f);
		m_fPosX = posX + CAMERA->getCamPosX();
		m_fPosY = posY + CAMERA->getCamPosY();
		m_alpha = 255;
		m_fScale = RANDOM->getFromFloatTo(0.5f, 2.0f);
		m_dieCount = TILE_DIS_DELAY;
		m_movePower = RANDOM->getFromFloatTo(4.0f, TILE_DIS_MOVE_POWER);
		m_colPower = 0.0f;
		break;
	}

	return S_OK;
}

void subObject::release()
{
}

void subObject::update()
{
	if (m_dieCount <= 0)
		m_isAlive = false;
	m_dieCount--;

	switch (m_type)
	{
	case tagObjectType::TILE_DIS:
		m_movePower -= 0.1f;
		m_colPower += 0.1f;
		if (m_dieCount <= TILE_DIS_DELAY / 2)
		{
			m_alpha += 50;
		}
		break;
	}

	movement();
}

void subObject::render(HDC hdc)
{
	m_imgObj->render(hdc, m_fPosX - CAMERA->getCamPosX(), m_fPosY - CAMERA->getCamPosY(), 0, 0, m_imgObj->getFrameWidth(), m_imgObj->getFrameHeight(), m_fScale, m_alpha);
}

void subObject::movement()
{
	switch (m_type)
	{
	case tagObjectType::TILE_DIS:
		if (m_movePower >= 0.0f)
		{
			m_fPosX += cosf(m_fAngle) * m_movePower;
			m_fPosY += -sinf(m_fAngle) * m_movePower;
		}
		m_fPosY += m_colPower;
		break;
	}

}

subObject::subObject()
{
}


subObject::~subObject()
{
}
