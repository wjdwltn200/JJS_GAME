#pragma once
#include "animation.h"

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

	enemy();
	~enemy();
};

