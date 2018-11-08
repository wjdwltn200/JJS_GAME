#pragma once
#include "animation.h"

#define IMG_TILE_SET_Y 5.0f
#define HERO_MOVE_DALEY 30

#define HERO_SKILL_HALING 30

#define HERO_SKILL_ATKBUFF 20
#define HERO_SKILL_ATKBUFF_POINT 5
#define HERO_SKILL_ATKBUFF_DALEY 1200

#define HERO_SKILL_DEFBUFF 20
#define HERO_SKILL_DEFBUFF_POINT 3
#define HERO_SKILL_DEFBUFF_DALEY 1200

#define HERO_SKILL_HASTE 25
#define HERO_SKILL_HASTE_SP_POINT 1.0f
#define HERO_SKILL_HASTE_MS_POINT 30
#define HERO_SKILL_HASTE_DALEY 600

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

	void damge();
	void setDamgePoint(int DamValue) { m_tHeroData.t_damgePoint = DamValue; }

	void skill_count();
	bool attSys();
	
	void buffIcon(HDC hdc);
	bool skill_Atk_0();
	bool skill_Haling();
	bool skill_AtkBuff();
	bool skill_DefBuff();
	bool skill_Haste();

	hero();
	~hero();
};

