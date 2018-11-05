#pragma once
#include "animation.h"

class bullet
{
private:
	enum eMoveState
	{
		UP, RIGHT, DOWN, LEFT, MOVE_NUM
	};

	SYNTHESIZE(tagBullet, m_tBulletInfo, tBulletInfo);
	animation m_ani;
	

public:
	HRESULT init(tagBullet * bulletInfo);
	void release();
	void update();
	void render(HDC hdc);

	void movement();

	void setIsAlive(bool isAilve) { m_tBulletInfo.t_isAilve = isAilve; }

	bullet();
	~bullet();
};

