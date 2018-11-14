#pragma once
#include "uiTxt.h"
#include "uiPopup.h"

class uiManager
{
private:
	vector<uiTxt*>	m_vecUiObjects;
	vector<uiTxt*>::iterator	m_ObjectsIter;

	vector<uiPopup*>	m_vecUiPopup;
	vector<uiPopup*>::iterator	m_PopupIter;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void addTxtBox(image * img, string txt, bool isMove, int txtCol);
	uiPopup * addPopup(image * img, float posX, float posY, int DesDaley);

	uiManager();
	~uiManager();
};
