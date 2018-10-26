#pragma once
#include "scene.h"
#include "tileMap.h"

#define TOP_IMG_SIZE_Y 300

class gameScene : public scene
{
private:
	image * m_imgTopBg;
	image * m_imgMidBg;

	tileMap * m_MapTile;

	PlayerInfo * m_tPlayer;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	gameScene();
	~gameScene();
};

