#pragma once
#include "scene.h"
#include "animation.h"

class titleScene : public scene
{
private:
	enum tagTITLE_BUTTON
	{
		T_BUTTON_GAMESTART, T_BUTTON_EXIT, T_BUTTON_NUM
	};

	image * m_ImgTitle;
	image * m_imgOverlord;

	animation m_ani;

	int m_ImgChangValue;
	bool m_isScreenChange;
	int m_screenChangeValue;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

public:

	titleScene();
	~titleScene();
};

