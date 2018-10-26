#include "stdafx.h"
#include "CameraManager.h"


void CameraManager::update(float MinX, float MinY, float MaxX, float MaxY)
{
	if (m_fCamPosX < MinX)
		m_fCamPosX = MinX;

	if (m_fCamPosY < MinY)
		m_fCamPosY = MinY;

	if (m_fCamPosX > MaxX)
		m_fCamPosX = MaxX;

	if (m_fCamPosY > MaxY)
		m_fCamPosY = MaxY;
}

CameraManager::CameraManager()
	: m_fCamPosX(0.0f)
	, m_fCamPosY(0.0f)
{
}


CameraManager::~CameraManager()
{
}
