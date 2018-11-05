#pragma once

class hero;
class tileMap;

class heroManager
{
private:
	vector<hero*>	m_vecHero;
	vector<hero*>::iterator m_iter;

	tileMap * m_pTileMap;

public:
	HRESULT init(tileMap * pTileMap);
	void release();
	void update();
	void render(HDC hdc);

	void heroDrop(tagHeroData* enemyInfo);

	vector<hero*> getVecHero() { return m_vecHero; }

	heroManager();
	~heroManager();
};

