#pragma once
#include "animation.h"

#define IMG_TILE_SET_Y 5.0f
#define HERO_MOVE_DALEY 30

class tileMap;
class aStarNode;
class aStarPathFinding;

class hero
{
private:


	enum eMoveState
	{
		UP, DOWN, LEFT, RIGHT, MOVE_NUM
	};
	SYNTHESIZE(tagHeroData, m_tHeroData, tHeroData);
	animation m_ani;

	int m_moveDaley;
	int m_eMoveState;
	bool m_isMoveAct;
	tileMap * m_pTileMapMag;

public:
	HRESULT init(tagHeroData * heroInfo, tileMap * pTileMap);
	void release();
	void update();
	void render(HDC hdc);

	void aStarNodeStart();

	bool moveRectCheck(int eMoveArrow);

	hero();
	~hero();
};

