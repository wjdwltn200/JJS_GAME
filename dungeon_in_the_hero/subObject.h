#pragma once

#define TILE_DIS_DELAY 100
#define TILE_DIS_MOVE_POWER 8.0f
#define TILE_DIS_COL_POWER 0.0f
 
class animation;

class subObject
{
private:
	SYNTHESIZE(bool, m_isAlive, isAlive);

	int m_type;

	image * m_imgObj;
	animation * m_aniObj;
	float m_fAngle;
	float m_fPosX;
	float m_fPosY;
	int m_alpha;
	int m_dieCount;
	float m_fScale;
	float m_movePower;
	float m_colPower;

public:
	HRESULT init(int typeNum, float posX, float posY);
	void release();
	void update();
	void render(HDC hdc);

	void movement();

	subObject();
	~subObject();
};

