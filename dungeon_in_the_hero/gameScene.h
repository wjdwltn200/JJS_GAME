#pragma once
#include "scene.h"
#include "tileMap.h"
#include "uiManager.h"

#define TOP_IMG_SIZE_Y 300
#define MOUSE_CURSOR_SET_SIZE 10
#define MOUSE_FOINT_SET_XY 30
#define MOUSE_FOINT_SET_NUMSIZE 4

class gameScene : public scene
{
private:
	image * m_imgTopBg;
	image * m_imgMidBg;
	image * m_imgMouseCur;

	tileMap * m_MapTile;
	uiManager * m_pUiMag;
	PlayerInfo * m_tPlayer;

	char szText[256];

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void screenUi(HDC hdc);

	gameScene();
	~gameScene();
};

