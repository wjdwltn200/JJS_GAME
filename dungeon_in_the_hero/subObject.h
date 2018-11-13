#pragma once

#define TILE_DIS_DELAY 100
#define TILE_DIS_MOVE_POWER 8.0f
#define TILE_DIS_COL_POWER 0.0f

#define HERO_START_DELAY 100
#define HERO_START_MOVE_POWER 15.0f
#define HERO_START_COL_POWER 0.0f
 
#define JEWEL_DIS_DELAY 120
#define JEWEL_DIS_MOVE_POWER 5.0f
#define JEWEL_DIS_COL_POWER 0.0f
#define JEWEL_DIS_SET_X 40.0f

class animation;

class subObject
{
private:
	SYNTHESIZE(bool, m_isAlive, isAlive);

	int m_type;

	image * m_imgObj;
	animation * m_aniObj;
	tagItemData m_tItemData;
	float m_fAngle;
	float m_fPosX;
	float m_fPosY;
	int m_alpha;
	int m_dieCount;
	float m_fScale;
	float m_movePower;
	float m_colPower;

public:
	HRESULT init(int typeNum, float posX, float posY, tagItemData itemData);
	void release();
	void update();
	void render(HDC hdc);

	void movement();

	subObject();
	~subObject();
};

