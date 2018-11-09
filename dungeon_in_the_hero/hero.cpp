#include "stdafx.h"
#include "hero.h"
#include "tileMap.h"
#include "aStarNode.h"
#include "aStarPathFinding.h"
#include "bulletManager.h"

HRESULT hero::init(tagHeroData * heroInfo, tileMap * pTileMag, bulletManager * pBulletMag)
{
	//// 타일 맵 주소 초기화
	m_pTileMapMag = pTileMag;
	m_pBulletMag = pBulletMag;

	m_tHeroData = *heroInfo;
	m_tHeroData.t_img_state = IMAGEMANAGER->findImage("hero_state_00");

	m_tHeroData.t_moveEndX = m_tHeroData.t_posX;
	m_tHeroData.t_moveEndY = m_tHeroData.t_posY;

	m_tHeroData.t_img = heroInfo->t_img_D;
	m_tHeroData.t_alphaValue = heroInfo->t_alphaValue = 255;
	m_ani.init(m_tHeroData.t_img->getWidth(), m_tHeroData.t_img->getHeight(), m_tHeroData.t_img->getFrameWidth(), m_tHeroData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(15);

	m_tHeroData.t_rc = RectMakeCenter(m_tHeroData.t_posX, m_tHeroData.t_posY, m_tHeroData.t_img->getFrameWidth(), m_tHeroData.t_img->getFrameHeight());

	m_moveDaley = m_tHeroData.t_moveDaley;
	m_aStarDepValue = 0;
	m_isDead = false;
	m_isAttAct = false;

	m_isMoveAct = false;
	m_eMoveState = eMoveState::DOWN;

	//aStarFind(m_pAStartNode->Create(20, 24), m_pAStartNode->Create(m_tHeroData.t_tilePosX, m_tHeroData.t_tilePosY));
	return S_OK;
}

void hero::release()
{
}

void hero::update()
{
	if (!m_tHeroData.t_isAilve) return;

	skill_count();
	damge();
	moveSys();
	currHp();
	m_ani.frameUpdate();
}

void hero::render(HDC hdc)
{
	if (!m_tHeroData.t_isAilve) return;

	m_ani.init(m_tHeroData.t_img->getWidth(), m_tHeroData.t_img->getHeight(), m_tHeroData.t_img->getFrameWidth(), m_tHeroData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, false);
	m_ani.setFPS(10);

	if (m_tHeroData.t_Skill.t_isHasteBuff)
		m_ani.setFPS(HERO_SKILL_HASTE_MS_POINT);

	if (!m_isDead && !m_isAttAct)
	{
		if (m_eMoveState == eMoveState::UP)
		{
			m_tHeroData.t_img = m_tHeroData.t_img_U;
		}
		else if (m_eMoveState == eMoveState::LEFT)
		{
			m_tHeroData.t_img = m_tHeroData.t_img_L;
		}
		else if (m_eMoveState == eMoveState::DOWN)
		{
			m_tHeroData.t_img = m_tHeroData.t_img_D;
		}
		else if (m_eMoveState == eMoveState::RIGHT)
		{
			m_tHeroData.t_img = m_tHeroData.t_img_R;
		}
		else if (m_eMoveState == eMoveState::SKILL)
		{
			m_tHeroData.t_img = m_tHeroData.t_img_S;
		}
	}



	// 스크린 랜더 조건
	if (!(m_tHeroData.t_rc.left < -TILE_SIZE || m_tHeroData.t_rc.left > WINSIZEX) &&
		!(m_tHeroData.t_rc.top < -TILE_SIZE || m_tHeroData.t_rc.top > WINSIZEY))
	{
		m_tHeroData.t_img->aniRender(hdc,
			(m_tHeroData.t_posX + (TILE_SIZE / 2) - (m_tHeroData.t_img->getFrameWidth() * m_tHeroData.t_scale) / 2) - CAMERA->getCamPosX(),
			((m_tHeroData.t_posY + TILE_SIZE / 2) - (m_tHeroData.t_img->getFrameHeight() * m_tHeroData.t_scale) / 2) - CAMERA->getCamPosY(),
			&m_ani, m_tHeroData.t_scale, true, m_tHeroData.t_alphaValue);

		buffIcon(hdc);
	}

	m_tHeroData.t_rc = RectMake(
		m_tHeroData.t_posX - CAMERA->getCamPosX(),
		m_tHeroData.t_posY - CAMERA->getCamPosY(),
		TILE_SIZE,
		TILE_SIZE);

	if (g_saveData.gisTest)
	{
		Rectangle(hdc, m_tHeroData.t_rc.left, m_tHeroData.t_rc.top, m_tHeroData.t_rc.right, m_tHeroData.t_rc.bottom);

		char szText[256];
		sprintf_s(szText, "%d,%d", m_tHeroData.t_atkPoint, m_tHeroData.t_defPoint);
		TextOut(hdc, m_tHeroData.t_posX - CAMERA->getCamPosX(), m_tHeroData.t_posY - CAMERA->getCamPosY(), szText, strlen(szText));
	}
}

int hero::aStarisMove(aStarNode * pos, list<aStarNode*>* vecNode)
{
	int tempTileX = m_tHeroData.t_tilePosX;
	int tempTileY = m_tHeroData.t_tilePosY;

	for (int i = 0; i < 4; i++)
	{
		int cy = pos->getPosY(), cx = pos->getPosX();
		if(cy <= -1 || cy == m_pTileMapMag->getTileSizeY() ||
			cx <= -1 || cx == m_pTileMapMag->getTileSizeX()) break;

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

void hero::aStarRoute()
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
		if (m_vecRoute.size() == 0) return;
		delete m_vecRoute.back();
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

void hero::currHp()
{
	if (m_isDead && m_ani.getIsPlaying())
	{
		m_tHeroData.t_alphaValue += 50;
		return;
	}
	else if (m_isDead && !m_ani.getIsPlaying())
	{
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo == &m_tHeroData)
			m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo = nullptr;
		Delete(true, true, true);
		m_tHeroData.t_isAilve = false;
	}

	if (m_tHeroData.t_currHp > m_tHeroData.t_MaxHp) // 최대 체력 초과 불가
		m_tHeroData.t_currHp = m_tHeroData.t_MaxHp;

	if (m_tHeroData.t_currHp <= 0) // 죽음
	{
		m_tHeroData.t_img = m_tHeroData.t_img_Dead;
		m_ani.start();
		m_isDead = true;
	}
}

void hero::moveSys()
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
			if (m_tHeroData.t_posY > m_tHeroData.t_moveEndY)
				m_tHeroData.t_posY -= m_tHeroData.t_moveSpeed;

			// 이동 거리 도착 시
			if (m_tHeroData.t_posY <= m_tHeroData.t_moveEndY)
			{
				// 위치를 이동하려고 했던 거리로 초기화
				m_tHeroData.t_posY = m_tHeroData.t_moveEndY;
				// 이동 액션 종료 bool 문
				m_isMoveAct = false;
				// 다음 이동 액션까지 딜레이
				m_moveDaley = m_tHeroData.t_moveDaley;
				//monActPattern();
			}
			break;
		case eMoveState::DOWN:
			if (m_tHeroData.t_posY < m_tHeroData.t_moveEndY)
				m_tHeroData.t_posY += m_tHeroData.t_moveSpeed;

			if (m_tHeroData.t_posY >= m_tHeroData.t_moveEndY)
			{
				m_tHeroData.t_posY = m_tHeroData.t_moveEndY;
				m_isMoveAct = false;
				m_moveDaley = m_tHeroData.t_moveDaley;
				//monActPattern();

			}
			break;
		case eMoveState::LEFT:
			if (m_tHeroData.t_posX > m_tHeroData.t_moveEndX)
				m_tHeroData.t_posX -= m_tHeroData.t_moveSpeed;

			if (m_tHeroData.t_posX <= m_tHeroData.t_moveEndX)
			{
				m_tHeroData.t_posX = m_tHeroData.t_moveEndX;
				m_isMoveAct = false;
				m_moveDaley = m_tHeroData.t_moveDaley;
				//monActPattern();

			}
			break;
		case eMoveState::RIGHT:
			if (m_tHeroData.t_posX < m_tHeroData.t_moveEndX)
				m_tHeroData.t_posX += m_tHeroData.t_moveSpeed;

			if (m_tHeroData.t_posX >= m_tHeroData.t_moveEndX)
			{
				m_tHeroData.t_posX = m_tHeroData.t_moveEndX;
				m_isMoveAct = false;
				m_moveDaley = m_tHeroData.t_moveDaley;
				//monActPattern();

			}
			break;
		case eMoveState::MOVE_NUM:
			m_isMoveAct = false;
			m_moveDaley = m_tHeroData.t_moveDaley;
			//monActPattern();

			break;
		}
	}
}

