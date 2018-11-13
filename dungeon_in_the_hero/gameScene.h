#pragma once
#include "scene.h"
#include "tileMap.h"
#include "uiManager.h"

#define TOP_IMG_SIZE_Y 300
#define MOUSE_CURSOR_SET_SIZE 12

#define NUMBER_SCALE_VALUE 120

class bulletManager;
class overlord;

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
	overlord * m_pOverlord;
	char szText[256];

	int m_currTileDesEne;
	float m_tileDesEneScale;

	bool m_isScreenChange;
	int m_screenChangeValue;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void screenUi(HDC hdc);
	void RD_UI(HDC hdc);
	void MouseCur(HDC hdc);
	void RectCol();
	void HitEff(int x, int y);
	
	gameScene();
	~gameScene();
};

