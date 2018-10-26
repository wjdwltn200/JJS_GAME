#include "stdafx.h"
#include "uiTxt.h"


HRESULT uiTxt::init(float dam, float posX, float posY)
{
	memset(&szText, NULL, sizeof(szText));

	m_TxtScale = RANDOM->getFromIntTo(16, 22);
	m_fDamageValue = dam;
	m_fPosX = posX;
	m_fPosY = posY;
	m_fSetPosX = posX;
	m_fSetPosY = posY;
	m_moveCount = MOVE_COUNT;

	m_isAlive = true;

	return S_OK;
}

void uiTxt::release()
{
}

void uiTxt::update()
{
	m_fPosX = m_fSetPosX;
	m_fPosY = m_fSetPosY;

	if (m_moveCount <= 0)
		m_isAlive = false;

	m_fSetPosY -= 1.5f;
	m_moveCount--;
}

void uiTxt::render(HDC hdc)
{
	//AddFontResourceA("BMHANNAAir_ttf.ttf");

	// 데미지 표기
	SetBkMode(hdc, TRANSPARENT);
	MY_UTIL::FontOption(hdc, m_TxtScale, 400);

	SetTextColor(hdc, RGB(255, 255, 255));
	sprintf_s(szText, "%.1f", m_fDamageValue);
	TextOut(hdc, m_fPosX, m_fPosY, szText, strlen(szText));
	MY_UTIL::FontDelete(hdc);
}

uiTxt::uiTxt()
{
}


uiTxt::~uiTxt()
{
}