bool hero::moveRectCheck(int eMoveArrow)
{
	switch (eMoveArrow)
	{
	case eMoveState::UP:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY - 1)].t_isAlive) return false;
		m_tHeroData.t_tilePosY -= 1;
		m_tHeroData.t_moveEndY = m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + m_tHeroData.t_tilePosY].t_rc.top + CAMERA->getCamPosY();
		break;
	case eMoveState::DOWN:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY + 1)].t_isAlive) return false;
		m_tHeroData.t_tilePosY += 1;
		m_tHeroData.t_moveEndY = m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY].t_rc.top + CAMERA->getCamPosY();
		break;
	case eMoveState::LEFT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tHeroData.t_tilePosY].t_isAlive) return false;
		m_tHeroData.t_tilePosX -= 1;
		m_tHeroData.t_moveEndX = m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + m_tHeroData.t_tilePosY].t_rc.left + CAMERA->getCamPosX();
		break;
	case eMoveState::RIGHT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tHeroData.t_tilePosY].t_isAlive) return false;
		m_tHeroData.t_tilePosX += 1;
		m_tHeroData.t_moveEndX = m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + m_tHeroData.t_tilePosY].t_rc.left + CAMERA->getCamPosX();
		break;
	case eMoveState::MOVE_NUM:
		m_ani.stop();
		break;
	}

	return true;
}

