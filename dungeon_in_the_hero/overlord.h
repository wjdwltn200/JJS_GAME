#pragma once
#include "animation.h"

class tileMap;

class overlord
{
private:
	SYNTHESIZE(tagOverlord, m_tOverlord, tOverlord);
	animation m_ani;
	
	tileMap * m_pTileMap;

	SYNTHESIZE(bool, m_isSetting, IsSetting);
	SYNTHESIZE(int, m_heroValue, HeroValue);

	bool m_isGameEnd;
	float m_popupMoveY;

public:
	HRESULT init(tileMap * pTileMap);
	void release();
	void update();
	void render(HDC hdc);
	void setTileXY(int tileX, int tileY, float posX, float posY);
	void setIsGet(bool isGet) { m_tOverlord.t_isGet = isGet; }


	overlord();
	~overlord();
};

