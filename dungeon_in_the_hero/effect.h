#pragma once
class image;
class animation;

class effect
{
private:
	image * m_pImg;
	animation*	m_pAnimation;
	bool	m_isAlive;
	float	m_fElapsedTime;

	int		m_nX;
	int		m_nY;

	bool	m_isCamXY;

	SYNTHESIZE(int, m_imgSizeX, imgSizeX);
	SYNTHESIZE(int, m_imgSizeY, imgSizeY);

public:
	HRESULT init(image* effectImage, 
		int frameWidth, int frameHeight,
		int fps, float elapsedTime, bool isCam);
	void release();
	void update();
	void render(HDC hdc);

	bool getIsAlive() {return m_isAlive;}

	void startEffect(int x, int y);
	void endEffect();

	effect();
	~effect();
};

