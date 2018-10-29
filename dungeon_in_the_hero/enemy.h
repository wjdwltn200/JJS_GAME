#pragma once
#include "animation.h"

#define IMG_TILE_SET_Y 5.0f
#define ENEMY_DIE_DALEY 60

class enemy
{
private:
	SYNTHESIZE(tagEnemyData, m_tEnemyData, tEnemyData);
	animation m_ani;

public:
	HRESULT init(tagEnemyData* enemyInfo);
	void release();
	void update();
	void render(HDC hdc);

	void currHp();

	enemy();
	~enemy();
};

