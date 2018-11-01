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
//	//  최초 초기화
//	Delete(); 
//
//	// 시작 위치는 노드에 저장한다
//	aStarNode * tempNode = &pStart->Clone();
//
//	// 최초 초기화 된 시작 위치 노드를 열린 노드에 넣어준다
//	m_vecOpenNode.push_back(tempNode);
//
//	// 깊이(찾은 비용)을 0으로 초기화해준다
//	int iDepth = 0;
//	tempNode->setDepth(iDepth);
//
//	// 자식 노드는 동적할당 하여 생성 한다
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
//	// 열린 노드의 첫번째를 iter에 주소로 넣어준다
//	m_iterOpenNode = m_vecOpenNode.begin();
//	// 열린 노드의 첫번째(최초)에 들어간 녀석을 삭제한다
//	m_vecOpenNode.pop_front();
//
//	// 도착했는지 체크한다 if가 도착지 x, y와 같을 경우 true로 리턴
//	if (pEnd->isSamePos(tempNode))
//	{
//		// 도착했다면 실행
//		while (tempNode != NULL)
//		{
//			vecPath.push_back(tempNode);
//			tempNode = &tempNode->getParent();
//		}
//		return true;
//	}
//
//	// 도착하지 못했다면 실행된다
//	// tempNode를 닫힌 list에 넣어준다
//	*tempNode = InsertCloseNode(tempNode);
//	
//	// 비용을 증가시킨다
//	++iDepth;
//
//	// 자식의 값의 원소를 제거 시킨다(값 초기화)
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
