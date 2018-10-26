#include "stdafx.h"
#include "titleScene.h"



HRESULT titleScene::init()
{
	m_ImgChangValue = 0;

	m_ImgTitle = IMAGEMANAGER->addImage("Title_Bg" ,"image/UI_Img/Title_Bg.bmp", WINSIZEX, WINSIZEY);
	m_ImgButton = IMAGEMANAGER->addImage("Title_Button", "image/UI_Img/Title_Button.bmp", 256, 512, 1, 4, true, true);

	return S_OK;
}

void titleScene::release()
{
}

void titleScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		switch (m_ImgChangValue)
		{
		case tagTITLE_BUTTON::T_BUTTON_GAMESTART:
			SCENEMANAGER->changeScene("inGame");
			break;
		case tagTITLE_BUTTON::T_BUTTON_CUSTOM:
			break;
		case tagTITLE_BUTTON::T_BUTTON_COLLECTION:
			break;
		case tagTITLE_BUTTON::T_BUTTON_EXIT:
			break;
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LEFT) && (m_ImgChangValue != tagTITLE_BUTTON::T_BUTTON_GAMESTART))
		m_ImgChangValue--;

	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && (m_ImgChangValue != tagTITLE_BUTTON::T_BUTTON_EXIT))
		m_ImgChangValue++;
}

void titleScene::render(HDC hdc)
{
	m_ImgTitle->render(hdc, 0, 0);

	m_ImgButton->frameRender(hdc, WINSIZEX / 2, WINSIZEY / 2 + WINSIZEY / 5, 1, m_ImgChangValue);
	if (m_ImgChangValue > tagTITLE_BUTTON::T_BUTTON_GAMESTART)
		m_ImgButton->frameRender(hdc, WINSIZEX / 2 - WINSIZEX / 4, WINSIZEY / 2 + WINSIZEY / 5, 1, m_ImgChangValue - 1, 0.8f);

	if (m_ImgChangValue < tagTITLE_BUTTON::T_BUTTON_EXIT)
		m_ImgButton->frameRender(hdc, WINSIZEX / 2 + WINSIZEX / 4, WINSIZEY / 2 + WINSIZEY / 5, 1, m_ImgChangValue + 1, 0.8f);
}

titleScene::titleScene()
{
}

titleScene::~titleScene()
{
}
