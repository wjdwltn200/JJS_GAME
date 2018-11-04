#include "stdafx.h"
#include "enemy.h"
#include "animation.h"
#include "tileMap.h"
#include "enemyManager.h"
#include "uiManager.h"
#include "aStarNode.h"

HRESULT enemy::init(tagEnemyData* enemyInfo, tileMap* pTileMag, enemyManager * pEnemyMag)
{
	//// 타일 맵 주소 초기화
	m_pTileMapMag = pTileMag;
	m_pEnemyMag = pEnemyMag;

	m_tEnemyData.t_isAilve = enemyInfo->t_isAilve;
	m_tEnemyData.t_posX = enemyInfo->t_posX;
	m_tEnemyData.t_posY = enemyInfo->t_posY;
	m_tEnemyData.t_tilePosX = enemyInfo->t_tilePosX;
	m_tEnemyData.t_tilePosY = enemyInfo->t_tilePosY;

	m_tEnemyData.t_moveEndX = m_tEnemyData.t_posX;
	m_tEnemyData.t_moveEndY = m_tEnemyData.t_posY;

	m_tEnemyData.t_img_R = enemyInfo->t_img_R;
	m_tEnemyData.t_img_L = enemyInfo->t_img_L;
	m_tEnemyData.t_img_RA = enemyInfo->t_img_RA;
	m_tEnemyData.t_img_LA = enemyInfo->t_img_LA;
	m_tEnemyData.t_img_Dead = enemyInfo->t_img_Dead;

	m_tEnemyData.t_img = enemyInfo->t_img_L;
	m_tEnemyData.t_scale = enemyInfo->t_scale;
	m_tEnemyData.t_alphaValue = enemyInfo->t_alphaValue = 255;
	m_ani.init(m_tEnemyData.t_img->getWidth(), m_tEnemyData.t_img->getHeight(), m_tEnemyData.t_img->getFrameWidth(), m_tEnemyData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, false);
	m_ani.setFPS(15);

	m_tEnemyData.t_currHp = enemyInfo->t_currHp;
	m_tEnemyData.t_MaxHp = enemyInfo->t_MaxHp;
	m_tEnemyData.t_currDef = enemyInfo->t_currDef;

	m_tEnemyData.t_moveSpeed = enemyInfo->t_moveSpeed;
	m_tEnemyData.t_moveDaley = enemyInfo->t_moveDaley;

	m_tEnemyData.t_atkPoint = enemyInfo->t_atkPoint;
	m_tEnemyData.t_attType = enemyInfo->t_attType;
	m_tEnemyData.t_atkDaley = enemyInfo->t_atkDaley;

	m_tEnemyData.t_defPoint = enemyInfo->t_defPoint;

	m_tEnemyData.t_DieCountDaley = ENEMY_DIE_DALEY;
	m_tEnemyData.t_setTileMapNum = enemyInfo->t_setTileMapNum;

	m_tEnemyData.t_FoodChainLv = enemyInfo->t_FoodChainLv;
	m_tEnemyData.t_currMana = enemyInfo->t_currMana;

	m_tEnemyData.t_rc = RectMakeCenter(m_tEnemyData.t_posX, m_tEnemyData.t_posY, m_tEnemyData.t_img->getFrameWidth(), m_tEnemyData.t_img->getFrameHeight());
	m_tEnemyData.t_enumType = enemyInfo->t_enumType;

	m_moveDaley = m_tEnemyData.t_moveDaley;
	m_aStarDepValue = 0;
	m_isDead = false;
	m_isAttAct = false;

	Delete(true, true);

	m_isMoveAct = false;
	switch (m_tEnemyData.t_enumType)
	{
	//case tagEnemyType::Rat:
	//	m_eMoveState = eMoveState::LEFT;
	//	break;
	case tagEnemyType::Slime:
		m_eMoveState = RANDOM->getFromIntTo(0, 3);
		break;
	//case tagEnemyType::Daemon:
	//	for (int i = 0; i < 4; i++)
	//	{
	//		if (moveRectCheck(i))
	//		{
	//			m_eMoveState = i;
	//			m_ani.start();
	//			m_isMoveAct = true;
	//			break;
	//		}
	//	}
	//	break;
	default:
		m_eMoveState = RANDOM->getFromIntTo(0, 3);
		break;
	}

	//aStarFind(m_pAStartNode->Create(15, 4), m_pAStartNode->Create(m_tEnemyData.t_tilePosX, m_tEnemyData.t_tilePosY));
	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	if (!m_tEnemyData.t_isAilve) return;

	RECT tempRc;
	if (
		KEYMANAGER->isStayKeyDown(VK_RBUTTON) &&
		(IntersectRect(&tempRc, &m_tEnemyData.t_rc, &g_MouseRc)))
	{
		enemySetTxt(m_tEnemyData.t_enumType);
		m_pEnemyMag->setIsEnemyInfoPopup(true);
		m_pEnemyMag->getEnemyPopup()->setImg(IMAGEMANAGER->findImage("EnemyPopup"));
		m_pEnemyMag->getEnemyPopup()->setDaley(POPUP_COUNT);
		m_pEnemyMag->getEnemyPopup()->setIsAilve(true);
		m_pEnemyMag->getEnemyPopup()->setXY(
			m_tEnemyData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2),
			m_tEnemyData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2) - TILE_POPUP_Y_SIZE);
	}


	moveSys();
	currHp();
	m_ani.frameUpdate();
}

