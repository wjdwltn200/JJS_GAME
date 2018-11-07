#pragma once

namespace MY_UTIL
{
	float getDistance(float startX, float startY, float endX, float endY);
	float getAngle(float startX, float startY, float endX, float endY);
	float getMouseAngle(float startX, float startY);

	void FontOption(HDC hdc, int FontSize, int FontBold);
	void FontDelete(HDC hdc);
	void NumberPont(HDC hdc, int num, int maxNum, float posX, float posY, int CorType, float scale = 1.0f);
	void HetSe(bool isBase = false, int seType = 0);
}

