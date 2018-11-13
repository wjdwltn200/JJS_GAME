#include "stdafx.h"
#include "subObject.h"

HRESULT subObject::init(int typeNum, float posX, float posY, tagItemData itemData)
{
	m_isAlive = true;
	m_type = typeNum;
	m_tItemData = itemData;
	switch (typeNum)
	{
	case tagObjectType::TILE_DIS:
		switch (RANDOM->getFromIntTo(0, 2))
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
		m_colPower = TILE_DIS_COL_POWER;
		break;

	case tagObjectType::ITEM_JEWEL:
		m_imgObj = IMAGEMANAGER->findImage("Item_Jewel");
		m_fAngle = PI / 180.0f * RANDOM->getFromFloatTo(70.0f, 110.0f);
		m_fPosX = posX + CAMERA->getCamPosX();
		m_fPosY = posY + CAMERA->getCamPosY();
		m_alpha = 0;
		m_fScale = 3.0f;
		m_dieCount = JEWEL_DIS_DELAY;
		m_movePower =JEWEL_DIS_MOVE_POWER;
		m_colPower = 0.0f;
		break;

	case tagObjectType::HERO_START:
		switch (RANDOM->getFromIntTo(0, 2))
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
		m_dieCount = HERO_START_DELAY;
		m_movePower = RANDOM->getFromFloatTo(8.0f, HERO_START_MOVE_POWER);
		m_colPower = HERO_START_COL_POWER;
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
	case tagObjectType::ITEM_JEWEL:
		m_movePower -= 0.1f;
		m_colPower += 0.1f;
		if (m_dieCount >= JEWEL_DIS_DELAY / 2)
		{
			m_alpha += 50;
		}
		else
		{
			m_alpha = 0;
		}
		break;
	case tagObjectType::HERO_START:
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
	switch (m_type)
	{
	case tagObjectType::TILE_DIS:
		m_imgObj->render(hdc, m_fPosX - CAMERA->getCamPosX(), m_fPosY - CAMERA->getCamPosY(), 0, 0, m_imgObj->getFrameWidth(), m_imgObj->getFrameHeight(), m_fScale, m_alpha);
		break;
	case tagObjectType::ITEM_JEWEL:
		m_imgObj->frameAlphaRender(hdc, m_fPosX - CAMERA->getCamPosX(), m_fPosY - CAMERA->getCamPosY(), m_tItemData.t_frameX, m_tItemData.t_frameY, m_fScale, true, m_alpha);
		if (m_dieCount <= JEWEL_DIS_DELAY / 2)
			MY_UTIL::NumberPont(hdc, m_tItemData.t_frameY + 1, 1, m_fPosX - CAMERA->getCamPosX() + JEWEL_DIS_SET_X, m_fPosY - CAMERA->getCamPosY(), 2, 4.0f);
		break;
	case tagObjectType::HERO_START:
		m_imgObj->render(hdc, m_fPosX - CAMERA->getCamPosX(), m_fPosY - CAMERA->getCamPosY(), 0, 0, m_imgObj->getFrameWidth(), m_imgObj->getFrameHeight(), m_fScale, m_alpha);
		break;
	}
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
	case tagObjectType::ITEM_JEWEL:
		if (m_movePower >= 0.0f)
		{
			m_fPosX += cosf(m_fAngle) * m_movePower;
			m_fPosY += -sinf(m_fAngle) * m_movePower;
		}
		break;
	case tagObjectType::HERO_START:
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