void enemy::render(HDC hdc)
{
	m_ani.init(m_tEnemyData.t_img->getWidth(), m_tEnemyData.t_img->getHeight(), m_tEnemyData.t_img->getFrameWidth(), m_tEnemyData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, false);
	m_ani.setFPS(15);

	if (g_saveData.gisTest)
	{
		Rectangle(hdc, m_tEnemyData.t_rc.left, m_tEnemyData.t_rc.top, m_tEnemyData.t_rc.right, m_tEnemyData.t_rc.bottom);

		char szText[256];
		sprintf_s(szText, "%d,%d", m_tEnemyData.t_currMana , m_moveDaley);
		TextOut(hdc, m_tEnemyData.t_posX - CAMERA->getCamPosX(), m_tEnemyData.t_posY - CAMERA->getCamPosY(), szText, strlen(szText));
	}

	if (!m_isDead && !m_isAttAct)
	{
		if (m_eMoveState == eMoveState::LEFT)
		{
			m_tEnemyData.t_img = m_tEnemyData.t_img_L;
		}
		else if (m_eMoveState == eMoveState::RIGHT)
		{
			m_tEnemyData.t_img = m_tEnemyData.t_img_R;
		}
	}

	

	// 스크린 랜더 조건
	if (!(m_tEnemyData.t_rc.left < -TILE_SIZE || m_tEnemyData.t_rc.left > WINSIZEX) &&
	!(m_tEnemyData.t_rc.top < -TILE_SIZE || m_tEnemyData.t_rc.top > WINSIZEY))
	{
		m_tEnemyData.t_img->aniRender(hdc,
			(m_tEnemyData.t_posX + (TILE_SIZE / 2) - (m_tEnemyData.t_img->getFrameWidth() * m_tEnemyData.t_scale) / 2) - CAMERA->getCamPosX(),
			((m_tEnemyData.t_posY + TILE_SIZE ) - (m_tEnemyData.t_img->getFrameHeight() * m_tEnemyData.t_scale)) - CAMERA->getCamPosY(),
			&m_ani, m_tEnemyData.t_scale, true, m_tEnemyData.t_alphaValue);
	}

	m_tEnemyData.t_rc = RectMake(
		m_tEnemyData.t_posX - CAMERA->getCamPosX(),
		m_tEnemyData.t_posY - CAMERA->getCamPosY(),
		TILE_SIZE,
		TILE_SIZE);
}