bool hero::moveIsRect(int eMoveArrow)
{
	switch (eMoveArrow)
	{
	case eMoveState::UP:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY - 1)].t_isAlive) return false;
		break;
	case eMoveState::DOWN:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY + 1)].t_isAlive) return false;
		break;
	case eMoveState::LEFT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tHeroData.t_tilePosY].t_isAlive) return false;
		break;
	case eMoveState::RIGHT:
		if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tHeroData.t_tilePosY].t_isAlive) return false;
		break;
	case eMoveState::MOVE_NUM:
		return false;
		break;
	}

	return true;
}

void hero::aStarMoveSys()
{
	//// 자신의 다음 이동위치는 하나씩 꺼낸다
	if (m_vecRoute.size() != 0)
	{
		aStarNode * tempNode = nullptr;
		for (m_iterRoute = m_vecRoute.begin(); m_iterRoute != m_vecRoute.end(); m_iterRoute++)
		{
			if ((*m_iterRoute)->getDepth() != 0)
			{
				tempNode = (*m_iterRoute);
				(*m_iterRoute)->setDepth(0);
				break;
			}
		}
		if (tempNode == nullptr)
			return;

		int x = tempNode->getPosX();
		int y = tempNode->getPosY();

		// 찾은 깊이 추가
		m_aStarDepValue++;

		if (tempNode->getDepth() == m_vecRoute.size())
			return;

		// 방향
		if (y < m_tHeroData.t_tilePosY)
		{
			moveRectCheck(eMoveState::UP);
			m_eMoveState = eMoveState::UP;
		}

		if (x > m_tHeroData.t_tilePosX)
		{
			moveRectCheck(eMoveState::RIGHT);
			m_eMoveState = eMoveState::RIGHT;
		}

		if (y > m_tHeroData.t_tilePosY)
		{
			moveRectCheck(eMoveState::DOWN);
			m_eMoveState = eMoveState::DOWN;
		}

		if (x < m_tHeroData.t_tilePosX)
		{
			moveRectCheck(eMoveState::LEFT);
			m_eMoveState = eMoveState::LEFT;
		}

		EFFMANAGER->play("MousePointEFF", m_pTileMapMag->getTileSetPoint()[x * m_pTileMapMag->getTileSizeY() + y].t_rc.left + TILE_SIZE / 2, m_pTileMapMag->getTileSetPoint()[x * m_pTileMapMag->getTileSizeY() + y].t_rc.top + TILE_SIZE / 2);
	}
}

