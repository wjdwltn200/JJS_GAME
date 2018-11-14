#pragma once
class loadingScene;
class titleScene;
class gameScene;

class mainGame
{
private:


	// DC
	HDC hdc;

	// ¹é¹öÆÛ
	image * m_pBackBuffer;
	void setBackBuffer();

	loadingScene * m_pLoadingScene;
	gameScene * m_pinGameScene;
	titleScene * m_pTitleScene;

	image * m_screenCamImg;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	mainGame();
	~mainGame();
};

