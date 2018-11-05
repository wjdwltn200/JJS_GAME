#pragma once

#define SET_BULLET_VEC 100

class bullet;

class bulletManager
{
private:
	vector<bullet*>	m_vecBullet;
	vector<bullet*>::iterator m_iterBullet;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void addBullet(tagBullet * bulletInfo);

	vector<bullet*> getVecBullet() { return m_vecBullet; }

	bulletManager();
	~bulletManager();
};