void hero::heroSetTxt(int enemyType)
{
	//tagTileTxt setTxt;

	//switch (enemyType)
	//{
	//case tagEnemyType::Slime:
	//	setTxt.t_txtName = "액괴";
	//	setTxt.t_txtInfo = "이래뵈도, 움직이는 영양덩어리 입니다.";
	//	break;
	//case tagEnemyType::Bug:
	//	setTxt.t_txtName = "볼레레";
	//	setTxt.t_txtInfo = "액괴를 먹는 볼레입니다 하얗고 귀엽죠?";
	//	break;
	//}

	//setTxt.t_AtkPoint = m_tHeroData.t_atkPoint;
	//setTxt.t_DefPoint = m_tHeroData.t_defPoint;
	//setTxt.t_HpPoint = m_tHeroData.t_currHp;
	//setTxt.t_MaxHpPoint = m_tHeroData.t_MaxHp;

	//m_pEnemyMag->getEnemyPopup()->setTxtInfo(setTxt);
	return;
}

void hero::movePattern()
{
	// 공격 여부는 이동하기 전에 행동하여 이동을 실시하지 않도록 해야한다
	if (m_isAttAct)
		m_isAttAct = false;

	//// 이동 시 자신의 위치에 자신의 정보가 있다면 지워준다
	if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo == &m_tHeroData)
		m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo = nullptr;

	//// enemyType에 맞춰 이동 패턴 적용
	if (!monActPattern())
	{
		switch (m_tHeroData.t_enumType)
		{
		default: // 패턴이 없을 경우 랜덤 이동
			//aStarMoveSys();
			m_eMoveState = RANDOM->getFromIntTo(0, 3);
			moveRectCheck(m_eMoveState);
			m_isMoveAct = true;
			break;
		}
	}
	//// tileMap 위치에 몬스터 정보 저장
	m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo = &m_tHeroData;

	//// 모션 재생
	m_ani.start();
}

bool hero::monActPattern()
{
	// 공격중이 아닐 경우(m_isAttAct)
	if (!m_isAttAct)
	{
		if (skill_FireWall()) return true;
		if (skill_Haling()) return true;
		if (skill_AtkBuff()) return true;
		if (skill_DefBuff()) return true;
		if (skill_Haste()) return true;
		if (attSys()) return true;

		return false;
	}

	return false;
}

