#pragma once

#define MOVE_COUNT 30

class uiTxt
{
private:
	char szText[256];

	image * m_imgBg;
	image * m_img;
	string m_sTxt;
	float m_fPosX;
	float m_fPosY;
	float m_fMoveSpeed;
	int m_txtClor;

	bool  m_isAlive;
public:
	HRESULT init(image * img, string txt, bool isMove, int txtClor = eTxtBoxColor::White);
	void release();
	void update();
	void render(HDC hdc);

	bool getisAlive() { return m_isAlive; }

	uiTxt();
	~uiTxt();
};

