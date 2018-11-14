#pragma once

class hero;
class tileMap;
class bulletManager;
class overlord;
class uiManager;

class heroManager
{
private:
	vector<hero*>	m_vecHero;
	vector<hero*>::iterator m_iter;

	tileMap * m_pTileMag;
	bulletManager * m_pBulletMag;
	overlord * m_pOverlord;
	uiManager * m_pUiMag;

public:
	HRESULT init(tileMap * pTileMag, bulletManager * pBulletMag, overlord * pOverlord, uiManager * pUiMag);
	void release();
	void update();
	void render(HDC hdc);

	void heroDrop(tagHeroData* enemyInfo);

	vector<hero*> getVecHero() { return m_vecHero; }

	heroManager();
	~heroManager();
};

