#pragma once
#include "scene.h"
#include "tileMap.h"
#include "uiManager.h"

#define TOP_IMG_SIZE_Y 300
#define MOUSE_CURSOR_SET_SIZE 12

#define MOUSE_FOINT_SET_X 30
#define MOUSE_FOINT_SET_Y 20
#define MOUSE_FOINT_SET_NUMSIZE 4
#define MOUSE_FOINT_CANG_VALUE 10

class bulletManager;

class gameScene : public scene
{
private:
	image * m_imgTopBg;
	image * m_imgMidBg;
	image * m_imgMouseCur;

	tileMap * m_MapTile;
	uiManager * m_pUiMag;
	PlayerInfo * m_tPlayer;
	enemyManager * m_pEnemyMag;
	heroManager * m_pHeroMag;
	bulletManager * m_pBulletMag;
	char szText[256];

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void screenUi(HDC hdc);
	void RectCol();
	void HitEff(int x, int y);
	
	gameScene();
	~gameScene();
};

