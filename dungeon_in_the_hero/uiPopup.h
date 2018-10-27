#pragma once
#define POPUP_TXT_SET_X 8.0f
#define POPUP_TXT_NAME_SET_Y 10.0f
#define POPUP_TXT_INFO_SET_Y 35.0f
#define POPUP_TXT_BOX_SEY_X 10.0f

class uiPopup
{
private:
	SYNTHESIZE(float, m_fPosX, PosX);
	SYNTHESIZE(float, m_fPosY, PosY);
	SYNTHESIZE(image*, m_img, Img);
	SYNTHESIZE(bool, m_isAilve, IsAilve);
	SYNTHESIZE(int, m_desDaley, Daley);
	SYNTHESIZE(PopupInfo, m_tTxtInfo, TxtInfo);

	int m_imgSizeX;
	int m_imgSizeY;

public:
	HRESULT init(image * img, float posX, float posY, int DesDaley);
	void release();
	void update();
	void render(HDC hdc);

	void setXY(float posX, float posY);

	uiPopup();
	~uiPopup();
};

