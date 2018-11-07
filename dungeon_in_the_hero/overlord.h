#pragma once
#include "animation.h"

class tileMap;

class overlord
{
private:
	SYNTHESIZE(tagOverlord, m_tOverlord, tOverlord);
	animation m_ani;
	
	tileMap * m_pTileMap;

	bool m_isSetting;

public:
	HRESULT init(tileMap * pTileMap);
	void release();
	void update();
	void render(HDC hdc);

	overlord();
	~overlord();
};

