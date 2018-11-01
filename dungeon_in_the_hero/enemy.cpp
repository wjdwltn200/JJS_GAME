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


	m_tEnemyData.t_img_L = enemyInfo->t_img_L;
	m_tEnemyData.t_img_R = enemyInfo->t_img_R;
	m_tEnemyData.t_img_D = enemyInfo->t_img_D;

	m_tEnemyData.t_img = enemyInfo->t_img_L;
	m_tEnemyData.t_scale = enemyInfo->t_scale;
	m_tEnemyData.t_alphaValue = enemyInfo->t_alphaValue = 255;
	m_ani.init(m_tEnemyData.t_img->getWidth(), m_tEnemyData.t_img->getHeight(), m_tEnemyData.t_img->getFrameWidth(), m_tEnemyData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(10);

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

	m_tEnemyData.t_rc = RectMakeCenter(m_tEnemyData.t_posX, m_tEnemyData.t_posY, m_tEnemyData.t_img->getFrameWidth(), m_tEnemyData.t_img->getFrameHeight());
	m_tEnemyData.t_enumType = enemyInfo->t_enumType;


	m_moveDaley = m_tEnemyData.t_moveDaley;

	Delete(true, true);

	m_isMoveAct = false;
	switch (m_tEnemyData.t_enumType)
	{
	case tagEnemyType::Rat:
		m_eMoveState = eMoveState::LEFT;
		break;
	case tagEnemyType::Slime:
		m_eMoveState = RANDOM->getFromIntTo(0, 3);
		break;
	case tagEnemyType::Daemon:
		for (int i = 0; i < 4; i++)
		{
			if (moveRectCheck(i))
			{
				m_eMoveState = i;
				m_ani.start();
				m_isMoveAct = true;
				break;
			}
		}
		break;
	default:
		m_eMoveState = RANDOM->getFromIntTo(0, 4);
		break;
	}


	aStarFind(m_pAStartNode->Create(15, 4), m_pAStartNode->Create(m_tEnemyData.t_tilePosX, m_tEnemyData.t_tilePosY));

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
	if (m_eMoveState == eMoveState::LEFT)
	{
		m_tEnemyData.t_img = m_tEnemyData.t_img_L;
	}
	else if (m_eMoveState == eMoveState::RIGHT)
	{
		m_tEnemyData.t_img = m_tEnemyData.t_img_R;
	}

	if (!(m_tEnemyData.t_rc.left < -TILE_SIZE || m_tEnemyData.t_rc.left > WINSIZEX) &&
	!(m_tEnemyData.t_rc.top < -TILE_SIZE || m_tEnemyData.t_rc.top > WINSIZEY))
	{
		m_tEnemyData.t_img->aniRender(hdc,
			m_tEnemyData.t_posX - CAMERA->getCamPosX(),
			m_tEnemyData.t_posY - CAMERA->getCamPosY(),
			&m_ani, m_tEnemyData.t_scale, true, m_tEnemyData.t_alphaValue);
	}


	m_tEnemyData.t_rc = RectMake(
		m_tEnemyData.t_posX - CAMERA->getCamPosX(),
		m_tEnemyData.t_posY - CAMERA->getCamPosY(),
		m_tEnemyData.t_img->getFrameWidth() * 2,
		m_tEnemyData.t_img->getFrameHeight() * 2);

	if (g_saveData.gisTest)
	{
		Rectangle(hdc, m_tEnemyData.t_rc.left, m_tEnemyData.t_rc.top, m_tEnemyData.t_rc.right, m_tEnemyData.t_rc.bottom);

		char szText[256];
		sprintf_s(szText, "%d,%d", m_tEnemyData.t_tilePosX, m_tEnemyData.t_tilePosY);
		TextOut(hdc, m_tEnemyData.t_posX - CAMERA->getCamPosX(), m_tEnemyData.t_posY - CAMERA->getCamPosY(), szText, strlen(szText));
	}


}

int enemy::aStarisMove(aStarNode * pos, list<aStarNode*> * vecNode)
{
	int distX[3] = { -1, 0, 1 };
	int distY[3] = { -1, 0, 1 };
	int tempTileX = m_tEnemyData.t_tilePosX;
	int tempTileY = m_tEnemyData.t_tilePosY;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			int cx = distX[x] + pos->getPosX();
			int cy = distX[y] + pos->getPosY();

			if (cx == pos->getPosX() && cy == pos->getPosY()) continue;

			if (aStarIsRect(cx, cy)) continue;
			vecNode->push_back(m_pAStartNode->Create(cx, cy));
			EFFMANAGER->play("MousePointEFF", m_pTileMapMag->getTileSetPoint()[cx * m_pTileMapMag->getTileSizeY() + cy].t_rc.left + TILE_SIZE / 2, m_pTileMapMag->getTileSetPoint()[cx * m_pTileMapMag->getTileSizeY() + cy].t_rc.top + TILE_SIZE / 2);

		}
	}

	return sizeof(vecNode);
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

	//list<aStarNode*> * vecChildsNode;
	//vecChildsNode = new list<aStarNode*>;
	//list<aStarNode*>::iterator	iterChildNode;

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


		//if (m_vecCloseNode.size() > 0)
		//{
		//	m_tempIter = m_vecCloseNode.begin();
		//}


 
		//SortOpenNode();
	}

	Delete(true, true);

	return false;
}

