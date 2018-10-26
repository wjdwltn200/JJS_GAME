#include "stdafx.h"
#include "sceneManager.h"
#include "scene.h"

DWORD CALLBACK loadingThread(LPVOID prc)
{
	// ���� ���� �ʱ�ȭ�Ѵ�
	if (SUCCEEDED(sceneManager::m_pNextScene->init()))
	{
		sceneManager::m_pCurrScene = sceneManager::m_pNextScene;

		// �ε� ���� �����Ѵ�
		if (sceneManager::m_pLoadingScene)
		{
			sceneManager::m_pLoadingScene->release();
			sceneManager::m_pLoadingScene = NULL;
		}
	}

	return 0;
}

scene* sceneManager::m_pCurrScene = NULL;
scene* sceneManager::m_pNextScene = NULL;
scene* sceneManager::m_pLoadingScene = NULL;


HRESULT sceneManager::init()
{
	return S_OK;
}

void sceneManager::release()
{
	// ���� ��� ���Ҹ� ���鼭 Ȯ��
	for (m_iter = m_mapScenes.begin(); m_iter != m_mapScenes.end(); )
	{
		// ������ value( scene* )�� ������
		if (m_iter->second != NULL)
		{
			// Ȯ���Ϸ��� ���� ���� ���̸� release ȣ��
			if (m_iter->second == m_pCurrScene)
				m_iter->second->release();

			// �޸� ����
			SAFE_DELETE(m_iter->second);
			// �ʿ��� ����
			m_iter = m_mapScenes.erase(m_iter);
		}
		else
		{
			m_iter++;
		}
	}
	m_mapScenes.clear();
}

void sceneManager::update()
{
	if (m_pCurrScene)
		m_pCurrScene->update();
}

void sceneManager::render(HDC hdc)
{
	if (m_pCurrScene)
		m_pCurrScene->render(hdc);
}

scene * sceneManager::addScene(string sceneName, scene * pScene)
{
	if (!pScene)	return NULL;

	m_mapScenes.insert(pair<string, scene*>(sceneName, pScene));
	//m_mapScenes.insert(make_pair(sceneName, pScene));

	return pScene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	// �ʿ��� �ٲٰ����ϴ� ���� ã�´�
	m_iter = m_mapScenes.find(sceneName);
	// �ʿ��� �ٲٰ����ϴ� ���� �� ã����
	if (m_iter == m_mapScenes.end())	return E_FAIL;

	// �ٲٰ����ϴ� ���� ������� ������
	if (m_iter->second == m_pCurrScene)	return S_OK;

	// �ٲٰ����ϴ� ���� ã������ �ʱ�ȭ
	if (SUCCEEDED(m_iter->second->init()))
	{
		// �ʱ�ȭ ���� ��, ���� ���� release
		if (m_pCurrScene)
			m_pCurrScene->release();

		// ���� ���� �ٲٰ����ϴ� ������ ��ü
		m_pCurrScene = m_iter->second;
		return S_OK;
	}

	return E_FAIL;
}

scene * sceneManager::addLoadingScene(string loadingSceneName, scene * pScene)
{
	if (!pScene)	return NULL;

	m_mapLoadingScenes.insert(pair<string, scene*>(
		loadingSceneName, pScene));

	return pScene;
}

HRESULT sceneManager::changeScene(string nextSceneName, string loadingSceneName)
{
	// �ʿ��� �ٲٰ����ϴ� ���� ã�´�
	m_iter = m_mapScenes.find(nextSceneName);
	// �ʿ��� �ٲٰ����ϴ� ���� �� ã����
	if (m_iter == m_mapScenes.end())	return E_FAIL;

	// �ٲٰ����ϴ� ���� ������� ������
	if (m_iter->second == m_pCurrScene)	return S_OK;

	// loading : �ε����� ã�´�
	m_iterLoading = m_mapLoadingScenes.find(loadingSceneName);

	if (m_iterLoading == m_mapLoadingScenes.end())
		return changeScene(nextSceneName);

	// �ٲٰ����ϴ� ���� ã������ �ʱ�ȭ
	if (SUCCEEDED(m_iterLoading->second->init()))
	{
		// �ʱ�ȭ ���� ��, ���� ���� release
		if (m_pCurrScene)
			m_pCurrScene->release();

		// ���� ���� �ٲٰ����ϴ� �ε������� ��ü
		m_pCurrScene = m_iterLoading->second;

		// ���� ���� �غ��Ѵ�
		m_pNextScene = m_iter->second;

		// �����带 �߰������ؼ� ���ÿ� �ε��ϸ鼭 �ε�ȭ�鵵 �����Ѵ�
		// CreateThread
		//LPTHREAD_START_ROUTINE
		//HANDLE hThread;
		CloseHandle(CreateThread(
			NULL,			// ������ Ŀ�� ���� Ư��
			0,				// ������ ���� ũ�� ����
			loadingThread,	// ����� �Լ�
			NULL,			// ����� �Լ��� �Ѱ����� �Ű�����
			0,				// ������ ���� �÷���
			NULL			// �������� ID
		));

		//CloseHandle(hThread);
		return S_OK;
	}

	return E_FAIL;
}

sceneManager::sceneManager()
{
}


sceneManager::~sceneManager()
{
}
