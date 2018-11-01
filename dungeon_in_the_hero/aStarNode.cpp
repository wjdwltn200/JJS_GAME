#include "stdafx.h"
#include "aStarNode.h"

void aStarNode::NodeCopy(aStarNode * pNode)
{
	m_posX = pNode->m_posX;
	m_posY = pNode->m_posY;
	m_dist = pNode->m_dist;
	m_depth = pNode->m_depth;
	pParent = pNode->pParent;

}

bool aStarNode::isSamePos(aStarNode * pNode)
{
	if (m_posX != pNode->m_posX || m_posY != pNode->m_posY) return false;

	return true;
}

aStarNode* aStarNode::Clone()
{
	aStarNode* pNode = new aStarNode;
	pNode->m_posX = m_posX;
	pNode->m_posY = m_posY;
	pNode->m_dist = m_dist;
	pNode->m_depth = m_depth;
	pNode->pParent = NULL;

	return  pNode;
}

aStarNode * aStarNode::Create(int sX, int sY, int dX, int dY, int dep)
{
	aStarNode * pNode = new aStarNode; // 최초 노드 생성
	pNode->m_posX = sX; // 최초 위치 X
	pNode->m_posY = sY; // 최초 위치 Y

	int deltX = dX - sX; // 목적지까지 최소 거리 X
	int deltY = dY - sY; // 목적지까지 최소 거리 Y
	pNode->m_dist = (deltX * deltX) + (deltY * deltY); // 최소 거리
	pNode->m_depth = dep; // 비용

	return pNode;
}

aStarNode * aStarNode::Create(int sX, int sY)
{
	aStarNode * pNode = new aStarNode;
	pNode->m_posX = sX;
	pNode->m_posY = sY;

	return pNode;
}

void aStarNode::CalcDist(aStarNode * pDest, int cdepth)
{
	int deltx = pDest->m_posX - m_posX;
	int delty = pDest->m_posY - m_posY;
	m_dist = (deltx * deltx) + (delty * delty);
	m_depth = cdepth;
}

aStarNode::aStarNode()
{
}


aStarNode::~aStarNode()
{
}