int enemy::aStarisMove(aStarNode * pos, list<aStarNode*> * vecNode)
{
	int tempTileX = m_tEnemyData.t_tilePosX;
	int tempTileY = m_tEnemyData.t_tilePosY;

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
	}

	return sizeof(vecNode);
}

void enemy::aStarRoute()
{
	if (m_vecRoute.size() != 0)
	{
		aStarNode * tempStar;
		m_iterRoute = m_vecRoute.begin();
		tempStar = (*m_iterRoute);
		while (true)
		{
			m_vecRoute.push_front(tempStar->getParent());
			tempStar = tempStar->getParent();

			if (tempStar->getDepth() == 0)
				break;
		}

	}
}

bool enemy::aStarFind(aStarNode * endXY, aStarNode * node)
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
		if (endXY->isSamePos(tempNode))
		{
			// 함수를 종료한다
			m_vecRoute.push_back(tempNode);
			tempNode = tempNode->getParent();

			aStarRoute();
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

void enemy::Delete(bool isOpen, bool isClose, bool isRoute)
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
		if (m_vecRoute.size() == 0) return;
		delete m_vecRoute.back();
		m_vecRoute.clear();
	}
}

void enemy::SortOpenNode()
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

bool enemy::NodeCompare(aStarNode * p1, aStarNode * p2)
{
	if (p1->getDist() < p2->getDist()) return true;
	if (p1->getDist() > p2->getDist()) return false;
	if (p1->getDepth() <= p2->getDepth()) return true;

	return false;
}

void enemy::InsertOpenNode(aStarNode * pNode)
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

bool enemy::FindFromCloseNode(aStarNode * pNode)
{
	for (m_iterCloseNode = m_vecCloseNode.begin();
		m_iterCloseNode != m_vecCloseNode.end();
		m_iterCloseNode++)
	{
		if ((*m_iterCloseNode)->isSamePos(pNode)) return true;
	}
	return false;
}

bool enemy::aStarIsRect(int x, int y)
{
	if (x < 0 || x >= m_pTileMapMag->getTileSizeX()) return false;
	if (y < 0 || y >= m_pTileMapMag->getTileSizeY()) return false;

	if (!m_pTileMapMag->getTileSetPoint()[x * m_pTileMapMag->getTileSizeY() + y].t_isAlive) return false;
	return true;
}

void enemy::currHp()
{
	if (m_isDead && m_ani.getIsPlaying())
	{
		m_tEnemyData.t_alphaValue += 50;
		return;
	}
	else if (m_isDead && !m_ani.getIsPlaying())
	{
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo == &m_tEnemyData)
			m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo = nullptr;
		Delete(true, true, true);
		isDieTileMana();
		m_tEnemyData.t_isAilve = false;
	}

	m_tEnemyData.t_DieCountDaley--;
	if (m_tEnemyData.t_DieCountDaley <= 0)
	{
		m_tEnemyData.t_currHp--;
		m_tEnemyData.t_DieCountDaley = ENEMY_DIE_DALEY;
	}

	if (m_tEnemyData.t_currHp > m_tEnemyData.t_MaxHp) // 최대 체력 초과 불가
		m_tEnemyData.t_currHp = m_tEnemyData.t_MaxHp;

	if (m_tEnemyData.t_currHp <= 0) // 죽음
	{
		m_tEnemyData.t_img = m_tEnemyData.t_img_Dead;
		m_ani.start();
		m_isDead = true;
	}
}

