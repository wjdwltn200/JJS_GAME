#include "stdafx.h"
#include "enemy.h"
#include "animation.h"
#include "tileMap.h"
#include "enemyManager.h"
#include "uiManager.h"

HRESULT enemy::init(tagEnemyData* enemyInfo, tileMap* pTileMag, enemyManager * pEnemyMag)
{
	//// Ÿ�� �� �ּ� �ʱ�ȭ
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

	m_tEnemyData.t_rc = RectMakeCenter(m_tEnemyData.t_posX, m_tEnemyData.t_posY, m_tEnemyData.t_img->getFrameWidth(), m_tEnemyData.t_img->getFrameHeight());
	m_tEnemyData.t_enumType = enemyInfo->t_enumType;


	m_moveDaley = m_tEnemyData.t_moveDaley;

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

void enemy::currHp()
{
	m_tEnemyData.t_DieCountDaley--;
	if (m_tEnemyData.t_DieCountDaley <= 0)
	{
		m_tEnemyData.t_currHp--;
		m_tEnemyData.t_DieCountDaley = ENEMY_DIE_DALEY;
	}

	if (m_tEnemyData.t_currHp > m_tEnemyData.t_MaxHp) // �ִ� ü�� �ʰ� �Ұ�
		m_tEnemyData.t_currHp = m_tEnemyData.t_MaxHp;

	if (m_tEnemyData.t_currHp <= 0) // ����
		m_tEnemyData.t_isAilve = false;
}

void enemy::moveSys()
{
	if (!m_isMoveAct)
	{
		if (m_moveDaley <= 0)
		{
			// ������ ��ġ�� ��ǥ �� ����
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

void enemy::enemySetTxt(int enemyType)
{
	tagTileTxt setTxt;

	switch (enemyType)
	{
	case tagEnemyType::Skeleton:
		setTxt.t_txtName = "���ٱ�";
		setTxt.t_txtInfo = "������ �����ϴ� ������ ��� ó�� ���Դϴ�.";
		break;
	case tagEnemyType::Slime:
		setTxt.t_txtName = "�ױ�";
		setTxt.t_txtInfo = "�̷��Ƶ�, �����̴� ���絢� �Դϴ�.";
		break;
	case tagEnemyType::Daemon:
		setTxt.t_txtName = "ö����";
		setTxt.t_txtInfo = "ö��ö�� ������ ���� �ò����� �༮ �Դϴ�.";
		break;
	case tagEnemyType::Bat:
		setTxt.t_txtName = "��ġ";
		setTxt.t_txtInfo = "������ ���㰡 ���� �� ���� �Ĵ��Ĵ�~";
		break;
	case tagEnemyType::Rat:
		setTxt.t_txtName = "��";
		setTxt.t_txtInfo = "�� ���� ������! ��� �ϴ±���..";
		break;
	case tagEnemyType::Spider:
		setTxt.t_txtName = "Ŀ��";
		setTxt.t_txtInfo = "�� �ٸ��� ������ �����̰� ��û���ϴ�!";
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
	int tempMoveState = m_eMoveState;

	switch (m_tEnemyData.t_enumType)
	{
	case tagEnemyType::Bat: // �̵��ϴٰ� ���� ��ȯ
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
	case tagEnemyType::Rat: // �� ¤�� �̵�

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
	case tagEnemyType::Slime: // ������ ���� �̵�
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
	case tagEnemyType::Daemon: // ���� ������ �ݴ� �������� �̵�
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
	default: // �����϶����� ����
		m_eMoveState = RANDOM->getFromIntTo(0, 4);
		moveRectCheck(m_eMoveState);
		m_isMoveAct = true;
		break;
	}
	m_ani.start();

}

enemy::enemy()
{
}


enemy::~enemy()
{
}
