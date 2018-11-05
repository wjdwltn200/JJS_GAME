#pragma once
#include "animation.h"

#define IMG_TILE_SET_Y 5.0f
#define HERO_MOVE_DALEY 30

class tileMap;
class aStarNode;

class hero
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
	SYNTHESIZE(tagHeroData, m_tHeroData, tHeroData);
	animation m_ani;

	int m_moveDaley;
	int m_eMoveState;
	bool m_isMoveAct;
	bool m_isAttAct;
	int m_aStarDepValue;
	bool m_isDead;

	tileMap * m_pTileMapMag;
	aStarNode * m_pAStartNode;

public:
	HRESULT init(tagHeroData * heroInfo, tileMap * pTileMap);
	void release();
	void update();
	void render(HDC hdc);

	int aStarisMove(aStarNode * pos, list<aStarNode*>* vecNode);
	void aStarRoute();
	bool aStarFind(aStarNode * endXY, aStarNode * node);
	void Delete(bool isOpen = false, bool isClose = false, bool isRoute = false);
	void SortOpenNode();
	bool NodeCompare(aStarNode * p1, aStarNode * p2);
	void InsertOpenNode(aStarNode * pNode);
	bool FindFromCloseNode(aStarNode * pNode);
	bool aStarIsRect(int x, int y);

	void currHp();
	void moveSys();
	bool moveRectCheck(int eMoveArrow);
	bool moveIsRect(int eMoveArrow);
	void aStarMoveSys();

	void heroSetTxt(int enemyType);
	void movePattern();

	bool monActPattern();

	bool IsEnemy(int eMoveArrow);

	void damge(int atkPoint);

	hero();
	~hero();
};