void enemy::moveSys()
{
	// 사망 대기 상태라면 움직이지 않는다
	if (m_isDead) return;

	// 행동 액션 여부
	if (!m_isMoveAct)
	{
		if (m_moveDaley <= 0 && !m_ani.getIsPlaying())
		{
			// 지정된 위치로 좌표 및 변경
			movePattern();
		}
		m_moveDaley--;
	}
	else
	{
		// 이동 액션 실행
		switch (m_eMoveState)
		{
		case eMoveState::UP:
			// 이동 거리
			if (m_tEnemyData.t_posY > m_tEnemyData.t_moveEndY)
				m_tEnemyData.t_posY -= m_tEnemyData.t_moveSpeed;

			// 이동 거리 도착 시
			if (m_tEnemyData.t_posY <= m_tEnemyData.t_moveEndY)
			{
				// 위치를 이동하려고 했던 거리로 초기화
				m_tEnemyData.t_posY = m_tEnemyData.t_moveEndY;
				// 이동 액션 종료 bool 문
				m_isMoveAct = false;
				// 다음 이동 액션까지 딜레이
				m_moveDaley = m_tEnemyData.t_moveDaley;
				// 움직임 정지
				eatActPattern();
			}
			break;
		case eMoveState::DOWN:
			if (m_tEnemyData.t_posY < m_tEnemyData.t_moveEndY)
				m_tEnemyData.t_posY += m_tEnemyData.t_moveSpeed;

			if (m_tEnemyData.t_posY >= m_tEnemyData.t_moveEndY)
			{
				m_tEnemyData.t_posY = m_tEnemyData.t_moveEndY;
				m_isMoveAct = false;
				m_moveDaley = m_tEnemyData.t_moveDaley;
				eatActPattern();
			}
			break;
		case eMoveState::LEFT:
			if (m_tEnemyData.t_posX > m_tEnemyData.t_moveEndX)
				m_tEnemyData.t_posX -= m_tEnemyData.t_moveSpeed;

			if (m_tEnemyData.t_posX <= m_tEnemyData.t_moveEndX)
			{
				m_tEnemyData.t_posX = m_tEnemyData.t_moveEndX;
				m_isMoveAct = false;
				m_moveDaley = m_tEnemyData.t_moveDaley;
				eatActPattern();
			}
			break;
		case eMoveState::RIGHT:
			if (m_tEnemyData.t_posX < m_tEnemyData.t_moveEndX)
				m_tEnemyData.t_posX += m_tEnemyData.t_moveSpeed;

			if (m_tEnemyData.t_posX >= m_tEnemyData.t_moveEndX)
			{
				m_tEnemyData.t_posX = m_tEnemyData.t_moveEndX;
				m_isMoveAct = false;
				m_moveDaley = m_tEnemyData.t_moveDaley;
				eatActPattern();
			}
			break;
		case eMoveState::MOVE_NUM:
			m_isMoveAct = false;
			m_moveDaley = m_tEnemyData.t_moveDaley;
			eatActPattern();
			break;
		}
	}
}

bool enemy::moveRectCheck(int eMoveArrow)
{
	switch (eMoveArrow)
	{
	case eMoveState::UP:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY - 1)].t_isAlive) return false;
		m_tEnemyData.t_tilePosY -= 1;
		m_tEnemyData.t_moveEndY = m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_rc.top + CAMERA->getCamPosY();
		break;
	case eMoveState::DOWN:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY + 1)].t_isAlive) return false;
		m_tEnemyData.t_tilePosY += 1;
		m_tEnemyData.t_moveEndY = m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_rc.top + CAMERA->getCamPosY();
		break;
	case eMoveState::LEFT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_isAlive) return false;
		m_tEnemyData.t_tilePosX -= 1;
		m_tEnemyData.t_moveEndX = m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_rc.left + CAMERA->getCamPosX();
		break;
	case eMoveState::RIGHT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_isAlive) return false;
		m_tEnemyData.t_tilePosX += 1;
		m_tEnemyData.t_moveEndX = m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_rc.left + CAMERA->getCamPosX();
		break;
	case eMoveState::MOVE_NUM:
		m_ani.stop();
		break;
	}

	return true;
}

