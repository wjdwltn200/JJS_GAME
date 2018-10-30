#pragma once
#include "animation.h"

#define IMG_TILE_SET_Y 5.0f
#define ENEMY_DIE_DALEY 60
#define ENEMY_MOVE_DALEY 60

class tileMap;

class enemy
{
private:
	enum eMoveState
	{
		UP, DOWN, LEFT, RIGHT, MOVE_NUM
	};
	SYNTHESIZE(tagEnemyData, m_tEnemyData, tEnemyData);
	animation m_ani;

	int m_moveDaley;
	int m_eMoveState;
	bool m_isMoveAct;
	tileMap * m_pTileMapMag;

public:
	HRESULT init(tagEnemyData* enemyInfo, tileMap* pTileMag);
	void release();
	void update();
	void render(HDC hdc);

	void currHp();
	void moveSys();
	bool moveRectCheck(int eMoveArrow);

	enemy();
	~enemy();
};
