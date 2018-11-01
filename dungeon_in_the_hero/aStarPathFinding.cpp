#include "stdafx.h"
#include "aStarPathFinding.h"
#include "aStarNode.h"
//
//bool aStarPathFinding::NodeCompare(aStarNode * p1, aStarNode * p2)
//{
//	if (p1->getDist() < p2->getDist()) return true;
//	if (p1->getDist() > p2->getDist()) return false;
//	if (p1->getDepth() <= p2->getDepth()) return true;
//
//	return false;
//}
//
//void aStarPathFinding::InsertOpenNode(aStarNode * pNode)
//{
//	for (m_iterOpenNode = m_vecOpenNode.begin();
//		m_iterOpenNode != m_vecOpenNode.end();
//		m_iterOpenNode++)
//	{
//		if ((*m_iterOpenNode)->isSamePos(pNode))
//		{
//			InsertCloseNode((*m_iterOpenNode));
//			(*m_iterOpenNode) = pNode;
//			return;
//		}
//	}
//
//	m_vecOpenNode.push_back(pNode);
//}
//aStarNode aStarPathFinding::InsertCloseNode(aStarNode * pNode)
//{
//	m_vecCloseNode.push_back(pNode);
//	return * pNode;
//}
//
//void aStarPathFinding::SortOpenNode()
//{
//	if (sizeof(m_vecOpenNode) < 2) return;
//	aStarNode * pNode;
//	bool bContinue = true;
//
//	while (bContinue)
//	{
//		bContinue = false;
//		for (m_iterOpenNode = m_vecOpenNode.begin();
//			m_iterOpenNode != m_vecOpenNode.end();
//			m_iterOpenNode++)
//		{
//			if (!NodeCompare((*m_iterOpenNode), (*m_iterOpenNode) + 1))
//			{
//				pNode = (*m_iterOpenNode);
//				*m_iterOpenNode = (*m_iterOpenNode) + 1;
//				*m_iterOpenNode++ = pNode;
//				bContinue = true;
//			}
//		}
//	}
//}
//
//bool aStarPathFinding::FindFromOpenNode(aStarNode * pNode)
//{
//	for (m_iterOpenNode = m_vecOpenNode.begin();
//		m_iterOpenNode != m_vecOpenNode.end();
//		m_iterOpenNode++)
//	{
//		if ((*m_iterOpenNode)->isSamePos(pNode)) return true;
//	}
//	return false;
//}
//
//bool aStarPathFinding::FindFromCloseNode(aStarNode * pNode)
//{
//	for (m_iterCloseNode = m_vecCloseNode.begin();
//		m_iterCloseNode != m_vecCloseNode.end();
//		m_iterCloseNode++)
//	{
//		if ((*m_iterCloseNode)->isSamePos(pNode)) return true;
//	}
//	return false;
//}
//
//
//void aStarPathFinding::Delete()
//{
//	for (m_iterOpenNode = m_vecOpenNode.begin();
//		m_iterOpenNode != m_vecOpenNode.end();
//		m_iterOpenNode++)
//	{
//		delete (*m_iterOpenNode);
//	}
//	m_vecOpenNode.clear();
//	
//	for (m_iterCloseNode = m_vecCloseNode.begin();
//		m_iterCloseNode != m_vecCloseNode.end();
//		m_iterCloseNode++)
//	{
//		delete (*m_iterCloseNode);
//	}
//	m_vecCloseNode.clear();
//}
//
//bool aStarPathFinding::FindPath(aStarNode * pStart, aStarNode * pEnd, list<aStarNode*> vecPath, hero * pNavi)
//{
//	//  ���� �ʱ�ȭ
//	Delete(); 
//
//	// ���� ��ġ�� ��忡 �����Ѵ�
//	aStarNode * tempNode = &pStart->Clone();
//
//	// ���� �ʱ�ȭ �� ���� ��ġ ��带 ���� ��忡 �־��ش�
//	m_vecOpenNode.push_back(tempNode);
//
//	// ����(ã�� ���)�� 0���� �ʱ�ȭ���ش�
//	int iDepth = 0;
//	tempNode->setDepth(iDepth);
//
//	// �ڽ� ���� �����Ҵ� �Ͽ� ���� �Ѵ�
//	list<aStarNode*> vecChilds;
//	list<aStarNode*>::iterator	iterChilds;
//	aStarNode * tempChilds = new aStarNode;
//	vecChilds.push_back(tempChilds);
//
//	// ??
//	while (true)
//	{
//		if (sizeof(m_vecOpenNode) == 0)
//		{
//			break;
//		}
//	}
//
//	// ���� ����� ù��°�� iter�� �ּҷ� �־��ش�
//	m_iterOpenNode = m_vecOpenNode.begin();
//	// ���� ����� ù��°(����)�� �� �༮�� �����Ѵ�
//	m_vecOpenNode.pop_front();
//
//	// �����ߴ��� üũ�Ѵ� if�� ������ x, y�� ���� ��� true�� ����
//	if (pEnd->isSamePos(tempNode))
//	{
//		// �����ߴٸ� ����
//		while (tempNode != NULL)
//		{
//			vecPath.push_back(tempNode);
//			tempNode = &tempNode->getParent();
//		}
//		return true;
//	}
//
//	// �������� ���ߴٸ� ����ȴ�
//	// tempNode�� ���� list�� �־��ش�
//	*tempNode = InsertCloseNode(tempNode);
//	
//	// ����� ������Ų��
//	++iDepth;
//
//	// �ڽ��� ���� ���Ҹ� ���� ��Ų��(�� �ʱ�ȭ)
//	vecChilds.clear();
//
//	for (iterChilds = vecChilds.begin(); iterChilds != vecChilds.end(); iterChilds++)
//	{
//		if (FindFromCloseNode((*iterChilds)))
//			continue;
//	}
//
//	(*iterChilds)->CalcDist(pEnd, iDepth);
//	(*iterChilds)->setParent(tempNode);
//	InsertOpenNode((*iterChilds));
//
//	SortOpenNode();
//
//	Delete();
//
//	return false;
//}

aStarPathFinding::aStarPathFinding()
{
}


aStarPathFinding::~aStarPathFinding()
{
}