bool hero::IsEnemy(int eMoveArrow)
{
	// 타일 검색 방향
	int tempMoveArrow;
	int tempArrowRange = 1;
	int tempAttArrow = 0;
	int tempCurrRange = 0;
	if (m_tHeroData.t_Skill.t_ArrowMagic)
		tempArrowRange = 4;

	for (int i = 0; i < tempArrowRange + 1; i++)
	{
		tempCurrRange = i;
		switch (eMoveArrow)
		{
		case eMoveState::UP:
			// 검색 방향
			tempMoveArrow = (m_tHeroData.t_tilePosX * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY - i;
			tempAttArrow = eMoveState::UP;
			break;
		case eMoveState::RIGHT:
			tempMoveArrow = ((m_tHeroData.t_tilePosX + i) * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY;
			tempAttArrow = eMoveState::RIGHT;
			break;
		case eMoveState::DOWN:
			tempMoveArrow = (m_tHeroData.t_tilePosX * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY + i;
			tempAttArrow = eMoveState::DOWN;
			break;
		case eMoveState::LEFT:
			tempMoveArrow = ((m_tHeroData.t_tilePosX - i) * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY;
			tempAttArrow = eMoveState::LEFT;
			break;
		}

		// 해당 위치에 enemy정보가 있을 경우 -> 없으면 return;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_isAlive ||
			(i == tempArrowRange && m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr)) return false;

		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo != nullptr) break;
	}

	if (tempCurrRange == 1) // 근접 공격
	{
		// 몬스터 정보가 있다면 자신의 공격력 만큼 해당 몬스터의 hp를 깍아낸다
		m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_atkPoint;
		EFFMANAGER->play("Hit_Eff_0", m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));

	}
	else // 스킬
	{
		if (!skill_ArrowMagic()) return false;

		tagBullet tempBullet;
		MY_UTIL::HetSe(true, tagEnemyType::Lili);

		//// 임시 bullet
		tempBullet.t_isAilve = true;
		tempBullet.t_img = IMAGEMANAGER->findImage("Bullet_1");
		tempBullet.t_posX = m_tHeroData.t_posX;
		tempBullet.t_posY = m_tHeroData.t_posY;
		tempBullet.t_atkPoint = m_tHeroData.t_atkPoint + HERO_SKILL_ATTOWMAGIC_DAMAGE;
		tempBullet.t_moveSpeed = 2.0f;
		tempBullet.t_scale = 1.0f;
		tempBullet.t_range = 150.0f;
		tempBullet.t_moveArrow = tempAttArrow;
		tempBullet.t_master = tagMaster::Hero;
		m_pBulletMag->addBullet(&tempBullet);
	}
	
	// true로 반환
	return true;
}

void hero::damge()
{
	if (m_tHeroData.t_damgePoint <= 0) return;

	MY_UTIL::HetSe();
	int tempPoint = (m_tHeroData.t_damgePoint - m_tHeroData.t_defPoint);
	if (tempPoint <= 0)
		tempPoint = 1;

	m_tHeroData.t_currHp -= tempPoint;
	m_tHeroData.t_damgePoint = 0;
}

void hero::skill_count()
{
	// atkBuff
	if (m_tHeroData.t_Skill.t_AtkCount > 0)
		m_tHeroData.t_Skill.t_AtkCount--;
	if (m_tHeroData.t_Skill.t_AtkCount <= 0 &&
		m_tHeroData.t_Skill.t_isAtkBuff)
	{
		m_tHeroData.t_Skill.t_isAtkBuff = false;
		m_tHeroData.t_atkPoint -= HERO_SKILL_ATKBUFF_POINT;
	}

	// defBuff
	if (m_tHeroData.t_Skill.t_DefCount > 0)
		m_tHeroData.t_Skill.t_DefCount--;
	if (m_tHeroData.t_Skill.t_DefCount <= 0 &&
		m_tHeroData.t_Skill.t_isDefBuff)
	{
		m_tHeroData.t_Skill.t_isDefBuff = false;
		m_tHeroData.t_defPoint -= HERO_SKILL_DEFBUFF_POINT;
	}

	// HasteBuff
	if (m_tHeroData.t_Skill.t_HasteCount > 0)
		m_tHeroData.t_Skill.t_HasteCount--;
	if (m_tHeroData.t_Skill.t_HasteCount <= 0 &&
		m_tHeroData.t_Skill.t_isHasteBuff)
	{
		m_tHeroData.t_Skill.t_isHasteBuff = false;
		m_tHeroData.t_moveSpeed -= HERO_SKILL_HASTE_SP_POINT;
	}

	if (m_tHeroData.t_Skill.t_fireWallData.t_actValue > 0)
	{
		int tempFireWallArrow = 0;

		if (m_tHeroData.t_Skill.t_fireWallData.t_actValue > 0 &&
			m_tHeroData.t_Skill.t_fireWallData.t_actCount <= 0)
		{
			m_tHeroData.t_Skill.t_fireWallData.t_actValue--;
			if (m_tHeroData.t_Skill.t_fireWallData.t_actValue <= 0)
				m_tHeroData.t_Skill.t_isFireWall = false;

			m_tHeroData.t_Skill.t_fireWallData.t_actCount = HERO_SKILL_FIREWALL_MOVE_DALEY;
			// fireWall을 이동
			switch (m_tHeroData.t_Skill.t_fireWallData.t_moveArrow)
			{
			case eMoveState::UP:
				m_tHeroData.t_Skill.t_fireWallData.t_tilePosY -= 1;
				break;
			case eMoveState::RIGHT:
				m_tHeroData.t_Skill.t_fireWallData.t_tilePosX += 1;
				break;
			case eMoveState::DOWN:
				m_tHeroData.t_Skill.t_fireWallData.t_tilePosY += 1;
				break;
			case eMoveState::LEFT:
				m_tHeroData.t_Skill.t_fireWallData.t_tilePosX -= 1;
				break;
			default:
				break;
			}
			// fireWall의 위치에 따른 타일 정보

			for (int i = 0; i < m_tHeroData.t_Skill.t_fireWallData.t_tileSizeValue; i++)
			{
				if (m_tHeroData.t_Skill.t_fireWallData.t_moveArrow == eMoveState::UP ||
					m_tHeroData.t_Skill.t_fireWallData.t_moveArrow == eMoveState::DOWN)
				{
					if (i == 0) // 최초 예외 처리
					{
						tempFireWallArrow = (m_tHeroData.t_Skill.t_fireWallData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_Skill.t_fireWallData.t_tilePosY);
						if (m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo != nullptr)
							m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_Skill.t_fireWallData.t_damage;
						if (!m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_isAlive)
							EFFMANAGER->play("FireWall_EFF_3", m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));
					}
					else
					{
						tempFireWallArrow = (m_tHeroData.t_Skill.t_fireWallData.t_tilePosX - i) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_Skill.t_fireWallData.t_tilePosY);
						if (!m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_isAlive)
							EFFMANAGER->play("FireWall_EFF_3", m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));
						if (m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo != nullptr)
							m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_Skill.t_fireWallData.t_damage;

						tempFireWallArrow = (m_tHeroData.t_Skill.t_fireWallData.t_tilePosX + i) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_Skill.t_fireWallData.t_tilePosY);
						if (!m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_isAlive)
							EFFMANAGER->play("FireWall_EFF_3", m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));
						if (m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo != nullptr)
							m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_Skill.t_fireWallData.t_damage;
					}
				}
				else if (m_tHeroData.t_Skill.t_fireWallData.t_moveArrow == eMoveState::LEFT || 
						m_tHeroData.t_Skill.t_fireWallData.t_moveArrow == eMoveState::RIGHT)
				{
					if (i == 0) // 최초 예외 처리
					{
						tempFireWallArrow = (m_tHeroData.t_Skill.t_fireWallData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_Skill.t_fireWallData.t_tilePosY);
						if (m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo != nullptr)
							m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_Skill.t_fireWallData.t_damage;
						if (!m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_isAlive)
							EFFMANAGER->play("FireWall_EFF_3", m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));
					}
					else
					{
						tempFireWallArrow = (m_tHeroData.t_Skill.t_fireWallData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_Skill.t_fireWallData.t_tilePosY - i);
						if (!m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_isAlive)
							EFFMANAGER->play("FireWall_EFF_3", m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));
						if (m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo != nullptr)
							m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_Skill.t_fireWallData.t_damage;

						tempFireWallArrow = (m_tHeroData.t_Skill.t_fireWallData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_Skill.t_fireWallData.t_tilePosY + i);
						if (!m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_isAlive)
							EFFMANAGER->play("FireWall_EFF_3", m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));
						if (m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo != nullptr)
							m_pTileMapMag->getTileSetPoint()[tempFireWallArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_Skill.t_fireWallData.t_damage;
					}
				}
			}
			m_tHeroData.t_Skill.t_fireWallData.t_tileSizeValue++;
		}
		m_tHeroData.t_Skill.t_fireWallData.t_actCount--;
	}
}

