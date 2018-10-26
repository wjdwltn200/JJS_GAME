#pragma once

#include "singletonBase.h"
#include <vector>
#include <string>

class txtData : public singletonBase<txtData>
{
public:
	HRESULT init();
	void release();

	// ���̺� 
	void txtSave(const char* saveFileName, std::vector<std::string> vecStr);
	char* vectorArrayCombine(std::vector<std::string> vecArray);

	// �ε�
	std::vector<std::string> txtLoad(const char* loadFileName);
	std::vector<std::string> charArraySeparation(char charArray[]);

	txtData();
	~txtData();
};

