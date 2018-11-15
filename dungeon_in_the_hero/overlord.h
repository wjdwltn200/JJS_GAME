#pragma once
#include "animation.h"

#define STAGE_CLEAR 25

class tileMap;
class uiManager;

class overlord
{
private:
	SYNTHESIZE(tagOverlord, m_tOverlord, tOverlord);
	animation m_ani;
	
	tileMap * m_pTileMap;
	uiManager * m_pUiMag;

	SYNTHESIZE(bool, m_isSetting, IsSetting);
	SYNTHESIZE(int, m_heroValue, HeroValue);

	bool m_isGameEnd;
	float m_popupMoveY;
	bool m_isGameOver;

public:
	HRESULT init(tileMap * pTileMap, uiManager * pUiMag);
	void release();
	void update();
	void render(HDC hdc);
	void setTileXY(int tileX, int tileY, float posX, float posY);
	void gameTxtBox(image * img, string txt, int txtCol);
	void setIsGet(bool isGet) { m_tOverlord.t_isGet = isGet; }


	overlord();
	~overlord();
};