bool hero::attSys()
{
	// 4방향을 모두 검색한다(UP, RIGHT, DOWN, LEFT)
	for (int i = 0; i < 4; i++)
	{
		if (IsEnemy(i))
		{
			// 공격 액션을 실시한다
			m_isAttAct = true;
			m_isMoveAct = false;
			// 바라보았던 방향에 따라 액션 위치를 설정한다
			if (i == eMoveState::UP)
			{
				m_tHeroData.t_img = m_tHeroData.t_img_UA;
			}
			else if (i == eMoveState::RIGHT)
			{
				m_tHeroData.t_img = m_tHeroData.t_img_RA;
			}
			else if (i == eMoveState::DOWN)
			{
				m_tHeroData.t_img = m_tHeroData.t_img_DA;
			}
			else if (i == eMoveState::LEFT)
			{
				m_tHeroData.t_img = m_tHeroData.t_img_LA;
			}
			m_eMoveState = eMoveState::MOVE_NUM;
			m_moveDaley = m_tHeroData.t_moveDaley;

			// 실행시 함수를 빠져나간다(재실행 예외처리)
			return true;
		}
	}
	return false;
}

void hero::buffIcon(HDC hdc)
{
#define SET_STATE_SIZE_Y 10.0f
	int tempSizeY = 0;
	int tempIconSize = m_tHeroData.t_img_state->getFrameHeight() * m_tHeroData.t_scale;

	if (m_tHeroData.t_Skill.t_isAtkBuff)
	{
		m_tHeroData.t_img_state->frameRender(hdc,
			m_tHeroData.t_posX + TILE_SIZE - CAMERA->getCamPosX(),
			(m_tHeroData.t_posY + TILE_SIZE / 2) + SET_STATE_SIZE_Y - (tempSizeY * tempIconSize) - CAMERA->getCamPosY(),
			0, 1, m_tHeroData.t_scale);
		tempSizeY++;

	}

	if (m_tHeroData.t_Skill.t_isDefBuff)
	{
		m_tHeroData.t_img_state->frameRender(hdc,
			m_tHeroData.t_posX + TILE_SIZE - CAMERA->getCamPosX(),
			(m_tHeroData.t_posY + TILE_SIZE / 2) + SET_STATE_SIZE_Y - (tempSizeY * tempIconSize) - CAMERA->getCamPosY(),
			1, 1, m_tHeroData.t_scale);
		tempSizeY++;

	}

	if (m_tHeroData.t_Skill.t_isHasteBuff)
	{
		m_tHeroData.t_img_state->frameRender(hdc,
			m_tHeroData.t_posX + TILE_SIZE - CAMERA->getCamPosX(),
			(m_tHeroData.t_posY + TILE_SIZE / 2) + SET_STATE_SIZE_Y - (tempSizeY * tempIconSize) - CAMERA->getCamPosY(),
			2, 1, m_tHeroData.t_scale);
		tempSizeY++;

	}


}

