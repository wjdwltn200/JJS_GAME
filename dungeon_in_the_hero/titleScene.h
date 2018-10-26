#pragma once
#include "scene.h"

class titleScene : public scene
{
private:
	enum tagTITLE_BUTTON
	{
		T_BUTTON_GAMESTART, T_BUTTON_CUSTOM, T_BUTTON_COLLECTION, T_BUTTON_EXIT, T_BUTTON_NUM
	};

	image * m_ImgTitle;
	image * m_ImgButton;

	int m_ImgChangValue;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

public:

	titleScene();
	~titleScene();
};

