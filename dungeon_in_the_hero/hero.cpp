#include "stdafx.h"
#include "hero.h"
#include "tileMap.h"
#include "aStarNode.h"
#include "aStarPathFinding.h"

HRESULT hero::init(tagHeroData * heroInfo, tileMap * pTileMap)
{
	//// Ÿ�� �� �ּ� �ʱ�ȭ
	m_pTileMapMag = pTileMap;

	m_tHeroData.t_isAilve = heroInfo->t_isAilve;
	m_tHeroData.t_posX = heroInfo->t_posX;
	m_tHeroData.t_posY = heroInfo->t_posY;
	m_tHeroData.t_tilePosX = heroInfo->t_tilePosX;
	m_tHeroData.t_tilePosY = heroInfo->t_tilePosY;

	m_tHeroData.t_moveEndX = m_tHeroData.t_posX;
	m_tHeroData.t_moveEndY = m_tHeroData.t_posY;

	m_tHeroData.t_img_U = heroInfo->t_img_U;
	m_tHeroData.t_img_R = heroInfo->t_img_R;
	m_tHeroData.t_img_D = heroInfo->t_img_D;
	m_tHeroData.t_img_L = heroInfo->t_img_L;

	m_tHeroData.t_img_UA = heroInfo->t_img_UA;
	m_tHeroData.t_img_RA = heroInfo->t_img_RA;
	m_tHeroData.t_img_DA = heroInfo->t_img_DA;
	m_tHeroData.t_img_LA = heroInfo->t_img_LA;
	m_tHeroData.t_img_Dead = heroInfo->t_img_Dead;

	m_tHeroData.t_img = heroInfo->t_img_D;
	m_tHeroData.t_scale = heroInfo->t_scale;
	m_tHeroData.t_alphaValue = heroInfo->t_alphaValue = 255;
	m_ani.init(m_tHeroData.t_img->getWidth(), m_tHeroData.t_img->getHeight(), m_tHeroData.t_img->getFrameWidth(), m_tHeroData.t_img->getFrameHeight());
	m_ani.setDefPlayFrame(false, true);
	m_ani.setFPS(15);

	m_tHeroData.t_currHp = heroInfo->t_currHp;
	m_tHeroData.t_MaxHp = heroInfo->t_MaxHp;
	m_tHeroData.t_currDef = heroInfo->t_currDef;

	m_tHeroData.t_moveSpeed = heroInfo->t_moveSpeed;
	m_tHeroData.t_moveDaley = heroInfo->t_moveDaley;

	m_tHeroData.t_atkPoint = heroInfo->t_atkPoint;
	m_tHeroData.t_attType = heroInfo->t_attType;
	m_tHeroData.t_atkDaley = heroInfo->t_atkDaley;

	m_tHeroData.t_defPoint = heroInfo->t_defPoint;

	m_tHeroData.t_setTileMapNum = heroInfo->t_setTileMapNum;
	m_tHeroData.t_damgePoint = heroInfo->t_damgePoint;

	m_tHeroData.t_rc = RectMakeCenter(m_tHeroData.t_posX, m_tHeroData.t_posY, m_tHeroData.t_img->getFrameWidth(), m_tHeroData.t_img->getFrameHeight());


	m_moveDaley = m_tHeroData.t_moveDaley;
	m_aStarDepValue = 0;
	m_isDead = false;
	m_isAttAct = false;

	Delete(true, true);

	m_isMoveAct = false;
	m_eMoveState = eMoveState::UP;

	//aStarFind(m_pAStartNode->Create(20, 15), m_pAStartNode->Create(m_tHeroData.t_tilePosX, m_tHeroData.t_tilePosY));

	

	return S_OK;
}

void hero::release()
{
}