void enemy::Delete(bool isOpen, bool isClose)
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

}

void enemy::SortOpenNode()
{
	if (m_vecOpenNode.size() < 2) return;
	aStarNode * pNode;
	list<aStarNode*>::iterator tempIter;

	bool bContinue = true;

	while (bContinue)
	{
		bContinue = false;
		for (m_iterOpenNode = m_vecOpenNode.begin();
			m_iterOpenNode != m_vecOpenNode.end()--;
			m_iterOpenNode++)
		{
			tempIter = m_iterOpenNode++;
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
		Delete(true, true);
		isDieTileMana();
		EFFMANAGER->play("Enemy_Die", m_tEnemyData.t_posX - CAMERA->getCamPosX() + (TILE_SIZE / 2), m_tEnemyData.t_posY - CAMERA->getCamPosY() + (TILE_SIZE / 2));
		m_tEnemyData.t_isAilve = false;
	}
}

void enemy::moveSys()
{
	if (!m_isMoveAct)
	{
		if (m_moveDaley <= 0)
		{
			// 지정된 위치로 좌표 및 변경
			movePattern();
		}	
		m_moveDaley--;
	}
	else
	{
		switch (m_eMoveState)
		{
		case eMoveState::UP:
			if (m_tEnemyData.t_posY > m_tEnemyData.t_moveEndY)
				m_tEnemyData.t_posY -= m_tEnemyData.t_moveSpeed;

			if (m_tEnemyData.t_posY <= m_tEnemyData.t_moveEndY)
			{
				m_tEnemyData.t_posY = m_tEnemyData.t_moveEndY;
				m_isMoveAct = false;
				m_moveDaley = m_tEnemyData.t_moveDaley;
				m_ani.stop();
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
				m_ani.stop();
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
				m_ani.stop();
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
				m_ani.stop();
			}
			break;
		case eMoveState::MOVE_NUM:
			m_isMoveAct = false;
			m_moveDaley = m_tEnemyData.t_moveDaley;
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
	switch (eMoveArrow)
	{
	case eMoveState::UP:
		if (!m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY - 1)].t_isAlive) return false;
		m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY - 1)].t_ManaValue += manaValue;
		break;
	case eMoveState::DOWN:
		if (!m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY + 1)].t_isAlive) return false;
		m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY + 1)].t_ManaValue += manaValue;
		break;
	case eMoveState::LEFT:
		if (!m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_isAlive) return false;
		m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_ManaValue += manaValue;
		break;
	case eMoveState::RIGHT:
		if (!m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_isAlive) return false;
		m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY].t_ManaValue += manaValue;
		break;
	}

	return true;
}

