#pragma once

#define SET_ENEMY_VEC 100

class enemy;
class tileMap;
class uiManager;
class uiPopup;

class enemyManager
{
private:
	vector<enemy*>	m_vecEnemy;
	vector<enemy*>::iterator	m_iter;

	SYNTHESIZE(bool, m_isEnemyInfoPopup, IsEnemyInfoPopup);
	SYNTHESIZE(uiPopup *, m_pEnemyPopup, EnemyPopup);
	tileMap * m_pTileMap;
	uiManager * m_pUiMag;

public:
	HRESULT init(tileMap * pTileMag, uiManager * pUiMag);
	void release();
	void update();
	void render(HDC hdc);

	void enemyDrop(tagEnemyData* enemyInfo);

	enemyManager();
	~enemyManager();
};

