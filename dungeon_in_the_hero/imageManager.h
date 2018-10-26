#pragma once
#include "singletonBase.h"
#include "image.h"

#include <map>
#include <string>

// key�� value�� pair���·� �����Ѵ�
// key�� map���� ������ ���̴�

class imageManager : public singletonBase<imageManager>
{
public:
	//int* pA;
	//image* pI;

	std::map<std::string, image*>	m_map;
	std::map<std::string, image*>::iterator m_iter;

public:
	imageManager();
	~imageManager();

	HRESULT init();
	void release();

	image* addImage(std::string strKey, int width, int height);
	image* addImage(std::string strKey, const char* szFileName,
		int width, int height, 
		bool trans = false, COLORREF transColor = RGB(0, 0, 0));
	image* addImage(std::string strKey, const char* szFileName, 
		int width, int height, int frameX, int frameY,
		bool trans, COLORREF transColor, float setX = 0.0f, float setY = 0.0f);
	image* findImage(std::string strKey);
	bool deleteImage(std::string strKey);
	bool deleteImageAll();
};

