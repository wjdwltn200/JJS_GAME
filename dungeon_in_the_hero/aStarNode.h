#pragma once
class aStarNode
{
private:
	int m_posX; 
	int m_posY; 
	SYNTHESIZE(int, m_dist, Dist);
	SYNTHESIZE(int, m_depth, Depth);

	aStarNode * pParent;

public:
	void NodeCopy(aStarNode * pNode);
	bool isSamePos(aStarNode * pNode);
	aStarNode Clone();
	static aStarNode Create(int sX, int sY, int dX, int dY, int dep);
	static aStarNode Create(int sX, int sY);

	void CalcDist(aStarNode pDest, int cdepth);

	void setParent(aStarNode * p) { pParent = p; }
	aStarNode getParent() { return * pParent; }

	aStarNode();
	~aStarNode();
};

