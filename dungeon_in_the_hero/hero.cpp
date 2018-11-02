#include "stdafx.h"
#include "hero.h"
#include "tileMap.h"
#include "aStarNode.h"
#include "aStarPathFinding.h"

HRESULT hero::init(tagHeroData * heroInfo, tileMap * pTileMap)
{
	//// 타일 맵 주소 초기화
	m_pTileMapMag = pTileMap;

	m_tHeroData.t_isAilve = heroInfo->t_isAilve;
	m_tHeroData.t_posX = heroInfo->t_posX + CAMERA->getCamPosX();
	m_tHeroData.t_posY = heroInfo->t_posY + CAMERA->getCamPosY();

	m_tHeroData.t_img = heroInfo->t_img;
	m_tHeroData.t_scale = heroInfo->t_scale;
	m_tHeroData.t_alphaValue = heroInfo->t_alphaValue = 255;
	m_ani.init(m_tHeroData.t_img->getWidth(), m_tHeroData.t_img->getHeight(), m_tHeroData.t_img->getFrameWidth(), m_tHeroData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(15);
	m_ani.start();

	m_tHeroData.t_currHp = heroInfo->t_currHp;
	m_tHeroData.t_MaxHp = heroInfo->t_MaxHp;
	m_tHeroData.t_currDef = heroInfo->t_currDef;

	m_tHeroData.t_moveType = heroInfo->t_moveType;
	m_tHeroData.t_moveDaley = heroInfo->t_moveDaley;

	m_tHeroData.t_attType = heroInfo->t_attType;
	m_tHeroData.t_atkDaley = heroInfo->t_atkDaley;

	m_tHeroData.t_setTileMapNum = heroInfo->t_setTileMapNum;

	m_moveDaley = HERO_MOVE_DALEY;
	m_eMoveState = eMoveState::MOVE_NUM;

	m_eMoveState = RANDOM->getFromIntTo(0, 3);
	m_isMoveAct = true;

	//aStarFind(m_pAStartNode->Create(15, 4), m_pAStartNode->Create(m_tHeroData.t_tilePosX, m_tHeroData.t_tilePosY));

	//					//// 자신의 다음 이동위치는 하나씩 꺼낸다
	//if (m_vecRoute.size() != 0)
	//{
	//	aStarNode * tempNode = nullptr;
	//	for (m_iterRoute = m_vecRoute.begin(); m_iterRoute != m_vecRoute.end(); m_iterRoute++)
	//	{
	//		if ((*m_iterRoute)->getDepth() != 0)
	//		{
	//			tempNode = (*m_iterRoute);
	//			(*m_iterRoute)->setDepth(0);
	//			break;
	//		}
	//	}
	//	if (tempNode == nullptr)
	//		return;

	//	int x = tempNode->getPosX();
	//	int y = tempNode->getPosY();

	//	// 찾은 깊이 추가
	//	m_aStarDepValue++;

	//	if (tempNode->getDepth() == m_vecRoute.size())
	//		return;





	//	// 방향
	//	if (y < m_tEnemyData.t_tilePosY)
	//	{
	//		moveRectCheck(eMoveState::UP);
	//		m_eMoveState = eMoveState::UP;
	//	}

	//	if (x > m_tEnemyData.t_tilePosX)
	//	{
	//		moveRectCheck(eMoveState::RIGHT);
	//		m_eMoveState = eMoveState::RIGHT;
	//	}

	//	if (y > m_tEnemyData.t_tilePosY)
	//	{
	//		moveRectCheck(eMoveState::DOWN);
	//		m_eMoveState = eMoveState::DOWN;
	//	}

	//	if (x < m_tEnemyData.t_tilePosX)
	//	{
	//		moveRectCheck(eMoveState::LEFT);
	//		m_eMoveState = eMoveState::LEFT;
	//	}
	//}

	return S_OK;
}

void hero::release()
{
}

void hero::update()
{
}

void hero::render(HDC hdc)
{
		m_tHeroData.t_img->aniRender(hdc,
		(m_tHeroData.t_posX - m_tHeroData.t_img->getFrameWidth()) - CAMERA->getCamPosX(),
		(m_tHeroData.t_posY - m_tHeroData.t_img->getFrameHeight() - IMG_TILE_SET_Y) - CAMERA->getCamPosY(),
		&m_ani, m_tHeroData.t_scale, true, m_tHeroData.t_alphaValue);
}

int hero::aStarisMove(aStarNode * pos, list<aStarNode*> * vecNode)
{
	//int distX[3] = { -1, 0, 1 };
	//int distY[3] = { -1, 0, 1 };
	//int tempTileX = m_tEnemyData.t_tilePosX;
	//int tempTileY = m_tEnemyData.t_tilePosY;

	//for (int y = 1; y < 3; y++)
	//{
	//	for (int x = 1; x < 3; x++)
	//	{
	//		//int cx = distX[x] + pos->getPosX();
	//		//int cy = distX[y] + pos->getPosY();

	//		int cx = distX[x] + pos->getPosX();
	//		int cy = distX[y] + pos->getPosY();

	//		if (cx == pos->getPosX() && cy == pos->getPosY()) continue;

	//		if (aStarIsRect(cx, cy)) continue;
	//		vecNode->push_back(m_pAStartNode->Create(cx, cy));
	//	}
	//}

	int tempTileX = m_tHeroData.t_tilePosX;
	int tempTileY = m_tHeroData.t_tilePosY;

	for (int i = 0; i < 4; i++)
	{
		int cy = pos->getPosY(), cx = pos->getPosX();
		switch (i)
		{
		case eMoveState::UP:
			cy = pos->getPosY() + 1;
			break;
		case eMoveState::RIGHT:
			cx = pos->getPosX() + 1;
			break;
		case eMoveState::DOWN:
			cy = pos->getPosY() - 1;
			break;
		case eMoveState::LEFT:
			cx = pos->getPosX() - 1;
			break;
		}

		if (cx == pos->getPosX() && cy == pos->getPosY()) continue;

		if (aStarIsRect(cx, cy)) continue;
		vecNode->push_back(m_pAStartNode->Create(cx, cy));
		EFFMANAGER->play("MousePointEFF", m_pTileMapMag->getTileSetPoint()[cx * m_pTileMapMag->getTileSizeY() + cy].t_rc.left + TILE_SIZE / 2, m_pTileMapMag->getTileSetPoint()[cx * m_pTileMapMag->getTileSizeY() + cy].t_rc.top + TILE_SIZE / 2);
	}

	return sizeof(vecNode);
}

bool hero::aStarFind(aStarNode * endXY, aStarNode * node)
{
	Delete(true, true);



	// 임시 aStar 생성
	aStarNode * tempNode = node->Clone();
	m_vecOpenNode.push_back(tempNode);
	// 검색 깊이(비용?)
	int iDepth = 0;
	tempNode->setDepth(iDepth);

	while (true)
	{
		if (m_vecOpenNode.size() == 0) break;

		// openList 첫번째를 담는다
		m_iterOpenNode = m_vecOpenNode.begin();
		tempNode = (*m_iterOpenNode);

		// 꺼낸 첫번째를 없앤다
		m_vecOpenNode.pop_front();

		// 목적지에 도착했다면
		if (tempNode->getPosX() == endXY->getPosX() &&
			tempNode->getPosY() == endXY->getPosY())
		{

			int tempX = endXY->getPosX();
			int tempY = endXY->getPosY();
			// 함수를 종료한다
			EFFMANAGER->play("MousePointEFF", m_pTileMapMag->getTileSetPoint()[tempX * m_pTileMapMag->getTileSizeY() + tempY].t_rc.left, m_pTileMapMag->getTileSetPoint()[tempX * m_pTileMapMag->getTileSizeY() + tempY].t_rc.top);
			return true;
		}


		// 도착 못했다면 닫힌 노드에 넣어준다
		m_vecCloseNode.push_back(tempNode);
		iDepth++; // 검색 깊이를 1 증가 시킨다
		m_vecChildsNode.clear();

		aStarisMove(tempNode, &m_vecChildsNode);
		for (m_iterChildNode = m_vecChildsNode.begin(); m_iterChildNode != m_vecChildsNode.end(); m_iterChildNode++)
		{
			if (FindFromCloseNode((*m_iterChildNode))) continue;

			(*m_iterChildNode)->CalcDist(endXY, iDepth);
			(*m_iterChildNode)->setParent(tempNode);
			InsertOpenNode((*m_iterChildNode));
		}

		SortOpenNode();
	}

	// 못찾았을 경우
	Delete(true, true);

	return false;
}

void hero::Delete(bool isOpen, bool isClose, bool isRoute)
{
	if (isOpen)
	{
		for (m_iterOpenNode = m_vecOpenNode.begin();
			m_iterOpenNode != m_vecOpenNode.end();
			m_iterOpenNode++)
		{
			delete (*m_iterOpenNode);
		}
		m_vecOpenNode.clear();
	}

	if (isClose)
	{
		for (m_iterCloseNode = m_vecCloseNode.begin();
			m_iterCloseNode != m_vecCloseNode.end();
			m_iterCloseNode++)
		{
			delete (*m_iterCloseNode);
		}
		m_vecCloseNode.clear();
	}

	if (isRoute)
	{
		for (int i = 0; i < m_vecRoute.size(); i++)
		{
			m_vecRoute.pop_front();
		}
		m_vecRoute.clear();
	}
}

void hero::SortOpenNode()
{
	if (m_vecOpenNode.size() < 2) return;
	aStarNode * pNode;
	list<aStarNode*>::iterator tempIter;
	int tempVec = m_vecOpenNode.size();
	bool bContinue = true;

	while (bContinue)
	{
		bContinue = false;
		for (m_iterOpenNode = m_vecOpenNode.begin();
			tempVec < m_vecOpenNode.size() - 1;
			m_iterOpenNode++)
		{
			tempVec--;
			tempIter = m_iterOpenNode;
			++tempIter;

			if (!NodeCompare((*m_iterOpenNode), (*tempIter)))
			{
				pNode = (*m_iterOpenNode);

				(*m_iterOpenNode) = (*tempIter);
				(*tempIter) = pNode;

				bContinue = true;
			}
		}
	}
}

bool hero::NodeCompare(aStarNode * p1, aStarNode * p2)
{
	if (p1->getDist() < p2->getDist()) return true;
	if (p1->getDist() > p2->getDist()) return false;
	if (p1->getDepth() <= p2->getDepth()) return true;

	return false;
}

void hero::InsertOpenNode(aStarNode * pNode)

{
	for (m_iterOpenNode = m_vecOpenNode.begin();
		m_iterOpenNode != m_vecOpenNode.end();
		m_iterOpenNode++)
	{
		if ((*m_iterOpenNode)->isSamePos(pNode))
		{
			m_vecCloseNode.push_back((*m_iterOpenNode));
			(*m_iterOpenNode) = pNode;
			return;
		}
	}

	m_vecOpenNode.push_back(pNode);
}

bool hero::FindFromCloseNode(aStarNode * pNode)
{
	for (m_iterCloseNode = m_vecCloseNode.begin();
		m_iterCloseNode != m_vecCloseNode.end();
		m_iterCloseNode++)
	{
		if ((*m_iterCloseNode)->isSamePos(pNode)) return true;
	}
	return false;
}

bool hero::aStarIsRect(int x, int y)
{
	if (x < 0 || x >= m_pTileMapMag->getTileSizeX()) return false;
	if (y < 0 || y >= m_pTileMapMag->getTileSizeY()) return false;

	if (!m_pTileMapMag->getTileSetPoint()[x * m_pTileMapMag->getTileSizeY() + y].t_isAlive) return false;
	return true;
}

bool hero::moveRectCheck(int eMoveArrow)
{
	switch (m_eMoveState)
	{
	case eMoveState::UP:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum - 1].t_isAlive) return false;
		break;
	case eMoveState::DOWN:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum + 1].t_isAlive) return false;
		break;
	case eMoveState::LEFT:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum - (m_pTileMapMag->getTileSizeY())].t_isAlive) return false;
		break;
	case eMoveState::RIGHT:
		if (m_pTileMapMag->getTileSetPoint()[m_tHeroData.t_setTileMapNum + (m_pTileMapMag->getTileSizeY())].t_isAlive) return false;
		break;
	}

	return true;
}

hero::hero()
{
}


hero::~hero()
{
}
