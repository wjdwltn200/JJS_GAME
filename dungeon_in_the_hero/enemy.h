#pragma once
#include "animation.h"

#define IMG_TILE_SET_Y 5.0f
#define ENEMY_DIE_DALEY 60
#define ENEMY_MOVE_DALEY 10

class tileMap;
class enemyManager;
class aStarNode;

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
	int m_aStarDepValue;

	tileMap * m_pTileMapMag;
	enemyManager * m_pEnemyMag;
	aStarNode * m_pAStartNode;

public:
	HRESULT init(tagEnemyData* enemyInfo, tileMap* pTileMag, enemyManager * pEnemyMag);
	void release();
	void update();
	void render(HDC hdc);

	bool aStarIsRect(int x, int y);
	bool aStarFind(aStarNode * endXY, aStarNode * node);
	void Delete(bool isOpen = false, bool isClose = false, bool isRoute = false);
	void SortOpenNode();
	bool NodeCompare(aStarNode * p1, aStarNode * p2);
	void InsertOpenNode(aStarNode * pNode);
	bool FindFromCloseNode(aStarNode * pNode);
	int aStarisMove(aStarNode * pos, list<aStarNode*> * vecNode);

	void aStarRoute();

	void currHp();
	void moveSys();
	bool moveRectCheck(int eMoveArrow);
	bool moveIsRect(int eMoveArrow);
	bool tileManaChg(int eMoveArrow, int manaValue);

	void enemySetTxt(int enemyType);
	void movePattern();
	void isDieTileMana();

	bool eatIsEnemy(int eMoveArrow);

	enemy();
	~enemy();
};