void hero::update()
{
	if (!m_tHeroData.t_isAilve) return;

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
	}



	// ��ũ�� ���� ����
	if (!(m_tHeroData.t_rc.left < -TILE_SIZE || m_tHeroData.t_rc.left > WINSIZEX) &&
		!(m_tHeroData.t_rc.top < -TILE_SIZE || m_tHeroData.t_rc.top > WINSIZEY))
	{
		m_tHeroData.t_img->aniRender(hdc,
			(m_tHeroData.t_posX + (TILE_SIZE / 2) - (m_tHeroData.t_img->getFrameWidth() * m_tHeroData.t_scale) / 2) - CAMERA->getCamPosX(),
			((m_tHeroData.t_posY + TILE_SIZE / 2) - (m_tHeroData.t_img->getFrameHeight() * m_tHeroData.t_scale) / 2) - CAMERA->getCamPosY(),
			&m_ani, m_tHeroData.t_scale, true, m_tHeroData.t_alphaValue);
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
		sprintf_s(szText, "%d", m_tHeroData.t_currHp);
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

	// �ӽ� aStar ����
	aStarNode * tempNode = node->Clone();
	m_vecOpenNode.push_back(tempNode);
	// �˻� ����(���?)
	int iDepth = 0;
	tempNode->setDepth(iDepth);

	while (true)
	{
		if (m_vecOpenNode.size() == 0) break;

		// openList ù��°�� ��´�
		m_iterOpenNode = m_vecOpenNode.begin();
		tempNode = (*m_iterOpenNode);

		// ���� ù��°�� ���ش�
		m_vecOpenNode.pop_front();

		// �������� �����ߴٸ�
		if (endXY->isSamePos(tempNode))
		{
			// �Լ��� �����Ѵ�
			m_vecRoute.push_back(tempNode);
			tempNode = tempNode->getParent();

			aStarRoute();
			return true;
		}

		// ���� ���ߴٸ� ���� ��忡 �־��ش�
		m_vecCloseNode.push_back(tempNode);
		iDepth++; // �˻� ���̸� 1 ���� ��Ų��
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

	// ��ã���� ���
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

	if (m_tHeroData.t_currHp > m_tHeroData.t_MaxHp) // �ִ� ü�� �ʰ� �Ұ�
		m_tHeroData.t_currHp = m_tHeroData.t_MaxHp;

	if (m_tHeroData.t_currHp <= 0) // ����
	{
		m_tHeroData.t_img = m_tHeroData.t_img_Dead;
		m_ani.start();
		m_isDead = true;
	}
}

void hero::moveSys()
{
	// ��� ��� ���¶�� �������� �ʴ´�
	if (m_isDead) return;

	// �ൿ �׼� ����
	if (!m_isMoveAct)
	{
		if (m_moveDaley <= 0 && !m_ani.getIsPlaying())
		{
			// ������ ��ġ�� ��ǥ �� ����
			movePattern();
		}
		m_moveDaley--;
	}
	else
	{
		// �̵� �׼� ����
		switch (m_eMoveState)
		{
		case eMoveState::UP:
			// �̵� �Ÿ�
			if (m_tHeroData.t_posY > m_tHeroData.t_moveEndY)
				m_tHeroData.t_posY -= m_tHeroData.t_moveSpeed;

			// �̵� �Ÿ� ���� ��
			if (m_tHeroData.t_posY <= m_tHeroData.t_moveEndY)
			{
				// ��ġ�� �̵��Ϸ��� �ߴ� �Ÿ��� �ʱ�ȭ
				m_tHeroData.t_posY = m_tHeroData.t_moveEndY;
				// �̵� �׼� ���� bool ��
				m_isMoveAct = false;
				// ���� �̵� �׼Ǳ��� ������
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
	//// �ڽ��� ���� �̵���ġ�� �ϳ��� ������
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

		// ã�� ���� �߰�
		m_aStarDepValue++;

		if (tempNode->getDepth() == m_vecRoute.size())
			return;

		// ����
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
	//	setTxt.t_txtName = "�ױ�";
	//	setTxt.t_txtInfo = "�̷��Ƶ�, �����̴� ���絢� �Դϴ�.";
	//	break;
	//case tagEnemyType::Bug:
	//	setTxt.t_txtName = "������";
	//	setTxt.t_txtInfo = "�ױ��� �Դ� �����Դϴ� �Ͼ�� �Ϳ���?";
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


	// ���� ���δ� �̵��ϱ� ���� �ൿ�Ͽ� �̵��� �ǽ����� �ʵ��� �ؾ��Ѵ�
	if (m_isAttAct)
		m_isAttAct = false;
	if (RANDOM->getInt(100) < 50)
	{
		m_tHeroData.t_img = m_tHeroData.t_img_L;
	}
	else
	{
		m_tHeroData.t_img = m_tHeroData.t_img_R;
	}

	//// ���� �ൿ ���� ����
	int tempMoveState = m_eMoveState;
	//// �̵� �� �ڽ��� ��ġ�� �ڽ��� ������ �ִٸ� �����ش�
	if (m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo == &m_tHeroData)
		m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo = nullptr;

	//// enemyType�� ���� �̵� ���� ����
	if (!monActPattern())
	{
		switch (m_tHeroData.t_enumType)
		{
		default: // ������ ���� ��� ���� �̵�
			//aStarMoveSys();
			m_eMoveState = RANDOM->getFromIntTo(0, 4);
			moveRectCheck(m_eMoveState);
			m_isMoveAct = true;
			break;
		}
	}


	//// tileMap ��ġ�� ���� ���� ����
	m_pTileMapMag->getTileSetPoint()[(m_tHeroData.t_tilePosX) * m_pTileMapMag->getTileSizeY() + (m_tHeroData.t_tilePosY)].t_heroInfo = &m_tHeroData;

	//// ��� ���
	m_ani.start();
}

bool hero::monActPattern()
{
	// �������� �ƴ� ���(m_isAttAct)
	if (!m_isAttAct)
	{
		// 4������ ��� �˻��Ѵ�(UP, RIGHT, DOWN, LEFT)
		for (int i = 0; i < 4; i++)
		{
			if (IsEnemy(i))
			{
				// ���� �׼��� �ǽ��Ѵ�
				m_isAttAct = true;
				m_isMoveAct = false;
				// �ٶ󺸾Ҵ� ���⿡ ���� �׼� ��ġ�� �����Ѵ�
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
				// ���� ����� �ǽ� �����ϵ��� moveDaley�� �ٸ� actDaley�� �ʱ�ȭ
				m_eMoveState = eMoveState::MOVE_NUM;
				m_moveDaley = m_tHeroData.t_moveDaley;
				m_ani.start();
				// ����� �Լ��� ����������(����� ����ó��)
				return true;
			}
		}
	}

	return false;
}

bool hero::IsEnemy(int eMoveArrow)
{
	// Ÿ�� �˻� ����
	int tempMoveArrow;

	switch (eMoveArrow)
	{
	case eMoveState::UP:
		// �˻� ����
		tempMoveArrow = (m_tHeroData.t_tilePosX * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY - 1;
		// �ش� ��ġ�� enemy������ ���� ��� -> ������ return;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	case eMoveState::RIGHT:
		tempMoveArrow = ((m_tHeroData.t_tilePosX + 1) * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	case eMoveState::DOWN:
		tempMoveArrow = (m_tHeroData.t_tilePosX * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY + 1;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	case eMoveState::LEFT:
		tempMoveArrow = ((m_tHeroData.t_tilePosX - 1) * m_pTileMapMag->getTileSizeY()) + m_tHeroData.t_tilePosY;
		if (m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo == nullptr) return false;
		break;
	}

	// ���� ������ �ִٸ� �ڽ��� ���ݷ� ��ŭ �ش� ������ hp�� ��Ƴ���
	m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_enemyInfo->t_damgePoint = m_tHeroData.t_atkPoint;
	EFFMANAGER->play("Hit_Eff_0", m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.left + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)), m_pTileMapMag->getTileSetPoint()[tempMoveArrow].t_rc.top + TILE_SIZE / 2 + (RANDOM->getFromFloatTo(-3.0f, 3.0f)));


	// true�� ��ȯ
	return true;
}

void hero::damge()
{
	if (m_tHeroData.t_damgePoint <= 0) return;

	int tempPoint = (m_tHeroData.t_damgePoint - m_tHeroData.t_defPoint);
	if (tempPoint <= 0)
		tempPoint = 1;

	m_tHeroData.t_currHp -= tempPoint;
	m_tHeroData.t_damgePoint = 0;
}

hero::hero()
{
}


hero::~hero()
{
}
