#pragma once
class aStarNode
{
private:
	SYNTHESIZE(int, m_posX, PosX);
	SYNTHESIZE(int, m_posY, PosY);
	SYNTHESIZE(int, m_dist, Dist);
	SYNTHESIZE(int, m_depth, Depth);

	aStarNode * pParent = nullptr;

public:
	void NodeCopy(aStarNode * pNode);
	bool isSamePos(aStarNode * pNode);
	aStarNode* Clone();
	aStarNode * Create(int sX, int sY, int dX, int dY, int dep);
	aStarNode * Create(int sX, int sY);

	void CalcDist(aStarNode * pDest, int cdepth);

	void setParent(aStarNode * p) { pParent = p; }
	aStarNode getParent() { return * pParent; }

	aStarNode();
	~aStarNode();
};

