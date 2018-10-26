#pragma once
#include <vector>
#include "uiTxt.h"

class uiObject;

class uiManager
{
private:
	vector<uiTxt*>	m_vecUIObjects;
	vector<uiTxt*>::iterator	m_iter;

public:
	HRESULT init(int vecMaxSize);
	void release();
	void update();
	void render(HDC hdc);

	void addHitTxt(float dam, float posX, float posY);

	uiManager();
	~uiManager();
};
