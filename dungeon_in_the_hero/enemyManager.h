#pragma once

#define SET_ENEMY_VEC 500

class enemy;
class tileMap;
class uiManager;
class uiPopup;
class bulletManager;

class enemyManager
{
private:
	vector<enemy*>	m_vecEnemy;
	vector<enemy*>::iterator	m_iter;

	SYNTHESIZE(bool, m_isEnemyInfoPopup, IsEnemyInfoPopup);
	SYNTHESIZE(uiPopup *, m_pEnemyPopup, EnemyPopup);
	tileMap * m_pTileMap;
	uiManager * m_pUiMag;
	bulletManager * m_pBulletMag;

public:
	HRESULT init(tileMap * pTileMag, uiManager * pUiMag, bulletManager * pBulletMag);
	void release();
	void update();
	void render(HDC hdc);

	void enemyDrop(tagEnemyData* enemyInfo);

	vector<enemy*> getVecEnemy() { return m_vecEnemy; }

	enemyManager();
	~enemyManager();
};

