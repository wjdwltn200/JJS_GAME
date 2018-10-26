#pragma once

#define MOVE_COUNT 30

class uiTxt
{
private:
	char szText[256];

	float m_fDamageValue;
	float m_fPosX;
	float m_fPosY;
	float m_fSetPosX;
	float m_fSetPosY;

	int m_TxtScale;
	int m_moveCount;

	bool  m_isAlive;

public:
	HRESULT init(float dam, float posX, float posY);
	void release();
	void update();
	void render(HDC hdc);

	bool getisAlive() { return m_isAlive; }

	uiTxt();
	~uiTxt();
};

