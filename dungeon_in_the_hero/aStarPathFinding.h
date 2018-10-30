#pragma once

class aStarNode;
class hero;

class aStarPathFinding
{
private:
	list<aStarNode*> m_vecOpenNode;
	list<aStarNode*>::iterator	m_iterOpenNode;
	list<aStarNode*> m_vecCloseNode;
	list<aStarNode*>::iterator	m_iterCloseNode;

	bool NodeCompare(aStarNode * p1, aStarNode * p2);
	void InsertOpenNode(aStarNode * pNode);
	aStarNode InsertCloseNode(aStarNode * pNode);
	void SortOpenNode();
	bool FindFromOpenNode(aStarNode * pNode);
	bool FindFromCloseNode(aStarNode * pNode);
public:
	void Delete();
	bool FindPath(aStarNode * pStart, aStarNode * pEnd, list<aStarNode*> vecPath, hero * pNavi);

	aStarPathFinding();
	~aStarPathFinding();
};

