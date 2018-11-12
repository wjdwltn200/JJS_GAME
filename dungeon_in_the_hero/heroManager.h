#pragma once

class hero;
class tileMap;
class bulletManager;
class overlord;

class heroManager
{
private:
	vector<hero*>	m_vecHero;
	vector<hero*>::iterator m_iter;

	tileMap * m_pTileMag;
	bulletManager * m_pBulletMag;
	overlord * m_pOverlord;

public:
	HRESULT init(tileMap * pTileMap, bulletManager * pBulletMag, overlord * m_pOverlord);
	void release();
	void update();
	void render(HDC hdc);

	void heroDrop(tagHeroData* enemyInfo);

	vector<hero*> getVecHero() { return m_vecHero; }

	heroManager();
	~heroManager();
};

