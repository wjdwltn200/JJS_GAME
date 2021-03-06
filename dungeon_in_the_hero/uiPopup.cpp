#include "stdafx.h"
#include "uiPopup.h"


HRESULT uiPopup::init(image * img, float posX, float posY, int DesDaley)
{
	m_isAilve = true;
	m_img = img;
	m_imgSizeX = NULL;
	m_imgSizeY = NULL;
	m_fPosX = posX;
	m_fPosY = posY;
	m_desDaley = DesDaley;
	m_tTxtInfo.t_txtInfo = "";
	m_tTxtInfo.t_txtName = "";

	return S_OK;
}

void uiPopup::release()
{
}

void uiPopup::update()
{
	if (m_desDaley <= 0)
	{
		m_isAilve = false;
	}

	m_desDaley--;
}

void uiPopup::render(HDC hdc)
{
	m_img->render(hdc, m_fPosX - CAMERA->getCamPosX(), m_fPosY - CAMERA->getCamPosY());

	char temp[256] = "";

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	MY_UTIL::FontOption(hdc, 18, 1000);
	sprintf_s(temp, "%s", m_tTxtInfo.t_txtName.c_str());
	TextOut(hdc, m_fPosX - CAMERA->getCamPosX() + POPUP_TXT_SET_X, m_fPosY - CAMERA->getCamPosY() + POPUP_TXT_NAME_SET_Y, temp, strlen(temp));
	MY_UTIL::FontDelete(hdc);

	MY_UTIL::FontOption(hdc, 12, 0);
	sprintf_s(temp, "%s", m_tTxtInfo.t_txtInfo.c_str());
	RECT rcTextArea = {
		m_fPosX - CAMERA->getCamPosX() + POPUP_TXT_SET_X,
		m_fPosY - CAMERA->getCamPosY() + POPUP_TXT_INFO_SET_Y,
		m_fPosX - CAMERA->getCamPosX() + POPUP_TXT_SET_X + m_imgSizeX - POPUP_TXT_BOX_SEY_X,
		m_fPosY - CAMERA->getCamPosY() + POPUP_TXT_INFO_SET_Y + m_imgSizeY };
	DrawText(hdc, temp, strlen(temp), &rcTextArea, DT_WORDBREAK);
	MY_UTIL::FontDelete(hdc);
}

void uiPopup::setXY(float posX, float posY)
{
	m_fPosX = posX + CAMERA->getCamPosX() - m_img->getFrameWidth() / 2;
	m_fPosY = posY + CAMERA->getCamPosY() - m_img->getFrameHeight() / 2;
	m_imgSizeX = m_img->getFrameWidth();
	m_imgSizeY = m_img->getFrameHeight();
}

uiPopup::uiPopup()
{
}


uiPopup::~uiPopup()
{
}
