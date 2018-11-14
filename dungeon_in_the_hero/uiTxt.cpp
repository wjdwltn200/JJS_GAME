#include "stdafx.h"
#include "uiTxt.h"

HRESULT uiTxt::init(image * img, string txt, bool isMove, int txtClor)
{
	m_imgBg = IMAGEMANAGER->findImage("HeroScreenTxt");
	m_img = img;
	m_sTxt = txt;
	m_fPosX = WINSIZEX + (m_imgBg->getWidth());
	m_fPosY = RANDOM->getFromFloatTo(50.0f, 400.0f);
	m_txtClor = txtClor;

	if (isMove) m_fMoveSpeed = 30.0f;
	if (!isMove) m_fMoveSpeed = 0.0f;

	m_isAlive = true;

	return S_OK;
}

void uiTxt::release()
{
}

void uiTxt::update()
{
	m_fPosX -= m_fMoveSpeed;

	if (m_fPosX > (WINSIZEX / 5) && 5.0f < m_fMoveSpeed)
		m_fMoveSpeed -= 0.5f;
	if (m_fPosX < (WINSIZEX / 4))
		m_fMoveSpeed += 1.5f;

	if (m_fPosX < -(m_imgBg->getWidth()))
		m_isAlive = false;
}

void uiTxt::render(HDC hdc)
{
	m_imgBg->alphaRender(hdc,
		m_fPosX - (m_imgBg->getWidth() / 2),
		m_fPosY - (m_imgBg->getHeight() / 2), 155);

	m_img->frameRender(hdc,
		m_fPosX - (m_img->getWidth() / 2) - 150.0f,
		m_fPosY - (m_img->getHeight() / 2) + 10.0f, 1, 1, 4.0f);

	SetBkMode(hdc, TRANSPARENT);
	MY_UTIL::FontOption(hdc, 42, 1000);

	// 글자 길이 구하기
	int tempTxtSize = strlen(m_sTxt.c_str()) / 2;
	switch (m_txtClor)
	{
	case eTxtBoxColor::Green:
		SetTextColor(hdc, RGB(0, 255, 0));
		break;
	case eTxtBoxColor::Purple:
		SetTextColor(hdc, RGB(255, 0, 255));
		break;
	case eTxtBoxColor::Red:
		SetTextColor(hdc, RGB(255, 0, 0));
		break;
	case eTxtBoxColor::White:
		SetTextColor(hdc, RGB(255, 255, 255));
		break;
	}
	sprintf_s(szText, "%s", m_sTxt.c_str());
	TextOut(hdc, m_fPosX - (tempTxtSize * 12), m_fPosY - 20.0f, szText, strlen(szText));
	MY_UTIL::FontDelete(hdc);
}

uiTxt::uiTxt()
{
}


uiTxt::~uiTxt()
{
}
