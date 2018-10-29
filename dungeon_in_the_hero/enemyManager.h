#pragma once


class enemy;

class enemyManager
{
private:
	vector<enemy*>	m_vecEnemy;
	vector<enemy*>::iterator	m_iter;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void enemyDrop(tagEnemyData* enemyInfo);

	enemyManager();
	~enemyManager();
};

