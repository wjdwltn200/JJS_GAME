#include "stdafx.h"
#include "utils.h"

namespace MY_UTIL
{
	float getDistance(float startX, float startY, float endX, float endY)
	{
		float a = endX - startX;
		float b = endY - startY;

		return sqrt(a * a + b * b);
	}

	float getAngle(float startX, float startY, float endX, float endY)
	{
		float a = endX - startX;
		float b = endY - startY;

		float d = sqrt(a * a + b * b);
		float angle = acosf(a / d);

		if (endY > startY)
		{
			angle = PI * 2 - angle;
		}

		return angle;
	}

	float getMouseAngle(float startX, float startY)
	{
		float a = g_ptMouse.x - startX;
		float b = g_ptMouse.y - startY;

		float d = sqrt(a * a + b * b);
		float angle = acosf(a / d);

		if (g_ptMouse.y > startY)
		{
			angle = PI * 2 - angle;
		}

		return angle;
	}


	void FontOption(HDC hdc ,int FontSize, int FontBold)
	{
		g_hFont = CreateFont(FontSize, 0, 0, 0, FontBold, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "¸ð¸®½º9");
		g_OldFont = (HFONT)SelectObject(hdc, g_hFont);
	}
	void FontDelete(HDC hdc)
	{
		SelectObject(hdc, g_OldFont);
		DeleteObject(g_hFont);
	}
}