bool hero::skill_ArrowMagic()
{
	if (!m_tHeroData.t_Skill.t_ArrowMagic ||
		m_tHeroData.t_currMana < HERO_SKILL_ARROWMAGIC ||
		RANDOM->getInt(100) <= 10) return false;

		EFFMANAGER->play("ArrowMagic_EFF_0",
			m_tHeroData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2),
			m_tHeroData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2));

		m_tHeroData.t_currMana -= HERO_SKILL_ARROWMAGIC;
		return true;
}


bool hero::skill_Haling()
{
	if (!m_tHeroData.t_Skill.t_haling ||
		m_tHeroData.t_currMana < HERO_SKILL_HALING ||
		RANDOM->getInt(100) <= 50) return false;

	if (m_tHeroData.t_currHp <= m_tHeroData.t_MaxHp / 2)
	{
		SOUNDMANAGER->play("Sound/SE/Haling_0.wav");
		EFFMANAGER->play("Haling_EFF_0",
			m_tHeroData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2),
			m_tHeroData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2));

		m_tHeroData.t_currHp += (m_tHeroData.t_MaxHp / 4);
		m_tHeroData.t_currMana -= HERO_SKILL_HALING;

		m_eMoveState = eMoveState::SKILL;
		m_moveDaley = m_tHeroData.t_moveDaley;
		return true;
	}
	return false;
}

bool hero::skill_AtkBuff()
{
	if (!m_tHeroData.t_Skill.t_AtkBuff ||
		m_tHeroData.t_currMana < HERO_SKILL_ATKBUFF ||
		RANDOM->getInt(100) <= 90) return false;

	if (m_tHeroData.t_Skill.t_AtkCount <= 0 &&
		!m_tHeroData.t_Skill.t_isAtkBuff)
	{
		m_tHeroData.t_Skill.t_isAtkBuff = true;
		m_tHeroData.t_Skill.t_AtkCount = HERO_SKILL_ATKBUFF_DALEY;
		m_tHeroData.t_atkPoint += HERO_SKILL_ATKBUFF_POINT;

		SOUNDMANAGER->play("Sound/SE/Haling_0.wav");
		EFFMANAGER->play("AtkBuff_EFF_1",
			m_tHeroData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2),
			m_tHeroData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2));

		m_tHeroData.t_currMana -= HERO_SKILL_ATKBUFF;

		m_eMoveState = eMoveState::SKILL;
		m_moveDaley = m_tHeroData.t_moveDaley;
		return true;
	}
	return false;
}