void enemy::enemySetTxt(int enemyType)
{
	tagTileTxt setTxt;

	switch (enemyType)
	{
	case tagEnemyType::Skeleton:
		setTxt.t_txtName = "뼈다귀";
		setTxt.t_txtInfo = "던전을 공략하다 실패한 용사 처럼 보입니다.";
		break;
	case tagEnemyType::Slime:
		setTxt.t_txtName = "액괴";
		setTxt.t_txtInfo = "이래뵈도, 움직이는 영양덩어리 입니다.";
		break;
	case tagEnemyType::Daemon:
		setTxt.t_txtName = "철갑추";
		setTxt.t_txtInfo = "철컹철컹 걸을때 마다 시끄러운 녀석 입니다.";
		break;
	case tagEnemyType::Bat:
		setTxt.t_txtName = "뱃치";
		setTxt.t_txtInfo = "던전에 박쥐가 빠질 수 없죠 파닥파닥~";
		break;
	case tagEnemyType::Rat:
		setTxt.t_txtName = "쥐";
		setTxt.t_txtInfo = "찍 찍찍 찍찍찍! 라고 하는군요..";
		break;
	case tagEnemyType::Spider:
		setTxt.t_txtName = "커미";
		setTxt.t_txtInfo = "저 다리좀 보세요 각선미가 엄청납니다!";
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
	//// 최초 행동 방향 저장
	int tempMoveState = m_eMoveState;
	if (m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo == &m_tEnemyData)
		m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo = nullptr;
	
	eatEnemy();

	switch (m_tEnemyData.t_enumType)
	{
	case tagEnemyType::Bat: // 이동하다가 방향 전환
		if (moveIsRect(m_eMoveState) && RANDOM->getInt(100) > 30)
		{
			moveRectCheck(m_eMoveState);
			m_isMoveAct = true;
		}
		else
		{
			m_eMoveState = RANDOM->getFromIntTo(0, 3);
			m_isMoveAct = true;
		}
		break;
	case tagEnemyType::Rat: // 벽 짚고 이동

		m_eMoveState = 0;
		if (moveIsRect(m_eMoveState) && eMoveState::DOWN != tempMoveState)
		{
			m_eMoveState = m_eMoveState;
			moveRectCheck(m_eMoveState);
		}
		else if (moveIsRect(m_eMoveState + 1) && eMoveState::LEFT != tempMoveState)
		{
			m_eMoveState = m_eMoveState + 1;
			moveRectCheck(m_eMoveState);
		}
		else if (moveIsRect(m_eMoveState + 2) && eMoveState::UP != tempMoveState)
		{
			m_eMoveState = m_eMoveState + 2;
			moveRectCheck(m_eMoveState);
		}
		else if (moveIsRect(m_eMoveState + 3) && eMoveState::RIGHT != tempMoveState)
		{
			m_eMoveState = m_eMoveState + 3;
			moveRectCheck(m_eMoveState);
		}
		else if ((!(moveIsRect(m_eMoveState) && !(moveIsRect(m_eMoveState + 1) && !(moveIsRect(m_eMoveState + 3))))))
		{
			m_eMoveState = m_eMoveState + 2;
			moveRectCheck(m_eMoveState);
		}

		m_isMoveAct = true;
 		break;
	case tagEnemyType::Slime: // 막을때 까지 이동
		if (moveRectCheck(m_eMoveState))
		{
			m_isMoveAct = true;
		}
		else
		{
			m_eMoveState = RANDOM->getFromIntTo(0, 3);
			m_isMoveAct = true;
		}
		break;
	case tagEnemyType::Daemon: // 벽에 닿으면 반대 방향으로 이동
		if (moveRectCheck(m_eMoveState))
		{
			m_isMoveAct = true;
		}
		else
		{
			if (m_eMoveState == eMoveState::UP)
			{
				m_eMoveState = eMoveState::DOWN;
			}
			else if (m_eMoveState == eMoveState::DOWN)
			{
				m_eMoveState = eMoveState::UP;
			}

			if (m_eMoveState == eMoveState::LEFT)
			{
				m_eMoveState = eMoveState::RIGHT;
			}
			else if (m_eMoveState == eMoveState::RIGHT)
			{
				m_eMoveState = eMoveState::LEFT;
			}

			m_isMoveAct = true;
		}
		break;
	default: // 움직일때마다 랜덤
		m_eMoveState = RANDOM->getFromIntTo(0, 4);
		moveRectCheck(m_eMoveState);
		m_isMoveAct = true;
		break;
	}

	//// tileMap 위치에 몬스터 정보 저장
	m_pTileMapMag->getTileSetPoint()[(m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY)].t_enemyInfo = &m_tEnemyData;
	m_ani.start();
}

void enemy::isDieTileMana()
{
	for (int i = 0; i < 4; i++)
	{
		int temp = RANDOM->getFromIntTo(0, 3);
		if (tileManaChg(temp, m_tEnemyData.t_MaxHp / 10)) return;
	}
}

bool enemy::eatIsEnemy(int eMoveArrow)
{
	int moveArrow;

	switch (eMoveArrow)
	{
	case eMoveState::UP:
		moveArrow = (m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY - 1);
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo == nullptr) return false;
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_FoodChainLv < m_tEnemyData.t_FoodChainLv)
		{
			m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_isAilve = false;
			m_tEnemyData.t_currHp += m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_currHp;
			EFFMANAGER->play("MousePointEFF", 100, 100);

			return true;
		}
		break;
	case eMoveState::DOWN:
		moveArrow = (m_tEnemyData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tEnemyData.t_tilePosY + 1);
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo == nullptr) return false;
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_FoodChainLv < m_tEnemyData.t_FoodChainLv)
		{
			m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_isAilve = false;
			m_tEnemyData.t_currHp += m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_currHp;
			EFFMANAGER->play("MousePointEFF", 100, 100);


			return true;
		}
		break;
	case eMoveState::LEFT:
		moveArrow = (m_tEnemyData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo == nullptr) return false;
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_FoodChainLv < m_tEnemyData.t_FoodChainLv)
		{
			m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_isAilve = false;
			m_tEnemyData.t_currHp += m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_currHp;
			EFFMANAGER->play("MousePointEFF", 100, 100);


			return true;
		}
		break;
	case eMoveState::RIGHT:
		moveArrow = (m_tEnemyData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY() + m_tEnemyData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo == nullptr) return false;
		if (m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_FoodChainLv < m_tEnemyData.t_FoodChainLv)
		{
			m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_isAilve = false;
			m_tEnemyData.t_currHp += m_pTileMapMag->getTileSetPoint()[moveArrow].t_enemyInfo->t_currHp;
			EFFMANAGER->play("MousePointEFF", 100, 100);

			return true;
		}
		break;
	}

	return false;
}

void enemy::eatEnemy()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_tEnemyData.t_currHp < m_tEnemyData.t_MaxHp / 3 && eatIsEnemy(i));
		return;
	}
}

enemy::enemy()
{
}


enemy::~enemy()
{
}
