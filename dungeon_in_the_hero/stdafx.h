// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <utility>


#include <string>
#include <vector>
#include <map>
#include <list>
#include <commdlg.h>


#define ENEMYKINDNUMBER 20
#define LOC_MINIMAPX 720
#define LOC_MINIMAPY 720
#define PI 3.141592f

// 클라이언트 사이즈
#define WINS_PRISON_MIN_X 60
#define WINS_PRISON_MIN_Y 83
#define WINS_PRISON_MAX_X WINSIZEX - 5
#define WINS_PRISON_MAX_Y WINSIZEY - 5

// 벡터 세팅
#define SET_VECTER 10

// 게임 화면 카메라 좌표 보정 값
#define CAMERA_MAX_Y -256.0f



using namespace std;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
//파일 경로 수정
//#include <shlwapi.h> #pragma comment(lib, "shlwapi")

#define SAFE_DELETE(p)	{ if (p) delete p; p = NULL; }

#define SYNTHESIZE(varType, varName, funcName)\
protected: varType varName;\
public: varType get##funcName() { return varName; }\
public: void set##funcName(varType var) { varName = var; }

#include "txtData.h"
#include "timeManager.h"
#include "keyManager.h"
#include "imageManager.h"
#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "sceneManager.h"
#include "CameraManager.h"
#include "effectManager.h"
#include "ObjectManager.h"
#include "utils.h"

typedef struct tagEnemyData
{
	bool t_isAilve;
	float t_posX;
	float t_posY;

	float t_moveEndX;
	float t_moveEndY;

	image * t_img;
	image * t_img_R;
	image * t_img_RA;
	image * t_img_L;
	image * t_img_LA;
	image * t_img_Dead;
	float t_scale;
	int t_alphaValue;

	int t_currHp;
	int t_MaxHp;
	int t_currDef;

	float t_moveSpeed;
	int t_moveDaley;

	int t_atkPoint;
	int t_attType;
	int t_atkDaley;

	int t_defPoint;

	int t_DieCountDaley;
	int t_setTileMapNum;

	int t_tilePosX;
	int t_tilePosY;

	int t_FoodChainLv;
	int t_currMana;

	RECT t_rc;

	int		t_enumType;
} EnemyInfo;

typedef struct tagTileData
{
	image * t_img;
	RECT	t_rc;
	bool	t_isAlive;
	bool	t_isNetDes;
	int		t_setImg;
	bool	t_isStart;
	bool	t_isShaking;
	int		t_ShakingCount;
	int		t_enumType;
	bool	t_isItem;
	int		t_enumItem;

	int		t_ManaValue;

	tagEnemyData * t_enemyInfo;

	int		t_setX;
	int		t_setY;
} TileInfo;

typedef struct tagTileTxt
{
	string t_txtName;
	string t_txtInfo;
	int t_AtkPoint;
	int t_DefPoint;
	int t_HpPoint;
	int t_MaxHpPoint;
	int t_TileMana;
	string t_TileEnemyName;
} PopupInfo;

typedef struct tagItemData
{
	int t_value;
	short t_frameX;
	short t_frameY;
} ItemInfo;

enum tagItemType
{
	JEWEL, ITEM_NUM
};

enum tagObjectType
{
	TILE_DIS, ITEM_JEWEL
};

enum tagEnemyType
{
	Slime, Flower, Bug
};

typedef struct tagPlayerData
{
	int t_TileDesEne;
} PlayerInfo;



typedef struct tagHeroData
{
	bool t_isAilve;
	float t_posX;
	float t_posY;

	image * t_img;
	int t_scale;
	int t_alphaValue;

	int t_currHp;
	int t_MaxHp;
	int t_currDef;

	int t_moveType;
	int t_moveDaley;

	int t_attType;
	int t_atkDaley;

	int t_tilePosX;
	int t_tilePosY;

	int t_setTileMapNum;
} HeroInfo;

typedef struct tagSaveData
{
	bool gisTest = false;
} SAVE_DATA;
#define  WINDOW_SCREEN	// FULL_SCREEN, WINDOW_SCREEN, MAP_TOOL

#ifdef FULL_SCREEN
	#define WINSTARTX 0
	#define WINSTARTY 0
	#define WINSIZEX GetSystemMetrics(SM_CXSCREEN)
	#define WINSIZEY GetSystemMetrics(SM_CYSCREEN)
#elif defined MAP_TOOL
	#define WINSTARTX 50
	#define WINSTARTY 50
	#define WINSIZEX 1600
	#define WINSIZEY 800
#else
	#define WINSTARTX 50.0f
	#define WINSTARTY 50.0f
	#define WINSIZEX 800.0f
	#define WINSIZEY 600.0f
#endif

#define RANDOM randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define TXTDATA txtData::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define CAMERA CameraManager::getSingleton()
#define EFFMANAGER effectManager::getSingleton()
#define OBJECTMANAGER ObjectManager::getSingleton()

extern HINSTANCE	g_hInstance;
extern HWND			g_hWnd;
extern HFONT		g_hFont;
extern HFONT		g_OldFont;
extern SAVE_DATA	g_saveData;
extern POINT		g_ptMouse;
extern RECT			g_MouseRc;