bool hero::skill_DefBuff()
{
	if (!m_tHeroData.t_Skill.t_DefBuff ||
		m_tHeroData.t_currMana < HERO_SKILL_DEFBUFF ||
		RANDOM->getInt(100) <= 90) return false;

	if (m_tHeroData.t_Skill.t_DefCount <= 0 &&
		!m_tHeroData.t_Skill.t_isDefBuff)
	{
		m_tHeroData.t_Skill.t_isDefBuff = true;
		m_tHeroData.t_Skill.t_DefCount = HERO_SKILL_ATKBUFF_DALEY;
		m_tHeroData.t_defPoint += HERO_SKILL_DEFBUFF_POINT;

		SOUNDMANAGER->play("Sound/SE/Haling_0.wav");
		EFFMANAGER->play("DefBuff_EFF_0",
			m_tHeroData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2),
			m_tHeroData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2));

		m_tHeroData.t_currMana -= HERO_SKILL_DEFBUFF;

		m_eMoveState = eMoveState::SKILL;
		m_moveDaley = m_tHeroData.t_moveDaley;
		return true;
	}
	return false;
}

bool hero::skill_Haste()
{
	if (!m_tHeroData.t_Skill.t_HasteBuff ||
		m_tHeroData.t_currMana < HERO_SKILL_HASTE ||
		RANDOM->getInt(100) <= 90) return false;

	if (m_tHeroData.t_Skill.t_HasteCount <= 0 &&
		!m_tHeroData.t_Skill.t_isHasteBuff)
	{
		m_tHeroData.t_Skill.t_isHasteBuff = true;
		m_tHeroData.t_Skill.t_HasteCount = HERO_SKILL_HASTE_DALEY;
		m_tHeroData.t_moveSpeed += HERO_SKILL_HASTE_SP_POINT;

		SOUNDMANAGER->play("Sound/SE/Haling_0.wav");
		EFFMANAGER->play("Haste_Eff_0",
			m_tHeroData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2),
			m_tHeroData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2));

		m_tHeroData.t_currMana -= HERO_SKILL_HASTE;

		m_eMoveState = eMoveState::SKILL;
		m_moveDaley = m_tHeroData.t_moveDaley;
		return true;
	}
	return false;
}

bool hero::skill_FireWall()
{
	// 발동 이전 상태
	if ((m_eMoveState == eMoveState::SKILL) ||
		(m_eMoveState == eMoveState::MOVE_NUM) ||
		!m_tHeroData.t_Skill.t_fireWall ||
		m_tHeroData.t_currMana < HERO_SKILL_FIREWALL ||
		m_tHeroData.t_Skill.t_isFireWall ||
		RANDOM->getInt(100) <= 90) return false;

	//if (!(m_tHeroData.t_currHp <= m_tHeroData.t_MaxHp / 3)) return false;

	m_tHeroData.t_Skill.t_isFireWall = true;
	m_tHeroData.t_Skill.t_fireWallData.t_actValue = 5;
	m_tHeroData.t_Skill.t_fireWallData.t_tileSizeValue = 1;
	m_tHeroData.t_Skill.t_fireWallData.t_tilePosX = m_tHeroData.t_tilePosX;
	m_tHeroData.t_Skill.t_fireWallData.t_tilePosY = m_tHeroData.t_tilePosY;

	// 불꽃 진행 방향 설정
	m_tHeroData.t_Skill.t_fireWallData.t_moveArrow = m_eMoveState;
	m_tHeroData.t_Skill.t_fireWallData.t_damage = HERO_SKILL_FIREWALL_DAMAGE;
	m_tHeroData.t_Skill.t_fireWallData.t_actCount = HERO_SKILL_FIREWALL_MOVE_DALEY;

	SOUNDMANAGER->play("Sound/SE/Haling_0.wav");
	EFFMANAGER->play("Haste_Eff_0",
		m_tHeroData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2),
		m_tHeroData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2));

	m_tHeroData.t_currMana -= HERO_SKILL_FIREWALL;

	m_eMoveState = eMoveState::SKILL;
	m_moveDaley = m_tHeroData.t_moveDaley;
	return true;
}

hero::hero()
{
}


hero::~hero()
{
}