bool enemy::moveIsRect(int eMoveArrow)
{
	switch (eMoveArrow)
	{
	case eMoveState::UP:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY - 1)].t_isAlive) return false;
		break;
	case eMoveState::DOWN:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY + 1)].t_isAlive) return false;
		break;
	case eMoveState::LEFT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_isAlive) return false;
		break;
	case eMoveState::RIGHT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_isAlive) return false;
		break;
	case eMoveState::MOVE_NUM:
		return false;
		break;
	}

	return true;
}

bool enemy::tileManaChg(int eMoveArrow, int manaValue)
{
	int tempMoveArrow;

	switch (eMoveArrow)
	{
	case eMoveState::UP:
		tempMoveArrow = (m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY - 1);
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (!m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_isAlive || m_tEnemyData.t_currMana < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue += manaValue;
		m_tEnemyData.t_currMana -= manaValue;
		break;
	case eMoveState::DOWN:
		tempMoveArrow = (m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY + 1);
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (!m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_isAlive || m_tEnemyData.t_currMana < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue += manaValue;
		m_tEnemyData.t_currMana -= manaValue;
		break;
	case eMoveState::LEFT:
		tempMoveArrow = (m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (!m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_isAlive || m_tEnemyData.t_currMana < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue += manaValue;
		m_tEnemyData.t_currMana -= manaValue;
		break;
	case eMoveState::RIGHT:
		tempMoveArrow = (m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (!m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_isAlive || m_tEnemyData.t_currMana < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue += manaValue;
		m_tEnemyData.t_currMana -= manaValue;
		break;
	}


	//EFFMANAGER->play("Enemy_Eat_R",
	//	m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.left + (TILE_SIZE / 2),
	//	m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.top + (TILE_SIZE / 2));

	return true;
}

bool enemy::tileManaDrain(int eMoveArrow, int manaValue)
{
	int tempMoveArrow;

	switch (eMoveArrow)
	{
	case eMoveState::UP:
		tempMoveArrow = (m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY - 1);
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue -= manaValue;
		m_tEnemyData.t_currMana += manaValue;
		break;
	case eMoveState::DOWN:
		tempMoveArrow = (m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY + 1);
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue -= manaValue;
		m_tEnemyData.t_currMana += manaValue;
		break;
	case eMoveState::LEFT:
		tempMoveArrow = (m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue -= manaValue;
		m_tEnemyData.t_currMana += manaValue;
		break;
	case eMoveState::RIGHT:
		tempMoveArrow = (m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue == -1) return false;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue < manaValue) return false;
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_ManaValue -= manaValue;
		m_tEnemyData.t_currMana += manaValue;
		break;
	}

	//EFFMANAGER->play("Enemy_Eat",
	//	m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.left + (TILE_SIZE / 2),
	//	m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.top + (TILE_SIZE / 2));
	return true;
}

void enemy::enemySetTxt(int enemyType)
{
	tagTileTxt setTxt;

	switch (enemyType)
	{
	case tagEnemyType::Slime:
		setTxt.t_txtName = "액괴";
		setTxt.t_txtInfo = "이래뵈도, 움직이는 영양덩어리 입니다.";
		break;
	case tagEnemyType::Bug:
		setTxt.t_txtName = "볼레레";
		setTxt.t_txtInfo = "액괴를 먹는 볼레입니다 하얗고 귀엽죠?";
		break;
	}

	setTxt.t_AtkPoint = m_tEnemyData.t_atkPoint;
	setTxt.t_DefPoint = m_tEnemyData.t_defPoint;
	setTxt.t_HpPoint = m_tEnemyData.t_currHp;
	setTxt.t_MaxHpPoint = m_tEnemyData.t_MaxHp;

	m_pEnemyMag->getEnemyPopup()->setTxtInfo(setTxt);
}

void enemy::movePattern()
{
	// 공격 여부는 이동하기 전에 행동하여 이동을 실시하지 않도록 해야한다
	m_isAttAct = false;
	if (RANDOM->getInt(100) < 50)
	{
		m_tEnemyData.t_img = m_tEnemyData.t_img_L;
	}
	else
	{
		m_tEnemyData.t_img = m_tEnemyData.t_img_R;
	}

	//// 최초 행동 방향 저장
	int tempMoveState = m_eMoveState;
	//// 이동 시 자신의 위치에 자신의 정보가 있다면 지워준다
	if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo == &m_tEnemyData)
		m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo = nullptr;

	//// enemyType에 맞춰 이동 패턴 적용
	switch (m_tEnemyData.t_enumType)
	{
	case tagEnemyType::Bug: // 이동하다가 방향 전환
		while (!moveRectCheck(m_eMoveState))
			m_eMoveState = RANDOM->getFromIntTo(0, 3);
		m_isMoveAct = true;
		break;
	//case tagEnemyType::Rat:
	//	m_eMoveState = 0;
	//	if (moveIsRect(m_eMoveState) && eMoveState::DOWN != tempMoveState)
	//	{
	//		m_eMoveState = m_eMoveState;
	//		moveRectCheck(m_eMoveState);
	//	}
	//	else if (moveIsRect(m_eMoveState + 1) && eMoveState::LEFT != tempMoveState)
	//	{
	//		m_eMoveState = m_eMoveState + 1;
	//		moveRectCheck(m_eMoveState);
	//	}
	//	else if (moveIsRect(m_eMoveState + 2) && eMoveState::UP != tempMoveState)
	//	{
	//		m_eMoveState = m_eMoveState + 2;
	//		moveRectCheck(m_eMoveState);
	//	}
	//	else if (moveIsRect(m_eMoveState + 3) && eMoveState::RIGHT != tempMoveState)
	//	{
	//		m_eMoveState = m_eMoveState + 3;
	//		moveRectCheck(m_eMoveState);
	//	}
	//	else if ((!(moveIsRect(m_eMoveState) && !(moveIsRect(m_eMoveState + 1) && !(moveIsRect(m_eMoveState + 3))))))
	//	{
	//		m_eMoveState = m_eMoveState + 2;
	//		moveRectCheck(m_eMoveState);
	//	}
	//	m_isMoveAct = true;
	//	break;
	case tagEnemyType::Flower:
		if (m_tEnemyData.t_currHp < m_tEnemyData.t_MaxHp / 2)
		{
			for (int i = 0; i < 2; i++)
			{
				m_pEnemyMag->enemyDrop(SlimeInfo());
			}

			m_tEnemyData.t_currHp = 0;
			return;
		}
		m_isMoveAct = true;
		break;
	case tagEnemyType::Slime: // 박을때 까지 이동
		//// Mana가 일정 이상 모였을 경우 꽃으로 변신
		if (m_tEnemyData.t_currMana >= ENEMY_FLOWER_MANA)
		{
			m_pEnemyMag->enemyDrop(FlowerInfo());
			m_tEnemyData.t_currHp = 0;
			return;
		}

		if (RANDOM->getInt(100) < 60) // 4방향을 검색하여 타일 Mana를 흡수
		{
			for (int i = 0; i < 4; i++)
			{
				if (tileManaDrain(i, 1))
				{
					m_tEnemyData.t_scale += 0.1f;
					break; // 1번이라도 흡수하면 이동한다
				}
			}
		}
		else // 4방향을 검색하여 자신의 Mana를 배출
		{
			for (int i = 0; i < 4; i++) 
			{
				if (tileManaChg(i, 1))
				{
					m_tEnemyData.t_scale -= 0.1f;
					break; // 1번이라도 배출하면 이동한다
				}
			}
		}

		// 벽에 충돌할때까지 이동 => 충돌했을 경우 다른 이동 가능한 곳을 검색하여 이동
		while (!moveRectCheck(m_eMoveState))
			m_eMoveState = RANDOM->getFromIntTo(0, 3);

		// 이동 설정을 true로 만들어 해당 유닛이 다음 위치까지 이동하도록 만들어준다.
		m_isMoveAct = true;
		break;
	//case tagEnemyType::Daemon: // 벽에 닿으면 반대 방향으로 이동
	//	if (moveRectCheck(m_eMoveState))
	//	{
	//		m_isMoveAct = true;
	//	}
	//	else
	//	{
	//		if (m_eMoveState == eMoveState::UP)
	//		{
	//			m_eMoveState = eMoveState::DOWN;
	//		}
	//		else if (m_eMoveState == eMoveState::DOWN)
	//		{
	//			m_eMoveState = eMoveState::UP;
	//		}
	//		if (m_eMoveState == eMoveState::LEFT)
	//		{
	//			m_eMoveState = eMoveState::RIGHT;
	//		}
	//		else if (m_eMoveState == eMoveState::RIGHT)
	//		{
	//			m_eMoveState = eMoveState::LEFT;
	//		}
	//		m_isMoveAct = true;
	//	}
	//	break;
	default: // 패턴이 없을 경우 랜덤 이동
		m_eMoveState = RANDOM->getFromIntTo(0, 4);
		moveRectCheck(m_eMoveState);
		m_isMoveAct = true;
		break;
	}

	//// tileMap 위치에 몬스터 정보 저장
	m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo = &m_tEnemyData;

	//// 모션 재생
	m_ani.start();
}

void enemy::isDieTileMana()
{
	for (int i = 0; i < 4; i++)
	{
		int temp = RANDOM->getFromIntTo(0, 3);
		if (tileManaChg(temp, m_tEnemyData.t_currMana)) return;
	}
}

void enemy::eatActPattern()
{
	// 공격중이 아닐 경우(m_isAttAct) 현재 체력이 1/3 경우
	if (!m_isAttAct && m_tEnemyData.t_currHp < m_tEnemyData.t_MaxHp / 4)
	{
		// 4방향을 모두 검색한다(UP, RIGHT, DOWN, LEFT)
		for (int i = 0; i < 4; i++)
		{
			if (eatIsEnemy(i))
			{
				// 자신보다 약한 enemy를 먹었을 경우(이때 해당 enemy의 Hp는 0으로 된다
				// 공격 액션을 실시한다
				m_isAttAct = true;
				// 바라보았던 방향에 따라 액션 위치를 설정한다
				if (i == eMoveState::LEFT || i == eMoveState::UP)
				{
					m_tEnemyData.t_img = m_tEnemyData.t_img_LA;
				}
				else if (i == eMoveState::RIGHT || i == eMoveState::DOWN)
				{
					m_tEnemyData.t_img = m_tEnemyData.t_img_RA;
				}
				// 공격 모션이 실시 가능하도록 moveDaley와 다른 actDaley로 초기화
				m_moveDaley = m_tEnemyData.t_moveDaley;
				m_ani.start();
				// 실행시 함수를 빠져나간다(재실행 예외처리)
				return;
			}
		}
	}

	return;
}

bool enemy::eatIsEnemy(int eMoveArrow)
{
	// 타일 검색 방향
	int tempMoveArrow;

	switch (eMoveArrow)
	{
	case eMoveState::UP:
		// 검색 방향
		tempMoveArrow = (m_tEnemyData.t_tilePosX * m_pTileMapMag->getTileSizeY()) + m_tEnemyData.t_tilePosY - 1;
		// 해당 위치에 enemy정보가 있을 경우 -> 없으면 return;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	case eMoveState::RIGHT:
		tempMoveArrow = ((m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY()) + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	case eMoveState::DOWN:
		tempMoveArrow = (m_tEnemyData.t_tilePosX * m_pTileMapMag->getTileSizeY()) + m_tEnemyData.t_tilePosY + 1;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	case eMoveState::LEFT:
		tempMoveArrow = ((m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY()) + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	}

	// 몬스터 정보가 있다면 해당 enemy의 FoodChainLv 확인 자신이 더 높을 경우 먹는다
	if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo->t_FoodChainLv < m_tEnemyData.t_FoodChainLv)
	{
		// 해당 enemy의 최대 체력 만큼 체력을 회복한다
		m_tEnemyData.t_currHp += m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo->t_MaxHp;
		// 해당 enemy의 체력은 0으로 만들어 죽도록 한다
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo->t_currHp = 0;
		// true로 반환
		return true;
	}

	// 먹을 수 없는 경우 return false 한다
	return false;
}

tagEnemyData * enemy::FlowerInfo()
{
	tempEnemy.t_img_R = IMAGEMANAGER->findImage("Flower_00_R");
	tempEnemy.t_img_L = IMAGEMANAGER->findImage("Flower_00_L");
	tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Flower_00_RA");
	tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Flower_00_LA");
	tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Flower_00_Dead");

	tempEnemy.t_isAilve = true;
	tempEnemy.t_currHp = 10;
	tempEnemy.t_MaxHp = tempEnemy.t_currHp;
	tempEnemy.t_posX = m_tEnemyData.t_posX;
	tempEnemy.t_posY = m_tEnemyData.t_posY;
	tempEnemy.t_tilePosX = m_tEnemyData.t_tilePosX;
	tempEnemy.t_tilePosY = m_tEnemyData.t_tilePosY;
	tempEnemy.t_scale = 2.0;
	tempEnemy.t_moveSpeed = 1.0f;
	tempEnemy.t_moveDaley = 120;
	tempEnemy.t_setTileMapNum = m_tEnemyData.t_setTileMapNum;
	tempEnemy.t_atkPoint = 5;
	tempEnemy.t_defPoint = 0;
	tempEnemy.t_FoodChainLv = 0;
	tempEnemy.t_currMana = 2;
	tempEnemy.t_enumType = tagEnemyType::Flower;

	return &tempEnemy;
}

tagEnemyData * enemy::SlimeInfo()
{
	tempEnemy.t_img_R = IMAGEMANAGER->findImage("Slime_00_R");
	tempEnemy.t_img_L = IMAGEMANAGER->findImage("Slime_00_L");
	tempEnemy.t_img_RA = IMAGEMANAGER->findImage("Slime_00_RA");
	tempEnemy.t_img_LA = IMAGEMANAGER->findImage("Slime_00_LA");
	tempEnemy.t_img_Dead = IMAGEMANAGER->findImage("Slime_00_Dead");

	tempEnemy.t_isAilve = true;
	tempEnemy.t_currHp = 25;
	tempEnemy.t_MaxHp = tempEnemy.t_currHp;
	tempEnemy.t_posX = m_tEnemyData.t_posX;
	tempEnemy.t_posY = m_tEnemyData.t_posY;
	tempEnemy.t_tilePosX = m_tEnemyData.t_tilePosX;
	tempEnemy.t_tilePosY = m_tEnemyData.t_tilePosY;
	tempEnemy.t_scale = 1.5f;
	tempEnemy.t_moveSpeed = RANDOM->getFromFloatTo(0.5f, 0.8f);
	tempEnemy.t_moveDaley = 0;
	tempEnemy.t_setTileMapNum = m_tEnemyData.t_setTileMapNum;
	tempEnemy.t_atkPoint = 3;
	tempEnemy.t_defPoint = 0;
	tempEnemy.t_FoodChainLv = 0;
	tempEnemy.t_currMana = RANDOM->getFromIntTo(1, 2);
	tempEnemy.t_enumType = tagEnemyType::Slime;

	return &tempEnemy;
}

enemy::enemy()
{
}


enemy::~enemy()
{
}
