#pragma once


class enemy;
class tileMap;

class enemyManager
{
private:
	vector<enemy*>	m_vecEnemy;
	vector<enemy*>::iterator	m_iter;

	tileMap * m_pTileMap;

public:
	HRESULT init(tileMap * pTileMag);
	void release();
	void update();
	void render(HDC hdc);

	void enemyDrop(tagEnemyData* enemyInfo);

	enemyManager();
	~enemyManager();
};

