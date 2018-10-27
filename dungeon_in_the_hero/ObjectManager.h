#pragma once
#include "singletonBase.h"

class subObject;
class tagItemData;

class ObjectManager : public singletonBase<ObjectManager>
{
private:
	vector<subObject*>	m_vecSubObjects;
	vector<subObject*>::iterator	m_iter;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void addTileDesObj(int typeNum, float posX, float posY, tagItemData itemData);
	ObjectManager();
	~ObjectManager();
};

