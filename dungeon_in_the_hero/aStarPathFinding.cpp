#include "stdafx.h"
#include "aStarPathFinding.h"
#include "aStarNode.h"


bool aStarPathFinding::NodeCompare(aStarNode * p1, aStarNode * p2)
{
	if (p1->getDist() < p2->getDist()) return true;
	if (p1->getDist() > p2->getDist()) return false;
	if (p1->getDepth() <= p2->getDepth()) return true;

	return false;
}

void aStarPathFinding::InsertOpenNode(aStarNode * pNode)
{
	for (m_iterOpenNode = m_vecOpenNode.begin();
		m_iterOpenNode != m_vecOpenNode.end();
		m_iterOpenNode++)
	{
		if ((*m_iterOpenNode)->isSamePos(pNode))
		{
			InsertCloseNode((*m_iterOpenNode));
			(*m_iterOpenNode) = pNode;
			return;
		}
	}

	m_vecOpenNode.push_back(pNode);
}
aStarNode aStarPathFinding::InsertCloseNode(aStarNode * pNode)
{
	m_vecCloseNode.push_back(pNode);
	return * pNode;
}

void aStarPathFinding::SortOpenNode()
{
	if (sizeof(m_vecOpenNode) < 2) return;
	aStarNode * pNode;
	bool bContinue = true;

	while (bContinue)
	{
		bContinue = false;
		for (m_iterOpenNode = m_vecOpenNode.begin();
			m_iterOpenNode != m_vecOpenNode.end();
			m_iterOpenNode++)
		{
			if (!NodeCompare((*m_iterOpenNode), (*m_iterOpenNode) + 1))
			{
				pNode = (*m_iterOpenNode);
				*m_iterOpenNode = (*m_iterOpenNode) + 1;
				*m_iterOpenNode++ = pNode;
				bContinue = true;
			}
		}
	}
}

bool aStarPathFinding::FindFromOpenNode(aStarNode * pNode)
{
	for (m_iterOpenNode = m_vecOpenNode.begin();
		m_iterOpenNode != m_vecOpenNode.end();
		m_iterOpenNode++)
	{
		if ((*m_iterOpenNode)->isSamePos(pNode)) return true;
	}
	return false;
}

bool aStarPathFinding::FindFromCloseNode(aStarNode * pNode)
{
	for (m_iterCloseNode = m_vecCloseNode.begin();
		m_iterCloseNode != m_vecCloseNode.end();
		m_iterCloseNode++)
	{
		if ((*m_iterCloseNode)->isSamePos(pNode)) return true;
	}
	return false;
}


void aStarPathFinding::Delete()
{
	for (m_iterOpenNode = m_vecOpenNode.begin();
		m_iterOpenNode != m_vecOpenNode.end();
		m_iterOpenNode++)
	{
		delete (*m_iterOpenNode);
	}
	m_vecOpenNode.clear();
	
	for (m_iterCloseNode = m_vecCloseNode.begin();
		m_iterCloseNode != m_vecCloseNode.end();
		m_iterCloseNode++)
	{
		delete (*m_iterCloseNode);
	}
	m_vecCloseNode.clear();
}

bool aStarPathFinding::FindPath(aStarNode * pStart, aStarNode * pEnd, list<aStarNode*> vecPath, tagTileData * pNavi)
{
	this->Delete();

	aStarNode * tempNode = &pStart->Clone();

	m_vecOpenNode.push_back(tempNode);

	int iDepth = 0;
	tempNode->setDepth(iDepth);

	list<aStarNode> * vecChilds;
	vecChilds = new list<aStarNode>;

	while (true)
	{
		if (sizeof(m_vecOpenNode) == 0)
		{
			break;
		}
	}

	m_iterOpenNode = m_vecOpenNode.begin();
	m_vecOpenNode.pop_front();

	if (pEnd->isSamePos(tempNode))
	{
		while (tempNode != NULL)
		{
			vecPath.push_back(tempNode);
			tempNode = &tempNode->getParent();
		}
		return true;
	}


	*tempNode = InsertCloseNode(tempNode);
	++iDepth;

	vecChilds->clear();
}

aStarPathFinding::aStarPathFinding()
{
}


aStarPathFinding::~aStarPathFinding()
{
}
