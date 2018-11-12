#pragma once
#include "animation.h"

#define IMG_TILE_SET_Y 5.0f
#define ENEMY_DIE_DALEY 60
#define ENEMY_ACT_DALEY 120

#define ENEMY_FLOWER_MANA 5


#define Flower_SET_RANDOM 50
#define FlowerV2_SET_RANDOM 90

#define BugV2_SET 3

class tileMap;
class enemyManager;
class aStarNode;
class bulletManager;

class enemy
{
private:
	list<aStarNode*> m_vecOpenNode;
	list<aStarNode*>::iterator	m_iterOpenNode;
	list<aStarNode*> m_vecCloseNode;
	list<aStarNode*>::iterator	m_iterCloseNode;
	list<aStarNode*> m_vecChildsNode;
	list<aStarNode*>::iterator	m_iterChildNode;
	list<aStarNode*> m_vecRoute;
	list<aStarNode*>::iterator	m_iterRoute;

	enum eMoveState
	{
		UP, RIGHT, DOWN, LEFT, MOVE_NUM
	};
	SYNTHESIZE(tagEnemyData, m_tEnemyData, tEnemyData);
	animation m_ani;

	int m_moveDaley;
	int m_eMoveState;
	bool m_isMoveAct;
	bool m_isAttAct;
	int m_aStarDepValue;
	SYNTHESIZE(int, m_backMoveArrow, BackMoveArrow);
	SYNTHESIZE(int, m_backValue, BackValue);
	SYNTHESIZE(bool, m_isBackMove, IsBackMove);
	float m_fBackMoveSpeed;

	tileMap * m_pTileMapMag;
	enemyManager * m_pEnemyMag;
	bulletManager * m_pBulletMag;
	aStarNode * m_pAStartNode;

	tagEnemyData tempEnemy;

	////2*2 enemy 타일 검사용 배열
	int enemySizeX[2] = { 0,1 };
	int enemySizeY[2] = { 0,1 };

public:
	HRESULT init(tagEnemyData* enemyInfo, tileMap* pTileMag, enemyManager * pEnemyMag, bulletManager * pBulletMag);
	void release();
	void update();
	void render(HDC hdc);

	int aStarisMove(aStarNode * pos, list<aStarNode*>* vecNode);
	void aStarRoute();
	bool aStarFind(aStarNode * endXY, aStarNode * node);
	void Delete(bool isOpen = false , bool isClose = false, bool isRoute = false);
	void SortOpenNode();
	bool NodeCompare(aStarNode * p1, aStarNode * p2);
	void InsertOpenNode(aStarNode * pNode);
	bool FindFromCloseNode(aStarNode * pNode);
	bool aStarIsRect(int x, int y);

	void currHp();
	void damge();
	void setDamgePoint(int DamValue) { m_tEnemyData.t_damgePoint = DamValue; }
	void moveSys();
	bool moveRectCheck(int eMoveArrow);
	bool backMoveCheck(int eMoveArrow);
	bool moveRectCheckBig(int eMoveArrow);
	bool moveIsRect(int eMoveArrow);
	bool tileManaChg(int eMoveArrow, int manaValue);
	bool tileManaDrain(int eMoveArrow, int manaValue);

	void enemySetTxt(int enemyType);
	void movePattern();
	void isBackMove(int isBackMove);
	void isDieTileMana();

	void backMoveArrow(int heroX, int heroY);

	bool eatActPattern();
	bool eatIsEnemy(int eMoveArrow);
	bool heroActPattern();
	bool isHero(int eMoveArrow);

	tagEnemyData * FlowerInfo();
	tagEnemyData * SlimeInfo();

	tagEnemyData * BugV2();


	enemy();
	~enemy();
};

