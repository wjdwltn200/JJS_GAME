#include "stdafx.h"
#include "effectManager.h"
#include "effect.h"


HRESULT effectManager::init()
{
	return S_OK;
}

void effectManager::release()
{

	vector<effect*>::iterator iterVE;
	vector<effect*> vecE;

	// m_mapEffects < key, value >
	// key : string / ( iterator->first )
	// value : vector<effect*> / ( iterator->second ) 
	// m_iter : m_mapEffects�� �����ϴ� ����(pair : key, value)��
	// ��� �ִ� �޸� �ּ� ( == ������)
	m_iter = m_mapEffects.begin();
	for (; m_iter != m_mapEffects.end(); )
	{
		// key : m_iter->first 
		// value : m_iter->second 
		// value�� �޾ƿ´� vecE (���⿡)
		vecE = (m_iter->second);

		// iterVE : vecE( vector<effect*> )�� �����ϴ� ���Ҹ�
		// ��� �ִ� �޸� �ּ�
		for (iterVE = vecE.begin(); iterVE != vecE.end(); iterVE++)
		{
			// (*iterVE) == effect* pEffect
			if ((*iterVE))
			{
				(*iterVE)->release();
				SAFE_DELETE((*iterVE));
			}
		}
		vecE.clear();

		m_iter = m_mapEffects.erase(m_iter);
	}
	m_mapEffects.clear();
}

void effectManager::update()
{
	vector<effect*> vecE;
	vector<effect*>::iterator iterVE;

	// effect�� �����͸� ���ҷ� ���� vector
	for (m_iter = m_mapEffects.begin();
		m_iter != m_mapEffects.end(); m_iter++)
	{
		vecE = (m_iter->second);
		for (iterVE = vecE.begin(); iterVE != vecE.end(); iterVE++)
		{
			(*iterVE)->update();
		}
	}
}

void effectManager::render(HDC hdc)
{
	//// key, value
	////m_mapEffects[key]
	//m_iter = m_mapEffects.begin();
	//m_iter->first; // key (string)
	//m_iter->second;	// value (vecEffect)


	vector<effect*> vecE;
	vector<effect*>::iterator iterVE;

	// effect�� �����͸� ���ҷ� ���� vector
	for (m_iter = m_mapEffects.begin();
		m_iter != m_mapEffects.end(); m_iter++)
	{
		vecE = (m_iter->second);
		for (iterVE = vecE.begin(); iterVE != vecE.end(); iterVE++)
		{
			(*iterVE)->render(hdc);
		}
	}
}

void effectManager::addEffect(string effectName,
	const char * imageName, int imageWidth, int imageHeight,
	int frameWidth, int frameHeight, int fps,
	int bufferCount, float elapsedTime /* = 0.0f */, bool isCam)
{
	image* pImage = IMAGEMANAGER->addImage(effectName, imageName,
		imageWidth, imageHeight, true, RGB(255, 0, 255));

	vecEffect vecEffects;
	for (int i = 0; i < bufferCount; i++)
	{
		effect* pEffect = new effect;
		pEffect->init(pImage,
			frameWidth, frameHeight, fps, elapsedTime, isCam);

		vecEffects.push_back(pEffect);
	}

	m_mapEffects.insert(
		pair<string, vecEffect>(effectName, vecEffects));
}

void effectManager::play(string effectName, int x, int y)
{
	vector<effect*> vecE;
	vector<effect*>::iterator iterVE;

	m_iter = m_mapEffects.find(effectName);
	vecE = (m_iter->second);

	for (iterVE = vecE.begin(); iterVE != vecE.end(); iterVE++)
	{
		if ((*iterVE)->getIsAlive()) continue;

		(*iterVE)->startEffect(x - (*iterVE)->getimgSizeX() / 2, y - (*iterVE)->getimgSizeY() / 2);
		return;
	}

}

effectManager::effectManager()
{
}


effectManager::~effectManager()
{
}
