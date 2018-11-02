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
		UP, DOWN, LEFT, RIGHT, MOVE_NUM
	};
	SYNTHESIZE(tagHeroData, m_tHeroData, tHeroData);
	animation m_ani;

	int m_moveDaley;
	int m_eMoveState;
	bool m_isMoveAct;
	tileMap * m_pTileMapMag;
	aStarNode * m_pAStartNode;

public:
	HRESULT init(tagHeroData * heroInfo, tileMap * pTileMap);
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
	int aStarisMove(aStarNode * pos, list<aStarNode*> * verNode);

	bool moveRectCheck(int eMoveArrow);

	hero();
	~hero();
};

