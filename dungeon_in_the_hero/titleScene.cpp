#include "stdafx.h"
#include "titleScene.h"



HRESULT titleScene::init()
{
	m_ImgChangValue = 0;
	m_isScreenChange = false;
	m_screenChangeValue = 0;

	IMAGEMANAGER->addImage("Title_Chang", "image/UI_Img/Title_Chang.bmp", WINSIZEX, WINSIZEY, true, RGB(255,0,255));
	SOUNDMANAGER->addSound("Sound/BGM/BGM_Title.wav", true, false);
	SOUNDMANAGER->addSound("Sound/SE/Button_0.wav", false, false);
	m_ImgTitle = IMAGEMANAGER->addImage("Title_Bg" ,"image/UI_Img/Title_Bg.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	m_imgOverlord = IMAGEMANAGER->addImage("Overlord", "image/inGameImg/ENEMY/Overlord_0.bmp", 72, 25, 4, 1, true, RGB(255, 0, 255));
	m_ani.init(m_imgOverlord->getWidth(), m_imgOverlord->getHeight(), m_imgOverlord->getFrameWidth(), m_imgOverlord->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(10);
	m_ani.start();

	SOUNDMANAGER->play("Sound/BGM/BGM_Title.wav", 0.5f);
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
			SOUNDMANAGER->stop("Sound/BGM/BGM_Title.wav");
			SOUNDMANAGER->play("Sound/SE/Button_0.wav");
			m_isScreenChange = true;
			m_screenChangeValue = 255;
			break;
		//case tagTITLE_BUTTON::T_BUTTON_CUSTOM:
		//	if (g_saveData.g_isOption)
		//		g_saveData.g_isOption = false;
		//	if (!g_saveData.g_isOption)
		//		g_saveData.g_isOption = true;
		//	break;
		case tagTITLE_BUTTON::T_BUTTON_EXIT:
			PostQuitMessage(0);
			break;
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LEFT) && (m_ImgChangValue != tagTITLE_BUTTON::T_BUTTON_GAMESTART))
	{
		m_ImgChangValue--;
		SOUNDMANAGER->play("Sound/SE/Button_0.wav");
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && (m_ImgChangValue != tagTITLE_BUTTON::T_BUTTON_EXIT))
	{
		m_ImgChangValue++;
		SOUNDMANAGER->play("Sound/SE/Button_0.wav");
	}

	if (m_isScreenChange && m_screenChangeValue <= 5)
		SCENEMANAGER->changeScene("inGame");

	if (m_isScreenChange && m_screenChangeValue > 1)
	{
		m_screenChangeValue -= 5;
		if (m_screenChangeValue <= 1)
			m_screenChangeValue = 5;
	}
}

void titleScene::render(HDC hdc)
{
	m_ImgTitle->render(hdc, 0, 0);
	m_imgOverlord->aniRender(hdc, 290.0f, 495.0f, &m_ani, 2.5f);



	char temp[256] = "";


	if (m_ImgChangValue == tagTITLE_BUTTON::T_BUTTON_GAMESTART)
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		MY_UTIL::FontOption(hdc, 72, 1000);
		sprintf_s(temp, "게임시작 ▶");
		TextOut(hdc, WINSIZEX / 2 + 50.0f, WINSIZEY / 2 + WINSIZEY / 4, temp, strlen(temp));
		MY_UTIL::FontDelete(hdc);
	}
	//else if (m_ImgChangValue == tagTITLE_BUTTON::T_BUTTON_CUSTOM)
	//{
	//	SetBkMode(hdc, TRANSPARENT);
	//	SetTextColor(hdc, RGB(255, 255, 255));
	//	MY_UTIL::FontOption(hdc, 72, 1000);
	//	sprintf_s(temp, "◀ 음량 옵션 ▶");
	//	TextOut(hdc, WINSIZEX / 2 - 25.0f , WINSIZEY / 2 + WINSIZEY / 4, temp, strlen(temp));
	//	MY_UTIL::FontDelete(hdc);
	//}
	else if (m_ImgChangValue == tagTITLE_BUTTON::T_BUTTON_EXIT)
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		MY_UTIL::FontOption(hdc, 72, 1000);
		sprintf_s(temp, "◀ 현실세계로");
		TextOut(hdc, WINSIZEX / 2 - 25.0f, WINSIZEY / 2 + WINSIZEY / 4, temp, strlen(temp));
		MY_UTIL::FontDelete(hdc);
	}

	if (m_screenChangeValue)
		IMAGEMANAGER->findImage("Title_Chang")->alphaRender(hdc, (-m_screenChangeValue));

	if (g_saveData.g_isOption)
	{
		IMAGEMANAGER->findImage("Title_Chang")->alphaRender(hdc, 155);
	}

	m_ani.frameUpdate();
}

titleScene::titleScene()
{
}

titleScene::~titleScene()
{
}
