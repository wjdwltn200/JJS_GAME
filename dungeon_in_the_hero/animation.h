#pragma once
#include <vector>

class animation
{
private:
	int		m_nFrameNum;				// �� ������ ��
	std::vector<POINT>	m_vecFrameList;	// �� �������� ���� x,y 
	std::vector<int>	m_vecPlayList;	// �÷��� ��ų ������ �ε��� ����

	int		m_nFrameWidth;				// �� ������ �ʺ�
	int		m_nFrameHeight;				// �� ������ ����

	bool	m_isLoop;			// �ݺ� ���� ����
	float	m_fFrameUpdateSec;	// ������ ������Ʈ �ð�
	float	m_fElapsedSec;		// �����ð�

	int		m_nNowPlayIdx;		// ���� �÷��� �ε���
								// (m_vecPlayList ���� �����Ѵ�)

	bool	m_isPlaying;		// ���� �÷��� ����

public:
	HRESULT init(int totalWidth, int totalHeight,
		int frameWidth, int frameHeight);

	void setDefPlayFrame(bool reverse = false, bool loop = false);	// �⺻����
	void setPlayFrame(int* arrPlay, int arrLen, bool loop = false);
	void setPlayFrame(int start, int end, bool reverse = false, bool loop = false);

	void setFPS(int fps);		// �ʴ� ���� Ƚ�� ����

	void frameUpdate(float elapsedTime = (TIMEMANAGER->getElapsedTime()));	// ������ ������Ʈ

	void start();
	void stop();
	void pause();
	void resume();

	inline POINT getFramePos() { return m_vecFrameList[m_vecPlayList[m_nNowPlayIdx]]; }

	//m_vecFrameList[0];		// �������� ���� �������� �ε����� �ִ´�
	// �÷��� ����Ʈ ���� : 9, 10, 11, 0, 1, 2 
	// m_nNowPlayIdx == 0 => 9 
	// m_nNowPlayIdx == 1 => 10
	// 0 == m_vecPlayList[m_nNowPlayIdx];

	inline int getFrameWidth() { return m_nFrameWidth; }
	inline int getFrameHeight() { return m_nFrameHeight; }

	inline bool getIsPlaying() { return m_isPlaying; }


	animation();
	~animation();
};

