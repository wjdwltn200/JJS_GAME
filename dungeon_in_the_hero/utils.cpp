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
	void NumberPont(HDC hdc, int num, int maxNum, float posX, float posY, int CorType, float scale)
	{
		int tempX = posX;
		int tempY = posY;
		int tempMaxNumber = maxNum;
		int tempNumber = NULL;
		int tempNumberCheck = 1;

		for (int i = 0; i < maxNum; i++)
		{
			if (i == 0)
			{
				tempNumber = (num % 10) % 10;
				IMAGEMANAGER->findImage("NumberPont")->frameRender(hdc,
					tempX + ((IMAGEMANAGER->findImage("NumberPont")->getFrameWidth() * scale) * maxNum),
					tempY, tempNumber, CorType, scale);
			}
			else
			{
				tempNumber = (num % (tempNumberCheck * 100)) / (tempNumberCheck * 10);
				IMAGEMANAGER->findImage("NumberPont")->frameRender(hdc,
					tempX + ((IMAGEMANAGER->findImage("NumberPont")->getFrameWidth() * scale) * (maxNum - i)),
					tempY, tempNumber, CorType, scale);
				tempNumberCheck *= 10;
			}
		}
	}

	void HetSe(bool isBase, int seType)
	{
		if (isBase)
		{
			switch (seType)
			{
			case Slime:
				break;
			case Flower:
				break;
			case FlowerV2:
				break;
			case Bug:
				break;
			case BugV2:
				break;
			case Lili:
				SOUNDMANAGER->play("Sound/SE/LiliAtt.wav");
				break;
			case Demon:
				break;
			case Lizardman:
				break;
			default:
				break;
			}
		}
		else
		{
			switch (RANDOM->getInt(3))
			{
			case 0:
				SOUNDMANAGER->play("Sound/SE/Het_0.wav");
				break;
			case 1:
				SOUNDMANAGER->play("Sound/SE/Het_1.wav");
				break;
			case 2:
				SOUNDMANAGER->play("Sound/SE/Het_2.wav");
				break;
			default:
				break;
			}
		}

		
	}
}